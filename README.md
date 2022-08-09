# CLAP Attack: A Combined Logical and Physical Attack on Logic Obfuscation

The CLAP attack is an open-source attack on logic obfuscation utilizing both logical and physical leakage to unlock an obfuscated circuit. The physical portion of the CLAP attack logically guides an electro-optical probe to extract key leakage through electro-optical frequency mapping (EOFM). The logical portion of the CLAP attack relies on the open-source SAT attack toolkit proposed by Subramanyan et al. from "Evaluating the security of logic encryption algorithms". An extensive overview of the CLAP attack on logic obfuscation can be found in the ICCAD'22 paper titled:

    A Combined Logical and Physical Attack on Logic Obfuscation
    Authors: Michael Zuzak, Yuntao Liu, Isaac McDaniel, Ankur Srivastava

This code-base contains code for the physical portion of the CLAP attack aimed at guiding an EOFM probe to extract key leakage from a die. It can be run entirely in simulation. This code is built on the Berkely ABC tool, which can be found at (https://people.eecs.berkeley.edu/~alanmi/abc/).

## Compiling:

To compile the CLAP attack, clone this repo and then type `make`.

## Compiling as C or C++

The current version of ABC can be compiled with C compiler or C++ compiler.

 * To compile as C code (default): make sure that `CC=gcc` and `ABC_NAMESPACE` is not defined.
 * To compile as C++ code without namespaces: make sure that `CC=g++` and `ABC_NAMESPACE` is not defined.
 * To compile as C++ code with namespaces: make sure that `CC=g++` and `ABC_NAMESPACE` is set to
   the name of the requested namespace. For example, add `-DABC_NAMESPACE=xxx` to OPTFLAGS.

## Running the CLAP Attack:

## Troubleshooting:

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
Assistant Professor of Computer Engineering, 
Rochester Institute of Technology (RIT) 
