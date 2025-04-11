#!/usr/bin/env python3

import argparse

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

def rename_outputs_and_nets(target_file_path, input_names, output_file_path=None):
    """
    Modifies the .bench file's outputs to match the names of the inputs from the locked circuit file,
    and renames all instances of these outputs throughout the file.
    If an output file path is provided, writes to that file. Otherwise, overwrites the target file.
    """
    with open(target_file_path, 'r') as file:
        content = file.readlines()
    
    output_names = [line.split('(')[-1].split(')')[0] for line in content if line.startswith('OUTPUT')]
    
    rename_map = {old: new for old, new in zip(output_names, input_names)}
    
    updated_content = []
    for line in content:
        for old_name, new_name in rename_map.items():
            if old_name in line:
                line = line.replace(old_name, new_name)
        updated_content.append(line)
    
    write_path = output_file_path if output_file_path else target_file_path
    with open(write_path, 'w') as file:
        file.writelines(updated_content)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Rename outputs and nets in a .bench file based on another .bench file's inputs.")
    parser.add_argument("locked_circuit_file", help="Path to the locked circuit .bench file")
    parser.add_argument("sequential_file", help="Path to the sequential .bench file")
    parser.add_argument("-o", "--output", help="Path to the output .bench file (optional). If not provided, the sequential file will be overwritten.", default=None)

    args = parser.parse_args()

    input_names = parse_inputs(args.locked_circuit_file)
    rename_outputs_and_nets(args.sequential_file, input_names, args.output)
