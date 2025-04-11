#!/usr/bin/env python3

import argparse
import os
import subprocess
import random
import re
import json
import time
import uuid
from colorama import Fore, Style
import csv
import tempfile

from match_io import parse_inputs, rename_outputs_and_nets  


script_dir = os.path.dirname(os.path.abspath(__file__))
clap_attack_dir = os.path.dirname(script_dir)

ABC_SCRIPT_DIR = f"{clap_attack_dir}/abc_scripts/tests/"
BASE_LOG_DIR = f"{clap_attack_dir}/logs/"
TMP_DIR = f"{clap_attack_dir}/tmp"
KEY_FILE_PATH = f"{clap_attack_dir}/probing_benchmarks/keys.json"


def load_keys():
    """
    Load the keys from the JSON file.
    """
    with open(KEY_FILE_PATH, 'r') as file:
        return json.load(file)

def get_key(benchmark_name, locking_method):
    """
    Get the key for the specified benchmark and locking method.
    """
    keys = load_keys()
    try:
        key = keys[benchmark_name][locking_method]
    except:
        key = None
    return key

def generate_file_base_name(locked_circuit, original_seq_circuit_file_name, unroll_factor, probe_resolution):
    path_parts = locked_circuit.split('/')
    benchmark_name, locking_method = path_parts[-3], path_parts[-2]
    
    seq_circ_suffix = os.path.basename(original_seq_circuit_file_name) if original_seq_circuit_file_name else "base"
    unroll_suffix = f"_u{unroll_factor}" if unroll_factor else ""
    probe_res_suffix = f"_r{probe_resolution}" if probe_resolution else ""
    
    base_name = f"{benchmark_name}_{locking_method}_{seq_circ_suffix}{unroll_suffix}{probe_res_suffix}"
    return base_name

def generate_log_file_path(base_name):
    log_file_name = f"{base_name}_log.txt"
    os.makedirs(BASE_LOG_DIR, exist_ok=True)
    return os.path.join(BASE_LOG_DIR, log_file_name)

def generate_verilog_file_path(base_name):
    os.makedirs(os.path.join(TMP_DIR, "partial_leakage_verilog"), exist_ok=True)
    return os.path.join(TMP_DIR, "partial_leakage_verilog", f"{base_name}.v")

def generate_abc_script_path(base_name):
    return os.path.join(TMP_DIR, f"{base_name}.abc")

def parse_inputs(file_path):
    """
    Parses the .bench file to find all input names excluding those containing 'key'.
    Returns a list of input names in the order they appear in the file.
    """
    input_names = []
    with open(file_path, 'r') as file:
        for line in file:
            if line.startswith('INPUT'):
                name = line.split('(')[-1].split(')')[0]
                if 'key' not in name:
                    input_names.append(name)
    return input_names


def extract_key_from_comments(file_path):
    """
    Searches the file at `file_path` for a comment specifying the key.
    Assumes the key is specified in a comment with the format `# key=XXXX`
    where `XXXX` is the key of variable length.
    Returns the key as a string if found, or None if not found.
    """
    key_pattern = re.compile(r'#\s*key=\s*([01]+)')
    with open(file_path, 'r') as file:
        for line in file:
            match = key_pattern.search(line)
            if match:
                return match.group(1) 
    return None 

def rename_outputs_and_nets(target_file_path, input_names, output_file_path=None):
    """
    Modifies the .bench file's outputs to match the names of the inputs from the locked circuit file,
    and renames all instances of these outputs throughout the file using exact matches.
    If an output file path is provided, writes to that file. Otherwise, overwrites the target file.
    """
    with open(target_file_path, 'r') as file:
        content = file.readlines()
    
    output_names = [line.split('(')[-1].split(')')[0].strip() for line in content if line.startswith('OUTPUT')]
    
    rename_map = {old: new for old, new in zip(output_names, input_names)}
    
    updated_content = []
    for line in content:
        for old_name, new_name in rename_map.items():
            # Use regex to match the whole word only to prevent partial replacements
            pattern = r'\b' + re.escape(old_name) + r'\b'
            line = re.sub(pattern, new_name, line)
        updated_content.append(line)
    
    write_path = output_file_path if output_file_path else target_file_path
    with open(write_path, 'w') as file:
        file.writelines(updated_content)

def count_key_inputs(file_path):
    def parse_inputs(file_path, key=False):
        names = []
        with open(file_path, 'r') as file:
            for line in file:
                if line.startswith('INPUT') and (('key' in line) == key):
                    name = line.split('(')[-1].split(')')[0]
                    names.append(name)
        return names
    return len(parse_inputs(file_path, key=True))

def generate_random_key(length):
    return ''.join(random.choice('01') for _ in range(length))


def prepare_abc_script_content(locked_circuit, custom_key, multi_node, max_key_inputs, key_space_min, sat_solver_output, probe_resolution, verbose, seq_circuit_file, unroll_factor):
    command_components = ["read_bench {}".format(locked_circuit)]
    clap_command = ["clap"]
    
    if custom_key: clap_command.append("-k {}".format(custom_key))
    if multi_node: clap_command.append("-m")
    if max_key_inputs is not None: clap_command.append("-c {}".format(max_key_inputs))
    if key_space_min is not None: clap_command.append("-l {}".format(key_space_min))
    if sat_solver_output: clap_command.append("-o {}".format(sat_solver_output))
    if probe_resolution is not None: clap_command.append("-r {}".format(probe_resolution))
    if seq_circuit_file: clap_command.append("-s {}".format(seq_circuit_file))
    if unroll_factor is not None: clap_command.append("-u {}".format(unroll_factor))
    if verbose: clap_command.append("-v")
    
    return "\n".join(command_components + [" ".join(clap_command)])


def count_partial_key_leakage(verilog_file_path):
    """
    Parses the given Verilog file to count the number of unique key inputs
    that appear in the file, indicating partial key leakage.
    """
    key_pattern = re.compile(r'keyinput\d+')
    found_keys = set()

    if os.path.exists(verilog_file_path):
        with open(verilog_file_path, 'r') as file:
            for line in file:
                found_keys.update(key_pattern.findall(line))

        os.remove(verilog_file_path)

    else:
        return 0

    return len(found_keys)


def execute_abc_script_with_logging(script_content, script_path, log_file_path, verilog_file_path):
    with open(script_path, "w") as tmp_script:
        tmp_script.write(script_content)
    
    # print(f"Running ABC script:\n{script_content}")
    start_time = time.time()
    print(f"Executing: './abc -f {script_path}'")
    result = subprocess.run(["./abc", "-f", script_path], capture_output=True, text=True)
    execution_time = time.time() - start_time

    stdout_output = result.stdout
    stderr_output = result.stderr



    full_key_pattern = r"We found (\d+) of (\d+) total keys using (\d+) probes"
    partial_key_pattern = r"Partial key leakage: (\d+)"
    verilog_file_pattern = r"Verilog file: (.*)"

    full_key_match = re.search(full_key_pattern, stdout_output)
    partial_key_match = re.search(partial_key_pattern, stdout_output)
    verilog_file_match = re.search(verilog_file_pattern, stdout_output)

    if verilog_file_match:
        generated_verilog_path = verilog_file_match.group(1)
        if os.path.exists(generated_verilog_path):
            os.rename(generated_verilog_path, verilog_file_path)
            print(f"Verilog file renamed to: {verilog_file_path}")
    else: 
        print(f"{Fore.RED}No verilog file found in the output.{Style.RESET_ALL}")

    with open(log_file_path, "w") as log_file:
        log_file.write(f"\nABC Script Commands:\n{script_content}\n\n--- ABC Output ---\n")
        log_file.write(stdout_output)
        if stderr_output:
            log_file.write("\n--- ABC Errors ---\n")
            log_file.write(stderr_output)
        log_file.write(f"\nExecution Time: {execution_time} seconds\n")

        if full_key_match:
            log_file.write(f"\nResults: {full_key_match.group(0)}\n")
        else:
            log_file.write("\nNo specific full key results pattern found in the output.\n")
            print(f"{Fore.RED}No specific results pattern found in the output.{Style.RESET_ALL}")

        if partial_key_match:
            log_file.write(f"\nPartial Key Leakage: {partial_key_match.group(1)}\n")
        else:
            log_file.write("\nNo specific partial key leakage pattern found in the output.\n")

    # os.remove(script_path)
    
    return {
        "execution_time": execution_time,
        "full_key_leakage": full_key_match.group(0) if full_key_match else "No results found",
        "partial_key_leakage": int(partial_key_match.group(1)) if partial_key_match else 0
    }
    


def append_results_to_csv(results, csv_path='results.csv'):
    headers = ['Locked Circuit', 'Prior Circuit', 'Unroll Factor', 'Probe Resolution', 
               'Execution Time', 'Full Key Leakage', 'Partial Key Leakage', 'key_source', 'key', 
               ]
    
    file_exists = os.path.isfile(csv_path) and os.path.getsize(csv_path) > 0
    
    with open(csv_path, 'a', newline='') as csvfile:
        writer = csv.DictWriter(csvfile, fieldnames=headers)
        
        if not file_exists:
            writer.writeheader()  
        
        writer.writerow({
            'Locked Circuit': results['locked_circuit'],
            'Prior Circuit': results['prior_circuit'],
            'Unroll Factor': results['unroll_factor'],
            'Probe Resolution': results['probe_resolution'],
            'Execution Time': results['execution_time'],
            'Full Key Leakage': results['full_key_leakage'],
            'Partial Key Leakage': results['partial_key_leakage'],
            'key_source': results['key_source'],
            'key': results['key'],
        })


def main():
    parser = argparse.ArgumentParser(description='Run the CLAP Attack on a locked circuit using ABC.')

    parser.add_argument('locked_circuit', type=str, help='Path to the locked circuit file')
    parser.add_argument('-s', '--seq_prior_circuit', type=str, help='Path to the sequential circuit preceding the locked circuit', default=None)
    parser.add_argument('-u', '--unroll_factor', type=int, help='Number of times the circuit is unrolled', default=None)
    parser.add_argument('-k', '--key', type=str, help='Optional specific key to use', default=None)
    parser.add_argument('-m', '--multi_node', action='store_true', help='Use multi-node probing algorithm')
    parser.add_argument('-c', '--max_key_inputs', type=int, help='Maximum number of key inputs for a node to be considered for EOFM probing', default=None)
    parser.add_argument('-l', '--key_space_min', type=float, help='Minimum portion of keyspace that must be eliminated for a multi-node probe', default=None)
    parser.add_argument('-o', '--sat_solver_output', type=str, help='Name of SAT solver output file', default=None)
    parser.add_argument('-r', '--probe_resolution', type=int, help='Probe resolution size', default=None)
    parser.add_argument('-v', '--verbose', action='store_true', help='Toggle printing verbose information')

    args = parser.parse_args()

    os.makedirs(TMP_DIR, exist_ok=True)
    
    modified_seq_circ_path = None
    original_seq_circ_file_name = None
    locked_circuit = None
    locking_method = None
    json_key = None

    if 'probing_benchmarks' in args.locked_circuit:
        locking_method = os.path.basename(os.path.dirname(args.locked_circuit))
        locked_circuit = os.path.basename(os.path.dirname(os.path.dirname(args.locked_circuit)))
        json_key = get_key(locked_circuit, locking_method)

    key_source = None
    if json_key is not None:
        key = json_key
        key_source = "Found key in keys.json"
    else:
        if args.key is not None:
            key = args.key
            key_source = "provided via command line"
        else:
            # Count the key inputs and generate a random key if not provided
            key_input_count = count_key_inputs(args.locked_circuit)
            key = generate_random_key(key_input_count)
            key_source = "auto-generated"

    if args.seq_prior_circuit:
        # Rename the outputs and nets of the sequential circuit to match the locked circuit
        original_seq_circ_file_name = os.path.basename(args.seq_prior_circuit)
        input_names = parse_inputs(args.locked_circuit)
        modified_seq_circ_path = os.path.join(TMP_DIR, f"{uuid.uuid4()}_{original_seq_circ_file_name}_modified.bench")

        rename_outputs_and_nets(args.seq_prior_circuit, input_names, modified_seq_circ_path)

    base_name = generate_file_base_name(args.locked_circuit, original_seq_circ_file_name, args.unroll_factor, args.probe_resolution)
    log_file_path = generate_log_file_path(base_name)
    verilog_file_path = generate_verilog_file_path(base_name)
    script_path = generate_abc_script_path(base_name)

    script_content = prepare_abc_script_content(args.locked_circuit, key, args.multi_node, args.max_key_inputs, args.key_space_min, args.sat_solver_output, args.probe_resolution, args.verbose, modified_seq_circ_path, args.unroll_factor)

    results = execute_abc_script_with_logging(script_content, script_path, log_file_path, verilog_file_path)
    

    # partial_count = count_partial_key_leakage('./global_keystore.v')
    # print(f"Partial key leakage: {partial_count}")
    
    append_results_to_csv({
        'locked_circuit': os.path.basename(args.locked_circuit),
        'prior_circuit': original_seq_circ_file_name,
        'unroll_factor': args.unroll_factor,
        'probe_resolution': args.probe_resolution,
        **results,
        'key': key,
        'key_source': key_source,
    })

if __name__ == "__main__":
    main()