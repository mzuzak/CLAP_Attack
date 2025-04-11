//
// Open-Source ABC Command for the Physical Portion of CLAP Attack
// From: ICCAD'22, A Combined Logical and Physical Attack on Logic Obfuscation
// Author: Michael Zuzak
// Email: mjzeec@rit.edu
//
// Note that the logical portion of the CLAP attack relies on the
// open-source SAT attack released by Pramod et al in Host'15
// (Evaluating the Security of Logic Locking)
//

#include <ctype.h> 
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "base/abc/abc.h"
#include "base/main/main.h"
#include "proof/fraig/fraig.h"

#define UUID_SIZE 64
#define PATH_SIZE 320

struct BSI_KeyData_t {
    int NumKeys;
    int *KeyValue;
    int Updated;
    Abc_Ntk_t *pKeyCnf;
};

struct SatMiterList {
    Abc_Obj_t **ppSatNode;
    int MatchedNodes;
    int NumKeys;
    char **KeyNames;
    Abc_Ntk_t *pMiter;
    int *pModel;
    float IdentifiableKeyBits;
    struct SatMiterList *pNext;
    struct SatMiterList *pPrev;
};

int ClapAttack_ClapAttackAbc(Abc_Frame_t *pAbc, char *pKey, char *pOutFile, int alg, int keysConsideredCutoff,
                             float keyElimCutoff, int probeResolutionSize, char *pSeqCircuitFile, int unrollTimes);
int ClapAttack_ClapAttack(Abc_Frame_t *pAbc, char *pKey, char *pOutFile, int alg, int keysConsideredCutoff,
                            float keyElimCutoff, int probeResolutionSize, Abc_Ntk_t* pUnrolled, int unrollTimes);
void ClapAttack_TraversalRecursive(Abc_Ntk_t *pNtk, Abc_Obj_t *pCurNode, struct BSI_KeyData_t *pGlobalBsiKeys,
                                   int *pOracleKey, int MaxKeysConsidered, Abc_Ntk_t **ppCurKeyCnf, int *pTotalProbes,
                                   int probeResolutionSize, Abc_Ntk_t* pUnrolled, int unrollTimes);
void ClapAttack_TraversalRecursiveHeuristic(Abc_Ntk_t *pNtk, Abc_Obj_t *pCurNode, struct BSI_KeyData_t *pGlobalBsiKeys,
                                            int MaxKeysConsidered, Abc_Ntk_t **ppCurKeyCnf,
                                            struct SatMiterList **ppSatMiterList, int *pNumProbes, int MaxProbes,
                                            int probeResolutionSize, Abc_Ntk_t* pUnrolled, int unrollTimes);
void ClapAttack_CombineMitersHeuristic(struct SatMiterList **ppSatMiterListOld, struct SatMiterList **ppSatMiterListNew,
                                       int *pMaxNodesConsidered, int MaxKeysConsidered, int MaxPiNum,
                                       int fConsiderAll, Abc_Ntk_t* pUnrolled, int unrollTimes);
void ClapAttack_InterpretDiHeuristic(Abc_Ntk_t *pNtk, Abc_Ntk_t *pNtkMiter, int *pModel, int NumKeys, int *KeyWithFreq,
                                     int *KeyNoFreq, int **ppDiFull);
void ClapAttack_EvalMultinodeProbe(struct SatMiterList *pSatMiter, Abc_Ntk_t *pNtk,
                                   struct BSI_KeyData_t *pGlobalBsiKeys, int *pOracleKey, Abc_Ntk_t **ppCurKeyCnf,
                                   int MaxKeysConsidered, int probeResolutionSize);
void ClapAttack_UpdateSatMiterList(struct SatMiterList **ppSatMiterList, Abc_Obj_t **ppNode, Abc_Ntk_t *pMiter,
                                   int NumKeys, char **KeyNames, int MaxNodesConsidered, float IdentifiableKeys,
                                   int *pModel);
void ClapAttack_FreeSatMiterList(struct SatMiterList **ppSatMiterList);
void ClapAttack_GenSatAttackConfig(Abc_Ntk_t *pNtk, struct BSI_KeyData_t *pGlobalBsiKeys, char *pOutFile);
int ClapAttack_UpdateGlobalKeyCnf(Abc_Ntk_t **ppCurKeyCnf, struct BSI_KeyData_t *pGlobalBsiKeys);
int ClapAttack_IsolateCone(Abc_Ntk_t *pNtk, Abc_Ntk_t **ppNtkCone, Abc_Obj_t *pProbe, int probeResolutionSize);
void ClapAttack_CleanCone(Abc_Ntk_t **ppNtk);
void ClapAttack_RenamePo(Abc_Ntk_t *pNtk, int PoIdx, char *NewPoName);
void ClapAttack_InitKeyCnf(Abc_Ntk_t **ppNtk, int NumKeys, int *WrongKeyValue, char **KeyNames);
void ClapAttack_UpdateKeyCnf(Abc_Ntk_t **ppNtk, int NumKeys, int *WrongKeyValue, char **KeyNames);
int ClapAttack_MakeMiterHeuristic(Abc_Ntk_t *pNtkCone, Abc_Ntk_t *pNtkPartialKey, Abc_Ntk_t **ppNtkMiter);
int ClapAttack_RunSat(Abc_Ntk_t *pNtk);
int ClapAttack_OracleInferKey(Abc_Ntk_t *pNtk, int *pSimInfo1, int *pSimInfo2);
void ClapAttack_OracleSimDi(Abc_Ntk_t *pNtk, int *pDi, int NumKeys, int *KeyWithFreq, int *KeyNoFreq,
                            int *WrongKeyValue);
void ClapAttack_UpdateKey(char *KeyNameTmp, int KeyValue, struct BSI_KeyData_t *pGlobalBsiKeys);
void ClapAttack_CleanKeyCnf(struct BSI_KeyData_t *pGlobalBsiKeys);
void ClapAttack_DelKnownKeys(Abc_Obj_t **ppNodeList, int NumKnownKeys);
int ClapAttack_SetKnownKeys(Abc_Ntk_t *pNtkCone, Abc_Obj_t *pKey, struct BSI_KeyData_t *pGlobalBsiKeys);
void ClapAttack_OracleSetConeKeys(Abc_Ntk_t *pNtk, int *pInp, int *pOracleKey);
void ClapAttack_PrintInp(Abc_Ntk_t *pNtk, int *pInp);
void ClapAttack_PrintOut(Abc_Ntk_t *pNtk, int *pOut);
int ClapAttack_WriteMiterBench(Abc_Ntk_t *pNtk, char *pFileName);
int ClapAttack_WriteMiterVerilog(Abc_Ntk_t *pNtk, char *pFileName);
int ClapAttack_RenameInput(Abc_Ntk_t *pNtk, char *PiSuffix, char *KeySuffix, char *PoSuffix, int nDigits);
int ClapAttack_RenameLogic(Abc_Ntk_t *ppNtk, char *PiSuffix, char *KeySuffix, char *PoSuffix, int KeepName);
int ClapAttack_RenameOutput(Abc_Ntk_t *pNtk, char *PoSuffix, int KeepName);
int ClapAttack_GetNumKeys(Abc_Ntk_t *pNtk);
void ClapAttack_InitKeyStore(int NumKeys, struct BSI_KeyData_t *pGlobalBsiKeys);
void ClapAttack_CopyKeyStore(struct BSI_KeyData_t *pGlobalBsiKeys, struct BSI_KeyData_t *pGlobalBsiKeysNew);
int ClapAttack_MiterPos(Abc_Ntk_t *pNtk, int fXorOr, int fXnorAnd);
int ClapAttack_MiterKeys(Abc_Ntk_t *pNtk);
int ClapAttack_PartialKeyInferenceMiter(Abc_Ntk_t *pNtk, Abc_Ntk_t **ppNtkMiter, char *CurKeyName);
int ClapAttack_CmpKeyName(char *KeyName1, char *KeyName2, int KeyLen1, int KeyLen2);
Abc_Ntk_t* ClapAttack_UnrollNetwork(Abc_Ntk_t *pNtk, int nFrames, int fInitial, int fVerbose);
int ClapAttack_VerifyCircuitCompatibility(Abc_Ntk_t* pSeqNtk, Abc_Ntk_t* pLockedNtk);
int ClapAttack_SaveNetworkToFile(Abc_Ntk_t *pNtk, const char *filename);
Abc_Ntk_t* ClapAttack_LoadNetworkFromFile(const char *filename);
void ClapAttack_MiterReachability(Abc_Ntk_t* pUnrolled, Abc_Ntk_t* pNtkMiter, int unrollTimes);
int ClapAttack_ExtractCycleNumber(const char* name);
int ClapAttack_SubstringExists(const char *mainStr, const char *subStr);
void ClapAttack_StrToLower(char *str);

ABC_NAMESPACE_IMPL_START

/* Global Var for Probe Point Counter
int nValidProbePoint;
int nAvgKeyCount;
/* End Global Var for Probe Point Counter */

// CLAP Attack wrapper function -- entry point to CLAP
int ClapAttack_ClapAttackAbc(Abc_Frame_t *pAbc, char *pKey, char *pOutFile,
                             int alg, int keysConsideredCutoff,
                             float keyElimCutoff, int probeResolutionSize,
                             char *pSeqCircuitFile, int unrollTimes) {
    Abc_Ntk_t *pNtk, *pSeqNtk, *pUnrolled = NULL;

    int result;

    // Get the network that is read into ABC
    pNtk = Abc_FrameReadNtk(pAbc);

    if (pNtk == NULL) {
        Abc_Print(-1, "ClapAttack_ClapAttackAbc: Getting the target network has failed.\n");
        return 0;
    }

    if (pSeqCircuitFile != NULL) {
        // Load the sequential circuit
        printf("Loading prior stage sequential file: %s\n", pSeqCircuitFile);
        pSeqNtk = ClapAttack_LoadNetworkFromFile(pSeqCircuitFile);
        if (pSeqNtk == NULL) {
            printf("Failed to load the sequential circuit file: %s\n", pSeqCircuitFile);
            return -1; 
        }

        printf("Verifying circuit compatibility...\n");
        fflush(stdout);
        // Verify that the sequential and logic-locked circuits are compatible
        if (!ClapAttack_VerifyCircuitCompatibility(pSeqNtk, pNtk)) {
            printf("Incompatibility between sequential and logic-locked circuits detected.\n");
            return -1;
        }

        printf("Unrolling the sequential network...\n");
        // Unroll the sequential network
        if ((pUnrolled = ClapAttack_UnrollNetwork(pSeqNtk, unrollTimes, 1, 1)) == NULL) {
            printf("Failed to unroll the sequential network.\n");
            return -1; 
        }

        // For debugging
        ClapAttack_SaveNetworkToFile(pUnrolled, "unrolled.bench");
        ClapAttack_SaveNetworkToFile(pSeqNtk, "seq.bench");
    }


    // Call the main function
    result = ClapAttack_ClapAttack(
        pAbc, pKey, pOutFile, alg, keysConsideredCutoff, keyElimCutoff,
        probeResolutionSize, pUnrolled, unrollTimes);

    return result;
}


void generate_simple_uuid(char *uuid_str, size_t size) {
    unsigned long rand1, rand2;
    srand(time(NULL) ^ getpid() ^ pthread_self());
    rand1 = (unsigned long)rand();
    rand2 = (unsigned long)rand();
    snprintf(uuid_str, size, "%lx-%lx", rand1, rand2);
}
int count_partial_key_leakage(const char *verilog_file_path) {
    regex_t regex;
    regmatch_t pmatch[1];
    int key_count = 0;
    char buffer[1024];
    char **found_keys = NULL;
    size_t found_keys_size = 0;

    // Compile the regex to match "keyinput[0-9]+"
    if (regcomp(&regex, "keyinput[0-9]+", REG_EXTENDED)) {
        fprintf(stderr, "Could not compile regex\n");
        return 0;
    }

    // Open the Verilog file
    FILE *file = fopen(verilog_file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open file %s\n", verilog_file_path);
        return 0;
    }

    // Read the file line by line
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        const char *p = buffer;
        while (regexec(&regex, p, 1, pmatch, 0) == 0) {
            // Extract the matched string
            int length = pmatch[0].rm_eo - pmatch[0].rm_so;
            char *matched_key = strndup(p + pmatch[0].rm_so, length);

            // Check if the key is already in the list of found keys
            int is_unique = 1;
            for (size_t i = 0; i < found_keys_size; i++) {
                if (strcmp(found_keys[i], matched_key) == 0) {
                    is_unique = 0;
                    break;
                }
            }

            // If the key is unique, add it to the list
            if (is_unique) {
                found_keys_size++;
                found_keys = realloc(found_keys, found_keys_size * sizeof(char *));
                found_keys[found_keys_size - 1] = matched_key;
                key_count++;
            } else {
                free(matched_key);
            }

            p += pmatch[0].rm_eo;  // Move pointer to the end of the match
        }
    }

    // Clean up
    regfree(&regex);
    fclose(file);

    // Free the list of found keys
    for (size_t i = 0; i < found_keys_size; i++) {
        free(found_keys[i]);
    }
    free(found_keys);

    return key_count;
}


int ClapAttack_ClapAttack(Abc_Frame_t *pAbc, char *pKey, char *pOutFile,
                          int alg, int keysConsideredCutoff,
                          float keyElimCutoff, int probeResolutionSize,
                          Abc_Ntk_t* pUnrolled, int unrollTimes) {
    int i, j = 0, NumKeys, KeyIndex, MaxKeysConsidered, KeysFound, MaxNodesConsidered;
    Abc_Ntk_t *pNtk, *pCurKeyCnf;
    Abc_Obj_t *pPi, *pNode;
    struct BSI_KeyData_t GlobalBsiKeys;
    int RunHeuristic = alg;
    int TotalProbes = 0;
    int fConsiderAll = 0;
    int NumProbes, MaxProbes;
    struct SatMiterList *pSatMiterList = NULL;
    struct SatMiterList *pSatMiterListNew = NULL;
    struct SatMiterList *pSatMiterListCur = NULL;

    /*  Global Var for Probe Point Counter
    nValidProbePoint = 0;
    nAvgKeyCount = 0;
    /* End  Global Var for Probe Point Counter */

    //
    // Convert Key to proper format string
    //

    // get length of string str
    int str_length = 0;
    for (i = 0; pKey[i] != '\0'; i++) {
        str_length++;
    }

    // create an array with size as string
    // length and initialize with 0
    int *pOracleKey = (int *)malloc(sizeof(int) * str_length);

    // Traverse the string
    for (i = 0; pKey[i] != '\0'; i++) {
        // subtract str[i] by 48 to convert it to int
        // (int)(str[i])
        pOracleKey[j] = (pKey[i] - 48);
        j++;
    }

    // Print out all configurable program state for user.
    printf("\nCLAP ATTACK CONFIGURATION:\n");
    printf("Physical attack algorithm (0: Fixed EOFM Probe, 1: Multinode Probe): %d\n", alg);
    printf("Keys Considered Cutoff (i.e. max fanin keys for node): %d\n", keysConsideredCutoff);
    printf(
        "Keys Eliminated Cutoff (minumum portion of keyspace that must be eliminated to probe -- MULTI-NODE PROBING "
        "ONLY!): %f\n",
        keyElimCutoff);

    // Print the oracle key value.
    printf("Correct Oracle Key: {");
    for (i = 0; i < str_length; i++) {
        printf("%d, ", pOracleKey[i]);
    }
    printf("}\n");

    printf("Output File for Logical Attack: %s\n", pOutFile);
    printf("\nLaunching CLAP Attack...\n\n");

    // Parameter for the maximum keys inputs that can be fanned into a node before ignoring it.
    // DEBUG: print information about the network
    // Abc_Print(1, "The network with name %s has:\n", Abc_NtkName(pNtk));
    // Abc_Print(1, "\t- %d primary inputs;\n", Abc_NtkPiNum(pNtk));
    // Abc_Print(1, "\t- %d primary outputs;\n", Abc_NtkPoNum(pNtk));
    // Abc_Print(1, "\t- %d AND gates;\n", Abc_NtkNodeNum(pNtk));

    // Get the network that is read into ABC ... THIS IS THE BACKUP COPY FOR NOW.
    pNtk = Abc_FrameReadNtk(pAbc);

    // Count keys
    NumKeys = ClapAttack_GetNumKeys(pNtk);
    ClapAttack_InitKeyStore(NumKeys, &GlobalBsiKeys);
    MaxProbes = 3;

    // Set visited for each node to 0
    Abc_NtkForEachNode(pNtk, pNode, i) pNode->fMarkC = 0;

    // How many nodes are we currently considering for the multi-node probe? This will iteratively increase
    // as we discover new nodes, so this should be left. It is not a parameter.
    MaxNodesConsidered = 2;

    // Goal: Iterate through each PI. Identify list of keys.
    // NOTE -- THIS RUNS ALGORITHM 2, MULTI-NODE PROBING.
    if (RunHeuristic) {
        // Infinite loop -- break when you run out of probe-able nodes
        while (1) {
            // Iteratively increase the number of keys we are willing to consider in the fan-in for a probe-able node
            // This ensures that only the best node at a given time will be considered for probing
            for (MaxKeysConsidered = 1; MaxKeysConsidered < keysConsideredCutoff; MaxKeysConsidered++) {
                // How many keys are we currently considering?
                printf("Set Number of Keys considered to: %d\n\n", MaxKeysConsidered);
                GlobalBsiKeys.Updated = 1;
                fConsiderAll = 1;

                // did we find any key leakage?
                while (GlobalBsiKeys.Updated) {
                    // Set the update var to 0. IF we change our keystore, set it back to 1 and re-loop over the tree.
                    GlobalBsiKeys.Updated = 0;

                    Abc_NtkForEachPi(pNtk, pPi, i) {
                        // Are we looking at a key input? And do we know it?-- If so, begin fanout
                        if (strstr(Abc_ObjName(pPi), "key")) {
                            // Get key index
                            KeyIndex = (int)strtol(&((Abc_ObjName(pPi))[8]), (char **)NULL, 10);

                            // Do we know this key index?
                            if (GlobalBsiKeys.KeyValue[KeyIndex] < 0) {
                                pCurKeyCnf = NULL;
                                NumProbes = 0;

                                // Start traversal of tree -- any new key inference will set the updated variable and
                                // restart new traversal
                                ClapAttack_TraversalRecursiveHeuristic(pNtk, pPi, &GlobalBsiKeys, MaxKeysConsidered,
                                                                       &GlobalBsiKeys.pKeyCnf, &pSatMiterList,
                                                                       &NumProbes, MaxProbes, probeResolutionSize,
                                                                       pUnrolled, unrollTimes);
                            }
                        }
                    }

                    // Now we must generate our best multi-node probes. Keep merging probe points until no sensitizing
                    // inputs can be found...
                    while (pSatMiterList) {
                        // Find the current best sensitizing inputs based on the sensitizable nodes available
                        ClapAttack_CombineMitersHeuristic(&pSatMiterList, &pSatMiterListNew, &MaxNodesConsidered,
                                                          MaxKeysConsidered, Abc_NtkPiNum(pNtk), fConsiderAll, pUnrolled, unrollTimes);

                        fConsiderAll = 0;

                        // Did we update the nodes we are considering probing
                        if (pSatMiterListNew) {
                            // Free old miter list in favor of new one.
                            ClapAttack_FreeSatMiterList(&pSatMiterList);
                            pSatMiterList = pSatMiterListNew;
                            pSatMiterListNew = NULL;
                            // Now consider combined probes...
                            MaxNodesConsidered++;
                        } else {
                            // Debug print -- What is the node we are going to probe? -- Print then break out and probe
                            // it!
                            pSatMiterListCur = pSatMiterList;
                            while (pSatMiterListCur) {
                                printf("Sat Miter Node Pairing: ");
                                for (j = 0; j < pSatMiterListCur->MatchedNodes; j++)
                                    printf("%s ", Abc_ObjName(pSatMiterListCur->ppSatNode[j]));
                                printf("\tIdentifiable Keys from Miter: %f\n", pSatMiterListCur->IdentifiableKeyBits);
                                pSatMiterListCur = pSatMiterListCur->pNext;
                            }
                            printf("\n");

                            break;
                        }

                        // Debug print
                        pSatMiterListCur = pSatMiterList;
                        while (pSatMiterListCur) {
                            printf("Sat Miter Node Pairing: ");
                            for (j = 0; j < pSatMiterListCur->MatchedNodes; j++)
                                printf("%s ", Abc_ObjName(pSatMiterListCur->ppSatNode[j]));
                            printf("\tIdentifiable Keys from Miter: %f\n", pSatMiterListCur->IdentifiableKeyBits);
                            pSatMiterListCur = pSatMiterListCur->pNext;
                        }
                        printf("\n");
                    }
                }
            }

            // If there were satisfying inputs identified, simulate the EOFM probe of these nodes with our inputs.
            if (pSatMiterList) {
                // Note that this is gated by eliminating a sufficient portion of the key space to make the probe
                // worthwhile
                if (pSatMiterList->IdentifiableKeyBits >= (keyElimCutoff)) {
                    // Simulate and infer from the EOFM probe.
                    TotalProbes++;
                    ClapAttack_EvalMultinodeProbe(pSatMiterList, pNtk, &GlobalBsiKeys, pOracleKey,
                                                  &GlobalBsiKeys.pKeyCnf, MaxKeysConsidered, probeResolutionSize);
                    ClapAttack_FreeSatMiterList(&pSatMiterList);
                    MaxNodesConsidered = 2;
                    pSatMiterList = NULL;

                    // Set visited for each node to 0
                    Abc_NtkForEachNode(pNtk, pNode, j) pNode->fMarkC = 0;

                } else {
                    break;
                }
            } else {
                // There is no more physical data to be had...
                break;
            }
        }
    }

    // NOTE -- THIS RUNS ALGORITHM 1, FIXED EOFM PROBE.
    else {
        // Starting from nodes with a single key input in their fan-in -- look for sensitizing inputs/nodes to probe.
        // Iteratively consider more key inputs until cutoff is reached.
        for (MaxKeysConsidered = 1; MaxKeysConsidered < keysConsideredCutoff; MaxKeysConsidered++) {
            /* Probe Point Counter
            MaxKeysConsidered = KeysConsideredCutoff-1;
            /* End Probe Point Counter */

            // INFO print -- How many keys are we considering?
            printf("Set Number of Keys considered to: %d\n\n", MaxKeysConsidered);
            GlobalBsiKeys.Updated = 1;

            // did we get any new key leakage? If so, see if this allows further leakage to be extracted now.
            while (GlobalBsiKeys.Updated) {
                // Set the update var to 0. IF we change our keystore, set it back to 1 and re-loop over the tree.
                GlobalBsiKeys.Updated = 0;
                MaxNodesConsidered = 1;

                // Iterate through fan-out of each key input looking for possible key leakage.
                Abc_NtkForEachPi(pNtk, pPi, i) {

                    // Are we looking at a key input? And do we know it?-- If so, begin fanout
                    if (strstr(Abc_ObjName(pPi), "key")) {
                        // Get key index
                        KeyIndex = (int)strtol(&((Abc_ObjName(pPi))[8]), (char **)NULL, 10);

                        // Do we know this key index?
                        if (GlobalBsiKeys.KeyValue[KeyIndex] < 0) {
                            // printf("Currently working on fanout for key index: %d\n", KeyIndex);
                            pCurKeyCnf = NULL;

                            // Recursively traverse from the current key inputs fan-out
                            ClapAttack_TraversalRecursive(pNtk, pPi, &GlobalBsiKeys, pOracleKey, MaxKeysConsidered,
                                                          &pCurKeyCnf, &TotalProbes, probeResolutionSize, 
                                                          pUnrolled, unrollTimes);
                        }
                    }
                }
            }

            /* Probe Point Counter
            printf("There are %d Probe Points, Averaging %f key inputs \n\n", nValidProbePoint,
            ((float)nAvgKeyCount)/((float)nValidProbePoint));
            /* End Probe Point Counter */
        }
    }


    char verilog_file_path[PATH_SIZE];
    char uuid_str[UUID_SIZE];

    // Generate a simple UUID for the unique filename
    generate_simple_uuid(uuid_str, sizeof(uuid_str));
    snprintf(verilog_file_path, sizeof(verilog_file_path), "./tmp/%s.v", uuid_str);

    // Output the Verilog to the unique file
    // ClapAttack_WriteMiterVerilog(GlobalBsiKeys.pKeyCnf, "./global_keystore.v");
    // ClapAttack_WriteMiterVerilog(GlobalBsiKeys.pKeyCnf, verilog_file_path);

    // Calculate partial key leakage from the Verilog file
    int partial_key_leakage_count = count_partial_key_leakage(verilog_file_path);
    printf("Verilog file: %s\n", verilog_file_path);
    printf("Partial key leakage: %d\n", partial_key_leakage_count);

    // Clean up the temporary Verilog file
    // remove(verilog_file_path);

    ClapAttack_GenSatAttackConfig(pNtk, &GlobalBsiKeys, pOutFile);

    // Print the final key value inferred from the attack.
    printf("KeyStore is now: {");
    KeysFound = GlobalBsiKeys.NumKeys;
    for (i = 0; i < GlobalBsiKeys.NumKeys; i++) {
        if (GlobalBsiKeys.KeyValue[i] == -1) KeysFound--;
        printf("%d, ", GlobalBsiKeys.KeyValue[i]);
    }
    printf("}\n\n");

    printf("We found %d of %d total keys using %d probes\n", KeysFound, GlobalBsiKeys.NumKeys, TotalProbes);

    // We are done -- cleanup and exit
    free(GlobalBsiKeys.KeyValue);

    // Reset markc so we can terminate without seg-fault
    Abc_NtkForEachNode(pNtk, pNode, i) pNode->fMarkC = 0;

    return 1;
}

// Traverse the network looking for probe-able nodes. Note, this is used for the fixed EOFM probe case -- Multinode
// probing is explored in the heuristic version of this function below.
void ClapAttack_TraversalRecursive(Abc_Ntk_t *pNtk, Abc_Obj_t *pCurNode, struct BSI_KeyData_t *pGlobalBsiKeys,
                                   int *pOracleKey, int MaxKeysConsidered, Abc_Ntk_t **ppCurKeyCnf, int *pTotalProbes,
                                   int probeResolutionSize, Abc_Ntk_t* pUnrolled, int unrollTimes) {
    int *pFullDi;
    int i, j, k, m, SatStatus, MiterStatus, reachabilityStatus, NumKeys, NumKnownKeys, *KeyWithFreq, *KeyNoFreq, *WrongKeyValue,
        KeyValue = 0, PartialKeySatStatus, fCurKeyCnfAlloc;
    Abc_Ntk_t *pNtkCone, *pNtkMiter, *pInferPartialKeyMiter, *ntkTmp;
    Abc_Obj_t *pNode, *pPi, *pKey, **ppNodeFreeList;
    char **KeyNameTmp;

    // Initialize partial key info to NULL
    fCurKeyCnfAlloc = 0;
    NumKeys = 0;

    // Goal: For each PI that is a key, follow fanout until it intersects with unknown key.
    Abc_ObjForEachFanout(pCurNode, pNode, i) {
        // Have we visited this node before?
        if (!pNode->fMarkC) {
            // Initialzie free list to the number of keys present...
            ppNodeFreeList = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * pGlobalBsiKeys->NumKeys);

            KeyNameTmp = (char **)malloc(sizeof(char *) * MaxKeysConsidered);
            for (k = 0; k < MaxKeysConsidered; k++) {
                KeyNameTmp[k] = (char *)malloc(sizeof(char) * 100);
            }

            // Check to make sure were not lookign at a PO. If so,
            // don't bother pursuing the fanout.
            if (Abc_ObjIsCo(pNode)) {
                for (k = 0; k < MaxKeysConsidered; k++) free(KeyNameTmp[k]);
                free(KeyNameTmp);
                free(ppNodeFreeList);
                continue;
            }

            // Check supports ... if only one is an unknown key,
            // process it and continue fanout
            ClapAttack_IsolateCone(pNtk, &pNtkCone, pNode, probeResolutionSize);

            // Loop over the miter generation phase until SAT fails
            do {
                NumKeys = 0;
                NumKnownKeys = 0;

                SatStatus = 1;
                MiterStatus = 1;
                reachabilityStatus = 0;

                Abc_NtkForEachPi(pNtkCone, pPi, j) {
                    // Are we looking at a key input?
                    if (strstr(Abc_ObjName(pPi), "key")) {
                        if (!ClapAttack_SetKnownKeys(pNtkCone, pPi, pGlobalBsiKeys)) {
                            if (NumKeys < MaxKeysConsidered) strcpy(KeyNameTmp[NumKeys], Abc_ObjName(pPi));
                            NumKeys++;
                            // printf("Keyname: %s, KeyNum: %d\n",  Abc_ObjName(pPi), NumKeys);
                        } else {
                            ppNodeFreeList[NumKnownKeys] = pPi;
                            NumKnownKeys++;
                        }
                    }
                }

                // Delete known keys from cone.
                if (NumKnownKeys) ClapAttack_DelKnownKeys(ppNodeFreeList, NumKnownKeys);

                // We're processing the node. Mark it so we don't do this again.
                // If it fails here, there is no information we can ever gain
                if ((NumKeys <= MaxKeysConsidered)) pNode->fMarkC = 1;

                if ((NumKeys <= MaxKeysConsidered) && NumKeys) {
                    printf("Evaluating node %s\n", Abc_ObjName(pNode));
                    printf("The number of keys is: %d\n", NumKeys);
                    printf("Generating miter circuit.\n");

                    // Generate miter to determine if key info leaks
                    MiterStatus = ClapAttack_MakeMiterHeuristic(pNtkCone, *ppCurKeyCnf, &pNtkMiter);

                    // Did we successfully construct the miter?
                    if (!MiterStatus) {
                        // Run SAT on the miter to discover sensitizing inputs.
                        printf("Running SAT on Miter circuit.\n");
                        SatStatus = ClapAttack_RunSat(pNtkMiter);

                        // Are there sensitizing inputs for this miter?
                        if (!SatStatus) {
                            /* Probe Point Counter
                            nValidProbePoint++;
                            nAvgKeyCount = nAvgKeyCount + NumKeys;
                            printf("We found a valid probe point: %s with %d keys\n", Abc_ObjName(pNode), NumKeys);
                            /* End Probe Point Counter */

                            if(pUnrolled){
                                Abc_Ntk_t* pUnrolledMiter = Abc_NtkDup(pUnrolled);
                                if (!pUnrolledMiter) {
                                    printf("Failed to duplicate the network for reachability analysis.\n");
                                    return;
                                }

                                ClapAttack_MiterReachability(pUnrolledMiter, pNtkMiter, unrollTimes);

                                // test SAT
                                reachabilityStatus = ClapAttack_RunSat(pUnrolledMiter);

                                // Free the unrolled miter
                                Abc_NtkDelete(pUnrolledMiter);
                            }



                            if (!pUnrolled || !reachabilityStatus) {
                                /* Begin Probe Point Counter Comment */
                                // Debug printing
                                printf("SAT successfully located a key for keys:  ");
                                for (k = 0; k < NumKeys; k++) printf("%s ", KeyNameTmp[k]);
                                printf("\n\n");

                                // Malloc key values from SAT to infer from
                                KeyWithFreq = (int *)malloc(sizeof(int) * NumKeys);
                                KeyNoFreq = (int *)malloc(sizeof(int) * NumKeys);

                                ClapAttack_InterpretDiHeuristic(pNtkCone, pNtkMiter, pNtkMiter->pModel, NumKeys,
                                                                KeyWithFreq, KeyNoFreq, &pFullDi);

                                // Oracle testing. Comment out with real probe.
                                ClapAttack_OracleSetConeKeys(pNtkCone, pFullDi, pOracleKey);

                                // Increase probe count by 1
                                (*pTotalProbes)++;

                                // Determine which keys we can eliminate based on simulated probe-ing of keyed oracle.
                                // Comment out for real-probeing case.
                                WrongKeyValue = (int *)malloc(sizeof(int) * NumKeys);
                                ClapAttack_OracleSimDi(pNtkCone, pFullDi, NumKeys, KeyWithFreq, KeyNoFreq, WrongKeyValue);

                                // Debug print
                                printf("Wrong KeyValue: ");
                                for (k = 0; k < NumKeys; k++) printf(" %d", WrongKeyValue[k]);
                                printf("\n\n");

                                // Initialize partial key logic if it doesnt exist
                                // Otherwise update it...
                                if (NumKeys > 1) {
                                    if (!(*ppCurKeyCnf)) {
                                        ClapAttack_InitKeyCnf(ppCurKeyCnf, NumKeys, WrongKeyValue, KeyNameTmp);
                                        fCurKeyCnfAlloc = 1;
                                    } else {
                                        ClapAttack_UpdateKeyCnf(ppCurKeyCnf, NumKeys, WrongKeyValue, KeyNameTmp);
                                    }
                                }

                                // Short term hack to handle case wehre key is of length 1
                                if (NumKeys == 1) {
                                    KeyValue = (WrongKeyValue[0] + 1) % 2;
                                    ClapAttack_UpdateKey(KeyNameTmp[0], KeyValue, pGlobalBsiKeys);
                                } else {
                                    // Evaluate partial key logic for complete key info
                                    for (k = 0; k < NumKeys; k++) {
                                        // This requires updating the partial key info miter and attempting to infer key
                                        // values
                                        ClapAttack_PartialKeyInferenceMiter(*ppCurKeyCnf, &pInferPartialKeyMiter,
                                                                            KeyNameTmp[k]);
                                        PartialKeySatStatus = ClapAttack_RunSat(pInferPartialKeyMiter);
                                        Abc_NtkDelete(pInferPartialKeyMiter);

                                        // Initially, this will always fall through. The second time, this will always break
                                        // unless a new key value is discovered. This is the case because it is possible
                                        // discovering one key value will allow others to be inferred, so we must check
                                        // again.
                                        if (PartialKeySatStatus && (PartialKeySatStatus != -1)) {
                                            // Again, are we SAT?
                                            if (!ClapAttack_RunSat(*ppCurKeyCnf)) {
                                                // Determine which key value we determined and update it
                                                Abc_NtkForEachPi(*ppCurKeyCnf, pKey, m) {
                                                    if (!strcmp(Abc_ObjName(pKey), KeyNameTmp[k])) {
                                                        KeyValue = (*ppCurKeyCnf)->pModel[m];

                                                        // Either remove the key or set it to constant 1.
                                                        ClapAttack_UpdateKey(KeyNameTmp[k], KeyValue, pGlobalBsiKeys);
                                                        printf("We determined that key %s is %d\n", Abc_ObjName(pKey),
                                                            KeyValue);
                                                        break;
                                                    }
                                                }

                                                // Optimize out key from partial CNF
                                                ntkTmp = Abc_NtkToLogic(*ppCurKeyCnf);
                                                Abc_NtkDelete(*ppCurKeyCnf);
                                                *ppCurKeyCnf = ntkTmp;
                                                Abc_NtkForEachPi(*ppCurKeyCnf, pKey, m) {
                                                    if (!strcmp(Abc_ObjName(pKey), KeyNameTmp[k])) {
                                                        Abc_ObjReplaceByConstant(pKey, KeyValue);
                                                        Abc_NtkDeleteObj(pKey);
                                                        break;
                                                    }
                                                }

                                                // Check if we optimized out all the PIs but one.
                                                // IF so, free the partial info
                                                ntkTmp = Abc_NtkStrash(*ppCurKeyCnf, 0, 1, 0);
                                                Abc_NtkDelete(*ppCurKeyCnf);
                                                *ppCurKeyCnf = ntkTmp;

                                                // Cleanup
                                                if ((Abc_NtkPiNum(*ppCurKeyCnf) < 2) && fCurKeyCnfAlloc) {
                                                    Abc_NtkDelete(*ppCurKeyCnf);
                                                    *ppCurKeyCnf = NULL;
                                                    break;
                                                }
                                            } else {
                                                // This should never occur... somehow the correct key was eliminated
                                                printf("The key is somehow UNSAT... Exiting.\n\n");
                                                exit(0);
                                            }
                                        }
                                    }
                                }

                                // Cleanup memory allocations
                                free(WrongKeyValue);
                                free(KeyWithFreq);
                                free(KeyNoFreq);

                                // Print off updated keystore
                                printf("KeyStore is now: {");
                                for (k = 0; k < pGlobalBsiKeys->NumKeys; k++) printf("%d, ", pGlobalBsiKeys->KeyValue[k]);
                                printf("}\n\n");

                                /* Probe Point Counter
                                SatStatus = 1;
                                /* End Probe Point Counter */
                            
                            }else{
                                printf("Non-reachable input, skipping\n");
                            }

                        }
                    } else {
                        printf("Mitering failed. Proceed.\n");
                    }

                    // Delete miter and move on.
                    Abc_NtkDelete(pNtkMiter);
                }
            } while (!SatStatus && !MiterStatus && !reachabilityStatus);

            // Cleanup and delete network for the isolated cone we
            // were traversing
            Abc_NtkDelete(pNtkCone);

            for (k = 0; k < MaxKeysConsidered; k++) free(KeyNameTmp[k]);
            free(KeyNameTmp);
            free(ppNodeFreeList);
        }

        // If we have fewer than the maximum allowable keys, continue fan-out traverse. (Recurse)
        if (NumKeys <= MaxKeysConsidered) {
            ClapAttack_TraversalRecursive(pNtk, pNode, pGlobalBsiKeys, pOracleKey, MaxKeysConsidered, ppCurKeyCnf,
                                          pTotalProbes, probeResolutionSize, pUnrolled, unrollTimes);
        }
    }

    // Save off any key cnf info we found to the global key CNF store
    if ((*ppCurKeyCnf) && fCurKeyCnfAlloc) {
        if (ClapAttack_UpdateGlobalKeyCnf(ppCurKeyCnf, pGlobalBsiKeys)) printf("Global CNF Update Failed. \n");
        Abc_NtkDelete(*ppCurKeyCnf);
    }
}

// In order to run a multinode probe, we must merge the miters for EVERY node we plan to extract leakage from in order
// to maximize the the leakage generated by the esnsitizing inputs for the circuit. Note, this only matters when
// multiple nodes are probed simultaneously.
void ClapAttack_CombineMitersHeuristic(struct SatMiterList **ppSatMiterListOld, 
                                       struct SatMiterList **ppSatMiterListNew,
                                       int *pMaxNodesConsidered, 
                                       int MaxKeysConsidered, 
                                       int MaxPiNum,
                                       int fConsiderAll, 
                                       Abc_Ntk_t* pUnrolled, 
                                       int unrollTimes) {
    int i, j, k, m, n, SatStatus, NumKeysOld, NumKeysNew, CurProbeCount, 
        CurKeyIdx, fNodePresent, CurKeyIdxNew, NumKeysNew_rev, MiterListLen, 
        reachabilityStatus;
    float IdentifiableKeys, IdentifiableKeysOld, IdentifiableKeysMax;
    Abc_Ntk_t *pNtkMiter, *pNtkMiterBase, *pUnrolledMiter = NULL;
    Abc_Obj_t **ppNode = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * 400);
    struct SatMiterList *pMiterListCurBase, *pMiterListCurForward;
    char **CurKeysInferred;
    char **CurKeysInferredMiter;

    // Allocate data structures to store the key names we are inferring information about.
    CurKeysInferred = (char **)malloc(sizeof(char *) * (MaxPiNum));
    CurKeysInferredMiter = (char **)malloc(sizeof(char *) * (MaxPiNum));
    for (i = 0; i < MaxPiNum; i++) {
        CurKeysInferred[i] = (char *)malloc(sizeof(char) * 100);
        CurKeysInferredMiter[i] = (char *)malloc(sizeof(char) * 100);
    }

    // Set miter list pointers to test combined miters for heuristic
    pMiterListCurBase = *ppSatMiterListOld;
    MiterListLen = 0;

    // Init identifiable keys
    IdentifiableKeysMax = pMiterListCurBase->IdentifiableKeyBits;

    // Combine pairwise compatible miters
    while (pMiterListCurBase && (MiterListLen < 100)) {
        // Set base miter without combination to combine into throughout inner loop
        pNtkMiterBase = Abc_NtkDup(pMiterListCurBase->pMiter);

        // Initialize ppNode list of matched nodes in miter
        for (i = 0; i < pMiterListCurBase->MatchedNodes; i++) {
            ppNode[i] = pMiterListCurBase->ppSatNode[i];
        }

        // Generate list of solved keys for this base-node
        for (CurKeyIdxNew = 0; CurKeyIdxNew < pMiterListCurBase->NumKeys; CurKeyIdxNew++) {
            // Are we looking at a key input?
            strcpy(CurKeysInferredMiter[CurKeyIdxNew], pMiterListCurBase->KeyNames[CurKeyIdxNew]);
        }

        // Set forward pointer
        pMiterListCurForward = pMiterListCurBase->pNext;
        CurProbeCount = 0;

        while (pMiterListCurForward) {
            // Reset SAT status flag and Identifiable key count
            SatStatus = 1;
            IdentifiableKeys = 0;
            i = pMiterListCurBase->MatchedNodes;
            k = 0;
            NumKeysNew_rev = CurKeyIdxNew;

            // Add remaining nodes to ppNode list
            for (j = 0; j < pMiterListCurForward->MatchedNodes; j++) {
                if (j == pMiterListCurForward->MatchedNodes - 1) {
                    // If nodes don't match, we cannot SAT.
                    if (pMiterListCurForward->ppSatNode[j] != ppNode[k]) {
                        // Update inferred keys
                        for (m = 0; m < pMiterListCurForward->NumKeys; m++) {
                            fNodePresent = 0;
                            for (n = 0; n < CurKeyIdxNew; n++) {
                                if (!strcmp(CurKeysInferredMiter[n], pMiterListCurForward->KeyNames[m])) {
                                    fNodePresent = 1;
                                }
                            }

                            if (!fNodePresent) {
                                strcpy(CurKeysInferredMiter[NumKeysNew_rev], pMiterListCurForward->KeyNames[m]);
                                NumKeysNew_rev++;
                            }
                        }

                        ppNode[i] = pMiterListCurForward->ppSatNode[j];
                        i++;
                    }

                    break;
                }

                if (pMiterListCurForward->ppSatNode[j] == ppNode[k]) {
                    k++;
                } else {
                    break;
                }
            }

            // Run SAT to determine leakage
            if (!SatStatus) {
                pNtkMiter = Abc_NtkDup(pMiterListCurForward->pMiter);
                if (!ClapAttack_RunSat(pNtkMiter)) {
                    // SAT passes, we proceed with reachability check
                    if (pUnrolled) {
                        pUnrolledMiter = Abc_NtkDup(pUnrolled);
                        ClapAttack_MiterReachability(pUnrolledMiter, pNtkMiter, unrollTimes);
                        reachabilityStatus = ClapAttack_RunSat(pUnrolledMiter);
                        Abc_NtkDelete(pUnrolledMiter);
                        if (reachabilityStatus) {
                            SatStatus = 0;
                        }
                    }

                    // Perform key updates if SAT and reachable
                    if (!SatStatus) {
                        // Handle combination logic here
                        // (remainder unchanged...)
                    }
                }

                Abc_NtkDelete(pNtkMiter);
            }

            pMiterListCurForward = pMiterListCurForward->pNext;
        }

        pMiterListCurBase = pMiterListCurBase->pNext;
        Abc_NtkDelete(pNtkMiterBase);
    }

    // Cleanup
    for (i = 0; i < MaxPiNum; i++) {
        free(CurKeysInferred[i]);
        free(CurKeysInferredMiter[i]);
    }
    free(CurKeysInferred);
    free(CurKeysInferredMiter);
    free(ppNode);
}

// Recurseively traverse nodes in the network to identify probe-able locations.
void ClapAttack_TraversalRecursiveHeuristic(Abc_Ntk_t *pNtk, Abc_Obj_t *pCurNode, struct BSI_KeyData_t *pGlobalBsiKeys,
                                            int MaxKeysConsidered, Abc_Ntk_t **ppCurKeyCnf,
                                            struct SatMiterList **ppSatMiterList, int *pNumProbes, int MaxProbes,
                                            int probeResolutionSize, Abc_Ntk_t* pUnrolled, int unrollTimes) {
    int i, j, k, SatStatus, MiterStatus, NumKeys, NumKnownKeys, fCurKeyCnfAlloc, reachabilityStatus;
    Abc_Ntk_t *pNtkCone, *pNtkMiter;
    Abc_Obj_t *pNode, *pPi, **ppNodeFreeList;
    char **KeyNameTmp;

    // Initialize partial key info to NULL
    fCurKeyCnfAlloc = 0;
    NumKeys = 0;
        
    // Goal: For each PI that is a key, follow fanout until it intersects with unknown key.
    Abc_ObjForEachFanout(pCurNode, pNode, i) {
        // Have we visited this node before?
        if (!pNode->fMarkC) {
            // Initialzie free list to the number of keys present...
            ppNodeFreeList = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * pGlobalBsiKeys->NumKeys);

            KeyNameTmp = (char **)malloc(sizeof(char *) * MaxKeysConsidered);
            for (k = 0; k < MaxKeysConsidered; k++) {
                KeyNameTmp[k] = (char *)malloc(sizeof(char) * 100);
            }

            // Check to make sure were not lookign at a PO. If so,
            // don't bother pursuing the fanout.
            if (Abc_ObjIsCo(pNode)) {
                /// printf("This node is a primary output. Done with fanout.\n");
                for (k = 0; k < MaxKeysConsidered; k++) free(KeyNameTmp[k]);
                free(KeyNameTmp);
                free(ppNodeFreeList);
                continue;
            }

            // Check supports ... if only one is an unknown key,
            // process it and continue fanout
            ClapAttack_IsolateCone(pNtk, &pNtkCone, pNode, probeResolutionSize);

            // Loop over the miter generation phase until SAT fails
            NumKeys = 0;
            NumKnownKeys = 0;
            SatStatus = 1;
            MiterStatus = 1;
            reachabilityStatus = 0;

            // Count the key inputs in the cone. If there are too many, ignore the node and halt traversal.
            Abc_NtkForEachPi(pNtkCone, pPi, j) {
                // Are we looking at a key input?
                if (strstr(Abc_ObjName(pPi), "key")) {
                    if (!ClapAttack_SetKnownKeys(pNtkCone, pPi, pGlobalBsiKeys)) {
                        if (NumKeys < MaxKeysConsidered) strcpy(KeyNameTmp[NumKeys], Abc_ObjName(pPi));
                        NumKeys++;
                    } else {
                        ppNodeFreeList[NumKnownKeys] = pPi;
                        NumKnownKeys++;
                    }
                }
            }

            // Delete known keys from cone.

            if (NumKnownKeys) ClapAttack_DelKnownKeys(ppNodeFreeList, NumKnownKeys);

            // Can we evaluate this node (i.e. does it have the currently considered number of keys as input)?
            if ((NumKeys == MaxKeysConsidered) && NumKeys) {
                // Generate the logical miter to infer whether key leakage occurs at this node.
                MiterStatus = ClapAttack_MakeMiterHeuristic(pNtkCone, *ppCurKeyCnf, &pNtkMiter);

                // Did the miter generate successfully?
                if (!MiterStatus) {
                    // Run SAT on the generated miter to find sensitizing inputs
                    SatStatus = ClapAttack_RunSat(pNtkMiter);

                    // Did SAT return sensitizing inputs?
                    if (!SatStatus) {

                        if(pUnrolled){
                            Abc_Ntk_t* pUnrolledMiter = Abc_NtkDup(pUnrolled);
                            if (!pUnrolledMiter) {
                                printf("Failed to duplicate the network for reachability analysis.\n");
                                return;
                            }

                            ClapAttack_MiterReachability(pUnrolledMiter, pNtkMiter, unrollTimes);

                            // test SAT
                            reachabilityStatus = ClapAttack_RunSat(pUnrolledMiter);
                            

                            // Free the unrolled miter
                            Abc_NtkDelete(pUnrolledMiter);
                        }

                        if (!pUnrolled || !reachabilityStatus) {
                            // Update SAT node list with new Sat miter
                            pNode->fMarkC = 1;
                            (*pNumProbes)++;
                            ClapAttack_UpdateSatMiterList(ppSatMiterList, &pNode, pNtkMiter, NumKeys, KeyNameTmp, 1,
                                                        1.0 / (1 << (MaxKeysConsidered - 1)), pNtkMiter->pModel);

                        }else{
                            pNode->fMarkC = 1;
                        }

                        
                    } else {
                        // Node is UNSAT, but has the right number of keys... It's useless so mark it
                        pNode->fMarkC = 1;
                    }

                } else {
                    printf("Mitering failed. Proceed.\n");
                }

                // Cleanup
                Abc_NtkDelete(pNtkMiter);
            }

            // Cleanup
            Abc_NtkDelete(pNtkCone);
            for (k = 0; k < MaxKeysConsidered; k++) free(KeyNameTmp[k]);
            free(KeyNameTmp);
            free(ppNodeFreeList);
        }

        // Recurse
        if ((NumKeys <= MaxKeysConsidered) && (*pNumProbes <= MaxProbes)) {
            ClapAttack_TraversalRecursiveHeuristic(pNtk, pNode, pGlobalBsiKeys, MaxKeysConsidered, ppCurKeyCnf,
                                                   ppSatMiterList, pNumProbes, MaxProbes, probeResolutionSize,
                                                   pUnrolled, unrollTimes);
        }
    }
}

// Evaluate/simulate a multinode EOFM probe and infer key information leakage that will be produced.
void ClapAttack_EvalMultinodeProbe(struct SatMiterList *pSatMiter, Abc_Ntk_t *pNtk,
                                   struct BSI_KeyData_t *pGlobalBsiKeys, int *pOracleKey, Abc_Ntk_t **ppCurKeyCnf,
                                   int MaxKeysConsidered, int probeResolutionSize) {
    int *pFullDi;
    int i, j, k, m, NumKeys, NumKnownKeys, *KeyWithFreq, *KeyNoFreq, *WrongKeyValue, KeyValue = 0, PartialKeySatStatus,
                                                                                     fCurKeyCnfAlloc, fRerunInfer;
    Abc_Ntk_t *pNtkCone, *pInferPartialKeyMiter, *ntkTmp;
    Abc_Obj_t *pNode, *pPi, *pKey, **ppNodeFreeList;
    char **KeyNameTmp;
    struct BSI_KeyData_t GlobalBsiKeysTmp;
    struct BSI_KeyData_t *pGlobalBsiKeysTmp;

    // Initialize data structure to hold nodes to be free'd/removed and current keys being considered for probing
    ppNodeFreeList = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * pGlobalBsiKeys->NumKeys);

    KeyNameTmp = (char **)malloc(sizeof(char *) * MaxKeysConsidered);
    for (j = 0; j < MaxKeysConsidered; j++) {
        KeyNameTmp[j] = (char *)malloc(sizeof(char) * 100);
    }

    // Temporarily freeze copy of keystore
    pGlobalBsiKeysTmp = &GlobalBsiKeysTmp;
    ClapAttack_CopyKeyStore(pGlobalBsiKeys, pGlobalBsiKeysTmp);

    // Check supports ... if only one is an unknown key,
    // process it and continue fanout
    for (i = 0; i < pSatMiter->MatchedNodes; i++) {
        pNode = pSatMiter->ppSatNode[i];
        ClapAttack_IsolateCone(pNtk, &pNtkCone, pSatMiter->ppSatNode[i], probeResolutionSize);

        // Loop over the miter generation phase until SAT fails
        NumKeys = 0;
        NumKnownKeys = 0;

        Abc_NtkForEachPi(pNtkCone, pPi, j) {
            // Are we looking at a key input?
            if (strstr(Abc_ObjName(pPi), "key")) {
                if (!ClapAttack_SetKnownKeys(pNtkCone, pPi, pGlobalBsiKeysTmp)) {
                    strcpy(KeyNameTmp[NumKeys], Abc_ObjName(pPi));
                    NumKeys++;
                } else {
                    ppNodeFreeList[NumKnownKeys] = pPi;
                    NumKnownKeys++;
                }
            }
        }

        // Delete known keys from cone.
        if (NumKnownKeys) ClapAttack_DelKnownKeys(ppNodeFreeList, NumKnownKeys);

        // Status Prints
        printf("Evaluating node %s\n", Abc_ObjName(pNode));
        printf("The number of keys is: %d\n", NumKeys);

        // Malloc key values from SAT to infer from
        KeyWithFreq = (int *)malloc(sizeof(int) * NumKeys);
        KeyNoFreq = (int *)malloc(sizeof(int) * NumKeys);

        ClapAttack_InterpretDiHeuristic(pNtkCone, pSatMiter->pMiter, pSatMiter->pModel, NumKeys, KeyWithFreq, KeyNoFreq,
                                        &pFullDi);

        // Oracle testing. Comment out with real probe.
        ClapAttack_OracleSetConeKeys(pNtkCone, pFullDi, pOracleKey);

        WrongKeyValue = (int *)malloc(sizeof(int) * NumKeys);
        ClapAttack_OracleSimDi(pNtkCone, pFullDi, NumKeys, KeyWithFreq, KeyNoFreq, WrongKeyValue);

        // Cleanup
        free(pFullDi);

        // Debug printing
        printf("Wrong KeyValue: ");
        for (k = 0; k < NumKeys; k++) printf(" %d", WrongKeyValue[k]);
        printf("\n\n");

        // Initialize partial key logic if it doesnt exist
        // Otherwise update it...
        if (NumKeys > 1) {
            if (!(*ppCurKeyCnf)) {
                ClapAttack_InitKeyCnf(ppCurKeyCnf, NumKeys, WrongKeyValue, KeyNameTmp);
                fCurKeyCnfAlloc = 1;
            } else {
                ClapAttack_UpdateKeyCnf(ppCurKeyCnf, NumKeys, WrongKeyValue, KeyNameTmp);
            }
        }

        // Short term hack to handle case where key is of length 1
        if (NumKeys == 1) {
            KeyValue = (WrongKeyValue[0] + 1) % 2;
            ClapAttack_UpdateKey(KeyNameTmp[0], KeyValue, pGlobalBsiKeys);
        } else {
            // Otherwise, evaluate partial key logic for complete key info
            for (k = 0; k < NumKeys; k++) {
                // Build a miter from all available partial key information
                ClapAttack_PartialKeyInferenceMiter(*ppCurKeyCnf, &pInferPartialKeyMiter, KeyNameTmp[k]);
                PartialKeySatStatus = ClapAttack_RunSat(pInferPartialKeyMiter);
                Abc_NtkDelete(pInferPartialKeyMiter);

                // Initially, this will always fall through. The second time, this will always break
                // unless a new key value is discovered. This is the case because it is possible
                // discovering one key value will allow others to be inferred, so we must check again
                if (PartialKeySatStatus && (PartialKeySatStatus != -1)) {
                    // Is miter SAT? Can we infer key bits?
                    if (!ClapAttack_RunSat(*ppCurKeyCnf)) {
                        // What can we infer? Iterate through each key input to find out.
                        Abc_NtkForEachPi(*ppCurKeyCnf, pKey, m) {
                            if (!strcmp(Abc_ObjName(pKey), KeyNameTmp[k])) {
                                KeyValue = (*ppCurKeyCnf)->pModel[m];

                                // Either remove the key or set it to constant 1.
                                ClapAttack_UpdateKey(KeyNameTmp[k], KeyValue, pGlobalBsiKeys);
                                printf("We determined that key %s is %d\n", Abc_ObjName(pKey), KeyValue);

                                break;
                            }
                        }

                        // Optimize out key from partial CNF
                        ntkTmp = Abc_NtkToLogic(*ppCurKeyCnf);
                        Abc_NtkDelete(*ppCurKeyCnf);
                        *ppCurKeyCnf = ntkTmp;
                        Abc_NtkForEachPi(*ppCurKeyCnf, pKey, m) {
                            if (!strcmp(Abc_ObjName(pKey), KeyNameTmp[k])) {
                                Abc_ObjReplaceByConstant(pKey, KeyValue);
                                Abc_NtkDeleteObj(pKey);
                                break;
                            }
                        }

                        // Check if we optimized out all the PIs but one.
                        ntkTmp = Abc_NtkStrash(*ppCurKeyCnf, 0, 1, 0);
                        Abc_NtkDelete(*ppCurKeyCnf);
                        *ppCurKeyCnf = ntkTmp;

                        // IF so, free the partial info
                        if ((Abc_NtkPiNum(*ppCurKeyCnf) < 2) && fCurKeyCnfAlloc) {
                            Abc_NtkDelete(*ppCurKeyCnf);
                            *ppCurKeyCnf = NULL;
                            break;
                        }
                    } else {
                        // This should never occur. It indicates we eliminated hte correct key.
                        // If we see it, terminate.
                        printf("The key is somehow UNSAT... Exiting.\n\n");
                        exit(0);
                    }
                }
            }
        }

        // Cleanup memory allocations
        free(WrongKeyValue);
        free(KeyWithFreq);
        free(KeyNoFreq);
        Abc_NtkDelete(pNtkCone);
    }

    // Free and Realloc keyname tmp so that we can infer all keys from partial cnf we generated
    for (k = 0; k < MaxKeysConsidered; k++) free(KeyNameTmp[k]);
    free(KeyNameTmp);

    KeyNameTmp = (char **)malloc(sizeof(char *) * pGlobalBsiKeys->NumKeys);
    for (j = 0; j < pGlobalBsiKeys->NumKeys; j++) {
        KeyNameTmp[j] = (char *)malloc(sizeof(char) * 100);
    }

    k = 0;
    Abc_NtkForEachPi(pNtk, pPi, j) {
        // Are we looking at a key input?
        if (strstr(Abc_ObjName(pPi), "key")) {
            strcpy(KeyNameTmp[k], Abc_ObjName(pPi));
            k++;
        }
    }

    // Initialize so we always run while loop initially
    fRerunInfer = 1;

    // Infer known key values from partial key information
    while (fRerunInfer) {
        // Clean up known keys in CNF
        ClapAttack_CleanKeyCnf(pGlobalBsiKeys);
        fRerunInfer = 0;

        // Evaluate partial key logic for complete key info
        for (k = 0; k < pGlobalBsiKeys->NumKeys; k++) {
            // Is there any partial key information produced from this probe?
            if (*ppCurKeyCnf) {
                ClapAttack_PartialKeyInferenceMiter(*ppCurKeyCnf, &pInferPartialKeyMiter, KeyNameTmp[k]);

                // Run SAT on teh partial key miter to determine if we have fully specified any key values that
                // can help us progress the attack.
                PartialKeySatStatus = ClapAttack_RunSat(pInferPartialKeyMiter);
                Abc_NtkDelete(pInferPartialKeyMiter);

                // Handle any partial key information
                if (PartialKeySatStatus && (PartialKeySatStatus != -1)) {
                    if (!ClapAttack_RunSat(*ppCurKeyCnf)) {
                        // Update known keys in the network.
                        Abc_NtkForEachPi(*ppCurKeyCnf, pKey, m) {
                            if (!strcmp(Abc_ObjName(pKey), KeyNameTmp[k])) {
                                // Either remove the key or set it to constant 1.
                                KeyValue = (*ppCurKeyCnf)->pModel[m];
                                ClapAttack_UpdateKey(KeyNameTmp[k], KeyValue, pGlobalBsiKeys);
                                printf("We determined that key %s is %d\n", Abc_ObjName(pKey), KeyValue);
                                fRerunInfer = 1;
                                break;
                            }
                        }

                        // Optimize out key from partial CNF
                        ntkTmp = Abc_NtkToLogic(*ppCurKeyCnf);
                        Abc_NtkDelete(*ppCurKeyCnf);
                        *ppCurKeyCnf = ntkTmp;
                        Abc_NtkForEachPi(*ppCurKeyCnf, pKey, m) {
                            if (!strcmp(Abc_ObjName(pKey), KeyNameTmp[k])) {
                                Abc_ObjReplaceByConstant(pKey, KeyValue);
                                Abc_NtkDeleteObj(pKey);
                                break;
                            }
                        }

                        // Check if we optimized out all the PIs but one.
                        ntkTmp = Abc_NtkStrash(*ppCurKeyCnf, 0, 1, 0);
                        Abc_NtkDelete(*ppCurKeyCnf);
                        *ppCurKeyCnf = ntkTmp;

                        // IF so, free the partial info
                        if ((Abc_NtkPiNum(*ppCurKeyCnf) < 2) && fCurKeyCnfAlloc) {
                            Abc_NtkDelete(*ppCurKeyCnf);
                            *ppCurKeyCnf = NULL;
                            break;
                        }

                    } else {
                        printf("The key is somehow UNSAT... Exiting.\n\n");
                        exit(0);
                    }
                }
            }
        }
    }

    // Print off updated keystore
    printf("KeyStore is now: {");
    for (k = 0; k < pGlobalBsiKeys->NumKeys; k++) printf("%d, ", pGlobalBsiKeys->KeyValue[k]);
    printf("}\n\n");

    // Cleanup memory allocations
    free(pGlobalBsiKeysTmp->KeyValue);

    for (k = 0; k < pGlobalBsiKeys->NumKeys; k++) free(KeyNameTmp[k]);
    free(KeyNameTmp);
    free(ppNodeFreeList);
}

// Add a probe-able node to the end of the SAT Miter List data structure.
void ClapAttack_UpdateSatMiterList(struct SatMiterList **ppSatMiterList, Abc_Obj_t **ppNode, Abc_Ntk_t *pMiter,
                                   int NumKeys, char **KeyNames, int MaxNodesConsidered, float IdentifiableKeys,
                                   int *pModel) {
    int i;
    struct SatMiterList *pSatMiterListCur;
    struct SatMiterList *pSatMiterList;
    Abc_Obj_t *pPi;

    // If not allocated, initialize the data structure.
    if (!(*ppSatMiterList)) {
        *ppSatMiterList = (struct SatMiterList *)malloc(sizeof(struct SatMiterList));
        pSatMiterList = *ppSatMiterList;
        pSatMiterListCur = pSatMiterList;

        // Set prev pointer to NULL as this is the head of the list
        pSatMiterListCur->pPrev = NULL;
    } else {
        pSatMiterList = *ppSatMiterList;
        // Find end of list.
        pSatMiterListCur = pSatMiterList;
        while (pSatMiterListCur->pNext) {
            pSatMiterListCur = pSatMiterListCur->pNext;
        }

        // Now that you found it... continue.
        pSatMiterListCur->pNext = (struct SatMiterList *)malloc(sizeof(struct SatMiterList));

        // Set prev pointer
        (pSatMiterListCur->pNext)->pPrev = pSatMiterListCur;
        pSatMiterListCur = pSatMiterListCur->pNext;
    }

    // Update data
    pSatMiterListCur->ppSatNode = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * MaxNodesConsidered);

    pSatMiterListCur->pModel = (int *)malloc(sizeof(int) * Abc_NtkPiNum(pMiter));
    Abc_NtkForEachPi(pMiter, pPi, i) { pSatMiterListCur->pModel[i] = pMiter->pModel[i]; }

    pSatMiterListCur->pMiter = Abc_NtkDup(pMiter);

    // Add each node to the list.
    for (i = 0; i < MaxNodesConsidered; i++) {
        pSatMiterListCur->ppSatNode[i] = ppNode[i];
    }

    pSatMiterListCur->MatchedNodes = MaxNodesConsidered;

    // Add keynames
    pSatMiterListCur->NumKeys = NumKeys;
    pSatMiterListCur->KeyNames = (char **)malloc(sizeof(char *) * NumKeys);
    for (i = 0; i < NumKeys; i++) {
        pSatMiterListCur->KeyNames[i] = (char *)malloc(sizeof(char) * 100);
        strcpy(pSatMiterListCur->KeyNames[i], KeyNames[i]);
    }

    // Terminate list a new endpoint
    pSatMiterListCur->pNext = NULL;

    // Calculate number of key bits found by miter
    pSatMiterListCur->IdentifiableKeyBits = IdentifiableKeys;
}

// Free the SATMiterList data structure to avoid memory leaking.
void ClapAttack_FreeSatMiterList(struct SatMiterList **ppSatMiterList) {
    int i;
    struct SatMiterList *pSatMiterListCur;
    struct SatMiterList *pSatMiterList;

    pSatMiterList = *ppSatMiterList;

    // If not allocated, we're done...
    if (!pSatMiterList) return;

    // Find end of list.
    pSatMiterListCur = pSatMiterList;
    while (pSatMiterListCur->pNext) {
        pSatMiterListCur = pSatMiterListCur->pNext;
    }

    // Starting from end of list, free iteratively towards the front
    while (pSatMiterListCur) {
        // Update data
        free(pSatMiterListCur->ppSatNode);

        // Clear miter ntk
        Abc_NtkDelete(pSatMiterListCur->pMiter);

        // Clear model if it exists
        free(pSatMiterListCur->pModel);

        // Free keyname list
        for (i = 0; i < pSatMiterListCur->NumKeys; i++) {
            free(pSatMiterListCur->KeyNames[i]);
        }
        free(pSatMiterListCur->KeyNames);

        // Go to previous node and free
        if (pSatMiterListCur->pPrev) {
            pSatMiterListCur = pSatMiterListCur->pPrev;
            free(pSatMiterListCur->pNext);
        } else {
            free(pSatMiterListCur);
            *ppSatMiterList = NULL;
            break;
        }
    }
}

// Update partial key information that is stored globally (rather than within a single CLAP iteration).
int ClapAttack_UpdateGlobalKeyCnf(Abc_Ntk_t **ppCurKeyCnf, struct BSI_KeyData_t *pGlobalBsiKeys) {
    // Handle Keystore partial CNF management
    if (*ppCurKeyCnf) {
        if (pGlobalBsiKeys->pKeyCnf) {
            if (!Abc_NtkAppendSilent(pGlobalBsiKeys->pKeyCnf, *ppCurKeyCnf, 1)) {
                Abc_NtkDelete(*ppCurKeyCnf);
                Abc_Print(-1, "Appending the networks failed.\n");
                return 1;
            }

            // And the Pos
            extern int Abc_NtkCombinePos(Abc_Ntk_t * pNtk, int fAnd, int fXor);

            if (!Abc_NtkCombinePos(pGlobalBsiKeys->pKeyCnf, 1, 0)) {
                Abc_Print(-1, "ANDing the POs has failed.\n");
                return 1;
            }
        } else {
            pGlobalBsiKeys->pKeyCnf = Abc_NtkDup(*ppCurKeyCnf);
        }

        // Rename Pos
        ClapAttack_RenamePo(pGlobalBsiKeys->pKeyCnf, 0, "not_key");

        // Reset the cur partial key pointer to NULL
        Abc_NtkDelete(*ppCurKeyCnf);
        *ppCurKeyCnf = NULL;
    }

    return 0;
}

// Isolate fan-in cone for a specific probe point (inclusive of probed node) for the network.
// Run cone command... and return the fanout cone as a separate network
int ClapAttack_IsolateCone(Abc_Ntk_t *pNtk, Abc_Ntk_t **ppNtkCone, Abc_Obj_t *pProbe, int probeResolution) {
    int fUseAllCis = 0;
    char PoTmpName[25];
    Abc_Ntk_t *pNtkConeTmp, *pNtkTmp;
    Abc_Obj_t *pPo, *pHeadNode, *pFanin, *pNode;
    int j;

    // Create cone for probed node
    *ppNtkCone = Abc_NtkCreateCone(pNtk, pProbe, Abc_ObjName(pProbe), fUseAllCis);

    // Remove PO for the cone -- this is the output of the probed node
    pPo = Abc_NtkPo(*ppNtkCone, 0);
    pHeadNode = Abc_ObjChild0(pPo);
    Abc_NtkDeleteObj(pPo);
    assert(Abc_NtkPoNum(*ppNtkCone) == 0);

    // Re-sort network
    Abc_NtkOrderCisCos(*ppNtkCone);

    // Set the output of the network to the input of the probe point (i.e. the probed node)
    Abc_ObjForEachFanin(pHeadNode, pFanin, j) {
        pNode = Abc_NtkCreatePo(*ppNtkCone);
        Abc_ObjAddFanin(pNode, pFanin);
        sprintf(PoTmpName, "po%d", j);
        Abc_ObjAssignName(pNode, PoTmpName, NULL);
    }

    // Re-sort network
    Abc_NtkOrderCisCos(*ppNtkCone);

    // Cut off the head when we're done with it. This essentially removes the probed-node itself.
    Abc_NtkDeleteObj(pHeadNode);

    int *ignoreFanins = (int *)malloc(probeResolution * sizeof(int));
    Abc_Obj_t **pProbes = (Abc_Obj_t **)malloc(probeResolution * sizeof(Abc_Obj_t *));

    Abc_Obj_t *pCurrentProbe = pProbe;
    int totalFanins = j;

    // Initial values for the first iteration
    ignoreFanins[0] = -1;
    pProbes[0] = pProbe;
    for (int resolutionLevel = 1; resolutionLevel < probeResolution && Abc_ObjFanoutNum(pCurrentProbe);
         resolutionLevel++) {
        Abc_Obj_t *pNextProbe = Abc_ObjFanout0(pCurrentProbe);

        if (!Abc_ObjIsPo(pNextProbe)) {
            int ignoreFanin = -1;
            int i;
            Abc_ObjForEachFanin(pNextProbe, pFanin, i) {
                if (pFanin == pCurrentProbe) {
                    ignoreFanin = i;
                }
            }

            // Only store the current values for future iterations if there is a next iteration
            if (resolutionLevel + 1 < probeResolution) {
                ignoreFanins[resolutionLevel + 1] = ignoreFanin;
                pProbes[resolutionLevel + 1] = pNextProbe;
            }

            pNtkConeTmp = Abc_NtkCreateCone(pNtk, pNextProbe, Abc_ObjName(pNextProbe), fUseAllCis);
            pPo = Abc_NtkPo(pNtkConeTmp, 0);
            pHeadNode = Abc_ObjChild0(pPo);
            Abc_NtkDeleteObj(pPo);
            assert(Abc_NtkPoNum(pNtkConeTmp) == 0);

            Abc_ObjForEachFanin(pHeadNode, pFanin, i) {
                if (i != ignoreFanin) {
                    pNode = Abc_NtkCreatePo(pNtkConeTmp);
                    Abc_ObjAddFanin(pNode, pFanin);
                    sprintf(PoTmpName, "po%d", totalFanins + i);
                    Abc_ObjAssignName(pNode, PoTmpName, NULL);
                }
            }
            totalFanins += i;

            Abc_NtkDeleteObj(pHeadNode);
            Abc_NtkOrderCisCos(pNtkConeTmp);

            pNtkTmp = Abc_NtkStrash(*ppNtkCone, 0, 1, 0);
            Abc_NtkDelete(*ppNtkCone);
            *ppNtkCone = pNtkTmp;

            pNtkTmp = Abc_NtkStrash(pNtkConeTmp, 0, 1, 0);
            Abc_NtkDelete(pNtkConeTmp);
            pNtkConeTmp = pNtkTmp;

            if (!Abc_NtkAppendSilent(*ppNtkCone, pNtkConeTmp, 1)) {
                Abc_Print(-1, "Appending the networks failed.\n");
                exit(0);
                return 1;
            }
            Abc_NtkDelete(pNtkConeTmp);

            pNtkTmp = Abc_NtkToLogic(*ppNtkCone);
            Abc_NtkDelete(*ppNtkCone);
            *ppNtkCone = pNtkTmp;

            pCurrentProbe = pNextProbe;
        }
    }

    // make sure that everything is okay
    if (!Abc_NtkCheck(*ppNtkCone)) {
        printf("Abc_NtkOrPos: The network check has failed.\n");
        free(ignoreFanins);
        free(pProbes);
        return 1;
    }

    free(ignoreFanins);
    free(pProbes);

    return 0;
}

// Clean the logic cone isolated as an individual network by removing unused/unneeded logic.
void ClapAttack_CleanCone(Abc_Ntk_t **ppNtk) {
    extern Abc_Ntk_t *Abc_NtkDarCleanupAig(Abc_Ntk_t * pNtk, int fCleanupPis, int fCleanupPos, int fVerbose);

    // Strash the cone.
    *ppNtk = Abc_NtkStrash(*ppNtk, 0, 1, 0);

    // Cleanup the unused logic.
    if (Abc_NtkIsStrash(*ppNtk)) *ppNtk = Abc_NtkDarCleanupAig(*ppNtk, 1, 1, 0);
}

// Initialize the partial key logic network that is used to store partial key information
// produced by EOFM probing.
void ClapAttack_InitKeyCnf(Abc_Ntk_t **ppNtk, int NumKeys, int *WrongKeyValue, char **KeyNames) {
    int i;
    Abc_Ntk_t *ntkTmp;
    Abc_Obj_t **pPi = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * NumKeys);
    Abc_Obj_t *pPo;

    // Allocate the network
    *ppNtk = Abc_NtkAlloc(ABC_NTK_STRASH, ABC_FUNC_AIG, 1);
    (*ppNtk)->pName = Extra_UtilStrsav("PartialKeyInfo");

    // Create a PI for each key
    for (i = 0; i < NumKeys; i++) {
        pPi[i] = Abc_NtkCreatePi(*ppNtk);
        Abc_ObjAssignName(pPi[i], KeyNames[i], NULL);
    }

    // Calculate logic based off wrong key value.
    for (i = 0; i < NumKeys; i++) {
        // Invert the key if necessary
        if (!WrongKeyValue[i]) {
            pPi[i] = Abc_ObjNot(pPi[i]);
        }

        if (i) pPi[i] = Abc_AigAnd((Abc_Aig_t *)(*ppNtk)->pManFunc, pPi[i - 1], pPi[i]);
    }

    // Final inversion ... this is not the key!
    pPi[NumKeys - 1] = Abc_ObjNot(pPi[NumKeys - 1]);

    // Now make this a PO
    pPo = Abc_NtkCreatePo(*ppNtk);
    Abc_ObjAssignName(pPo, "not_key_partial", NULL);
    Abc_ObjAddFanin(pPo, pPi[NumKeys - 1]);

    // Strash the network
    ntkTmp = Abc_NtkStrash(*ppNtk, 0, 1, 0);
    Abc_NtkDelete(*ppNtk);
    *ppNtk = ntkTmp;

    // Make sure we did not damage the network while modifying it.
    if (!Abc_NtkCheck(*ppNtk)) fprintf(stdout, "Abc_NtkCreateFromNode(): Network check has failed.\n");

    // Free the malloced array
    free(pPi);
}

// Rename a PO in a network by index.
void ClapAttack_RenamePo(Abc_Ntk_t *pNtk, int PoIdx, char *NewPoName) {
    Abc_Obj_t *pNode, *pPo;

    // Get fanin for PO
    pNode = Abc_ObjChild0(Abc_NtkPo(pNtk, PoIdx));

    // remove the POs and their names
    Abc_NtkDeleteObj(Abc_NtkPo(pNtk, PoIdx));

    // create the new PO
    pPo = Abc_NtkCreatePo(pNtk);
    Abc_ObjAddFanin(pPo, pNode);
    Abc_ObjAssignName(pPo, NewPoName, NULL);
    Abc_NtkOrderCisCos(pNtk);
}

// Update the partial logic for the correct key values based on the results of an EOFM probe.
void ClapAttack_UpdateKeyCnf(Abc_Ntk_t **ppNtk, int NumKeys, int *WrongKeyValue, char **KeyNames) {
    int i, j, fPiExists;
    Abc_Ntk_t *ntkTmp;
    Abc_Obj_t **pPi = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * NumKeys);
    Abc_Obj_t *pNode, *pMiter;
    Fraig_Params_t Params;

    // Create a PI for each key that doesnt exist
    for (i = 0; i < NumKeys; i++) {
        fPiExists = 0;

        // Do we already have the PI?
        Abc_NtkForEachPi(*ppNtk, pNode, j) {
            if (!strcmp(Abc_ObjName(pNode), KeyNames[i])) {
                fPiExists = 1;
                pPi[i] = pNode;
            }
        }

        // If not, allocate it.
        if (!fPiExists) {
            pPi[i] = Abc_NtkCreatePi(*ppNtk);
            Abc_ObjAssignName(pPi[i], KeyNames[i], NULL);
        }
    }

    // Calculate logic based off wrong key value.
    for (i = 0; i < NumKeys; i++) {
        // Invert the key if necessary
        if (!WrongKeyValue[i]) {
            pPi[i] = Abc_ObjNot(pPi[i]);
        }

        // And all the outputs together (i.e. for the key to be correct ALL of these clauses must be true)
        if (i) pPi[i] = Abc_AigAnd((Abc_Aig_t *)(*ppNtk)->pManFunc, pPi[i - 1], pPi[i]);
    }

    // Final inversion ... this is not the key!
    pPi[NumKeys - 1] = Abc_ObjNot(pPi[NumKeys - 1]);

    // And this node with the previous PO to create the miter
    pMiter = Abc_AigAnd((Abc_Aig_t *)(*ppNtk)->pManFunc, pPi[NumKeys - 1], Abc_ObjChild0(Abc_NtkPo(*ppNtk, 0)));

    // remove the POs and their names
    Abc_NtkDeleteObj(Abc_NtkPo(*ppNtk, 0));
    assert(Abc_NtkPoNum(*ppNtk) == 0);

    // create the new PO
    pNode = Abc_NtkCreatePo(*ppNtk);
    Abc_ObjAddFanin(pNode, pMiter);
    Abc_ObjAssignName(pNode, "not_key_partial", NULL);
    Abc_NtkOrderCisCos(*ppNtk);

    // Fraig the network
    Fraig_ParamsSetDefault(&Params);
    *ppNtk = Abc_NtkFraig(ntkTmp = *ppNtk, &Params, 0, 0);
    Abc_NtkDelete(ntkTmp);

    // Strash the network again
    ntkTmp = Abc_NtkStrash(*ppNtk, 0, 1, 0);
    Abc_NtkDelete(*ppNtk);
    *ppNtk = ntkTmp;

    // Make sure we did not damage the network
    if (!Abc_NtkCheck(*ppNtk)) fprintf(stdout, "Abc_NtkCreateFromNode(): Network check has failed.\n");

    // Free the malloced array
    free(pPi);
}

// Make the logic formula for the CLAP attack indicating whether probing a given node produces key leakage.
// Essentially 3 miter circuits are generated for the circuit to assess the leakage produced by probing the node.
int ClapAttack_MakeMiterHeuristic(Abc_Ntk_t *pNtkCone, Abc_Ntk_t *pNtkPartialKey, Abc_Ntk_t **ppNtkMiter) {
    Abc_Ntk_t *pNtkTmpMiter1, *pNtkMiter2, *pNtkTmpMiter2, *pKeyMiter, *pNtkPartialKeyMiter, *pNtkPartialKeyMiterTmp,
        *ntkTmp;
    Abc_Obj_t *pNode;
    Fraig_Params_t Params;
    int fCheck;
    int fComb;
    int fImplic;
    int fMulti;
    int nPartSize;
    int NumKeys;
    int i;

    // Named function params for clarity
    fComb = 0;
    fCheck = 1;
    fImplic = 0;
    fMulti = 0;
    nPartSize = 0;
    NumKeys = 0;

    // Miter circuit 1 init
    *ppNtkMiter = Abc_NtkDup(pNtkCone);
    pNtkTmpMiter1 = Abc_NtkDup(*ppNtkMiter);

    // Miter circuit 2 init
    pNtkMiter2 = Abc_NtkDup(pNtkCone);
    pNtkTmpMiter2 = Abc_NtkDup(pNtkCone);

    // Strash the networks
    ntkTmp = Abc_NtkStrash(*ppNtkMiter, 0, 1, 0);
    Abc_NtkDelete(*ppNtkMiter);
    *ppNtkMiter = ntkTmp;

    ntkTmp = Abc_NtkStrash(pNtkTmpMiter1, 0, 1, 0);
    Abc_NtkDelete(pNtkTmpMiter1);
    pNtkTmpMiter1 = ntkTmp;

    ntkTmp = Abc_NtkStrash(pNtkMiter2, 0, 1, 0);
    Abc_NtkDelete(pNtkMiter2);
    pNtkMiter2 = ntkTmp;

    ntkTmp = Abc_NtkStrash(pNtkTmpMiter2, 0, 1, 0);
    Abc_NtkDelete(pNtkTmpMiter2);
    pNtkTmpMiter2 = ntkTmp;

    // Build partial key logic miter.
    pNtkPartialKeyMiter = NULL;
    if (pNtkPartialKey) {
        // Duplicate partial key logic to generate miter
        pNtkPartialKeyMiter = Abc_NtkDup(pNtkPartialKey);

        // Cleanup duplicated circuit
        ntkTmp = Abc_NtkStrash(pNtkPartialKeyMiter, 0, 1, 0);
        Abc_NtkDelete(pNtkPartialKeyMiter);
        pNtkPartialKeyMiter = ntkTmp;

        // Duplicate the partial key logic again in order to create the miter
        pNtkPartialKeyMiterTmp = Abc_NtkDup(pNtkPartialKeyMiter);
        ntkTmp = Abc_NtkStrash(pNtkPartialKeyMiterTmp, 0, 1, 0);
        Abc_NtkDelete(pNtkPartialKeyMiterTmp);
        pNtkPartialKeyMiterTmp = ntkTmp;

        // rename the key miter logic cones to standard and unique values for each copy
        ClapAttack_RenameLogic(pNtkPartialKeyMiter, "_1", "_1", "_cone1", 1);
        ClapAttack_RenameLogic(pNtkPartialKeyMiterTmp, "_2", "_2", "_cone2", 1);

        // Append copies of network to make key miter
        if (!Abc_NtkAppendSilent(pNtkPartialKeyMiter, pNtkPartialKeyMiterTmp, 1)) {
            Abc_NtkDelete(pNtkPartialKeyMiterTmp);
            Abc_Print(-1, "Appending the networks failed 2.\n");
            exit(0);
            return 1;
        }
        Abc_NtkDelete(pNtkPartialKeyMiterTmp);

        // Strash the circuit to clean it up and remove any dangling logic
        ntkTmp = Abc_NtkStrash(pNtkPartialKeyMiter, 0, 1, 0);
        Abc_NtkDelete(pNtkPartialKeyMiter);
        pNtkPartialKeyMiter = ntkTmp;

        // Make the final miter
        extern int Abc_NtkCombinePos(Abc_Ntk_t * pNtk, int fAnd, int fXor);

        // Miter the keymiter network output
        if (!Abc_NtkCombinePos(pNtkPartialKeyMiter, 1, 0)) {
            Abc_Print(-1, "ANDing the POs has failed.\n");
            return 1;
        }

        // Rename primary output of the key miter to standard value for later
        ClapAttack_RenamePo(pNtkPartialKeyMiter, 0, "not_key_miter");

        // make sure that everything is okay
        if (!Abc_NtkCheck(pNtkPartialKeyMiter)) {
            printf("Abc_NtkOrPos: The network check has failed.\n");
            return 1;
        }
    }

    // Rename I/O in miter circuits to uniform/standardized values
    ClapAttack_RenameLogic(*ppNtkMiter, "_1", "_1", "_cone1", 1);
    ClapAttack_RenameLogic(pNtkTmpMiter1, "_2", "_1", "_cone2", 1);
    ClapAttack_RenameLogic(pNtkMiter2, "_1", "_2", "_cone3", 1);
    ClapAttack_RenameLogic(pNtkTmpMiter2, "_2", "_2", "_cone4", 1);

    // Append the two networks we prepped for miter 1
    if (!Abc_NtkAppendSilent(*ppNtkMiter, pNtkTmpMiter1, 1)) {
        Abc_NtkDelete(pNtkTmpMiter1);
        Abc_Print(-1, "Appending the networks failed 3.\n");
        exit(0);
        return 1;
    }

    // Cleanup temp miter
    Abc_NtkDelete(pNtkTmpMiter1);

    // Append the two networks we prepped for miter 2
    if (!Abc_NtkAppendSilent(pNtkMiter2, pNtkTmpMiter2, 1)) {
        Abc_NtkDelete(pNtkTmpMiter2);
        Abc_Print(-1, "Appending the networks failed 4.\n");
        exit(0);
        return 1;
    }
    Abc_NtkDelete(pNtkTmpMiter2);

    // Generate the first miter cone
    if (!ClapAttack_MiterPos(*ppNtkMiter, 1, 0)) {
        Abc_Print(-1, "XOR/ORing the POs has failed.\n");
        exit(0);
        return 1;
    }

    // Generate the second miter cone
    if (!ClapAttack_MiterPos(pNtkMiter2, 0, 1)) {
        Abc_Print(-1, "XNOR/ANDing the POs has failed.\n");
        exit(0);
        return 1;
    }

    // Merge Miters
    ClapAttack_RenameOutput(*ppNtkMiter, "_1", 1);
    ClapAttack_RenameOutput(pNtkMiter2, "_2", 1);
    if (!Abc_NtkAppendSilent(*ppNtkMiter, pNtkMiter2, 1)) {
        Abc_NtkDelete(pNtkMiter2);
        Abc_Print(-1, "Appending the networks failed 5.\n");
        exit(0);
        return 1;
    }

    // Cleanup temp logic
    Abc_NtkDelete(pNtkMiter2);

    // check to see if there are any keys left in the optimized
    // circuit. If not, we don't need to bother checking anything
    Abc_NtkForEachPi(*ppNtkMiter, pNode, i) {
        // Is it a key?
        if (strstr(Abc_ObjName(pNode), "key")) NumKeys++;
    }

    // No keys... don't bother continuing
    if (!NumKeys) {
        if (pNtkPartialKey) {
            Abc_NtkDelete(pNtkPartialKeyMiter);
        }
        // printf("We failed out because there are no keys for some reason....\n\n\n\n");
        return 1;
    }

    // Finally, we must make the key miter --
    // We will operate off of the final miter and
    // add a circuit XORing each key pair.
    // If we remove other POs, the optimization
    // engine will dump the remainder of the circuit.
    // It's a hack.... for now....
    pKeyMiter = Abc_NtkDup(*ppNtkMiter);
    if (!ClapAttack_MiterKeys(pKeyMiter)) {
        Abc_Print(-1, "XORing the keys has failed.\n");
        exit(0);
        return 1;
    }

    // Merge in key miter
    if (!Abc_NtkAppendSilent(*ppNtkMiter, pKeyMiter, 1)) {
        Abc_NtkDelete(pKeyMiter);
        Abc_Print(-1, "Appending the networks failed 6.\n");
        exit(0);
        return 1;
    }

    // Cleanup temporary network
    Abc_NtkDelete(pKeyMiter);

    // Merge in partial key logic miter
    if (pNtkPartialKeyMiter) {
        if (!Abc_NtkAppendSilent(*ppNtkMiter, pNtkPartialKeyMiter, 1)) {
            Abc_NtkDelete(pNtkPartialKeyMiter);
            Abc_Print(-1, "Appending the networks failed 7.\n");
            exit(0);
            return 1;
        }

        // Cleanup temporary network
        Abc_NtkDelete(pNtkPartialKeyMiter);
    }

    // Generate the final miter cone
    extern int Abc_NtkCombinePos(Abc_Ntk_t * pNtk, int fAnd, int fXor);

    if (!Abc_NtkCombinePos(*ppNtkMiter, 1, 0))  //! ClapAttack_MiterPos( *ppNtkMiter, 0, 0, 1 ) )
    {
        Abc_Print(-1, "ANDing the POs has failed.\n");
        exit(0);
        return 1;
    }

    // Cleanup program state and memory
    Fraig_ParamsSetDefault(&Params);
    *ppNtkMiter = Abc_NtkFraig(ntkTmp = *ppNtkMiter, &Params, 0, 0);
    Abc_NtkDelete(ntkTmp);

    return 0;
}

// Run DSAT from ABC on the constructed miter network to find
// satisfying inputs for physical portion of CLAP attack
int ClapAttack_RunSat(Abc_Ntk_t *pNtk) {
    int RetValue;
    int fAlignPol;
    int fAndOuts;
    int fNewSolver;
    int fSilent;
    int fShowPattern;
    int fVerbose;
    int nConfLimit;
    int nLearnedStart;
    int nLearnedDelta;
    int nLearnedPerce;
    int nInsLimit;
    Abc_Cex_t *pCex;

    extern int Abc_NtkDSat(Abc_Ntk_t * pNtk, ABC_INT64_T nConfLimit, ABC_INT64_T nInsLimit, int nLearnedStart,
                           int nLearnedDelta, int nLearnedPerce, int fAlignPol, int fAndOuts, int fNewSolver,
                           int fVerbose);
    // set defaults
    fAlignPol = 0;
    fAndOuts = 0;
    fNewSolver = 0;
    fSilent = 0;
    fShowPattern = 1;
    fVerbose = 0;
    nConfLimit = 0;
    nInsLimit = 0;
    nLearnedStart = 0;
    nLearnedDelta = 0;
    nLearnedPerce = 0;

    RetValue = Abc_NtkDSat(pNtk, (ABC_INT64_T)nConfLimit, (ABC_INT64_T)nInsLimit, nLearnedStart, nLearnedDelta,
                           nLearnedPerce, fAlignPol, fAndOuts, fNewSolver, fVerbose);

    // verify that the pattern is correct
    if (RetValue == 0 && Abc_NtkPoNum(pNtk) == 1) {
        int *pSimInfo = Abc_NtkVerifySimulatePattern(pNtk, pNtk->pModel);
        if (pSimInfo[0] != 1) Abc_Print(1, "ERROR in Abc_NtkMiterSat(): Generated counter example is invalid.\n");

        pCex = Abc_CexCreate(0, Abc_NtkPiNum(pNtk), pNtk->pModel, 0, 0, 0);

        // DEBUG: Print full input/output
        // ClapAttack_PrintInp( pNtk, pNtk->pModel );
        // ClapAttack_PrintOut( pNtk, pSimInfo );

        ABC_FREE(pSimInfo);
        ABC_FREE(pCex);
    }

    return RetValue;
}

// Note that this fails on underscore characters for some reason...
// Determine which key value leads to presence/absence of an EOFM probe frequency component.
// This is later used to infer the key value based on teh presence/absence of frequency component.
// I.e. to infer the key value.
void ClapAttack_InterpretDiHeuristic(Abc_Ntk_t *pNtk, Abc_Ntk_t *pNtkMiter, int *pModel, int NumKeys, int *KeyWithFreq,
                                     int *KeyNoFreq, int **ppDiFull) {
    int i, j, idx1, idx2, KeyIndex;
    Abc_Obj_t *pPi, *pPiTmp, *pPo;
    int *pInp1, *pInp2, *pDi1, *pDi2, *pKey1, *pKey2, *pDiFull;

    pInp1 = (int *)malloc(sizeof(int) * (Abc_NtkPiNum(pNtk) * 2));
    pInp2 = (int *)malloc(sizeof(int) * (Abc_NtkPiNum(pNtk) * 2));
    pDi1 = (int *)malloc(sizeof(int) * Abc_NtkPiNum(pNtk));
    pDi2 = (int *)malloc(sizeof(int) * Abc_NtkPiNum(pNtk));
    pKey1 = (int *)malloc(sizeof(int) * NumKeys);
    pKey2 = (int *)malloc(sizeof(int) * NumKeys);
    pDiFull = (int *)malloc(sizeof(int) * (Abc_NtkPiNum(pNtk) * 2));
    *ppDiFull = pDiFull;

    // Init.
    idx1 = 0;
    idx2 = Abc_NtkPiNum(pNtk);

    // Organize larger miter model into shorter cone model for oracle
    Abc_NtkForEachPi(pNtk, pPi, i) {
        Abc_NtkForEachPi(pNtkMiter, pPiTmp, j) {
            // Check if PI names match. If so, save off the model value
            if (strstr(Abc_ObjName(pPiTmp), Abc_ObjName(pPi)) &&
                (strlen(Abc_ObjName(pPiTmp)) == (strlen(Abc_ObjName(pPi)) + 2))) {
                // Part of first DI
                if (strstr(Abc_ObjName(pPiTmp), "_1")) {
                    pDiFull[idx1] = pModel[j];
                    idx1++;
                }

                // Part of second DI
                if (strstr(Abc_ObjName(pPiTmp), "_2")) {
                    pDiFull[idx2] = pModel[j];
                    idx2++;
                }
            }
        }
    }

    for (i = 0; i < (Abc_NtkPiNum(pNtk) * 2); i++) {
        pInp1[i] = pDiFull[i];
        pInp2[i] = pDiFull[i];
    }

    // Assign key->0

    // Goal: Iterate through each PI. Identify list of keys.
    KeyIndex = 0;
    Abc_NtkForEachPi(pNtk, pPi, i) {
        // Are we looking at a key input?
        if (strstr(Abc_ObjName(pPi), "key")) {
            pInp1[i] = pInp1[i + Abc_NtkPiNum(pNtk)];
            // pInp1[i+Abc_NtkPiNum(pNtk)] = 0;
            // pInp2[i] = 1;
            pInp2[i + Abc_NtkPiNum(pNtk)] = pInp2[i];
            // Abc_Print(1, "match!: %d=%s %d %d\n", i, Abc_ObjName(pPi), pInp[i], pOracleKey[keyindex] );

            // Save off each key value to return
            pKey1[KeyIndex] = pInp1[i];
            pKey2[KeyIndex] = pInp2[i];
            KeyIndex++;
        }
    }

    // Save off each input separtely
    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        pDi1[i] = pInp1[i];
        pDi2[i] = pInp1[i + Abc_NtkPiNum(pNtk)];
    }

    // Simulate Pattern 1
    int *pSimInfo1 = Abc_NtkVerifySimulatePattern(pNtk, pDi1);

    // Simulate Pattern 2
    int *pSimInfo2 = Abc_NtkVerifySimulatePattern(pNtk, pDi2);

    // Print pattern 2 input/output
    // ClapAttack_PrintInp( pNtk, pDi1 );
    // ClapAttack_PrintOut( pNtk, pSimInfo1 );
    // ClapAttack_PrintInp( pNtk, pDi2 );
    // ClapAttack_PrintOut( pNtk, pSimInfo2 );

    Abc_NtkForEachPo(pNtk, pPo, i) {
        if (pSimInfo1[i] - pSimInfo2[i]) {
            printf("Interpret: We had a frequency component for key=0!\n");
            for (KeyIndex = 0; KeyIndex < NumKeys; KeyIndex++) {
                KeyWithFreq[KeyIndex] = pKey1[KeyIndex];
                KeyNoFreq[KeyIndex] = pKey2[KeyIndex];
            }
        }
    }

    ABC_FREE(pSimInfo1);
    ABC_FREE(pSimInfo2);

    // Test for Key -> 1
    // Save off each input separtely
    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        pDi1[i] = pInp2[i];
        pDi2[i] = pInp2[i + Abc_NtkPiNum(pNtk)];
    }

    // Simulate Pattern 1
    pSimInfo1 = Abc_NtkVerifySimulatePattern(pNtk, pDi1);

    // Simulate Pattern 2
    pSimInfo2 = Abc_NtkVerifySimulatePattern(pNtk, pDi2);

    // Print pattern 2 input/output
    // ClapAttack_PrintInp( pNtk, pDi1 );
    // ClapAttack_PrintOut( pNtk, pSimInfo1 );
    // ClapAttack_PrintInp( pNtk, pDi2 );
    // ClapAttack_PrintOut( pNtk, pSimInfo2 );

    Abc_NtkForEachPo(pNtk, pPo, i) {
        if (pSimInfo1[i] - pSimInfo2[i]) {
            printf("Interpret: We had a frequency component for key=1!\n");
            for (KeyIndex = 0; KeyIndex < NumKeys; KeyIndex++) {
                KeyWithFreq[KeyIndex] = pKey2[KeyIndex];
                KeyNoFreq[KeyIndex] = pKey1[KeyIndex];
            }
        }
    }

    ABC_FREE(pSimInfo1);
    ABC_FREE(pSimInfo2);
    free(pDi1);
    free(pDi2);
    free(pInp1);
    free(pInp2);
    free(pKey1);
    free(pKey2);
}

// Infer key from simulated information in circuit. Essentially, do we have a
// frequency component?
int ClapAttack_OracleInferKey(Abc_Ntk_t *pNtk, int *pSimInfo1, int *pSimInfo2) {
    int i, fKeyFreq;
    Abc_Obj_t *pPo;

    // Initialize key to no frequency component value. If we see a frequency component...
    // Flip it!
    fKeyFreq = 0;
    Abc_NtkForEachPo(pNtk, pPo, i) {
        if (pSimInfo1[i] - pSimInfo2[i]) {
            printf("Oracle: We observed a frequency component in our probe!\n");
            fKeyFreq = 1;
        }
    }

    return fKeyFreq;
}

// Simulate the oracle circuit with the known key applied with our DI. This simulates the EOFM probing that occurs
// during the CLAP attack.
void ClapAttack_OracleSimDi(Abc_Ntk_t *pNtk, int *pDi, int NumKeys, int *KeyWithFreq, int *KeyNoFreq,
                            int *WrongKeyValue) {
    int i;
    int *pDi1, *pDi2;
    int fKeyFreq;

    // Malloc input arrays
    pDi1 = (int *)malloc(sizeof(int) * Abc_NtkPiNum(pNtk));
    pDi2 = (int *)malloc(sizeof(int) * Abc_NtkPiNum(pNtk));

    // Save off each input separtely
    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        pDi1[i] = pDi[i];
        pDi2[i] = pDi[i + Abc_NtkPiNum(pNtk)];
    }

    // Simulate Pattern 1
    int *pSimInfo1 = Abc_NtkVerifySimulatePattern(pNtk, pDi1);

    // DEBUG: Print pattern 1 input/output
    // ClapAttack_PrintInp( pNtk, pDi1 );
    // ClapAttack_PrintOut( pNtk, pSimInfo1 );

    // Simulate Pattern 2
    int *pSimInfo2 = Abc_NtkVerifySimulatePattern(pNtk, pDi2);

    // DEBUG: Print pattern 2 input/output
    // ClapAttack_PrintInp( pNtk, pDi2 );
    // ClapAttack_PrintOut( pNtk, pSimInfo2 );

    // Interpret key value based on oracle simulation
    fKeyFreq = ClapAttack_OracleInferKey(pNtk, pSimInfo1, pSimInfo2);

    // Note that were storing the wrong key value as we can't infer the correct one
    // only eliminate the wrong one...
    for (i = 0; i < NumKeys; i++) WrongKeyValue[i] = (fKeyFreq) ? KeyNoFreq[i] : KeyWithFreq[i];

    // Cleanup
    ABC_FREE(pSimInfo1);
    ABC_FREE(pSimInfo2);
    free(pDi1);
    free(pDi2);
}

// Update the global keystore with any key values that were determined by a CLAP iteration.
void ClapAttack_UpdateKey(char *KeyNameTmp, int KeyValue, struct BSI_KeyData_t *pGlobalBsiKeys) {
    int keyindex;

    // Isolate key index
    keyindex = (int)strtol(&(KeyNameTmp[8]), (char **)NULL, 10);

    // update key store with newly known key value
    pGlobalBsiKeys->KeyValue[keyindex] = KeyValue;

    // Mark that we changed the keystore.
    pGlobalBsiKeys->Updated = 1;
}

// Perform logical optimization and clean up our
// network based on known keys after each
// CLAP attack iteration.
void ClapAttack_CleanKeyCnf(struct BSI_KeyData_t *pGlobalBsiKeys) {
    int i;
    Abc_Obj_t *pPi;
    Abc_Ntk_t *pNtkTmp;

    // Replace known keys in the partial key cnf by constants.
    // Convert to logic so we can use existing constant functions
    if (pGlobalBsiKeys->pKeyCnf) {
        pNtkTmp = Abc_NtkToLogic(pGlobalBsiKeys->pKeyCnf);
        Abc_NtkDelete(pGlobalBsiKeys->pKeyCnf);
        pGlobalBsiKeys->pKeyCnf = pNtkTmp;

        // Set known keys in circuit
        Abc_NtkForEachPi(pGlobalBsiKeys->pKeyCnf, pPi, i) {
            // Are we looking at a key input?
            if (strstr(Abc_ObjName(pPi), "key")) {
                if (ClapAttack_SetKnownKeys(pGlobalBsiKeys->pKeyCnf, pPi, pGlobalBsiKeys)) Abc_NtkDeleteObj(pPi);
            }
        }

        // Strash network, delete the old one, latch the new cleaned/reduced one as
        // the global network.
        pNtkTmp = Abc_NtkStrash(pGlobalBsiKeys->pKeyCnf, 0, 1, 0);
        Abc_NtkDelete(pGlobalBsiKeys->pKeyCnf);
        pGlobalBsiKeys->pKeyCnf = pNtkTmp;
    }
}

// Delete and remove any PI nodes in the network for known key inputs
void ClapAttack_DelKnownKeys(Abc_Obj_t **ppNodeList, int NumKnownKeys) {
    int i;

    for (i = 0; i < NumKnownKeys; i++) {
        // Remove the node.
        Abc_NtkDeleteObj(ppNodeList[i]);
    }
}

// hardcode the known key value into our network for any inferred key values during the CLAP attack.
// This replaces the PI for the key input with a hard-coded constant and logically reduces the network.
int ClapAttack_SetKnownKeys(Abc_Ntk_t *pNtkCone, Abc_Obj_t *pKey, struct BSI_KeyData_t *pGlobalBsiKeys) {
    int keyindex;

    // Grab the key index
    keyindex = (int)strtol(&((Abc_ObjName(pKey))[8]), (char **)NULL, 10);

    // Check if we know this key index
    if (pGlobalBsiKeys->KeyValue[keyindex] != -1) {
        // We know it! -- Replace it with a constant of the proper value
        if (pGlobalBsiKeys->KeyValue[keyindex])
            Abc_ObjReplaceByConstant(pKey, 1);
        else
            Abc_ObjReplaceByConstant(pKey, 0);

        return 1;
    }

    return 0;
}

// Apply the oracle circuits key to the global network in order to properly simulate
// EOFM probing.
void ClapAttack_OracleSetConeKeys(Abc_Ntk_t *pNtk, int *pInp, int *pOracleKey) {
    int keyindex, i;
    Abc_Obj_t *pPi;

    // Goal: Iterate through each PI. Identify list of keys.
    Abc_NtkForEachPi(pNtk, pPi, i) {
        // Are we looking at a key input?
        if (strstr(Abc_ObjName(pPi), "key")) {
            // Grab the key index
            keyindex = (int)strtol(&((Abc_ObjName(pPi))[8]), (char **)NULL, 10);

            pInp[i] = pOracleKey[keyindex];
            pInp[i + Abc_NtkPiNum(pNtk)] = pOracleKey[keyindex];
        }
    }
}

// Print the input value applied to the network to terminal.
void ClapAttack_PrintInp(Abc_Ntk_t *pNtk, int *pInp) {
    int i;

    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        Abc_Print(1, "Input %d: %d\n", i, pInp[i]);
    }
}

// Print the output value of the network to terminal.
void ClapAttack_PrintOut(Abc_Ntk_t *pNtk, int *pOut) {
    int i;

    for (i = 0; i < Abc_NtkPoNum(pNtk); i++) {
        Abc_Print(1, "Output %d: %d\n", i, pOut[i]);
    }
}

// Debug command to dump a network passed as an argument to a filename passed as an argument.
// Dumps file as a bench file.
int ClapAttack_WriteMiterBench(Abc_Ntk_t *pNtk, char *pFileName) {
    Abc_Ntk_t *pNtkTemp;
    pNtkTemp = Abc_NtkToNetlist(pNtk);
    Abc_NtkToAig(pNtkTemp);
    Io_WriteBenchLut(pNtkTemp, pFileName);
    Abc_NtkDelete(pNtkTemp);

    return 0;
}

// Debug command to dump a network passed as an argument to a filename passed as an argument.
// Dumps file as a verilog file.
int ClapAttack_WriteMiterVerilog(Abc_Ntk_t *pNtk, char *pFileName) {
    Io_Write(pNtk, pFileName, IO_FILE_VERILOG);

    return 0;
}

// Function that renames both PIs and POs in the circuit to generic names.
// To avoid name collision and standardize PI/PO names, rename them to a generic names.
// It's a hack for now. It is extremely hard to track PI/PO relationships between
// copies of circuit otherwise.
int ClapAttack_RenameLogic(Abc_Ntk_t *pNtk, char *PiSuffix, char *KeySuffix, char *PoSuffix, int KeepName) {
    Abc_Obj_t *pObj;
    int nDigits, i, k, CountCur, CountMax = 0;
    char *pName, PrefLi[100], PrefLo[100], **NameTmp;

    // How many zeros od we need to prepend on pi names...
    nDigits = Abc_Base10Log(Abc_NtkPiNum(pNtk));

    // Malloc name array
    NameTmp = (char **)malloc(sizeof(char *) * Abc_NtkPiNum(pNtk));
    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        NameTmp[i] = (char *)malloc(sizeof(char) * 100);
    }

    // Generate Pi Name List
    if (KeepName) {
        Abc_NtkForEachPi(pNtk, pObj, i) {
            if (strstr(Abc_ObjName(pObj), "key"))
                sprintf(NameTmp[i], "%s%s", Abc_ObjName(pObj), KeySuffix);
            else
                sprintf(NameTmp[i], "%s%s", Abc_ObjName(pObj), PiSuffix);
        }
    } else {
        Abc_NtkForEachPi(pNtk, pObj, i) {
            if (strstr(Abc_ObjName(pObj), "key"))
                sprintf(NameTmp[i], "pi%s_key%s%s", Abc_ObjNameDummy("", i, nDigits), &((Abc_ObjName(pObj))[8]),
                        KeySuffix);
            else
                sprintf(NameTmp[i], "pi%s%s", Abc_ObjNameDummy("", i, nDigits), PiSuffix);
        }
    }

    // Delete all names
    Nm_ManFree(pNtk->pManName);
    pNtk->pManName = Nm_ManCreate(Abc_NtkCiNum(pNtk) + Abc_NtkCoNum(pNtk) + Abc_NtkBoxNum(pNtk));

    // Rename Pis
    Abc_NtkForEachPi(pNtk, pObj, i) { Abc_ObjAssignName(pObj, NameTmp[i], NULL); }

    // Rename Pos
    nDigits = Abc_Base10Log(Abc_NtkPoNum(pNtk));
    Abc_NtkForEachPo(pNtk, pObj, i) Abc_ObjAssignName(pObj, Abc_ObjNameDummy("po", i, nDigits), PoSuffix);

    // Other logic
    // if PIs/POs already have nodes with what looks like latch names
    // we need to add different prefix for the new latches
    Abc_NtkForEachPi(pNtk, pObj, i) {
        CountCur = 0;
        pName = Abc_ObjName(pObj);
        for (k = 0; pName[k]; k++)
            if (pName[k] == 'l')
                CountCur++;
            else
                break;
        CountMax = Abc_MaxInt(CountMax, CountCur);
    }
    Abc_NtkForEachPo(pNtk, pObj, i) {
        CountCur = 0;
        pName = Abc_ObjName(pObj);
        for (k = 0; pName[k]; k++)
            if (pName[k] == 'l')
                CountCur++;
            else
                break;
        CountMax = Abc_MaxInt(CountMax, CountCur);
    }

    assert(CountMax < 100 - 2);
    for (i = 0; i <= CountMax; i++) PrefLi[i] = PrefLo[i] = 'l';
    PrefLi[i] = 'i';
    PrefLo[i] = 'o';
    PrefLi[i + 1] = 0;
    PrefLo[i + 1] = 0;

    // create latch names
    assert(!Abc_NtkIsNetlist(pNtk));
    nDigits = Abc_Base10Log(Abc_NtkLatchNum(pNtk));
    Abc_NtkForEachLatch(pNtk, pObj, i) {
        Abc_ObjAssignName(pObj, Abc_ObjNameDummy("l", i, nDigits), PoSuffix);
        Abc_ObjAssignName(Abc_ObjFanin0(pObj), Abc_ObjNameDummy(PrefLi, i, nDigits), PoSuffix);
        Abc_ObjAssignName(Abc_ObjFanout0(pObj), Abc_ObjNameDummy(PrefLo, i, nDigits), PoSuffix);
    }

    // Cleanup allocated names
    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        free(NameTmp[i]);
    }
    free(NameTmp);

    return 0;
}

// To avoid input collision and standardize inputs, rename them to a generic names.
// It's a hack for now. It is extremely hard to track PI/PO relationships between
// copies of circuit otherwise.
int ClapAttack_RenameInput(Abc_Ntk_t *pNtk, char *PiSuffix, char *KeySuffix, char *PoSuffix, int nDigits) {
    Abc_Obj_t *pObj;
    int i, k, CountCur, CountMax = 0;
    char *pName, PrefLi[100], PrefLo[100], **NameTmpPo, **NameTmpPi;

    // Malloc name array
    NameTmpPo = (char **)malloc(sizeof(char *) * Abc_NtkPoNum(pNtk));
    for (i = 0; i < Abc_NtkPoNum(pNtk); i++) {
        NameTmpPo[i] = (char *)malloc(sizeof(char) * 100);
    }

    // Generate Po Name List
    Abc_NtkForEachPo(pNtk, pObj, i) { sprintf(NameTmpPo[i], "%s%s", Abc_ObjName(pObj), PoSuffix); }

    // Malloc name array
    NameTmpPi = (char **)malloc(sizeof(char *) * Abc_NtkPiNum(pNtk));
    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        NameTmpPi[i] = (char *)malloc(sizeof(char) * 100);
    }

    // Generate Pi Name List
    Abc_NtkForEachPi(pNtk, pObj, i) {
        if (strstr(Abc_ObjName(pObj), "key"))
            sprintf(NameTmpPi[i], "pi%s_key%s%s", Abc_ObjNameDummy("", i, nDigits), &((Abc_ObjName(pObj))[8]),
                    KeySuffix);
        else
            sprintf(NameTmpPi[i], "pi%s%s", Abc_ObjNameDummy("", i, nDigits), PiSuffix);
    }

    // Delete all names
    Nm_ManFree(pNtk->pManName);
    pNtk->pManName = Nm_ManCreate(Abc_NtkCiNum(pNtk) + Abc_NtkCoNum(pNtk) + Abc_NtkBoxNum(pNtk));

    // Rename POs
    Abc_NtkForEachPo(pNtk, pObj, i) { Abc_ObjAssignName(pObj, NameTmpPo[i], NULL); }

    // Rename PIs
    Abc_NtkForEachPi(pNtk, pObj, i) { Abc_ObjAssignName(pObj, NameTmpPi[i], NULL); }

    // Other logic
    // if PIs/POs already have nodes with what looks like latch names
    // we need to add different prefix for the new latches
    Abc_NtkForEachPi(pNtk, pObj, i) {
        CountCur = 0;
        pName = Abc_ObjName(pObj);
        for (k = 0; pName[k]; k++)
            if (pName[k] == 'l')
                CountCur++;
            else
                break;
        CountMax = Abc_MaxInt(CountMax, CountCur);
    }
    Abc_NtkForEachPo(pNtk, pObj, i) {
        CountCur = 0;
        pName = Abc_ObjName(pObj);
        for (k = 0; pName[k]; k++)
            if (pName[k] == 'l')
                CountCur++;
            else
                break;
        CountMax = Abc_MaxInt(CountMax, CountCur);
    }

    assert(CountMax < 100 - 2);
    for (i = 0; i <= CountMax; i++) PrefLi[i] = PrefLo[i] = 'l';
    PrefLi[i] = 'i';
    PrefLo[i] = 'o';
    PrefLi[i + 1] = 0;
    PrefLo[i + 1] = 0;

    // create latch names
    assert(!Abc_NtkIsNetlist(pNtk));
    nDigits = Abc_Base10Log(Abc_NtkLatchNum(pNtk));
    Abc_NtkForEachLatch(pNtk, pObj, i) {
        Abc_ObjAssignName(pObj, Abc_ObjNameDummy("l", i, nDigits), PoSuffix);
        Abc_ObjAssignName(Abc_ObjFanin0(pObj), Abc_ObjNameDummy(PrefLi, i, nDigits), PoSuffix);
        Abc_ObjAssignName(Abc_ObjFanout0(pObj), Abc_ObjNameDummy(PrefLo, i, nDigits), PoSuffix);
    }

    // CLeanup PI/PO name storage
    for (i = 0; i < Abc_NtkPoNum(pNtk); i++) {
        free(NameTmpPo[i]);
    }
    free(NameTmpPo);

    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        free(NameTmpPi[i]);
    }
    free(NameTmpPi);

    return 0;
}

// To avoid output collision and standardize outputs, rename them to a generic name.
// It's a hack for now. It is extremely hard to track PI/PO relationships between
// copies of circuit otherwise.
int ClapAttack_RenameOutput(Abc_Ntk_t *pNtk, char *PoSuffix, int KeepName) {
    Abc_Obj_t *pObj;
    int nDigits, i, k, CountCur, CountMax = 0;
    char *pName, PrefLi[100], PrefLo[100], **NameTmp;

    // How many zeros do we need to prepend on pi names...
    nDigits = Abc_Base10Log(Abc_NtkPiNum(pNtk));

    // Malloc name array
    NameTmp = (char **)malloc(sizeof(char *) * Abc_NtkPiNum(pNtk));
    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        NameTmp[i] = (char *)malloc(sizeof(char) * 100);
    }

    // Generate Pi Name List
    Abc_NtkForEachPi(pNtk, pObj, i) { sprintf(NameTmp[i], "%s", Abc_ObjName(pObj)); }

    // Delete all old PO names
    Nm_ManFree(pNtk->pManName);
    pNtk->pManName = Nm_ManCreate(Abc_NtkCiNum(pNtk) + Abc_NtkCoNum(pNtk) + Abc_NtkBoxNum(pNtk));

    // Iterate PIs
    Abc_NtkForEachPi(pNtk, pObj, i) { Abc_ObjAssignName(pObj, NameTmp[i], NULL); }

    // Now do the same for POs
    nDigits = Abc_Base10Log(Abc_NtkPoNum(pNtk));
    Abc_NtkForEachPo(pNtk, pObj, i) Abc_ObjAssignName(pObj, Abc_ObjNameDummy("po", i, nDigits), PoSuffix);

    // Other logic
    // if PIs/POs already have nodes with what looks like latch names
    // we need to add different prefix for the new latches
    Abc_NtkForEachPi(pNtk, pObj, i) {
        CountCur = 0;
        pName = Abc_ObjName(pObj);
        for (k = 0; pName[k]; k++)
            if (pName[k] == 'l')
                CountCur++;
            else
                break;
        CountMax = Abc_MaxInt(CountMax, CountCur);
    }
    Abc_NtkForEachPo(pNtk, pObj, i) {
        CountCur = 0;
        pName = Abc_ObjName(pObj);
        for (k = 0; pName[k]; k++)
            if (pName[k] == 'l')
                CountCur++;
            else
                break;
        CountMax = Abc_MaxInt(CountMax, CountCur);
    }
    assert(CountMax < 100 - 2);
    for (i = 0; i <= CountMax; i++) PrefLi[i] = PrefLo[i] = 'l';
    PrefLi[i] = 'i';
    PrefLo[i] = 'o';
    PrefLi[i + 1] = 0;
    PrefLo[i + 1] = 0;

    // Generate new names and assign them to POs
    assert(!Abc_NtkIsNetlist(pNtk));
    nDigits = Abc_Base10Log(Abc_NtkLatchNum(pNtk));
    Abc_NtkForEachLatch(pNtk, pObj, i) {
        Abc_ObjAssignName(pObj, Abc_ObjNameDummy("l", i, nDigits), PoSuffix);
        Abc_ObjAssignName(Abc_ObjFanin0(pObj), Abc_ObjNameDummy(PrefLi, i, nDigits), PoSuffix);
        Abc_ObjAssignName(Abc_ObjFanout0(pObj), Abc_ObjNameDummy(PrefLo, i, nDigits), PoSuffix);
    }

    // Cleanup
    for (i = 0; i < Abc_NtkPiNum(pNtk); i++) {
        free(NameTmp[i]);
    }
    free(NameTmp);

    return 0;
}

// Determine number of key inputs in network
int ClapAttack_GetNumKeys(Abc_Ntk_t *pNtk) {
    int i, NumKeys = 0;
    Abc_Obj_t *pPi;

    Abc_NtkForEachPi(pNtk, pPi, i) {
        // Are we looking at a key input?
        if (strstr(Abc_ObjName(pPi), "keyinput")) {
            NumKeys++;
        }
    }

    return NumKeys;
}

// Initialize keystore which holds known key bits generated from the CLAP attack.
void ClapAttack_InitKeyStore(int NumKeys, struct BSI_KeyData_t *pGlobalBsiKeys) {
    int i;

    pGlobalBsiKeys->NumKeys = NumKeys;
    pGlobalBsiKeys->Updated = 1;
    pGlobalBsiKeys->KeyValue = (int *)malloc(sizeof(int) * NumKeys);

    for (i = 0; i < NumKeys; i++) {
        pGlobalBsiKeys->KeyValue[i] = -1;
    }

    pGlobalBsiKeys->pKeyCnf = NULL;
}

// Create a duplciate copy of the known key values
void ClapAttack_CopyKeyStore(struct BSI_KeyData_t *pGlobalBsiKeys, struct BSI_KeyData_t *pGlobalBsiKeysNew) {
    int i;

    ClapAttack_InitKeyStore(pGlobalBsiKeys->NumKeys, pGlobalBsiKeysNew);

    pGlobalBsiKeysNew->Updated = pGlobalBsiKeys->Updated;

    for (i = 0; i < pGlobalBsiKeys->NumKeys; i++) {
        pGlobalBsiKeysNew->KeyValue[i] = pGlobalBsiKeys->KeyValue[i];
    }
}

// Similar to miterkeys, we must also miter the identical POs constructed in the
// duplicated network in order to construct our SAT formultaion.
// Essentially at t1/t2 the output must be different from the attacked node,
// necessitating this miter circuit to be constructed to identify sensitizing inputs.
int ClapAttack_MiterPos(Abc_Ntk_t *pNtk, int fXorOr, int fXnorAnd) {
    Abc_Obj_t *pNode, **pMiterTmp, *pMiter = NULL;
    int i, NumPos;
    Abc_Obj_t **pPos;
    char PoTmpName[100];

    // Check strash status
    assert(Abc_NtkIsStrash(pNtk));

    // Check that we have an even number of Pos
    NumPos = Abc_NtkPoNum(pNtk);
    if (NumPos == 1) return 1;
    pPos = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * NumPos);
    pMiterTmp = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * (NumPos) / 2);

    // Generate PO list
    Abc_NtkForEachPo(pNtk, pNode, i) { pPos[i] = pNode; }

    for (i = 0; i < (NumPos / 2); i++) {
        // Generate miter connections between the 2 primary outputs with the same index at different times
        if (fXorOr)
            pMiterTmp[i] =
                Abc_AigXor((Abc_Aig_t *)pNtk->pManFunc, Abc_ObjChild0(pPos[i]), Abc_ObjChild0(pPos[i + (NumPos / 2)]));
        else
            pMiterTmp[i] = Abc_ObjNot(
                Abc_AigXor((Abc_Aig_t *)pNtk->pManFunc, Abc_ObjChild0(pPos[i]), Abc_ObjChild0(pPos[i + (NumPos / 2)])));
    }

    // remove the POs and their names
    for (i = Abc_NtkPoNum(pNtk) - 1; i >= 0; i--) Abc_NtkDeleteObj(Abc_NtkPo(pNtk, i));
    assert(Abc_NtkPoNum(pNtk) == 0);

    // create new POs
    for (i = 0; i < (NumPos / 2); i++) {
        pNode = Abc_NtkCreatePo(pNtk);
        Abc_ObjAddFanin(pNode, pMiterTmp[i]);
        if (NumPos / 2 == 1) {
            Abc_ObjAssignName(pNode, "miter", NULL);
        } else {
            sprintf(PoTmpName, "miter_%d", i);
            Abc_ObjAssignName(pNode, PoTmpName, NULL);
        }
        Abc_NtkOrderCisCos(pNtk);
    }

    // make sure that everything is okay
    if (!Abc_NtkCheck(pNtk)) {
        printf("Abc_NtkOrPos: The network check has failed.\n");
        return 0;
    }

    // Generate PO list
    Abc_NtkForEachPo(pNtk, pNode, i) {
        // Generate miter connections between the 2 copies of each key input
        if (i == 0) {
            pMiter = Abc_ObjChild0(pNode);
        } else {
            if (fXorOr) {
                pMiter = Abc_AigOr((Abc_Aig_t *)pNtk->pManFunc, pMiter, Abc_ObjChild0(pNode));
            } else {
                pMiter = Abc_AigAnd((Abc_Aig_t *)pNtk->pManFunc, pMiter, Abc_ObjChild0(pNode));
            }
        }
    }

    // remove the POs and their names
    for (i = Abc_NtkPoNum(pNtk) - 1; i >= 0; i--) Abc_NtkDeleteObj(Abc_NtkPo(pNtk, i));
    assert(Abc_NtkPoNum(pNtk) == 0);

    // create the new PO
    pNode = Abc_NtkCreatePo(pNtk);
    Abc_ObjAddFanin(pNode, pMiter);
    Abc_ObjAssignName(pNode, "miter", NULL);
    Abc_NtkOrderCisCos(pNtk);

    // make sure that everything is okay
    if (!Abc_NtkCheck(pNtk)) {
        printf("Abc_NtkOrPos: The network check has failed.\n");
        return 0;
    }

    // Memory management...
    free(pPos);
    free(pMiterTmp);

    return 1;
}

// Miter key inputs -- We are constructing the key miter
// That ensures that key1 and key2 satisfying the
// CLAP formulation are different.
int ClapAttack_MiterKeys(Abc_Ntk_t *pNtk) {
    Abc_Obj_t *pNode, **pMiterTmp, *pMiter = NULL;
    int i, j, NumKeys = 0, KeyIdx1 = 0, KeyIdx2 = 0;
    Abc_Obj_t **pKey1, **pKey2;
    char PoTmpName[100];

    // Check strash status
    assert(Abc_NtkIsStrash(pNtk));

    // Check that we have an even number of Keys
    Abc_NtkForEachPi(pNtk, pNode, i) {
        // Is it a key?
        if (strstr(Abc_ObjName(pNode), "key")) {
            NumKeys++;
        }
    }

    // Ensure we have an even number of keys
    if ((NumKeys % 2)) return 1;
    pKey1 = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * (NumKeys / 2));
    pKey2 = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * (NumKeys / 2));
    pMiterTmp = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * (NumKeys / 2));

    // Check that we have an even number of Keys
    Abc_NtkForEachPi(pNtk, pNode, i) {
        // Is it a key?
        if ((strstr(Abc_ObjName(pNode), "key")) && (strstr(Abc_ObjName(pNode), "_1"))) {
            pKey1[KeyIdx1] = pNode;
            KeyIdx1++;
        }
        if ((strstr(Abc_ObjName(pNode), "key")) && (strstr(Abc_ObjName(pNode), "_2"))) {
            pKey2[KeyIdx2] = pNode;
            KeyIdx2++;
        }
    }

    // Find the key in each circuit that shares an index. They need to be mitered to guarantee that they differ.
    for (i = 0; i < KeyIdx1; i++) {
        for (j = 0; j < KeyIdx2; j++) {
            // Generate miter connections between the two versions of the same key
            if (!ClapAttack_CmpKeyName(Abc_ObjName(pKey1[i]), Abc_ObjName(pKey2[j]), strlen(Abc_ObjName(pKey1[i])),
                                       strlen(Abc_ObjName(pKey2[j])))) {
                pMiterTmp[i] = Abc_AigXor((Abc_Aig_t *)pNtk->pManFunc, pKey1[i], pKey2[j]);
                break;
            }
        }
    }

    // remove the POs and their names
    for (i = Abc_NtkPoNum(pNtk) - 1; i >= 0; i--) Abc_NtkDeleteObj(Abc_NtkPo(pNtk, i));
    assert(Abc_NtkPoNum(pNtk) == 0);

    // create new POs
    for (i = 0; i < (NumKeys / 2); i++) {
        pNode = Abc_NtkCreatePo(pNtk);
        Abc_ObjAddFanin(pNode, pMiterTmp[i]);
        if (NumKeys / 2 == 1) {
            Abc_ObjAssignName(pNode, "key_miter", NULL);
        } else {
            sprintf(PoTmpName, "key_miter_%d", i);
            Abc_ObjAssignName(pNode, PoTmpName, NULL);
        }
        Abc_NtkOrderCisCos(pNtk);
    }

    // Make sure we did not damage the network while modifying it
    if (!Abc_NtkCheck(pNtk)) {
        printf("Abc_NtkOrPos: The network check has failed.\n");
        return 0;
    }

    // Generate PO list
    Abc_NtkForEachPo(pNtk, pNode, i) {
        // Generate miter connections between oones
        if (i == 0) {
            pMiter = Abc_ObjChild0(pNode);
        } else {
            pMiter = Abc_AigOr((Abc_Aig_t *)pNtk->pManFunc, pMiter, Abc_ObjChild0(pNode));
        }
    }

    // remove the POs and their names
    for (i = Abc_NtkPoNum(pNtk) - 1; i >= 0; i--) Abc_NtkDeleteObj(Abc_NtkPo(pNtk, i));
    assert(Abc_NtkPoNum(pNtk) == 0);

    // create the new PO
    pNode = Abc_NtkCreatePo(pNtk);
    Abc_ObjAddFanin(pNode, pMiter);
    Abc_ObjAssignName(pNode, "keymiter", NULL);
    Abc_NtkOrderCisCos(pNtk);

    // Make sure we did not damage the network while modifying it
    if (!Abc_NtkCheck(pNtk)) {
        printf("Abc_NtkOrPos: The network check has failed.\n");
        return 0;
    }

    // Free all allocated memory
    free(pKey1);
    free(pKey2);
    free(pMiterTmp);

    return 1;
}

// Generate the partial key miter capable of excluding any keys that were determined to
// NOT be capable of producing EOFM observed behavior. This effectively eliminates these
// eliminated keys from being valid logical solutions to the CLAP miter circuit to identify
// key leakage.
int ClapAttack_PartialKeyInferenceMiter(Abc_Ntk_t *pNtk, Abc_Ntk_t **ppNtkMiter, char *CurKeyName) {
    Abc_Ntk_t *pNtkMiterTmp, *ntkTmp;
    Abc_Obj_t *pNode, **ppNodeFreeList;
    int i, numKeysRemoved;
    int nDigits;

    // Initialzie free list to the number of keys present...
    ppNodeFreeList = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * (Abc_NtkPiNum(pNtk) * 2));
    numKeysRemoved = 0;

    *ppNtkMiter = Abc_NtkDup(pNtk);
    pNtkMiterTmp = Abc_NtkDup(pNtk);

    // Convert to logic
    ntkTmp = Abc_NtkToLogic(*ppNtkMiter);
    Abc_NtkDelete(*ppNtkMiter);
    *ppNtkMiter = ntkTmp;

    ntkTmp = Abc_NtkToLogic(pNtkMiterTmp);
    Abc_NtkDelete(pNtkMiterTmp);
    pNtkMiterTmp = ntkTmp;

    // Replace key of interest by constants
    Abc_NtkForEachPi(*ppNtkMiter, pNode, i) {
        if (!strcmp(Abc_ObjName(pNode), CurKeyName)) {
            Abc_ObjReplaceByConstant(pNode, 0);
            ppNodeFreeList[numKeysRemoved] = pNode;
            numKeysRemoved++;
        }
    }

    Abc_NtkForEachPi(pNtkMiterTmp, pNode, i) {
        if (!strcmp(Abc_ObjName(pNode), CurKeyName)) {
            Abc_ObjReplaceByConstant(pNode, 1);
            ppNodeFreeList[numKeysRemoved] = pNode;
            numKeysRemoved++;
        }
    }

    // Delete known keys from the network copy
    if (numKeysRemoved) ClapAttack_DelKnownKeys(ppNodeFreeList, numKeysRemoved);

    // Clean up allocated memory here to avoid memory leak
    free(ppNodeFreeList);

    // Rename Po to avoid collision
    nDigits = Abc_Base10Log(Abc_NtkPiNum(*ppNtkMiter));
    ClapAttack_RenameInput(*ppNtkMiter, "", "_1", "_1", nDigits);
    ClapAttack_RenameInput(pNtkMiterTmp, "", "_2", "_2", nDigits);

    // Strash the networks
    ntkTmp = Abc_NtkStrash(*ppNtkMiter, 0, 1, 0);
    Abc_NtkDelete(*ppNtkMiter);
    *ppNtkMiter = ntkTmp;

    ntkTmp = Abc_NtkStrash(pNtkMiterTmp, 0, 1, 0);
    Abc_NtkDelete(pNtkMiterTmp);
    pNtkMiterTmp = ntkTmp;

    // Append the two networks we prepped for miter 1
    if (!Abc_NtkAppendSilent(*ppNtkMiter, pNtkMiterTmp, 1)) {
        Abc_Print(-1, "Appending the networks failed 8.\n");
        return 1;
    }

    Abc_NtkDelete(pNtkMiterTmp);

    // Generate the final miter cone
    extern int Abc_NtkCombinePos(Abc_Ntk_t * pNtk, int fAnd, int fXor);

    if (!Abc_NtkCombinePos(*ppNtkMiter, 1, 0)) {
        Abc_Print(-1, "ANDing the POs has failed.\n");
        return 1;
    }

    return 0;
}

// Compare 2 key names to see if they are the same.
int ClapAttack_CmpKeyName(char *KeyName1, char *KeyName2, int KeyLen1, int KeyLen2) {
    char *Key1Tmp = (char *)malloc(sizeof(char) * (KeyLen1 + 1));
    char *Key2Tmp = (char *)malloc(sizeof(char) * (KeyLen2 + 1));
    int tmpFlag;

    // Copy the strings to their temporary holders
    strcpy(Key1Tmp, KeyName1);
    strcpy(Key2Tmp, KeyName2);

    // Cut the suffix off (i.e. "_1")
    Key1Tmp[KeyLen1 - 2] = '\0';
    Key2Tmp[KeyLen2 - 2] = '\0';

    tmpFlag = strcmp(Key1Tmp, Key2Tmp);
    free(Key1Tmp);
    free(Key2Tmp);

    return tmpFlag;
}

// After running the physical portion of the CLAP attack, we need to dump all physically
// leaked key info in to a format that can be read in by the open-source SAT attack tool
// by Pramod et al (HOST'15)
void ClapAttack_GenSatAttackConfig(Abc_Ntk_t *pNtk, struct BSI_KeyData_t *pGlobalBsiKeys, char *pOutFile) {
    Abc_Ntk_t *pNtkTmp, *pNtkTmpSwap, *pNtkFinal;
    Abc_Obj_t *pObj, *pPi, *pPo, *pNode, *pNode2, **ppKnownKeys, *pObj2, **ppPoVec;
    Fraig_Params_t Params;
    int i, j, numKnownKeys;
    char tmpName[100];

    ppPoVec = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * Abc_NtkPoNum(pNtk));

    // Miter in partial key info.
    pNtkFinal = Abc_NtkDup(pNtk);

    // Strash the networks
    pNtkTmpSwap = Abc_NtkStrash(pNtkFinal, 0, 1, 0);
    Abc_NtkDelete(pNtkFinal);
    pNtkFinal = pNtkTmpSwap;

    // Convert to logic so we can use existing constant functions
    pNtkTmpSwap = Abc_NtkToLogic(pNtkFinal);
    Abc_NtkDelete(pNtkFinal);
    pNtkFinal = pNtkTmpSwap;

    numKnownKeys = 0;
    // Set known keys in circuit
    Abc_NtkForEachPi(pNtkFinal, pPi, i) {
        // Are we looking at a key input?
        if (strstr(Abc_ObjName(pPi), "key")) {
            if (ClapAttack_SetKnownKeys(pNtkFinal, pPi, pGlobalBsiKeys)) {
                printf("Deleted key: %s\n", Abc_ObjName(pPi));
                numKnownKeys++;
            }
        }
    }

    // Iterate over the entire network and replace any known keys
    ppKnownKeys = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * numKnownKeys);
    j = 0;
    Abc_NtkForEachPi(pNtkFinal, pPi, i) {
        // Are we looking at a key input?
        if (strstr(Abc_ObjName(pPi), "key")) {
            if (ClapAttack_SetKnownKeys(pNtkFinal, pPi, pGlobalBsiKeys)) {
                ppKnownKeys[j] = pPi;
                j++;
            }
        }
    }

    for (j = 0; j < numKnownKeys; j++) Abc_NtkDeleteObj(ppKnownKeys[j]);

    free(ppKnownKeys);

    // Strash the networks
    pNtkTmpSwap = Abc_NtkStrash(pNtkFinal, 0, 1, 0);
    Abc_NtkDelete(pNtkFinal);
    pNtkFinal = pNtkTmpSwap;

    // Edge case - handle any partial key information --
    // Essentially we are mitering in any partial key logic
    // that has been generated over the course of hte attack
    if (pGlobalBsiKeys->pKeyCnf) {
        // Iterate over every PO in the circuit
        Abc_NtkForEachCo(pNtkFinal, pObj2, j) { ppPoVec[j] = pObj2; }

        pNtkTmp = Abc_NtkDup(pGlobalBsiKeys->pKeyCnf);

        // Strash the networks
        pNtkTmpSwap = Abc_NtkStrash(pNtkTmp, 0, 1, 0);
        Abc_NtkDelete(pNtkTmp);
        pNtkTmp = pNtkTmpSwap;

        // Convert to logic so we can use existing constant functions
        pNtkTmpSwap = Abc_NtkToLogic(pNtkTmp);
        Abc_NtkDelete(pNtkTmp);
        pNtkTmp = pNtkTmpSwap;

        numKnownKeys = 0;
        // Set known keys in circuit
        Abc_NtkForEachPi(pNtkTmp, pPi, i) {
            // Are we looking at a key input?
            if (strstr(Abc_ObjName(pPi), "key")) {
                if (ClapAttack_SetKnownKeys(pNtkTmp, pPi, pGlobalBsiKeys)) {
                    printf("Deleted key: %s\n", Abc_ObjName(pPi));
                    numKnownKeys++;
                }
            }
        }

        // Find known keys and replace them with their intended value to simplify
        // circuit prior to launching SAT attack
        ppKnownKeys = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * numKnownKeys);
        j = 0;
        Abc_NtkForEachPi(pNtkTmp, pPi, i) {
            // Are we looking at a key input?
            if (strstr(Abc_ObjName(pPi), "key")) {
                if (ClapAttack_SetKnownKeys(pNtkTmp, pPi, pGlobalBsiKeys)) {
                    ppKnownKeys[j] = pPi;
                    j++;
                }
            }
        }

        // Delete any known keys in the network
        for (j = 0; j < numKnownKeys; j++) Abc_NtkDeleteObj(ppKnownKeys[j]);

        // Strash the network to clean now that we have removed
        // known key input values
        pNtkTmpSwap = Abc_NtkStrash(pNtkTmp, 0, 1, 0);
        Abc_NtkDelete(pNtkTmp);
        pNtkTmp = pNtkTmpSwap;

        // DEBUG, dump final circuit without partial key logic appended
        // Io_Write( pNtkTmp, "final_sat_dump_miter.v", IO_FILE_VERILOG );

        // Append together partial key logic
        if (!Abc_NtkAppendSilent(pNtkFinal, pNtkTmp, 1)) {
            Abc_NtkDelete(pNtkTmp);
            Abc_Print(-1, "Appending the networks failed.\n");
            return;
        }

        // Get fanin for PO
        Abc_NtkForEachCo(pNtkFinal, pObj, i) {
            if ((!strcmp(Abc_ObjName(pObj), "not_key_partial")) || (!strcmp(Abc_ObjName(pObj), "not_key"))) break;
        }

        // Get child of node so we can remove it without destroying
        // network data structure -- If we dont save this, we lose
        // ancestors of node.
        pNode = Abc_ObjChild0(Abc_NtkPo(pNtkFinal, i));

        // remove the PO and their names
        Abc_NtkDeleteObj(Abc_NtkPo(pNtkFinal, i));

        for (j = 0; j < Abc_NtkPoNum(pNtk); j++) {
            // Get child of node so we can remove it without destroying
            // network data structure
            pNode2 = Abc_ObjChild0(ppPoVec[j]);

            // remove the PO and their names
            strcpy(tmpName, Abc_ObjName(ppPoVec[j]));
            Abc_NtkDeleteObj(ppPoVec[j]);

            // create the new PO
            pPo = Abc_NtkCreatePo(pNtkFinal);
            Abc_ObjAddFanin(pPo, Abc_AigAnd((Abc_Aig_t *)(pNtkFinal)->pManFunc, pNode, pNode2));
            Abc_ObjAssignName(pPo, tmpName, NULL);
        }

        // Order the new POs added
        Abc_NtkOrderCisCos(pNtkFinal);

        // Delete temp files to prevent memory leak
        Abc_NtkDelete(pNtkTmp);
    }

    // Strash the networks
    pNtkTmpSwap = Abc_NtkStrash(pNtkFinal, 0, 1, 0);
    Abc_NtkDelete(pNtkFinal);
    pNtkFinal = pNtkTmpSwap;

    // Fraig the networks
    Fraig_ParamsSetDefault(&Params);
    pNtkTmpSwap = Abc_NtkFraig(pNtkFinal, &Params, 0, 0);
    Abc_NtkDelete(pNtkFinal);
    pNtkFinal = pNtkTmpSwap;

    // Dump final SAT attack bench file
    Io_Write(pNtkFinal, pOutFile, IO_FILE_BENCH);
}

Abc_Ntk_t* ClapAttack_UnrollNetwork(Abc_Ntk_t *pNtk, int nFrames, int fInitial, int fVerbose) {
    Abc_Ntk_t *pNtkTemp, *pNtkRes;


    printf("Unrolling network: %s, nFrames: %d, fInitial: %d, fVerbose: %d\n",
           pNtk ? pNtk->pName : "NULL", nFrames, fInitial, fVerbose);

    if (pNtk == NULL) {
        Abc_Print(-1, "Empty network.\n");
        return NULL;
    }

    if(nFrames == 0){
        Abc_Print(-1, "nFrames must be greater than 0.\n");
        return NULL;
    }

    // Check if network needs to be strashed
    if (!Abc_NtkIsStrash(pNtk)) {
        printf("Strashing network.\n");
        pNtkTemp = Abc_NtkStrash(pNtk, 0, 0, 0);
        if (pNtkTemp == NULL) {
            Abc_Print(-1, "Strashing failed.\n");
            return NULL;
        }
        pNtkRes = Abc_NtkFrames(pNtkTemp, nFrames, fInitial, fVerbose);
        Abc_NtkDelete(pNtkTemp);
    } else {
        printf("Network is already strashed.\n");
        pNtkRes = Abc_NtkFrames(pNtk, nFrames, fInitial, fVerbose);
    }

    // Check if unrolling was successful
    if (pNtkRes == NULL) {
        Abc_Print(-1, "Unrolling the network has failed.\n");
        return NULL;
    }

    printf("Unrolled network successfully.\n");

    return pNtkRes;
}


int ClapAttack_VerifyCircuitCompatibility(Abc_Ntk_t* pSeqNtk, Abc_Ntk_t* pLockedNtk) {
    Abc_Obj_t* pObjSeq;
    Abc_Obj_t* pObjLocked;
    int i, j;
    char* seqOutputName;
    char* lockedInputName;
    int found;

    Abc_NtkForEachPi(pLockedNtk, pObjLocked, i) {
        lockedInputName = Abc_ObjName(pObjLocked);

        const char* key = "key";
        int containsKey = 0; // Flag to indicate if 'key' is found in the name
        for (const char* p = lockedInputName; *p; p++) {
            if (tolower(*p) == tolower(key[0])) {
                const char* q = p, *r = key;
                while (*q && *r && tolower(*q) == tolower(*r)) {
                    q++, r++;
                }
                if (!*r) {
                    containsKey = 1; // Found 'key'
                    break; // Exit the loop as 'key' is found
                }
            }
        }

        if (containsKey) {
            continue; // Skip this input as it is labeled with 'key'
        }

        found = 0;

        // For each input, search through outputs of the sequential circuit
        Abc_NtkForEachPo(pSeqNtk, pObjSeq, j) {
            seqOutputName = Abc_ObjName(Abc_ObjFanin0(pObjSeq));

            if (strcmp(lockedInputName, seqOutputName) == 0) {
                found = 1; // Exact match found
                break;
            }
        }

        if (!found) {
            printf("Mismatch: Logic-locked input '%s' has no corresponding sequential output.\n", lockedInputName);
            return 0; // Incompatibility found
        }
    }

    return 1; // Compatible
}

int ClapAttack_SaveNetworkToFile(Abc_Ntk_t *pNtk, const char *filename) {
    if (pNtk == NULL) {
        printf("No current network to save.\n");
        return -1;
    }

    pNtk = Abc_NtkStrash(pNtk, 0, 0, 0);
    if (pNtk == NULL) {
        printf("Strashing failed.\n");
        return -1;
    }

    char tempFilename[strlen(filename) + 1]; // +1 for null terminator
    strcpy(tempFilename, filename);
    Io_Write(pNtk, tempFilename, IO_FILE_BENCH);
    return 0;
}


Abc_Ntk_t* ClapAttack_LoadNetworkFromFile(const char *filename) {
    int fCheck = 1; // Assuming we always want to perform the check as default

    // No need for tempFilename, Io_Read can handle const char* directly
    Abc_Ntk_t* pNtk = Io_Read((char *)filename, IO_FILE_BENCH, fCheck, 0);

    if (pNtk == NULL) {
        printf("Failed to load network from file: %s\n", filename);
        return NULL; // Indicate failure
    }

    return pNtk; // Return the loaded network
}


int ClapAttack_ExtractCycleNumber(const char* name) {
    const char* underscore = strrchr(name, '_');
    return underscore ? atoi(underscore + 1) : -1;
}


void ClapAttack_MiterReachability(Abc_Ntk_t* pUnrolledMiter, Abc_Ntk_t* pNtkMiter, int unrollTimes) {

    if (!Abc_NtkIsStrash(pUnrolledMiter)) {
        printf("The network is not in AIG format. Converting...\n");
        pUnrolledMiter = Abc_NtkStrash(pUnrolledMiter, 0, 0, 0);
    }

    Abc_Obj_t *pObj;
    int i = 0;

    char **nonKeyInputNames1 = NULL, **nonKeyInputNames2 = NULL; 
    int *nonKeyInputTargets1 = NULL, *nonKeyInputTargets2 = NULL; 
    int nonKeyInputCount1 = 0, nonKeyInputCount2 = 0;
    i = 0;

    // Count inputs for each cycle
    Abc_NtkForEachPi(pNtkMiter, pObj, i) {
        const char* objName = Abc_ObjName(pObj);
        if (!ClapAttack_SubstringExists(objName, "key")) {
            if (strstr(objName, "_1") != NULL) {
                nonKeyInputCount1++;
            } else if (strstr(objName, "_2") != NULL) {
                nonKeyInputCount2++;
            }
        }
    }

    // Allocate memory based on counts
    nonKeyInputNames1 = (char**)malloc(nonKeyInputCount1 * sizeof(char*));
    nonKeyInputTargets1 = (int*)malloc(nonKeyInputCount1 * sizeof(int));
    nonKeyInputNames2 = (char**)malloc(nonKeyInputCount2 * sizeof(char*));
    nonKeyInputTargets2 = (int*)malloc(nonKeyInputCount2 * sizeof(int));

    if (!nonKeyInputNames1 || !nonKeyInputNames2 || !nonKeyInputTargets1 || !nonKeyInputTargets2) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Identifies the inputs that are not key as well as their target value
    int j1 = 0, j2 = 0; // Indexes for each i/o set 
    Abc_NtkForEachPi(pNtkMiter, pObj, i) {
        const char* objName = Abc_ObjName(pObj);
        if (!ClapAttack_SubstringExists(objName, "key")) {
            int nameLength = strchr(objName, '_') ? (strchr(objName, '_') - objName) : strlen(objName);
            if (strstr(objName, "_1") != NULL) {
                nonKeyInputNames1[j1] = (char*)malloc((nameLength + 1) * sizeof(char));
                strncpy(nonKeyInputNames1[j1], objName, nameLength);
                nonKeyInputNames1[j1][nameLength] = '\0'; // Null-terminate
                nonKeyInputTargets1[j1] = pNtkMiter->pModel[i];
                j1++;
            } else if (strstr(objName, "_2") != NULL) {
                nonKeyInputNames2[j2] = (char*)malloc((nameLength + 1) * sizeof(char));
                strncpy(nonKeyInputNames2[j2], objName, nameLength);
                nonKeyInputNames2[j2][nameLength] = '\0'; // Null-terminate
                nonKeyInputTargets2[j2] = pNtkMiter->pModel[i];
                j2++;
            }
        }
    }
    
    Abc_Obj_t* orResult1 = Abc_ObjNot(Abc_AigConst1(pUnrolledMiter)); // Start with 0 for OR operation within the cycle for input 1
    Abc_Obj_t* orResult2 = Abc_ObjNot(Abc_AigConst1(pUnrolledMiter)); // Start with 0 for OR operation within the cycle for input 2

    for (size_t cycle = 0; cycle < unrollTimes; cycle++) {
        Abc_Obj_t* andResult1 = Abc_AigConst1(pUnrolledMiter); // Start with 1 for AND operation within the cycle for input 1
        Abc_Obj_t* andResult2 = Abc_AigConst1(pUnrolledMiter); // Start with 1 for AND operation within the cycle for input 2
        
        // Process distinguishing input 1
        Abc_NtkForEachPo(pUnrolledMiter, pObj, i) {
            if (ClapAttack_ExtractCycleNumber(Abc_ObjName(pObj)) == cycle) {
                for (size_t j = 0; j < nonKeyInputCount1; j++) {
                    if (ClapAttack_SubstringExists(Abc_ObjName(pObj), nonKeyInputNames1[j])) {
                        Abc_Obj_t* pSignal = Abc_ObjChild0(pObj); // Get the signal driving the PO
                        if (nonKeyInputTargets1[j] == 0) {
                            pSignal = Abc_ObjNot(pSignal); // Invert the signal if target is 0
                        }
                        andResult1 = Abc_AigAnd((Abc_Aig_t *)pUnrolledMiter->pManFunc, andResult1, pSignal); // AND with the accumulated result
                    }
                }
            }
        }

        // Process distinguishing input 2
        Abc_NtkForEachPo(pUnrolledMiter, pObj, i) {
            if (ClapAttack_ExtractCycleNumber(Abc_ObjName(pObj)) == cycle) {
                for (size_t j = 0; j < nonKeyInputCount2; j++) {
                    if (ClapAttack_SubstringExists(Abc_ObjName(pObj), nonKeyInputNames2[j])) {
                        Abc_Obj_t* pSignal = Abc_ObjChild0(pObj); // Get the signal driving the PO
                        if (nonKeyInputTargets2[j] == 0) {
                            pSignal = Abc_ObjNot(pSignal); // Invert the signal if target is 0
                        }
                        andResult2 = Abc_AigAnd((Abc_Aig_t *)pUnrolledMiter->pManFunc, andResult2, pSignal); // AND with the accumulated result
                    }
                }
            }
        }

        orResult1 = Abc_AigOr((Abc_Aig_t *)pUnrolledMiter->pManFunc, orResult1, andResult1); // OR it with the accumulated result for input 1
        orResult2 = Abc_AigOr((Abc_Aig_t *)pUnrolledMiter->pManFunc, orResult2, andResult2); // OR it with the accumulated result for input 2
    
        printf("\n");
    }

    Abc_Obj_t* finalResult = Abc_AigAnd((Abc_Aig_t *)pUnrolledMiter->pManFunc, orResult1, orResult2); 


    // Collect all POs in a list/array
    int nPos = Abc_NtkPoNum(pUnrolledMiter); // Get the number of POs
    Abc_Obj_t **poArray = (Abc_Obj_t **)malloc(nPos * sizeof(Abc_Obj_t *)); // Allocate memory for PO pointers
    i = 0;
    Abc_NtkForEachPo(pUnrolledMiter, pObj, i) {
        poArray[i] = pObj; 
    }

    // Saving fanin nodes of old outputs before deletion
    Abc_Obj_t **oldOutputFanins = (Abc_Obj_t **)malloc(nPos * sizeof(Abc_Obj_t *));
    int oldOutputSize = 0;
    i = 0;
    Abc_NtkForEachPo(pUnrolledMiter, pObj, i) {
        // Save the fanin node driving this PO (assuming single fanin per PO)
        oldOutputFanins[i] = Abc_ObjFanin0(pObj);
        oldOutputSize++;
    }

    // Disconnect and delete each PO
    for (i = 0; i < nPos; i++) {
        pObj = poArray[i];
        
        // Detach PO from its fanin
        // pFanin = Abc_ObjFanin0(pObj);
        // Abc_ObjRemoveFanins(pObj);

        // Delete the PO.
        Abc_NtkDeleteObj(pObj);
    }

    // Free the array of PO pointers
    free(poArray);

    // // Create a new PO and connect it to finalResult
    Abc_Obj_t* pNewPo = Abc_NtkCreatePo(pUnrolledMiter);
    Abc_ObjAddFanin(pNewPo, finalResult);

    // free(poArray); // Free the allocated memory for the PO pointers
    for (i = 0; i < nonKeyInputCount1; i++) {
    free(nonKeyInputNames1[i]);
    }
    for (i = 0; i < nonKeyInputCount2; i++) {
        free(nonKeyInputNames2[i]);
    }

    free(oldOutputFanins);
    free(nonKeyInputNames1);
    free(nonKeyInputNames2);
    free(nonKeyInputTargets1);
    free(nonKeyInputTargets2);

}


void ClapAttack_StrToLower(char *str) {
    while (*str) {
        *str = tolower((unsigned char) *str);
        str++;
    }
}

// if subStr exists in mainStr, return true
int ClapAttack_SubstringExists(const char *mainStr, const char *subStr) {
    char mainLower[strlen(mainStr) + 1];
    char subLower[strlen(subStr) + 1];

    strcpy(mainLower, mainStr);
    ClapAttack_StrToLower(mainLower);

    strcpy(subLower, subStr);
    ClapAttack_StrToLower(subLower);

    return strstr(mainLower, subLower) != NULL;
}

ABC_NAMESPACE_IMPL_END
