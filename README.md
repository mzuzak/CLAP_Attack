# CLAP Attack: A Combined Logical and Physical Attack on Logic Obfuscation

The CLAP attack is an open-source attack on logic obfuscation utilizing both logical and physical leakage to unlock an obfuscated circuit. The physical portion of the CLAP attack logically guides an electro-optical probe to extract key leakage through electro-optical frequency mapping (EOFM). The logical portion of the CLAP attack relies on the open-source SAT attack toolkit proposed by Subramanyan et al. from "Evaluating the security of logic encryption algorithms". An extensive overview of the CLAP attack on logic obfuscation can be found in the ICCAD'22 paper titled:

    A Combined Logical and Physical Attack on Logic Obfuscation
    Authors: Michael Zuzak, Yuntao Liu, Isaac McDaniel, Ankur Srivastava

This code-base contains code for the physical portion of the CLAP attack aimed at guiding an EOFM probe to extract key leakage from a die. It can be run entirely in simulation. This code is built on the Berkely ABC tool, which can be found at (https://people.eecs.berkeley.edu/~alanmi/abc/).

## Compiling:

To compile the CLAP attack, clone this repo and then type `make`.

### Compiling as C or C++

The current version of ABC can be compiled with C compiler or C++ compiler.

 * To compile as C code (default): make sure that `CC=gcc` and `ABC_NAMESPACE` is not defined.
 * To compile as C++ code without namespaces: make sure that `CC=g++` and `ABC_NAMESPACE` is not defined.
 * To compile as C++ code with namespaces: make sure that `CC=g++` and `ABC_NAMESPACE` is set to
   the name of the requested namespace. For example, add `-DABC_NAMESPACE=xxx` to OPTFLAGS.

## Running the CLAP Attack:

The CLAP attack is run within the ABC synthesis tool. After successfully compiling abc, the CLAP attack can be run from the abc command line. To do so:

* Launch the abc command line tool

    
    ```./abc```

* Read in an obfuscated benchmark circuit. Several have been provided in the probing_benchmarks directory of this repo.

    ```read_bench <BENCHMARK_NAME>```

* Launch the CLAP attack against this benchmark. The usage notes for the command is shown below:

    ```abc 01> clap -h
    usage: clap [-mclovh] -k <key> 
                   The physical portion of the CLAP attack in ABC.
        -k <key>   : input the correct oracle key value for EOFM probing simulation 
        -m         : use multi-node probing algorithm (alg. 2) for CLAP attack, omitting this command uses fixed EOFM probe algorithm (alg. 1)
        -c <int>   : maximum number of key inputs for a node to be considered for EOFM probing [default = 7]
        -l <float> : minimum portion of keyspace that must be eliminated for a multi-node probe to be run [default = 0.006125]
        -o <str>   : set name of SAT solver output file from physical portion of CLAP attack [default = "physical_clap_out.bench"]
        -v         : toggle printing verbose information [default = no]
        -h         : print the command usage```

* After attack termination, the CLAP attack outputs the known key values as well as a partially unlocked circuit in the file `physical_clap_out.bench' (unless another name was selected). This circuit can be passed directly to the open-source SAT attack toolkit from Subramanyan et al. in "Evaluating the security of logic encryption algorithms" to initiate the logical portion of the CLAP attack. By doing so, the SAT attack will build upon the physical-attack-limited keyspace produced by this tool.
    
## CLAP Attack Example Runs

### CLAP Attack Example 1
The commands below launch the fixed EOFM probing algorithm (algo. 1) for the CLAP attack against the c1908 benchmark circuit obfuscated with SLL. The attack considers nodes with no more than 7 fan-in key inputs.
    
    moo@moo-VirtualBox:~/Research/CLAP_Attack$ ./abc
    UC Berkeley, ABC 1.01 (compiled Aug  9 2022 11:42:27)
    abc 01> read_bench ./probing_benchmarks/c1908/SLL/c1908_SLL.bench 
    abc 02> clap -k 0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111
    
The output of this sample should be:
    
    KeyStore is now: {-1, -1, -1, -1, -1, -1, 0, 0, 1, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, -1, 0, 0, 1, 1, -1, 1, -1, -1, 1, -1, 0, 1, -1, -1, 1, -1, 1, 1, 1, -1, 1, 0, 0, 1, 1, 1, -1, -1, -1, 0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, }

    We found 53 of 88 total keys using 335 probes

Additionally, one can find the partially unlocked `physical_clap_out.bench` file in the launch directory. The SAT attack can be run directly against this file.
    
### CLAP Attack Example 2
The commands below launch the multi-node probing algorithm (algo. 2) for the CLAP attack against the c1908 benchmark circuit obfuscated with SLL. The attack considers nodes with no more than 7 fan-in key inputs and a given multinode probe must eliminate at least 0.5% of the keyspace in order to be run.
    
    moo@moo-VirtualBox:~/Research/CLAP_Attack$ ./abc
    UC Berkeley, ABC 1.01 (compiled Aug  9 2022 11:42:27)
    abc 01> read_bench ./probing_benchmarks/c1908/SLL/c1908_SLL.bench 
    abc 02> clap -m -c 7 -l 0.5 -k 0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111
    
The output of this sample should be:
    
    KeyStore is now: {-1, -1, -1, -1, -1, -1, 0, 0, 1, 0, 0, 0, 1, 1, -1, -1, 1, 1, -1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, -1, 0, 0, 1, 1, -1, 1, -1, -1, 1, -1, 0, 1, -1, -1, 1, -1, 1, 1, 1, -1, 1, -1, 0, 1, 1, -1, -1, -1, -1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1, -1, -1, -1, -1, }

    We found 49 of 88 total keys using 12 probes

Additionally, one can find the partially unlocked `physical_clap_out.bench` file in the launch directory. The SAT attack can be run directly against this file.
    
## Benchmarks

All benchmarks from the ICCAD'22 manuscript can be found in the `probing_benchmarks` directory. The correct key value for each of these benchmarks is contained in the `probing_benchmarks/iccad_benchmark_keys.txt`.

## Troubleshooting:

**NOTE:** Currently, the underscore character ("_") is a restricted character for signal/net names in benchmark files. You will see memory leaks and seg-faults if you have these in the file. If you find/replace these characters with something else (e.g., a dash "-"), you can temporarily work around this issue. This will be patched in the near future. Thank you to Likhitha Mankali (NYU) for reaching out to me with this issue!

 1. If compilation does not start because of the cyclic dependency check, 
try touching all files as follows: `find ./ -type f -exec touch "{}" \;`
 1. If compilation fails because readline is missing, install 'readline' library or
compile with `make ABC_USE_NO_READLINE=1`
 1. If compilation fails because pthreads are missing, install 'pthread' library or
compile with `make ABC_USE_NO_PTHREADS=1`
    * See http://sourceware.org/pthreads-win32/ for pthreads on Windows
    * Precompiled DLLs are available from ftp://sourceware.org/pub/pthreads-win32/dll-latest
 1. If compilation fails in file "src/base/main/libSupport.c", try the following:
    * Remove "src/base/main/libSupport.c" from "src/base/main/module.make"
    * Comment out calls to `Libs_Init()` and `Libs_End()` in "src/base/main/mainInit.c"
 1. On some systems, readline requires adding '-lcurses' to Makefile.

## Citations:

If you have found the CLAP attack useful for your research, I would greatly appreciate citations to the original work. 

    @inproceedings{zuzak2022clap,
      title={A Combined Logical and Physical Attack on Logic Obfuscation},
      author={Zuzak, Michael and Liu, Yuntao and McDaniel, Isaac and Srivastava, Ankur},
      booktitle={IEEE/ACM International Conference on Computer-Aided Design (ICCAD)},
      year={2022},
      organization={ACM}
    }

## Final remarks:

Please do not hesitate to reach out to me with any questions/comments/issues regarding the repo or the work itself:

    Michael Zuzak <mjzeec@rit.edu>
    Assistant Professor of Computer Engineering
    Rochester Institute of Technology (RIT) 
    
## Acknowledgements:

A special thanks to the NSF for supporting this work under Grant 1953285.

