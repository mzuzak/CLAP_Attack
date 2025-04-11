#!/usr/bin/env python3

import argparse
import os
import subprocess
import random
import signal
import sys
from concurrent.futures import ThreadPoolExecutor, as_completed

script_dir = os.path.dirname(os.path.abspath(__file__))
clap_attack_dir = os.path.dirname(script_dir)


LOCKED_CIRCUITS = [
    f"{clap_attack_dir}/probing_benchmarks/b14/AntiSAT/b14_BA16_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/b14/Full-Lock/b14_locked_40_40_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/b14/SFLL/b14_SFLL_277_gate_level_final.v.benchfix",
    f"{clap_attack_dir}/probing_benchmarks/b14/SLL/b14_SLL.bench",
    
    f"{clap_attack_dir}/probing_benchmarks/c1908/AntiSAT/c1908_BA16_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/c1908/Full-Lock/c1908_locked_32_32_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/c1908/SFLL/c1908_33_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/c1908/SLL/c1908_SLL.bench",
    
    f"{clap_attack_dir}/probing_benchmarks/c5315/AntiSAT/c5315_BA16_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/c5315/Full-Lock/c5315_locked_40_40_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/c5315/SFLL/c5315_SFLL_178_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/c5315/SLL/c5315_sll.bench",
    
    f"{clap_attack_dir}/probing_benchmarks/des/AntiSAT/des_BA16_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/des/Full-Lock/des_locked_40_40_gate_level_final.v.bench",
    f"{clap_attack_dir}/probing_benchmarks/des/SFLL/des_SFLL_256_gate_level_final.v.benchfix",
    f"{clap_attack_dir}/probing_benchmarks/des/SLL/des_SLL.bench",
]

# For Algorithm 2, use this single prior circuit to avoid large runtimes
PRIOR_CIRCUITS = [
    f"{clap_attack_dir}/bench_files/inputs/s38584.1.bench"
]

NUM_THREADS = 1024
executor = None

PRIOR_STAGE_DIR = f"{clap_attack_dir}/bench_files/inputs/"
CLAP_WRAPPER_SCRIPT = f"{script_dir}/clap_wrapper.py"

UNROLL_FACTORS = list(range(2, 31))
PROBE_RESOLUTIONS = list(range(1, 21))


def generate_random_key(length):
    """Generates a random binary string of the specified length."""
    return ''.join(random.choice(['0', '1']) for _ in range(length))

def count_key_inputs(locked_circuit):
    """Counts the number of key inputs in the given locked circuit file."""
    key_input_count = 0
    with open(locked_circuit, 'r') as file:
        for line in file:
            if line.startswith('INPUT') and 'key' in line:
                key_input_count += 1
    return key_input_count

def parse_inputs(file_path):
    """
    Parses the .bench file to find all input names excluding those containing 'key'.
    Returns a list of input names in the order they appear in the file.
    """
    input_names = []
    with open(file_path, 'r') as file:
        for line in file:
            if line.startswith('INPUT') and 'key' not in line:
                name = line.split('(')[-1].split(')')[0]
                input_names.append(name)
    return input_names

def count_outputs(file_path):
    """
    Counts the number of outputs in a given .bench file.
    """
    with open(file_path, 'r') as file:
        return sum(1 for line in file if line.startswith('OUTPUT'))

def duplicate_outputs(file_path, num_duplications):
    """
    Creates a copy of the given bench file in which each original output is duplicated
    (num_duplications) times. Returns the path to the new file.
    """
    base_name = os.path.basename(file_path)
    dir_name = os.path.dirname(file_path)
    par_dir = os.path.dirname(dir_name)

    modified_dir = os.path.join(par_dir, 'modified')
    if not os.path.exists(modified_dir):
        os.makedirs(modified_dir)
    
    new_file_path = os.path.join(modified_dir, f"modified_{num_duplications}x_{base_name}")

    with open(file_path, 'r') as original, open(new_file_path, 'w') as modified:
        original_lines = original.readlines()
        # Copy all original lines
        modified.writelines(original_lines)

        # Process each line to check for OUTPUT declarations
        for line in original_lines:
            line = line.strip()
            if line.startswith("OUTPUT"):
                # Extract the net name
                net = line[line.find('(') + 1:line.find(')')].strip()
                # Generate duplicate OUTPUT lines and assignments
                for i in range(1, num_duplications + 1):
                    new_net_name = f"{net}M{i}"
                    new_output_line = f"OUTPUT({new_net_name})"
                    modified.write(new_output_line + '\n')
                    # Simple duplication so it doesn't change logic
                    new_assignment_line_invert = f"{new_net_name}_not = NOT({net})"
                    modified.write(new_assignment_line_invert + '\n')
                    new_assignment_line_assign = f"{new_net_name} = NOT({new_net_name}_not)"
                    modified.write(new_assignment_line_assign + '\n')

    return new_file_path

def count_lines(filename):
    """Counts the number of lines in a given file."""
    with open(filename, 'r') as file:
        return sum(1 for _ in file)

def sort_files_by_line_count(files):
    """
    Sorts a list of file paths by the number of lines in each file, in ascending order,
    and returns the sorted list of file paths.
    """
    file_line_counts = [(count_lines(f), f) for f in files]
    file_line_counts.sort()
    return [f for _, f in file_line_counts]

def find_compatible_circuits(locked_circuit):
    """
    Identifies compatible prior stage circuits based on the number of inputs and outputs.
    If a prior circuit does not have enough outputs for the locked circuit's inputs,
    we create a duplicated version with sufficient outputs.
    """
    locked_inputs_count = len(parse_inputs(locked_circuit))
    compatible_files = []

    accepted_files = [
        'b01', 'b03', 'b04', 'b06', 'b07',
        'b08', 'b09', 'b10', 'b11', 'b12',
        'b13', 's38584.1'
    ]

    for root, _, files in os.walk(PRIOR_STAGE_DIR):
        for file in files:
            if not file.endswith(".bench"):
                continue

            # If the file name doesn't contain any of the accepted patterns, skip it
            if not any(accepted in file for accepted in accepted_files):
                continue

            file_path = os.path.join(root, file)
            file_outputs_count = count_outputs(file_path)

            if file_outputs_count >= locked_inputs_count:
                # Enough outputs already
                compatible_files.append(file_path)
            else:
                # Duplicate outputs
                required_duplications = (
                    (locked_inputs_count + file_outputs_count - 1) // file_outputs_count
                )
                new_file_path = duplicate_outputs(file_path, required_duplications)
                compatible_files.append(new_file_path)

    return compatible_files

def run_subprocess(command):
    """Runs a subprocess command and checks for errors."""
    subprocess.run(command, check=True)
    # print(command)

def signal_handler(sig, frame):
    print("Interrupt received, shutting down...")
    if executor:
        executor.shutdown(wait=False)
    sys.exit(0)

def run_tests(locked_circuit, key, algorithm, compatible_files):
    """
    Dispatches runs to the CLAP_WRAPPER_SCRIPT script, controlling whether we pass '-m' or not
    and which prior circuits to try, based on 'algorithm'.
    
    - For Algo 1 (no '-m'):
      (1) For each compatible_file, run all unroll_factors.
      (2) Then vary probe resolutions with no prior circuit, but unroll=16.
      (3) Then vary probe resolutions with a prior circuit (all compatible), unroll=16.

    - For Algo 2 (with '-m'):
      Same as above, except we use the smaller PRIOR_CIRCUITS set instead of all.
      (1) For each prior circuit, run all unroll_factors.
      (2) Vary probe resolutions with no prior circuit, unroll=16.
      (3) Vary probe resolutions with prior circuit, unroll=16.
    """
    global executor

    # Decide if we are using -m or not.
    use_m = (algorithm == 2)
    # If algorithm=1, we use the large set of 'compatible_files', else the small set in PRIOR_CIRCUITS.
    # 'compatible_files' already set from main.

    with ThreadPoolExecutor(max_workers=NUM_THREADS) as executor:
        futures = []

        # Run default, no sequential circuit, no unroll factor, no probe res
        cmd = [
            "python3", CLAP_WRAPPER_SCRIPT,
            locked_circuit,
            "-c", "6",
            "-k", key
        ]
        if use_m:
            cmd.append("-m")
        futures.append(executor.submit(run_subprocess, cmd))

        # -------------------------------------------------------------------------
        # PART (1) Unroll tests (all unroll_factors) for each prior circuit
        # If no prior circuit => skip. We'll do that in the probe resolution test.
        # -------------------------------------------------------------------------
        for prior_file in compatible_files:
            for uf in UNROLL_FACTORS:
                cmd = [
                    "python3", CLAP_WRAPPER_SCRIPT,
                    locked_circuit,
                    "-c", "6",
                    "-k", key
                ]
                if use_m:
                    cmd.append("-m")
                cmd += ["-s", prior_file, "-u", str(uf)]
                futures.append(executor.submit(run_subprocess, cmd))

        # -------------------------------------------------------------------------
        # PART (2) Probe resolution sweep with NO prior circuit
        # -------------------------------------------------------------------------
        for pr in PROBE_RESOLUTIONS:
            cmd = [
                "python3", CLAP_WRAPPER_SCRIPT,
                locked_circuit,
                "-c", "6",
                "-k", key
            ]
            if use_m:
                cmd.append("-m")
            cmd += [ "-r", str(pr)]
            futures.append(executor.submit(run_subprocess, cmd))

        # -------------------------------------------------------------------------
        # PART (3) Probe resolution sweep WITH prior circuit(s), unroll=16
        # -------------------------------------------------------------------------
        for prior_file in compatible_files:
            for pr in PROBE_RESOLUTIONS:
                cmd = [
                    "python3", CLAP_WRAPPER_SCRIPT,
                    locked_circuit,
                    "-c", "6",
                    "-k", key
                ]
                if use_m:
                    cmd.append("-m")
                # use prior_file, unroll=16
                cmd += ["-s", prior_file, "-u", "16", "-r", str(pr)]
                futures.append(executor.submit(run_subprocess, cmd))

        # Wait for all submissions to complete
        for future in as_completed(futures):
            try:
                future.result()
            except Exception as exc:
                print(f"Error in one of the runs: {exc}")

def main():
    signal.signal(signal.SIGINT, signal_handler)

    parser = argparse.ArgumentParser(
        description='Run tests on locked circuits with optional prior circuits.'
    )
    parser.add_argument('-a', '--algorithm', type=int, choices=[1, 2], required=True,
                        help='Specify the algorithm to run: 1 (no -m) or 2 (with -m).')
    args = parser.parse_args()
    algorithm = args.algorithm

    for locked_circuit in LOCKED_CIRCUITS:
        # Generate a random key for each locked circuit
        key_length = count_key_inputs(locked_circuit)
        key = generate_random_key(key_length)

        if algorithm == 1:
            # Algo 1 => large set of compatible prior circuits
            compatible_files = find_compatible_circuits(locked_circuit)
        else:
            # Algo 2 => smaller set
            compatible_files = PRIOR_CIRCUITS

        print(f"Locked circuit: {locked_circuit}")
        print(f"Key: {key}")
        print(f"Algorithm {algorithm} -> #Compatible files: {len(compatible_files)}")

        run_tests(locked_circuit, key, algorithm, compatible_files)

if __name__ == "__main__":
    main()
