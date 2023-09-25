#!/bin/bash

MIN_R_VALUE=1
MAX_R_VALUE=20

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR=$SCRIPT_DIR/../..

cd $REPO_DIR
make

OUTPUT_FILE="$SCRIPT_DIR/test_output.txt"
# Clear the output file
> $OUTPUT_FILE

ABC_LOG="$SCRIPT_DIR/abc_log.txt"
# Clear the ABC log file
> $ABC_LOG

run_test() {
    local r_value=$1
    local abc_script_template=$2
    local correct_keystore=$3
    local test_id=$4

    # Replace the placeholder in the .abc script with the actual -r value
    local abc_script=$(echo "$abc_script_template" | sed "s/__R_VALUE__/$r_value/")

    # Run ABC with the modified commands
    echo "$abc_script" | ./abc | tee -a "$ABC_LOG"

    # Extract the "KeyStore is now" line from the output
    local keystore_line=$(grep "KeyStore is now" "$ABC_LOG" | tail -n 1)

    # Extract the keys from the strings
    local extracted_keys=${keystore_line#*: }
    extracted_keys=${extracted_keys%,*} # Trim the trailing comma
    local correct_keys=${correct_keystore%,*} # Trim the trailing comma

    # Convert string of keys to arrays
    IFS=', ' read -ra extracted_key_array <<< "$extracted_keys"
    IFS=', ' read -ra correct_key_array <<< "$correct_keys"

    local matching_bits=0
    local undetermined_bits=0

    # Compare the extracted keys with the correct keys
    for i in "${!extracted_key_array[@]}"; do
        if [ "${extracted_key_array[i]}" == "${correct_key_array[i]}" ]; then
            ((matching_bits++))
        elif [ "${extracted_key_array[i]}" == "-1" ]; then
            ((undetermined_bits++))
        else
            echo "$test_id: Problem at index $i: Extracted bit is ${extracted_key_array[i]}, but correct bit is ${correct_key_array[i]}" >> "$OUTPUT_FILE"
        fi
    done

    # Output the results
    echo "$test_id: Matching bits: $matching_bits" >> "$OUTPUT_FILE"
    echo "$test_id: Undetermined bits: $undetermined_bits" >> "$OUTPUT_FILE"

    if [ $((matching_bits + undetermined_bits)) -eq "${#correct_key_array[@]}" ]; then
        echo "$test_id: All identified bits match bits found with -r 1" >> "$OUTPUT_FILE"
    else
        echo "$test_id: There is a problem with the bit comparison!" >> "$OUTPUT_FILE"
    fi

    echo -en "\n" >> "$OUTPUT_FILE"
}

abc_script_template1="read_bench ./probing_benchmarks/c1908/SLL/c1908_SLL.bench 
clap -v -r __R_VALUE__ -k 0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111
quit
"
correct_keystore1="{-1, -1, -1, -1, -1, -1, 0, 0, 1, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 1, 1, -1, 1, -1, -1, 1, -1, 0, 1, -1, -1, 1, -1, 1, 1, 1, -1, 1, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, }"


abc_script_template2="read_bench ./probing_benchmarks/c1908/SLL/c1908_SLL.bench 
clap -v -m -c 7 -l 0.5 -r __R_VALUE__ -k 0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111
quit
"
correct_keystore2="{-1, -1, -1, -1, -1, -1, 0, 0, 1, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 1, -1, 1, -1, -1, 1, -1, 0, 1, -1, -1, 1, -1, 1, 1, 1, -1, 1, -1, 0, 1, 1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, }"


# Loop over the desired -r values and run the tests
for r_value in $(seq $MIN_R_VALUE $MAX_R_VALUE)
do
    echo "Running test 1 with -r $r_value" >> "$OUTPUT_FILE"
    run_test $r_value "$abc_script_template1" "$correct_keystore1" "Test 1 -r $r_value"
done

for r_value in $(seq $MIN_R_VALUE $MAX_R_VALUE)
do
    echo "Running test 2 with -r $r_value" >> "$OUTPUT_FILE"
    run_test $r_value "$abc_script_template2" "$correct_keystore2" "Test 2 -r $r_value"
done
