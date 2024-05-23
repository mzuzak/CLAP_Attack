#!/usr/bin/env python3

import argparse
import os
import subprocess
import random


LOCKED_CIRCUITS = [
    "./probing_benchmarks/b14/AntiSAT/b14_BA16_gate_level_final.v.bench",
    "./probing_benchmarks/b14/Full-Lock/b14_locked_40_40_gate_level_final.v.bench",
    "./probing_benchmarks/b14/SFLL/b14_SFLL_277_gate_level_final.v.benchfix",
    "./probing_benchmarks/b14/SLL/b14_SLL.bench",
    
    "./probing_benchmarks/c1908/AntiSAT/c1908_BA16_gate_level_final.v.bench",
    "./probing_benchmarks/c1908/Full-Lock/c1908_locked_32_32_gate_level_final.v.bench",
    "./probing_benchmarks/c1908/SFLL/c1908_33_gate_level_final.v.bench",
    "./probing_benchmarks/c1908/SLL/c1908_SLL.bench",
    
    "./probing_benchmarks/c5315/AntiSAT/c5315_BA16_gate_level_final.v.bench",
    "./probing_benchmarks/c5315/Full-Lock/c5315_locked_40_40_gate_level_final.v.bench",
    "./probing_benchmarks/c5315/SFLL/c5315_SFLL_178_gate_level_final.v.bench",
    "./probing_benchmarks/c5315/SLL/c5315_sll.bench",
    
    "./probing_benchmarks/des/AntiSAT/des_BA16_gate_level_final.v.bench",
    "./probing_benchmarks/des/Full-Lock/des_locked_40_40_gate_level_final.v.bench",
    "./probing_benchmarks/des/SFLL/des_SFLL_256_gate_level_final.v.benchfix",
    "./probing_benchmarks/des/SLL/des_SLL.bench",
]


PRIOR_CIRCUITS = [  
    "./prior_stage_benchmarks/inputs/s38584.1.bench",
    "./prior_stage_benchmarks/inputs/s38584.1.bench",
]


PRIOR_STAGE_DIR = "./prior_stage_benchmarks/inputs/"
CLAP_WRAPPER = "./clap_wrapper.py" 

# UNROLL_FACTORS = list(range(2, 17))
UNROLL_FACTORS = [16]
# UNROLL_FACTORS = [3]
# PROBE_RESOLUTIONS = list(range(1, 21))
PROBE_RESOLUTIONS = list(range(11,21))


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
    base_name = os.path.basename(file_path)
    dir_name = os.path.dirname(file_path)
    par_dir = os.path.dirname(dir_name)

    modified_dir = os.path.join(par_dir, 'modified')
    if not os.path.exists(modified_dir):
        os.makedirs(modified_dir)
    
    new_file_path = os.path.join(modified_dir, f"modified_{num_duplications}x_{base_name}")

    with open(file_path, 'r') as original, open(new_file_path, 'w') as modified:
        # Copy all original lines to the modified file
        original_lines = original.readlines()
        modified.writelines(original_lines)

        # Process each line to check for OUTPUT declarations
        for line in original_lines:
            line = line.strip()
            # Check if the line is an output definition
            if line.startswith("OUTPUT"):
                # Extract the net name from between the parentheses
                net = line[line.find('(') + 1:line.find(')')].strip()
                # Generate duplicate OUTPUT lines and assignments
                for i in range(1, num_duplications + 1):
                    new_net_name = f"{net}M{i}"
                    new_output_line = f"OUTPUT({new_net_name})"
                    # Write the new OUTPUT declaration
                    modified.write(new_output_line + '\n')
                    # Assign the original net to the new net
                    new_assignment_line_invert = f"{new_net_name}_not = NOT({net})"
                    modified.write(new_assignment_line_invert + '\n')
                    new_assignment_line_assign = f"{new_net_name} = NOT({new_net_name}_not)"
                    modified.write(new_assignment_line_assign + '\n')

    return new_file_path


def count_lines(filename):
    """Counts the number of lines in a given file."""
    with open(filename, 'r') as file:
        return sum(1 for line in file)
    
def sort_files_by_line_count(files):
    """Sorts a list of file paths by the number of lines in each file, in descending order, and returns the sorted list of file paths."""
    file_line_counts = [(count_lines(file), file) for file in files]
    file_line_counts.sort()
    sorted_files = [file for _, file in file_line_counts]

    return sorted_files

    
def find_compatible_circuits(locked_circuit):
    """
    Identifies compatible prior stage circuits based on the number of inputs and outputs.
    """
    locked_inputs_count = len(parse_inputs(locked_circuit))
    compatible_files = []

    accepted_files = ['b01', 'b03', 'b04', 'b06', 'b07', 'b08', 'b09', 'b10', 'b11', 'b12', 'b13', 's38584.1']
    # accepted_files = ['b07', 's38584.1']

    for root, _, files in os.walk(PRIOR_STAGE_DIR):
        for file in files:
            if file.endswith(".bench"):

                # file_path = os.path.join(root, file)
                # if count_outputs(file_path) >= locked_inputs_count:
                #     compatible_files.append(file_path)
            

                # if none of the accepted files are in the file path, skip it
                if not any(accepted_file in file for accepted_file in accepted_files):
                    continue


                file_path = os.path.join(root, file)
                file_outputs_count = count_outputs(file_path)
                if file_outputs_count >= locked_inputs_count:
                    compatible_files.append(file_path)
                else:
                    required_duplications = (locked_inputs_count + file_outputs_count - 1) // file_outputs_count
                    new_file_path = duplicate_outputs(file_path, required_duplications)
                    compatible_files.append(new_file_path)


    return compatible_files


def run_tests(locked_circuit, compatible_files, key):
    """
    Runs various tests using the run_test.py script for each compatible file.
    """
    print(f"compatible files: {compatible_files}")


    # Base run without a prior circuit
    # print(f"running: 'python3 {RUN_TEST_SCRIPT} {locked_circuit} -k {key}'")
    # subprocess.run(["python3", RUN_TEST_SCRIPT, locked_circuit, "-k", key])

    # Runs with varying unroll lengths
    # for file in compatible_files:
    #     for unroll_factor in UNROLL_FACTORS:
    #         print(f"running: 'python3 {RUN_TEST_SCRIPT} {locked_circuit} -s {file} -u {unroll_factor} -k {key}'")
    #         subprocess.run(["python3", RUN_TEST_SCRIPT, locked_circuit, "-s", file, "-u", str(unroll_factor), "-k", key])


    # # Runs varying probe resolution with base circuit
    # for probe_resolution in PROBE_RESOLUTIONS:
    #     print(f"running: 'python3 {RUN_TEST_SCRIPT} {locked_circuit} -r {probe_resolution} -k {key}'")
    #     subprocess.run(["python3", RUN_TEST_SCRIPT, locked_circuit, "-r", str(probe_resolution), "-k", key])


    
    # Run with varying probe resolution and unroll factors together
    for file in compatible_files:
        for unroll_factor in UNROLL_FACTORS:
            for probe_resolution in PROBE_RESOLUTIONS:
                print(f"running: 'python3 {CLAP_WRAPPER} {locked_circuit} -s {file} -u {unroll_factor} -r {probe_resolution}'")
                subprocess.run(["python3", CLAP_WRAPPER, locked_circuit, "-s", file, "-u", str(unroll_factor), "-r", str(probe_resolution)])


def main():
    parser = argparse.ArgumentParser(description='Run tests on locked circuits with optional prior circuits.')
    parser.add_argument('-p', '--prior-circuits', nargs='*', help='Specify prior circuits to use directly', default=None)
    args = parser.parse_args()

    prior_circuits = args.prior_circuits

    for locked_circuit in LOCKED_CIRCUITS:
        # print(f"Processing: {locked_circuit}")
        key_length = count_key_inputs(locked_circuit)
        key = generate_random_key(key_length)

        if prior_circuits is None:
            # Find compatible circuits if no specific prior circuits are provided
            compatible_files = find_compatible_circuits(locked_circuit)
            compatible_files = sort_files_by_line_count(compatible_files)
            
        else:
            # compatible_files = prior_circuits
            compatible_files = PRIOR_CIRCUITS
        
        run_tests(locked_circuit, compatible_files, key)

if __name__ == "__main__":
    main()