#include "base/main/main.h"
#include "testfirst.h"

ABC_NAMESPACE_IMPL_START

// Declarations
static int TestFirst_CommandTestFirst(Abc_Frame_t * pAbc, int argc, int **argv);

// Function Definitions
void TestFirst_Init(Abc_Frame_t * pAbc) {
  Cmd_CommandAdd(pAbc, "Various", "firstcmd", TestFirst_CommandTestFirst, 0);
}

int TestFirst_CommandTestFirst(Abc_Frame_t * pAbc, int argc, int ** argv) {
  int fVerbose;
  int c, result;

  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "vh")) != EOF) {
    switch (c) {
    case 'v':
      fVerbose ^= 1;
      break;
    case 'h':
      goto usage;
    default:
      goto usage;
    }
  }

  // call the main function
  result = TestFirst_FirstFunctionAbc(pAbc);

  // print verbose information if the verbose mode is on
  if (fVerbose) {
    Abc_Print(1, "\nVerbose mode is on.\n");
    if (result)
      Abc_Print(1, "The command finished successfully.\n");
    else Abc_Print(1, "The command execution has failed.\n");
  }

  return 0;
 usage:
  Abc_Print(-2, "usage: firstcmd [-vh] \n");
  Abc_Print(-2, "\t         The physical portion of the CLAP attack in ABC.\n");
  Abc_Print(-2, "\t-v     : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
  Abc_Print(-2, "\t-h     : print the command usage \n");
  return 1;
}
