#ifndef CLAPATTACK_h
#define CLAPATTACK_h

// Includes
#include "base/main/main.h"
#include "base/abc/abc.h"
#include "base/main/mainInt.h"

// Parameters
ABC_NAMESPACE_HEADER_START

// Basic Types

// Function Declarations
extern int ClapAttack_ClapAttackAbc(Abc_Frame_t * pAbc, char *pKey, char *pOutFile, int alg, int keysConsideredCutoff, float keyElimCutoff, int probeResolutionSize);

#endif

ABC_NAMESPACE_HEADER_END

