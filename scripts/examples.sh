#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
REPO_DIR=$SCRIPT_DIR/..

cd $REPO_DIR
make

OUTPUT_FILE="$SCRIPT_DIR/test_output.txt"
# Clear the output file
> $OUTPUT_FILE

ABC_LOG="$SCRIPT_DIR/abc_log.txt"
# Clear the ABC log file
> $ABC_LOG

run_abc_test() {
    local abc_script=$1
    local correct_keystore=$2
    local test_id=$3

    # Run ABC with the commands
    echo "$abc_script" | ./abc | tee $ABC_LOG

    local keystore_line=$(grep "KeyStore is now" $OUTPUT_FILE | tail -n 1)
    local extracted_keys=${keystore_line#*: }
    extracted_keys=${extracted_keys%,*}
    local correct_keys=${correct_keystore%,*}

    IFS=', ' read -ra extracted_key_array <<< "$extracted_keys"
    IFS=', ' read -ra correct_key_array <<< "$correct_keys"

    local matching_bits=0
    local undetermined_bits=0

    for i in "${!extracted_key_array[@]}"; do
        if [ "${extracted_key_array[i]}" == "${correct_key_array[i]}" ]; then
            ((matching_bits++))
        elif [ "${extracted_key_array[i]}" == "-1" ]; then
            ((undetermined_bits++))
        else
            echo "$test_id: Problem at index $i: Extracted bit is ${extracted_key_array[i]}, but correct bit is ${correct_key_array[i]}" >> $OUTPUT_FILE
        fi
    done

    echo "$test_id: Matching bits: $matching_bits" >> $OUTPUT_FILE
    echo "$test_id: Undetermined bits: $undetermined_bits" >> $OUTPUT_FILE

    if [ $((matching_bits + undetermined_bits)) -eq "${#correct_key_array[@]}" ]; then
        echo "$test_id: All bits are either matching or undetermined!" >> $OUTPUT_FILE
    else
        echo "$test_id: There is a problem with the bit comparison!" >> $OUTPUT_FILE
    fi

    echo -en "\n" >> "$OUTPUT_FILE"
}

# Define the ABC scripts and the correct KeyStore values
abc_script1="read_bench ./probing_benchmarks/c1908/SLL/c1908_SLL.bench  
clap -k 0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111
quit
"
correct_keystore1="{-1, -1, -1, -1, -1, -1, 0, 0, 1, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 1, 1, -1, 1, -1, -1, 1, -1, 0, 1, -1, -1, 1, -1, 1, 1, 1, -1, 1, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, }"

abc_script2="read_bench ./probing_benchmarks/c1908/SLL/c1908_SLL.bench 
clap -m -c 7 -l 0.5 -k 0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111
quit
"
correct_keystore2= "{-1, -1, -1, -1, -1, -1, 0, 0, 1, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 1, -1, 1, -1, -1, 1, -1, 0, 1, -1, -1, 1, -1, 1, 1, 1, -1, 1, -1, 0, 1, 1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, }"

# Run the tests
run_abc_test "$abc_script1" "$correct_keystore1" "Test1"
run_abc_test "$abc_script2" "$correct_keystore2" "Test2"
