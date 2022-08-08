//
// Zuzak Test File
//

#include "base/abc/abc.h"
#include "base/main/main.h"
#include "proof/fraig/fraig.h"

struct BSI_KeyData_t {
  int NumKeys;
  int * KeyValue;
  int Updated;
  Abc_Ntk_t *pKeyCnf;
};

struct SatMiterList {
  Abc_Obj_t ** ppSatNode;
  int MatchedNodes;
  int NumKeys;
  char **KeyNames;
  Abc_Ntk_t *pMiter;
  int *pModel;
  float IdentifiableKeyBits;
  struct SatMiterList * pNext;
  struct SatMiterList * pPrev;
};

int ClapAttack_ClapAttackAbc(Abc_Frame_t * pAbc);
int ClapAttack_ClapAttack(Abc_Frame_t * pAbc);
void ClapAttack_TraversalRecursive( Abc_Ntk_t * pNtk, Abc_Obj_t * pCurNode, struct BSI_KeyData_t * pGlobalBsiKeys, int *pOracleKey, int MaxKeysConsidered, Abc_Ntk_t ** ppCurKeyCnf, int *pTotalProbes );
void ClapAttack_TraversalRecursiveHeuristic( Abc_Ntk_t * pNtk, Abc_Obj_t * pCurNode, struct BSI_KeyData_t * pGlobalBsiKeys, int MaxKeysConsidered, Abc_Ntk_t ** ppCurKeyCnf, struct SatMiterList ** ppSatMiterList, int *pNumProbes, int MaxProbes );
void ClapAttack_CombineMitersHeuristic( struct SatMiterList ** ppSatMiterListOld, struct SatMiterList ** ppSatMiterListNew, int * pMaxNodesConsidered, int MaxKeysConsidered, int MaxPiNum, int fConsiderAll );
void ClapAttack_InterpretDiHeuristic(Abc_Ntk_t *pNtk, Abc_Ntk_t *pNtkMiter, int *pModel, int NumKeys, int *KeyWithFreq, int *KeyNoFreq, int **ppDiFull);
void ClapAttack_EvalMultinodeProbe( struct SatMiterList *pSatMiter, Abc_Ntk_t *pNtk, struct BSI_KeyData_t * pGlobalBsiKeys, int *pOracleKey, Abc_Ntk_t ** ppCurKeyCnf, int MaxKeysConsidered );
void ClapAttack_UpdateSatMiterList( struct SatMiterList ** ppSatMiterList, Abc_Obj_t ** ppNode, Abc_Ntk_t *pMiter, int NumKeys, char **KeyNames, int MaxNodesConsidered, float IdentifiableKeys, int *pModel );
void ClapAttack_FreeSatMiterList( struct SatMiterList ** ppSatMiterList );
void ClapAttack_GenSatAttackConfig( Abc_Ntk_t * pNtk, struct BSI_KeyData_t * pGlobalBsiKeys );
int ClapAttack_UpdateGlobalKeyCnf ( Abc_Ntk_t **ppCurKeyCnf, struct BSI_KeyData_t * pGlobalBsiKeys );
int ClapAttack_IsolateCone(Abc_Ntk_t * pNtk, Abc_Ntk_t ** ppNtkCone, Abc_Obj_t * pProbe);
void ClapAttack_CleanCone( Abc_Ntk_t ** ppNtk );
void ClapAttack_RenamePo( Abc_Ntk_t * pNtk, int PoIdx, char *NewPoName);
void ClapAttack_InitKeyCnf( Abc_Ntk_t ** ppNtk, int NumKeys, int * WrongKeyValue, char ** KeyNames );
void ClapAttack_UpdateKeyCnf( Abc_Ntk_t ** ppNtk, int NumKeys, int * WrongKeyValue, char ** KeyNames );
int ClapAttack_BuildPartialKeyMiter( Abc_Ntk_t ** ppNtk );
int ClapAttack_MakeMiter(Abc_Ntk_t * pNtkCone, Abc_Ntk_t * pNtkPartialKey, Abc_Ntk_t ** ppNtkMiter);
int ClapAttack_MakeMiterHeuristic(Abc_Ntk_t * pNtkCone, Abc_Ntk_t * pNtkPartialKey, Abc_Ntk_t ** ppNtkMiter);
int ClapAttack_RunSat(Abc_Ntk_t *pNtk);
void ClapAttack_InterpretDi(Abc_Ntk_t *pNtk, int *pDiFull, int NumKeys, int *KeyWithFreq, int *KeyNoFreq);
int ClapAttack_OracleInferKey(Abc_Ntk_t *pNtk, int * pSimInfo1, int * pSimInfo2);
void ClapAttack_OracleSimDi(Abc_Ntk_t *pNtk, int * pDi, int NumKeys, int *KeyWithFreq, int *KeyNoFreq, int *WrongKeyValue);
void ClapAttack_UpdateKey(char *KeyNameTmp, int KeyValue, struct BSI_KeyData_t *pGlobalBsiKeys);
void ClapAttack_CleanKeyCnf( struct BSI_KeyData_t *pGlobalBsiKeys );
void ClapAttack_DelKnownKeys(Abc_Obj_t **ppNodeList, int NumKnownKeys );
int ClapAttack_SetKnownKeys(Abc_Ntk_t *pNtkCone, Abc_Obj_t *pKey, struct BSI_KeyData_t *pGlobalBsiKeys );
void ClapAttack_OracleSetConeKeys(Abc_Ntk_t * pNtk, int * pInp, int * pOracleKey);
void ClapAttack_PrintInp(Abc_Ntk_t *pNtk, int * pInp);
void ClapAttack_PrintOut(Abc_Ntk_t *pNtk, int * pOut);
int ClapAttack_WriteMiterBench(Abc_Ntk_t *pNtk, char *pFileName);
int ClapAttack_WriteMiterVerilog(Abc_Ntk_t *pNtk, char *pFileName);
int ClapAttack_RenameInput( Abc_Ntk_t * pNtk, char *PiSuffix, char *KeySuffix, char *PoSuffix, int nDigits );
int ClapAttack_RenameLogic( Abc_Ntk_t * ppNtk, char * PiSuffix, char * KeySuffix, char * PoSuffix, int KeepName );
int ClapAttack_RenameOutput( Abc_Ntk_t * pNtk, char *PoSuffix, int KeepName );
int ClapAttack_GetNumKeys( Abc_Ntk_t *pNtk );
void ClapAttack_InitKeyStore( int NumKeys, struct BSI_KeyData_t *pGlobalBsiKeys );
void ClapAttack_CopyKeyStore( struct BSI_KeyData_t *pGlobalBsiKeys, struct BSI_KeyData_t *pGlobalBsiKeysNew );
int ClapAttack_MiterPos( Abc_Ntk_t * pNtk, int fXorOr, int fXnorAnd);
int ClapAttack_MiterKeys( Abc_Ntk_t * pNtk );
int ClapAttack_PartialKeyInferenceMiter( Abc_Ntk_t * pNtk, Abc_Ntk_t ** ppNtkMiter, char * CurKeyName );
int ClapAttack_CmpKeyName( char *KeyName1, char *KeyName2, int KeyLen1, int KeyLen2 );

ABC_NAMESPACE_IMPL_START


/* Global Var for Probe Point Counter  
int nValidProbePoint;
int nAvgKeyCount;
/* End Global Var for Probe Point Counter */

int ClapAttack_ClapAttackAbc(Abc_Frame_t * pAbc) {
  Abc_Ntk_t * pNtk;
  int result;

  // Get the network that is read into ABC
  pNtk = Abc_FrameReadNtk(pAbc);

  if(pNtk == NULL) {
    Abc_Print(-1, "ClapAttack_ClapAttackAbc: Getting the target network has failed.\n");
    return 0;
  }

  // Call the main function
  result = ClapAttack_ClapAttack(pAbc);

  return result;
}

int ClapAttack_ClapAttack(Abc_Frame_t * pAbc) {
  int i, j, NumKeys, KeyIndex, MaxKeysConsidered, KeysConsideredCutoff, KeysFound, MaxNodesConsidered;
  Abc_Ntk_t *pNtk, *pCurKeyCnf; 
  Abc_Obj_t *pPi, *pNode;
  struct BSI_KeyData_t GlobalBsiKeys;
  int RunHeuristic = 0;
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

  // Paper benchmarks -- c1908
  // RLL
  //int pOracleKey[88] = {0,0,0,0,0,1,0,0,1,0,0,0,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,0,1,1,1,1,1,0,0,1,1,1,1,0,0,0,0,0,1,0,1,0,0,1,1,0,1,1,0,1,1,1};
  // OA32
  //int pOracleKey[174] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,1,0,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,1,0,0,1,0,0,0,1,1,0,1,1,1,1,1,0,0,0,1,0,0,1,1,0,1,1,0,1,1,0,1,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,1,0,1,0,1,0,1};
  // BA32
  //int pOracleKey[110] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,1,1,1,0,1,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1,0,1,1,1,0,0,0,1,0,0,1,0,0,0,1,1,0,1,1,1,1,1,0};
  // BA16
  //int pOracleKey[78] = {1,1,0,0,1,0,0,1,0,0,1,0,0,1,1,1,1,0,1,0,0,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,1,1,1,0,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,0,1,0,1,0,0,0,0,0};
  // WF
  //int pOracleKey[144] = {0,1,1,0,1,1,0,0,0,0,1,0,0,1,1,0,0,1,1,1,1,0,1,1,1,1,1,0,0,0,1,0,1,1,1,0,0,0,1,0,1,1,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,1,0,1,1,1,1,0,0,1,1,0,1,0,0,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,1,0,0,1,1,1,1,0,0,0,1,1,1,0,0,0,0,1,0,1,1,1,0,1,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,1,0,0,1,0,1,1,1};
  //SFLL
  //int pOracleKey[33] = {0,1,1,1,0,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,1,1,0};
  // WF-40
  //   int pOracleKey[540] = {1,1,0,1,1,1,1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,1,1,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,1,1,1,0,0,1,0,1,0,1,1,1,1,0,0,1,0,0,0,0,1,0,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,0,1,0,1,1,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0};
  // WF-32
  //int pOracleKey[384] = {0,0,0,1,0,0,1,1,1,0,1,0,1,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,0,1,1,0,1,1,0,0,0,1,1,1,0,1,1,0,0,1,1,1,1,0,0,0,1,0,0,1,1,0,1,1,1,1,0,0,1,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,1,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,1,1,1,0,1,0,0,1,1,0,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,0,0,1,1,0,1,0,1,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,0,1,1,0,1,0,0,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,0,0,0,1,0,1,1,1,1,0,0,1,0,1,1,1,1,0,0,1,0,1,0,1,0,0,1,1,1,1,0,1,0,0,0,0,0,0,0,1,1,0,1,1,1,1,0,0,1,1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,0,1,0,0,1,1,0,0,1,0,0,0,1,1,1,1,0,0,1,0,1,1,1,1,0,1,0,1,1,1,0,1,1,0,0,1,1,1,1,0,0,1,0,0,1,0,1,1,1,0,1,0,0,1,0,0,1,0,1,0,1,1,1,1,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,0,1,0,0};
  
  // Paper benchmarks -- c5315
  // RLL
  int pOracleKey[231] = {1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,1,1,1,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,1,0,1,1,0,1,1,0,1,0,0,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,0,0,0,1,0,1,0,1,0};
  // OA32
  //int pOracleKey[252] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,1,1,0,0,1,0,1,1,1,0,0,1,0,1,1,0,0,1,1,1,1,0,1,1,1,0,0,0,0,1,0,1,0,1,0,1,1,1,0,0,1,1,0,1,1,1,1,1,1,1,0,1,1,0,0,0,1,0,0,1,1,0,1,0,1,1,0,1,1,0,1,1,0,0,0,0,0,1,1,0,1,0,1,0,0,0,1,1,1,0,0,1,0,1,0,1,1,0,0,0};
  // WF
  //int pOracleKey[144] = {1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,1,0,1,1,1,0,0,1,1,1,0,1,0,0,1,0,1,0,1,0,1,1,0,0,0,0,0,1,0,1,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,0,1,1,0,0,1,1,1,1,1,1,0,1,0,1,0,1,1,0,0,0,1,0,0,1,0,1,0,0};
  //SFLL-178
  //int pOracleKey[178] = {0,1,0,0,1,1,1,0,1,1,0,0,0,1,0,0,1,0,0,1,0,1,1,0,0,1,1,0,0,1,1,0,1,1,0,1,1,0,0,0,0,0,0,1,0,0,0,1,1,1,0,0,1,1,1,0,0,0,1,1,1,0,0,1,1,1,1,0,1,1,1,1,0,1,0,1,0,0,1,0,1,1,0,1,0,1,0,0,0,0,0,0,1,1,0,0,0,1,0,0,0,1,1,1,0,0,0,0,1,1,0,0,1,1,0,0,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,1,1,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,1,0,0,0,1,1,1,1,1,0,0,0,0,0,1,0,0,0,1,0,1};
  //BA16
  //int pOracleKey[156] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,1,1,0,0,0,1,1,0,1,0,1,1,0,0,0,0,1,0,1,0,0,0,0,1,1,0,0};
  // WF-40
  //int pOracleKey[540] = {1,1,0,1,1,1,1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,1,1,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,1,1,1,0,0,1,0,1,0,1,1,1,1,0,0,1,0,0,0,0,1,0,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,0,1,0,1,1,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0};
  
  // Paper benchmarks -- des
  // RLL
  //int pOracleKey[256] = {1,0,0,0,1,0,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,0,0,1,1,0,1,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,0,0,1,1,0,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0,0,1,0,1,0,0,0,0,1,0,1,1,1};
  // BA16
  //int pOracleKey[368] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0};
  // SFLL
  //int pOracleKey[256] = {1,0,0,1,1,1,0,0,1,1,0,1,0,1,1,0,1,0,1,0,1,0,0,1,1,1,1,1,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,1,1,1,0,1,1,1,0,1,0,0,1,1,1,1,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,0,1,1,1,1,0,1,1,0,1,1,0,0,0,1,0,0,0,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,1,1,1,1,1,0,1,0,1,0,0,1,0,0,1,0,0,1,0,0,0,1,1,0,0,0,0,1,1,0,1,0,1,0,0,1,1,0,0,1,0,1,1,0,0,0,0,1,0,0,1,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,0,0,1,0,1,0,0,1,1,0,0,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,1,1,0,0,0,0,1,0,1,1,1,0,1,1,0,1,0,0,0,0,0,1,0,1,1,0,0,1,0,0,0,0,1,0,0,1,0,1,1,0,1,0,1,1,1,1,0,0,1,0,0,1,0,1,0};
  // WF-40
  //int pOracleKey[540] = {1,1,0,1,1,1,1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,1,1,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,1,1,1,0,0,1,0,1,0,1,1,1,1,0,0,1,0,0,0,0,1,0,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,0,1,0,1,1,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0};

  // Paper benchmarks -- b14
  // RLL
  //int pOracleKey[256] = {1,0,1,1,0,1,1,0,1,0,1,1,0,0,1,0,1,1,0,1,0,1,0,1,0,0,1,0,1,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,1,1,0,0,1,0,0,1,0,0,1,0,1,1,0,1,0,1,0,1,0,1,0,0,0,1,0,1,0,1,1,1,0,1,0,0,0,1,0,1,0,1,1,0,1,1,0,1,0,0,0,1,0,0,1,1,1,1,0,0,1,0,0,1,0,1,0,1,1,0,0,1,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1,0,1,0,0,1,0,1,1,0,0,1,1};
  // OA32
  //int pOracleKey[500] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,1,0,0,1,1,1,0,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,1,0,0,0,1,0,1,0,1};
  // BA32
  //int pOracleKey[436] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1,1,0,1,1,0,1,1,1,1,0,0,1,1,1,0,1,0,1,0,0,0,0,1,0,1,1,0,0,1,1,1};
  // BA16
  //int pOracleKey[404] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,1,0,1,0,0,0,0,0,1,1,1,1,0,1,1,0,0,1,1,1,0,1,0,1,1,1,0,0,1,1,0,0,0,0,0,1,0,1,1,0,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1};
  // SFLL
  //int pOracleKey[277] = {1,0,1,1,1,0,1,1,1,0,1,1,0,0,1,0,1,0,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,0,1,1,0,0,0,1,0,0,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,1,0,0,1,1,0,1,0,1,1,0,0,1,1,0,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,1,1,0,0,1,1,0,1,0,0,0,1,1,0,0,0,1,1,0,1,1,0,1,1,0,0,1,1,1,0,1,0,0,0,1,0,1,1,0,1,0,0,0,0,1,1,0,0,1,1,1,0,0,0,1,1,0,1,0,1,1,1,1,1,1,0,1,1,1,1,0,0,1,1,1,0,0,1,0,1,1,1,0,0,1,1,1,1,0,1,0,0,1,0,0,0,1,1,1,0,1,1,0,0,1,0,0,0,0,1,1,0,0,1,0,0,1,1,0,0,0,1,1,1,0,1,0,1,0,1,1,0,0,1,0,0,1,0,1,1,0,1,0,0,0,0,1,0,1,0,1,1,0,1,1,0,0,1,0,1,0,0,0,0,1,0,0,0,0,0,0,1,0,1,1,0,0,0,1,0,1,0,1,0,0,0,1,1,1};
  //WF-40
  //int pOracleKey[540] = {1,1,0,1,1,1,1,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0,1,1,0,1,1,0,1,1,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,0,1,1,0,0,1,0,1,0,1,1,0,1,0,1,0,0,1,1,0,1,1,0,0,0,0,0,0,0,1,0,1,1,0,1,1,0,0,1,0,1,0,0,0,1,1,0,0,1,0,1,0,0,1,1,0,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,1,1,1,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,1,0,1,1,1,1,0,1,0,1,1,0,0,0,0,1,1,0,1,1,1,1,1,1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,1,1,1,0,0,1,1,0,1,1,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,1,1,1,0,1,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,1,1,1,0,0,1,0,1,0,1,1,1,1,0,0,1,0,0,0,0,1,0,1,1,1,0,0,1,0,0,1,1,1,1,0,0,1,1,1,1,0,1,1,0,0,0,1,0,1,0,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,1,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,1,0,0,1,1,0,0,0,1,1,1,0,0,0,1,0,0,1,0,1,1,0,0,1,1,0,1,0,0,0,0,1,1,0,1,1,0,0,0,0,0,1,1,1,0,1,0,1,0,1,0,1,1,1,1,0,0,1,1,0,1,1,0,1,1,1,0,1,0,0,1,1,1,1,0,1,0,1,1,1,1,0,1,1,0,0,1,0,1,1,0,1,1,0,0,1,0,0,0,1,1,0,1,0,0,1,1,0,1,1,0,1,0,1,1,0,1,0};

  //key=11101001001100110111110011100010001000011001000100101110101001111110010010000001101100111101100110100110001100000101011001001110000000000101110111100000011101000111110110000000100000110011001110100000000011101000111100110000000110011111011110110110101011001000100000011110000011010000111111111100101101101100110111001010000010011110010101111111011011101000000100110000
  

  // Yuntao - c1908_SFLL_16x1
  //int pOracleKey[16] = {0,1,1,1,0,1,0,0,0,1,1,1,0,0,0,0};
  // Yuntao - c1908_SFLL_33x1
  //int pOracleKey[33] = {0,1,1,1,0,1,0,0,0,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,1,1,1,0};
  // Yuntao - c1908 2x2WF
  //int pOracleKey[12] = {0,1,1,0,1,1,1,0,0,0,1,1};
  
  //int pOracleKey[19] = {0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1};  

  // c499_enc05
  //int pOracleKey[10] = {1,1,1,0,1,0,0,1,0,0};
  //c880_enc05
  //int pOracleKey[19] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1};  
  // ./../tooling-dev/benchmarks/antisat/c1355_OA4.bench
  //int pOracleKey[45] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0};
  // ./../lazy-sat-attack-master/benchmarks/rnd/dalu_enc10.bench
  //int pOracleKey[230] = {0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,1,0,1,0,0,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,1,1,0,0,0,0,1,1,0,0,1,0,1,1,1,0,0,0,1,1,0,1,1,0,1,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,0,1,1,0,0,1,0,0,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,1,1,1,0,0,0,1,1,0,0,0,0,1,1,0,1,0,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,1,1,1,0,1,1,1,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,1,0,1,1,0,0,1,0,1,1,0,0,1,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,1,1};
  // ./../tooling-dev/benchmarks/antisat/des_BA16.bench
  //int pOracleKey[368] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0};
  // ./../tooling-dev/benchmarks/antisat/dalu_BA8.bench
  //int pOracleKey[135] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,0};
  // ./../tooling-dev/benchmarks/antisat/dalu_OA8.bench
  //int pOracleKey[151] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,0,1,1,0,0,0,1,1,1,1,0,1,1,0,0,1,1};
  // ./../tooling-dev/benchmarks/antisat/dalu_BA16.bench
  //int pOracleKey[151] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,1,1,1,1,0,1,1,1,1,0,0,0,1,1,0,1,0,1,0,1,1,1,0,1,0,0,0,1,0,0,0};
  // ./../lazy-sat-attack-master/benchmarks/antisat/des_BA16.bench
  //int pOracleKey[368] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0};
  // ./../lazy-sat-attack-master/benchmarks/antisat/des_OA16.bench
  //int pOracleKey[400] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1,0,0,1,0,0,0,1,0,0,1,0,1,1,1,0,1,0,1,0,0,1,1,1,1,1,1,0,0,1,0,0,1,0,0,0,0,0,0,1,1,0,1,1,0,0,1,1,1,1,0,1,1,0,0,1,1,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,1,1,0,0,1,1,0,0,1,1,1,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,1,0,1,1,1,1,0,1,1,0,1,1,0,1,0,1,0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,1,1,0,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,0,1,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,1,1,0,1,0,0,0,1,0,0,1,1,1,1,0,0,0,1,0,0,0,1,1,1,0,0,1,0,1,1,0,1};
  // ./../lazy-sat-attack-master/benchmarks/rnd/des_enc10.bench
  //int pOracleKey[647] = {1,0,0,0,1,0,0,1,0,1,0,0,0,1,1,1,0,1,1,1,1,1,0,0,1,1,0,1,0,0,0,1,0,0,1,0,1,0,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,0,0,1,1,0,1,0,1,0,0,1,1,1,0,0,1,0,1,0,0,1,1,0,1,1,0,0,0,0,1,1,0,1,0,1,1,0,0,0,0,1,1,1,0,0,1,1,0,1,1,1,0,1,0,0,1,0,0,0,1,1,0,1,0,1,1,1,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,0,1,1,0,0,1,0,0,1,1,0,1,1,0,1,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,0,0,0,1,0,0,1,1,0,1,0,0,0,0,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,1,0,1,1,0,1,1,0,1,0,0,0,1,1,1,0,0,0,1,0,0,1,1,0,0,0,0,1,0,1,0,0,0,0,1,0,1,1,1,1,1,1,0,1,0,0,0,1,1,0,1,0,0,0,1,0,1,0,0,0,0,1,0,1,1,0,1,0,0,0,0,1,0,0,0,1,0,1,0,1,1,1,0,0,1,1,1,0,0,1,1,1,0,1,0,1,1,1,1,1,0,0,1,0,0,0,0,1,1,1,0,0,1,1,1,1,1,0,1,0,1,0,1,0,0,1,1,1,0,1,1,0,0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,0,1,1,0,1,1,1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,0,1,1,1,0,0,0,0,1,1,0,1,0,1,0,1,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,1,1,0,1,0,0,1,0,1,1,1,1,1,1,1,1,1,0,0,1,0,1,0,0,0,1,1,0,0,0,0,1,1,1,1,0,1,0,0,0,0,1,0,1,1,1,1,1,0,0,1,1,1,0,1,0,0,1,1,0,0,0,1,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,1,1,0,0,1,1,1,1,0,0,0,1,0,1,0,1,1,1,0,0,0,0,0,1,0,0,0,0,1,1,0,1,0,0,1,0,0,0,1,0,0,1,1,0,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,1,0,0,1,1,0,0,1,1,0,0,0,0,1,1,0,1};

  // ./../../low_error_locking/TLL_benchmarks/b14/b14_sfll_k10.bench
  //int pOracleKey[10] = {1,0,1,0,1,0,1,0,1,1};
  
  // ./../lazy-sat-attack-master/benchmarks/antisat/c1355_OA4.bench
  //int pOracleKey[45] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0};
  // ./../lazy-sat-attack-master/benchmarks/antisat/c1355_BA4.bench
  //int pOracleKey[37] = {1,1,1,0,1,0,0,1,0,0,1,1,0,0,1,1,0,1,1,1,1,1,0,0,1,1,1,0,0,0,1,0,0,0,0,0,0};
  //./../lazy-sat-attack-master/benchmarks/rnd/c1355_enc10.bench
  //int pOracleKey[55] = {1,0,0,1,0,1,1,0,1,1,0,1,1,0,0,1,1,0,1,1,1,0,0,1,0,1,0,0,0,0,0,1,0,0,1,1,1,1,0,0,1,0,1,1,1,1,1,1,0,1,1,0,0,1,0};
  
  // check if the network is strashed
  //if(!Abc_NtkIsStrash(pNtk)) {
  //  Abc_Print(-1, "ClapAttack_ClapAttack: This command is only applicable to strashed networks.\n");
  //  return 0;
  //}

  // print information about the network
  //Abc_Print(1, "The network with name %s has:\n", Abc_NtkName(pNtk));
  //Abc_Print(1, "\t- %d primary inputs;\n", Abc_NtkPiNum(pNtk));
  //Abc_Print(1, "\t- %d primary outputs;\n", Abc_NtkPoNum(pNtk));
  //Abc_Print(1, "\t- %d AND gates;\n", Abc_NtkNodeNum(pNtk));

  // Get the network that is read into ABC ... THIS IS THE BACKUP COPY FOR NOW.
  pNtk = Abc_FrameReadNtk(pAbc);

  // Count keys
  NumKeys = ClapAttack_GetNumKeys( pNtk );
  ClapAttack_InitKeyStore( NumKeys, &GlobalBsiKeys );
  KeysConsideredCutoff = 7;
  MaxProbes = 3;
  
  // Set visited for each node to 0
  Abc_NtkForEachNode( pNtk, pNode, i )
    pNode->fMarkC = 0;

  MaxNodesConsidered = 2;
  // Todo -> prevent re-traversal of nodes within an pass
  // Todo -> second pass with known keys updated.
  // Todo -> Handle known keys
  // Goal: Iterate through each PI. Identify list of keys.
  if(RunHeuristic) {

    KeysConsideredCutoff = 11;
    
    while (1) {
      
      for ( MaxKeysConsidered = 1; MaxKeysConsidered  < KeysConsideredCutoff; MaxKeysConsidered++ ) {

	printf("Set Number of Keys considered to: %d\n\n", MaxKeysConsidered);
	GlobalBsiKeys.Updated = 1;
	fConsiderAll = 1;

	while ( GlobalBsiKeys.Updated ) {

	  // Set the update var to 0. IF we change our keystore, set it back to 1 and re-loop over the tree.
	  GlobalBsiKeys.Updated = 0;
	  
	  Abc_NtkForEachPi( pNtk, pPi, i ) {
	    
	    // Are we looking at a key input? And do we know it?-- If so, begin fanout
	    if( strstr(Abc_ObjName(pPi), "key") ) {
	      
	      // Get key index
	      KeyIndex = (int) strtol(&((Abc_ObjName(pPi))[8]), (char **)NULL, 10);
	      
	      // Do we know this key index?
	      if( GlobalBsiKeys.KeyValue[KeyIndex] < 0 ) {
		//printf("Currently working on fanout for key index: %d\n", KeyIndex);
		pCurKeyCnf = NULL;
		NumProbes = 0;
		
		//ClapAttack_TraversalRecursiveHeuristic( pNtk, pPi, &GlobalBsiKeys, MaxKeysConsidered, &pCurKeyCnf, &pSatMiterList );
		ClapAttack_TraversalRecursiveHeuristic( pNtk, pPi, &GlobalBsiKeys, MaxKeysConsidered, &GlobalBsiKeys.pKeyCnf, &pSatMiterList, &NumProbes, MaxProbes );
	      }
	    }
	  }
	
	  // Keep merging until no nodes remain...
	  while ( pSatMiterList ) {
	    
	    ClapAttack_CombineMitersHeuristic( &pSatMiterList, &pSatMiterListNew, &MaxNodesConsidered, MaxKeysConsidered, Abc_NtkPiNum(pNtk), fConsiderAll );

	    fConsiderAll = 0;
	    
	    if ( pSatMiterListNew ) {
	      // Free old miter list in favor of new one.
	      ClapAttack_FreeSatMiterList(&pSatMiterList);
	      pSatMiterList = pSatMiterListNew;
	      pSatMiterListNew = NULL;
	      // Now consider combined probes...
	      MaxNodesConsidered++;
	    } else {
	      
	      // Debug print
	      pSatMiterListCur = pSatMiterList;
	      while (pSatMiterListCur) {
		printf("Sat Miter Node Pairing: ");
		for ( j=0; j<pSatMiterListCur->MatchedNodes; j++ )
		  printf("%s ", Abc_ObjName(pSatMiterListCur->ppSatNode[j]));
		printf("\tIdentifiable Keys from Miter: %f\n", pSatMiterListCur->IdentifiableKeyBits);
		pSatMiterListCur = pSatMiterListCur->pNext;
	      }
	      printf("\n");

	      // Set visited for each node to 0
	      //Abc_NtkForEachNode( pNtk, pNode, j )
	      //pNode->fMarkC = 0;

	      break;
	    }
	    
	    // Debug print
	    pSatMiterListCur = pSatMiterList;
	    while (pSatMiterListCur) {
	      printf("Sat Miter Node Pairing: ");
	      for ( j=0; j<pSatMiterListCur->MatchedNodes; j++ )
		printf("%s ", Abc_ObjName(pSatMiterListCur->ppSatNode[j]));
	      printf("\tIdentifiable Keys from Miter: %f\n", pSatMiterListCur->IdentifiableKeyBits);
	      pSatMiterListCur = pSatMiterListCur->pNext;
	    }
	    printf("\n");
	  }
	}
      }
      
      // If there were SAT nodes, run the probe.
      if (pSatMiterList) {

	if (pSatMiterList->IdentifiableKeyBits >= (0.0625)) {
	  TotalProbes++;
	  //ClapAttack_EvalMultinodeProbe ( pSatMiterList, pNtk, &GlobalBsiKeys, pOracleKey, &pCurKeyCnf, MaxKeysConsidered );
	  ClapAttack_EvalMultinodeProbe ( pSatMiterList, pNtk, &GlobalBsiKeys, pOracleKey, &GlobalBsiKeys.pKeyCnf, MaxKeysConsidered );
	  ClapAttack_FreeSatMiterList(&pSatMiterList);
	  MaxNodesConsidered = 2;
	  pSatMiterList = NULL;
	  
	  // Set visited for each node to 0
	  Abc_NtkForEachNode( pNtk, pNode, j )
	    pNode->fMarkC = 0;

	} else {
	  break;
	}
      } else {
	// There is no more physical data to be had...
	break;
      }
    }
  } else {
    
    for ( MaxKeysConsidered = 1; MaxKeysConsidered  < KeysConsideredCutoff; MaxKeysConsidered++ ) {


      /* Probe Point Counter  
      MaxKeysConsidered = KeysConsideredCutoff-1;
      /* End Probe Point Counter */
      
      printf("Set Number of Keys considered to: %d\n\n", MaxKeysConsidered);
      GlobalBsiKeys.Updated = 1;
      
      while ( GlobalBsiKeys.Updated ) {
	
	// Set the update var to 0. IF we change our keystore, set it back to 1 and re-loop over the tree.
	GlobalBsiKeys.Updated = 0;
	MaxNodesConsidered = 1;
	
	Abc_NtkForEachPi( pNtk, pPi, i ) {
	  
	  // Are we looking at a key input? And do we know it?-- If so, begin fanout
	  if( strstr(Abc_ObjName(pPi), "key") ) {
	    
	    // Get key index
	    KeyIndex = (int) strtol(&((Abc_ObjName(pPi))[8]), (char **)NULL, 10);
	    
	    // Do we know this key index?
	    if( GlobalBsiKeys.KeyValue[KeyIndex] < 0 ) {
	      //printf("Currently working on fanout for key index: %d\n", KeyIndex);
	      pCurKeyCnf = NULL;

	      ClapAttack_TraversalRecursive( pNtk, pPi, &GlobalBsiKeys, pOracleKey, MaxKeysConsidered, &pCurKeyCnf, &TotalProbes );
	    }
	  }
	}
      }

      /* Probe Point Counter  
      printf("There are %d Probe Points, Averaging %f key inputs \n\n", nValidProbePoint, ((float)nAvgKeyCount)/((float)nValidProbePoint));
      /* End Probe Point Counter */
      
    }
  }

  
  // Append known keys into partial key CNF for finalized circuit formulation
  ClapAttack_WriteMiterVerilog(GlobalBsiKeys.pKeyCnf, "global_keystore.v");
  ClapAttack_GenSatAttackConfig( pNtk, &GlobalBsiKeys );
  

  // Print the final key value inferred from the attack.
  printf("KeyStore is now: {");
  KeysFound = GlobalBsiKeys.NumKeys;
  for (i=0; i<GlobalBsiKeys.NumKeys; i++){
    if ( GlobalBsiKeys.KeyValue[i] == -1)
      KeysFound--;
    printf("%d, ", GlobalBsiKeys.KeyValue[i]);
  }
  printf("}\n\n");

  printf("We found %d of %d total keys using %d probes\n", KeysFound, GlobalBsiKeys.NumKeys, TotalProbes );

  free( GlobalBsiKeys.KeyValue );
  
  // Reset markc so we can terminate without seg-fault
  Abc_NtkForEachNode( pNtk, pNode, i )
    pNode->fMarkC = 0;
    
  return 1;
}


void ClapAttack_TraversalRecursive( Abc_Ntk_t * pNtk, Abc_Obj_t * pCurNode, struct BSI_KeyData_t * pGlobalBsiKeys, int *pOracleKey, int MaxKeysConsidered, Abc_Ntk_t ** ppCurKeyCnf, int *pTotalProbes ) {

  int *pFullDi;
  int i, j, k, m, SatStatus, MiterStatus, NumKeys, NumKnownKeys, *KeyWithFreq, *KeyNoFreq, *WrongKeyValue, KeyValue, PartialKeySatStatus, fCurKeyCnfAlloc;
  Abc_Ntk_t *pNtkCone, *pNtkMiter, *pInferPartialKeyMiter, *ntkTmp;
  Abc_Obj_t * pNode, * pPi, * pKey, **ppNodeFreeList;
  char ** KeyNameTmp;



  
  // Initialize partial key info to NULL
  //if ( ppCurKeyCnf )
  // pCurKeyCnf = *ppCurKeyCnf;
  //else
  //  pCurKeyCnf = NULL;
  fCurKeyCnfAlloc = 0;
  NumKeys = 0;
  
  // Goal: For each PI that is a key, follow fanout until it intersects with unknown key.
  Abc_ObjForEachFanout( pCurNode, pNode, i ) {
    

    // Have we visited this node before?
    if (!pNode->fMarkC) {



      // Initialzie free list to the number of keys present...
      ppNodeFreeList = (Abc_Obj_t **)malloc( sizeof(Abc_Obj_t *) * pGlobalBsiKeys->NumKeys);
      
      KeyNameTmp = (char **)malloc( sizeof(char *) * MaxKeysConsidered);
      for (k=0; k<MaxKeysConsidered; k++) {
	KeyNameTmp[k] = (char *)malloc( sizeof(char) * 100);
      }
      


    
      // Check to make sure were not lookign at a PO. If so,
      // don't bother pursuing the fanout.
      if ( Abc_ObjIsCo(pNode) ) {
	//printf("This node is a primary output. Done with fanout.\n");       
	for(k=0; k<MaxKeysConsidered; k++)
	  free(KeyNameTmp[k]);
	free(KeyNameTmp);
	free(ppNodeFreeList);
	continue;
      }



      
      
      // Check supports ... if only one is an unknown key,
      // process it and continue fanout
      ClapAttack_IsolateCone(pNtk, &pNtkCone, pNode);
      
      // Make sure only one cone input is a key... for now.
      // ToDo -> Make this more efficient...
      //ClapAttack_WriteMiterVerilog(pNtkCone, "cone_preconst.v");

      // Loop over the miter generation phase until SAT fails
      do {
	NumKeys = 0;
	NumKnownKeys = 0;

	SatStatus = 1;
	MiterStatus = 1;

	Abc_NtkForEachPi( pNtkCone, pPi, j ) {
	  // Are we looking at a key input?
	  if( strstr(Abc_ObjName(pPi), "key") ) {
	    if ( !ClapAttack_SetKnownKeys( pNtkCone, pPi, pGlobalBsiKeys ) ) {
	      if( NumKeys < MaxKeysConsidered )
		strcpy(KeyNameTmp[NumKeys], Abc_ObjName(pPi) );
	      NumKeys++;
	      //printf("Keyname: %s, KeyNum: %d\n",  Abc_ObjName(pPi), NumKeys);
	    } else {
	      ppNodeFreeList[NumKnownKeys] = pPi;
	      NumKnownKeys++;	    
	    }
	  }
	}
	
	// Delete known keys from cone.
	if ( NumKnownKeys )
	  ClapAttack_DelKnownKeys( ppNodeFreeList, NumKnownKeys );



	// We're processing the node. Mark it so we don't do this again.
	// If it fails here, there is no information we can ever gain
	if ( (NumKeys <= MaxKeysConsidered) )
	  pNode->fMarkC = 1;
	
	if ( (NumKeys <= MaxKeysConsidered) && NumKeys ) {

	  printf("Evaluating node %s\n", Abc_ObjName(pNode) );	  
	  printf("The number of keys is: %d\n", NumKeys);	  
	  // Remove unused logic and clean the PIs
	  //ClapAttack_CleanCone(&pNtkCone);
	  //ClapAttack_WriteMiterVerilog(pNtkCone, "cone.v");
	  
	  // We're processing the node. Mark it so we don't do this again.
	  // If it fails here, there is no information we can ever gain
	  //pNode->fMarkC = 1;
	
	  printf("Generating miter circuit.\n");       
	  //ClapAttack_WriteMiterVerilog(pNtkCone, "cone_test1.v");
	  //MiterStatus = ClapAttack_MakeMiter( pNtkCone, *ppCurKeyCnf, &pNtkMiter );
	  MiterStatus = ClapAttack_MakeMiterHeuristic( pNtkCone, *ppCurKeyCnf, &pNtkMiter );
	  //Abc_NtkPrintIo( stdout, pNtkMiter, 1 );
	  //Abc_NtkPrintIo( stdout, pNtkCone, 1 );
	  
	  if (!MiterStatus) {
	    
	    printf("Running SAT on Miter circuit.\n");       
	    
	    SatStatus = ClapAttack_RunSat(pNtkMiter);
	    //ClapAttack_WriteMiterVerilog(pNtkMiter, Abc_ObjName(pNode));
	  
	    if (!SatStatus) {
	      //ClapAttack_WriteMiterVerilog(pNtkCone, "cone.v");
	      //ClapAttack_WriteMiterVerilog(pNtkMiter, "miter.v");

	      /* Probe Point Counter  
	      nValidProbePoint++;
	      nAvgKeyCount = nAvgKeyCount + NumKeys;
	      printf("We found a valid probe point: %s with %d keys\n", Abc_ObjName(pNode), NumKeys);
	      /* End Probe Point Counter */
	      
	      /* Begin Probe Point Counter Comment */  
	      printf("SAT successfully located a key for keys:  ");
	      for (k=0; k<NumKeys; k++)
		printf("%s ", KeyNameTmp[k]);
	      printf("\n\n");
	      
	      // Malloc key values from SAT to infer from
	      KeyWithFreq = (int *)malloc( sizeof(int) * NumKeys );
	      KeyNoFreq = (int *)malloc( sizeof(int) * NumKeys );
	      
	      //ClapAttack_InterpretDi(pNtkCone, pNtkMiter->pModel, NumKeys, KeyWithFreq, KeyNoFreq);
	      ClapAttack_InterpretDiHeuristic(pNtkCone, pNtkMiter, pNtkMiter->pModel, NumKeys, KeyWithFreq, KeyNoFreq, &pFullDi);
	      
	      // Oracle testing. Comment out with real probe.
	      //ClapAttack_OracleSetConeKeys(pNtkCone, pNtkMiter->pModel, pOracleKey);
	      ClapAttack_OracleSetConeKeys(pNtkCone, pFullDi, pOracleKey);

	      // Increase probe count by 1
	      (*pTotalProbes)++;
	      
	      
	      
	      WrongKeyValue = (int *)malloc( sizeof(int) * NumKeys);
	      //ClapAttack_OracleSimDi(pNtkCone, pNtkMiter->pModel, NumKeys, KeyWithFreq, KeyNoFreq, WrongKeyValue);
	      ClapAttack_OracleSimDi(pNtkCone, pFullDi, NumKeys, KeyWithFreq, KeyNoFreq, WrongKeyValue);


	      printf("Wrong KeyValue: ");
	      for (k=0; k<NumKeys; k++)
		printf(" %d", WrongKeyValue[k]);
	      printf("\n\n");
	      
	      // Initialize partial key logic if it doesnt exist
	      // Otherwise update it...
	      if( NumKeys > 1 ) {
		if ( !(*ppCurKeyCnf) ) {
		  ClapAttack_InitKeyCnf( ppCurKeyCnf, NumKeys, WrongKeyValue, KeyNameTmp );
		  fCurKeyCnfAlloc = 1;
		  //ppCurKeyCnf = &pCurKeyCnf;
		} else {
		  ClapAttack_UpdateKeyCnf( ppCurKeyCnf, NumKeys, WrongKeyValue, KeyNameTmp );
		}
	      }
	      
	      // Short term hack to handle case wehre key is of length 1
	      if ( NumKeys == 1) {
		KeyValue = (WrongKeyValue[0] + 1) % 2;
		ClapAttack_UpdateKey(KeyNameTmp[0], KeyValue, pGlobalBsiKeys);
		//Abc_NtkDelete( pNtkMiter );
		//ABC_FREE(pNtkMiter);
		//break;
	      } else {
		// Evaluate partial key logic for complete key info
		for (k=0; k<NumKeys; k++) {
		  //printf("KeyName: %s \n\n\n\n\n", KeyNameTmp[k]);
		  ClapAttack_PartialKeyInferenceMiter( *ppCurKeyCnf, &pInferPartialKeyMiter, KeyNameTmp[k] );
		  PartialKeySatStatus = ClapAttack_RunSat(pInferPartialKeyMiter);
		  Abc_NtkDelete( pInferPartialKeyMiter );
		  
		  if ( PartialKeySatStatus && (PartialKeySatStatus != -1) ) {		    
		    if ( !ClapAttack_RunSat(*ppCurKeyCnf) ){
		      
		      Abc_NtkForEachPi( *ppCurKeyCnf, pKey, m ) {
			if ( !strcmp(Abc_ObjName(pKey), KeyNameTmp[k]) ) {
			  KeyValue = (*ppCurKeyCnf)->pModel[m];
			  ClapAttack_UpdateKey(KeyNameTmp[k], KeyValue, pGlobalBsiKeys);
			  printf("We determined that key %s is %d\n", Abc_ObjName(pKey), KeyValue);
			  //exit(0);
			  // Either remove the key or set it to constant 1.
			  break;
			}
		      }
		      // Optimize out key from partial CNF
		      ntkTmp = Abc_NtkToLogic( *ppCurKeyCnf );
		      Abc_NtkDelete( *ppCurKeyCnf );
		      *ppCurKeyCnf = ntkTmp;
		      Abc_NtkForEachPi( *ppCurKeyCnf, pKey, m ) {
			if ( !strcmp(Abc_ObjName(pKey), KeyNameTmp[k]) ) {
			  Abc_ObjReplaceByConstant(pKey, KeyValue);
			  Abc_NtkDeleteObj(pKey);
			  break;
			}
		      }
		      
		      // Check if we optimized out all the PIs but one.
		      // IF so, free the partial info
		      ntkTmp = Abc_NtkStrash( *ppCurKeyCnf, 0, 1, 0 );
		      Abc_NtkDelete( *ppCurKeyCnf );
		      *ppCurKeyCnf = ntkTmp;
			
		      if ( (Abc_NtkPiNum(*ppCurKeyCnf) < 2) && fCurKeyCnfAlloc) {
			//ABC_FREE(pCurKeyCnf);
			Abc_NtkDelete( *ppCurKeyCnf );
			*ppCurKeyCnf = NULL;
			break;
		      } 
		      //printf("After\n");
		    } else {
		      printf("The key is somehow UNSAT... Exiting.\n\n");
		      exit(0);
		    }		      
		  }
		  //ABC_FREE(pInferPartialKeyMiter);
		  
		  //exit(0);
		}

	      }
	      free( WrongKeyValue );
	      free( KeyWithFreq );
	      free( KeyNoFreq );
	    
	    //printf("Key successfully simualted on oracle. The new inferred key is %s : %d \n", KeyNameTmp[0], KeyValue);
	    
	    // Print off updated keystore
	      printf("KeyStore is now: {");
	      for (k=0; k<pGlobalBsiKeys->NumKeys; k++)
		printf("%d, ", pGlobalBsiKeys->KeyValue[k]);
	      printf("}\n\n");
	      //exit(0);
	      // Key found!
	      // TODO - Update Keystore
	      //free(ppNodeFreeList);
	      //for(k=0; k<MaxKeysConsidered; k++)
	      //  free(KeyNameTmp[k]);
	      //free(KeyNameTmp);
	      //return 1;
	      /* End Probe Point Counter Comment */

	      /* Probe Point Counter 
	      SatStatus = 1;
	      /* End Probe Point Counter */
	    }
	    //ABC_FREE(pNtkMiter);
	  } else {
	    //ABC_FREE(pNtkMiter);
	    printf("Mitering failed. Proceed.\n");
	  }
	  Abc_NtkDelete( pNtkMiter );
	  //if (!MiterStatus)
	  //  Abc_NtkDelete( pNtkMiter );
	} //else {
	  //printf("This cone has %d key inputs. For now we only handle nodes with up to %d keys. Proceed.\n", NumKeys, MaxKeysConsidered);
	//}
      } while( !SatStatus && !MiterStatus);
      Abc_NtkDelete( pNtkCone );
      //ABC_FREE(pNtkCone);

      for(k=0; k<MaxKeysConsidered; k++)
	free(KeyNameTmp[k]);
      free(KeyNameTmp);
      free(ppNodeFreeList);
    } //else
      //printf("We already visited this node.\n");




    
    if( NumKeys <= MaxKeysConsidered ) {
      ClapAttack_TraversalRecursive( pNtk, pNode, pGlobalBsiKeys, pOracleKey, MaxKeysConsidered, ppCurKeyCnf, pTotalProbes );
    }
  }
  
  // Save off any key cnf info we found to the global key CNF store
  if ( (*ppCurKeyCnf) && fCurKeyCnfAlloc ) {
    ////printf("In we go!\n\n\n\n\n\n\n");
    if( ClapAttack_UpdateGlobalKeyCnf ( ppCurKeyCnf, pGlobalBsiKeys ) )
      printf("Global CNF Update Failed. \n");
    Abc_NtkDelete( *ppCurKeyCnf );
  }


    



}

void ClapAttack_CombineMitersHeuristic( struct SatMiterList ** ppSatMiterListOld, struct SatMiterList ** ppSatMiterListNew, int * pMaxNodesConsidered, int MaxKeysConsidered, int MaxPiNum, int fConsiderAll ) {


  int i, j, k, m, n, SatStatus, NumKeysOld, NumKeysNew, CurProbeCount, CurKeyIdx, fNodePresent, CurKeyIdxNew, NumKeysNew_rev, MiterListLen;
  float IdentifiableKeys, IdentifiableKeysOld, IdentifiableKeysMax;
  Abc_Ntk_t *pNtkMiter, *pNtkMiterBase;
  Abc_Obj_t ** ppNode = (Abc_Obj_t **) malloc ( sizeof(Abc_Obj_t *) * 400 );
  struct SatMiterList *pMiterListCurBase, *pMiterListCurForward;
  char **CurKeysInferred;
  char **CurKeysInferredMiter;


  CurKeysInferred = (char **)malloc( sizeof(char *) * (MaxPiNum));
  for (i=0; i<MaxPiNum; i++) {
    CurKeysInferred[i] = (char *)malloc( sizeof(char) * 100);
  }

  CurKeysInferredMiter = (char **)malloc( sizeof(char *) * (MaxPiNum));
  for (i=0; i<MaxPiNum; i++) {
    CurKeysInferredMiter[i] = (char *)malloc( sizeof(char) * 100);
  }
    
  // Set miter list pointers to test combined miters for heuristic
  pMiterListCurBase = *ppSatMiterListOld;
  MiterListLen = 0;

  // Init identifiable keys
  IdentifiableKeysMax = pMiterListCurBase->IdentifiableKeyBits;
  
  // Combine pairwise compatible miters
  while ( pMiterListCurBase && (MiterListLen < 100)) {
    
    // Set base miter without combination to combine into throughout inner loop
    pNtkMiterBase =  Abc_NtkDup( pMiterListCurBase->pMiter );

    // Initialize ppNode list of matched nodes in miter
    for ( i=0; i<pMiterListCurBase->MatchedNodes; i++ ) {
      ppNode[i] = pMiterListCurBase->ppSatNode[i];
      //printf(" Node Added Base: %s\n", Abc_ObjName(pMiterListCurBase->ppSatNode[i]));
    }

    // Generate list of solved for keys for this base-node
    for( CurKeyIdxNew=0; CurKeyIdxNew<pMiterListCurBase->NumKeys; CurKeyIdxNew++ ) {
      // Are we looking at a key input?
      //printf("Old: %s\n",  pMiterListCurBase->KeyNames[CurKeyIdxNew]);
      strcpy(CurKeysInferredMiter[CurKeyIdxNew], pMiterListCurBase->KeyNames[CurKeyIdxNew]);
    }    

    /*
    CurKeyIdx = 0;
    Abc_NtkForEachPi( pMiterListCurBase->pMiter, pPi, n ) {
      // Are we looking at a key input?
      if( strstr(Abc_ObjName(pPi), "key") ) {
	if( strstr(Abc_ObjName(pPi), "_1") ) {
	  strcpy(CurKeysInferred[CurKeyIdx], Abc_ObjName(pPi));
	  //printf("New: %s\n", CurKeysInferred[CurKeyIdx]);
	  CurKeyIdx++;
	}
      }
      }*/
    // Generate list of solved for keys for this base-node
    for( CurKeyIdx=0; CurKeyIdx<pMiterListCurBase->NumKeys; CurKeyIdx++ ) {
      // Are we looking at a key input?
      //printf("Old: %s\n",  pMiterListCurBase->KeyNames[CurKeyIdxNew]);
      strcpy(CurKeysInferred[CurKeyIdx], pMiterListCurBase->KeyNames[CurKeyIdx]);
    }    
    
    // Set forward pointer
    pMiterListCurForward = pMiterListCurBase->pNext;
    CurProbeCount = 0;
    
    while ( pMiterListCurForward ) {

      // Reset SAT status flag and Identifiable key count
      SatStatus = 1;
      IdentifiableKeys = 0;
      i=pMiterListCurBase->MatchedNodes;
      k=0;
      NumKeysNew_rev = CurKeyIdxNew;
      
      // Add remaining nodes to ppNode list
      for ( j=0; j<pMiterListCurForward->MatchedNodes; j++ ) {
	
	// Reset flag that avoids duplicate nodes
	//fNodePresent = 0;

	
	// If not, add the node to ppnode list
	if ( j == pMiterListCurForward->MatchedNodes-1 ) {

	  // If the number of considered nodes is more than 1, we can only
	  // Consider miters that share at least one node... otherwise,
	  // they will never be SAT.
	  // Miter doesnt have overlap... it will be UNSAT. Break
	  //if ( (i >= *pMaxNodesConsidered) && (i > 2) ) {
	  //  SatStatus = 1;
	  //  break;
	  //}
	  if (pMiterListCurForward->ppSatNode[j] != ppNode[k]) {


	    // See if we're actually generating a new key...


	    for( m=0; m<pMiterListCurForward->NumKeys; m++ ) {
	      // Are we looking at a key input?
	      fNodePresent = 0;
	      for( n = 0; n < CurKeyIdxNew; n++) {
		//printf("Compare %s %s\n", CurKeysInferredMiter[n], pMiterListCurForward->KeyNames[m]);
		if ( !strcmp(CurKeysInferredMiter[n], pMiterListCurForward->KeyNames[m]) ) {
		  fNodePresent = 1;
		}
	      }
	      
	      // We have a new key
	      if (!fNodePresent) {
		strcpy(CurKeysInferredMiter[NumKeysNew_rev], pMiterListCurForward->KeyNames[m]);
		NumKeysNew_rev++;
		//printf("Key Name Added1: %s\n", pMiterListCurForward->KeyNames[m]);
		//SatStatus = 0;
		//printf("OLD\n");
	      }
	    }

	    for( m=0; m<pMiterListCurForward->NumKeys; m++ ) {
	      // Are we looking at a key input?
	      fNodePresent = 0;
	      for( n = 0; n < CurKeyIdx; n++) {
		if ( !strcmp(CurKeysInferred[n], pMiterListCurForward->KeyNames[m]) ) {
		  fNodePresent = 1;
		}
	      }
	      
	      // We have a new key
	      if ((!fNodePresent) || (MaxKeysConsidered > 2)) {
		SatStatus = 0;
	      }
	    }

	    /*
	    // See if we're actually generating a new key...
	    fNodePresent = 0;
	    Abc_NtkForEachPi( pMiterListCurForward->pMiter, pPi, m ) {
	      // Are we looking at a key input?
	      if( strstr(Abc_ObjName(pPi), "key") ) {
		if( strstr(Abc_ObjName(pPi), "_1") ) {
		  for( n = 0; n < CurKeyIdx; n++) {
		    if ( !strcmp(CurKeysInferred[n], Abc_ObjName(pPi)) ) {
		      fNodePresent = 1;
		    }
		  }

		  if (!fNodePresent) {
		    SatStatus = 0;
		    printf("NEW\n");
		    //printf("Key Name New: %s\n", Abc_ObjName(pPi));
		  }
		}
	      }
	      }*/
	    
	    ppNode[i] = pMiterListCurForward->ppSatNode[j];
	    i++;
	  }
	  
	  break;
	} //else {
	//  printf(" Node NOT Added: %s\n", Abc_ObjName(pMiterListCurForward->ppSatNode[j]));
	//}
	
	// Check if node is already present in ppnode list
	if ( pMiterListCurForward->ppSatNode[j] == ppNode[k] ) {
	  k++;
	} else {
	  break;
	}
	//	  fNodePresent = 1;	
      }


           
      // Edge case... too much overlap. We're not gaining info.
      //if ( i < *pMaxNodesConsidered )
      //SatStatus = 1;

      //printf("Here, Sat Status is: %d\n\n", SatStatus);
      
      if (!SatStatus) {

	// Set miter that will be combined into.
	pNtkMiter = Abc_NtkDup( pMiterListCurForward->pMiter );

	
	// Merge miters
	if ( !Abc_NtkAppendSilentAnd( pNtkMiter, pNtkMiterBase, 0 ) ) {
	  Abc_Print( -1, "Appending the networks failed.\n" );
	  return;
	}

	//printf("Running SAT on Combined Miter circuit.\n");
	
	
	// Is miter SAT?
	if ( !ClapAttack_RunSat(pNtkMiter) ) {

	  
	  /*// Calculate how many keys we will discover
	  NumKeysNew = 0;
	  Abc_NtkForEachPi( pNtkMiter, pPi, k ) {
	    // Are we looking at a key input?
	    if( strstr(Abc_ObjName(pPi), "key") ) {
	      //printf("Key Name New: %s\n", Abc_ObjName(pPi));
	      NumKeysNew++;
	    }
	  }

	  // Calculate how many keys we discovered with old miter
	  NumKeysOld = 0;
	  Abc_NtkForEachPi( pMiterListCurForward->pMiter, pPi, k ) {
	    // Are we looking at a key input?
	    if( strstr(Abc_ObjName(pPi), "key") ) {
	      NumKeysOld++;
	      //printf("Key Name Old: %s\n\n\n", Abc_ObjName(pPi));
	    }
	    }*/
	  
	  NumKeysNew = NumKeysNew_rev;
	  NumKeysOld = CurKeyIdxNew;

	  //ClapAttack_WriteMiterVerilog(pMiterListCurForward->pMiter, "old_miter.v");
	  //ClapAttack_WriteMiterVerilog(pNtkMiter, "new_miter.v");
	  
	  // Calculate conservative estimate of identifiable keys
	  //printf("Old: %d, New: %d\n", NumKeysOld, NumKeysNew);
	  
	  // We will always at least find as many keys as the best submiter...
	  IdentifiableKeysOld = ( pMiterListCurForward->IdentifiableKeyBits >  pMiterListCurBase->IdentifiableKeyBits ) ? pMiterListCurForward->IdentifiableKeyBits :  pMiterListCurBase->IdentifiableKeyBits;
	  //printf("Old bits: %f, New bits: %f\n\n\n", pMiterListCurForward->IdentifiableKeyBits, pMiterListCurBase->IdentifiableKeyBits);
	  
	  // We also may find additional keys
	  if ( (*pMaxNodesConsidered == (pMiterListCurBase->MatchedNodes+1)) && (NumKeysNew > NumKeysOld) ) {
	    IdentifiableKeys = IdentifiableKeysOld + (1.0/(1<<(MaxKeysConsidered-1)));

	    if ( IdentifiableKeys >= IdentifiableKeysMax ) {

	      IdentifiableKeysMax = IdentifiableKeys;

	      // See if we're actually generating a new key...
	      for( m=0; m<pMiterListCurForward->NumKeys; m++ ) {
		fNodePresent = 0;
		for( n = 0; n < CurKeyIdx; n++) {
		  if ( !strcmp(CurKeysInferred[n], pMiterListCurForward->KeyNames[m]) ) {
		    fNodePresent = 1;
		  }
		}
		
		// We have a new key
		if (!fNodePresent) {
		  strcpy(CurKeysInferred[CurKeyIdx], pMiterListCurForward->KeyNames[m]);
		  CurKeyIdx++;
		}
	      }
	      
	      /*
	      // See if we're actually generating a new key...
	      fNodePresent = 0;
	      Abc_NtkForEachPi( pNtkMiter, pPi, m ) {
	      // Are we looking at a key input?
	      if( strstr(Abc_ObjName(pPi), "key") ) {
	      if( strstr(Abc_ObjName(pPi), "_1") ) {
	      for( n = 0; n < CurKeyIdx; n++) {
	      if ( !strcmp(CurKeysInferred[n], Abc_ObjName(pPi)) ) {
	      fNodePresent = 1;
	      }
	      }
	      
	      if (!fNodePresent) {
	      printf("Key Name Added: %s\n", Abc_ObjName(pPi));
	      strcpy(CurKeysInferred[CurKeyIdx], Abc_ObjName(pPi));
	      CurKeyIdx++;
	      }
	      
	      }
	      }
	      }*/

	      // Increase length of miter list
	      MiterListLen++;
	      
	      // Update the miterlist with a new functional pairing	      
	      ClapAttack_UpdateSatMiterList( ppSatMiterListNew, ppNode, pNtkMiter, NumKeysNew_rev, CurKeysInferredMiter, *pMaxNodesConsidered, IdentifiableKeys, pNtkMiter->pModel );
	    }
	  }/* else {
	    printf("Fail1\n");

	    if (MaxKeysConsidered == 2) {
	      printf("Old: %d, New: %d\n", NumKeysOld, NumKeysNew);
	      printf("Considered: %d, Actual %d\n", *pMaxNodesConsidered, (pMiterListCurBase->MatchedNodes)+1);
	      ClapAttack_WriteMiterVerilog(pNtkMiterBase, "old_miter.v");
	      ClapAttack_WriteMiterVerilog(pNtkMiter, "new_miter.v");
	      //exit(0);
	    }
	    }*/
	  //else
	    //IdentifiableKeys = IdentifiableKeysOld;
	} //else
	  //printf("Fail2\n");
	
	// Cleanup
	Abc_NtkDelete( pNtkMiter );
      } else {
	if ( !fConsiderAll ) {
	  break;
	}
      }

      // Consider next forward node...
      pMiterListCurForward = pMiterListCurForward->pNext;
      
    }

    // Consider next base node
    pMiterListCurBase = pMiterListCurBase->pNext;
    Abc_NtkDelete( pNtkMiterBase );
    
  }

  // Cleanup
  for (i=0; i<MaxPiNum; i++) {
    free(CurKeysInferred[i]);
  }
  free(CurKeysInferred);

  for (i=0; i<MaxPiNum; i++) {
    free(CurKeysInferredMiter[i]);
  }
  free(CurKeysInferredMiter);
  
  free(ppNode);
}


  /*
  int i, j, k, m, n, SatStatus, NumKeysOld, NumKeysNew, CurProbeCount, CurKeyIdx, fNodePresent, NewMaxNodesConsidered, fCurNodeDone;
  float MaxIdentifiableKeys = 0;
  float IdentifiableKeys, IdentifiableKeysOld;
  Abc_Ntk_t *pNtkMiter, *pNtkMiterBase;
  Abc_Obj_t ** ppNode;
  struct SatMiterList *pMiterListCurBase, *pMiterListCurForward;
  char **CurKeysInferred;

    CurKeysInferred = (char **)malloc( sizeof(char *) * (MaxPiNum));
  for (i=0; i<MaxPiNum; i++) {
    CurKeysInferred[i] = (char *)malloc( sizeof(char) * 100);
  }

  // Figure out max list length for mallco of ppnode list
  NewMaxNodesConsidered = 0;
  for ( pMiterListCurBase = *ppSatMiterListOld; pMiterListCurBase->pNext; pMiterListCurBase = pMiterListCurBase->pNext) {
    if (pMiterListCurBase->MatchedNodes + 1 > NewMaxNodesConsidered)
      NewMaxNodesConsidered = pMiterListCurBase->MatchedNodes + 1;
  }

  // malloc ppnode based on longest possible matched list
  ppNode = (Abc_Obj_t **) malloc ( sizeof(Abc_Obj_t *) * NewMaxNodesConsidered );

  // Reset Set miter list pointers to test combined miters for heuristic
  pMiterListCurBase = *ppSatMiterListOld;
  
  // Combine pairwise compatible miters
  while ( pMiterListCurBase ) {
    
    // Set base miter without combination to combine into throughout inner loop
    pNtkMiterBase =  Abc_NtkDup( pMiterListCurBase->pMiter );

    // Initialize ppNode list of matched nodes in miter
    for ( i=0; i<pMiterListCurBase->MatchedNodes; i++ ) {
      ppNode[i] = pMiterListCurBase->ppSatNode[i];
      //printf(" Node Added Base: %s\n", Abc_ObjName(pMiterListCurBase->ppSatNode[i]));
    }

    // Set forward pointer
    pMiterListCurForward = pMiterListCurBase->pNext;
    CurProbeCount = 0;

    // Generate list of solved for keys for this base-node
    for( CurKeyIdx=0; CurKeyIdx<pMiterListCurBase->NumKeys; CurKeyIdx++ ) {
      // Are we looking at a key input?
      strcpy(CurKeysInferred[CurKeyIdx], pMiterListCurBase->KeyNames[CurKeyIdx]);
      printf(" Current Keys: %s\n", CurKeysInferred[CurKeyIdx]);
    }
    
    while ( pMiterListCurForward ) {

      // Reset SAT status flag and Identifiable key count
      SatStatus = 1;
      IdentifiableKeys = 0;
      i=pMiterListCurBase->MatchedNodes;
      k=0;
      NumKeysNew = CurKeyIdx;
      fCurNodeDone = 0;
     
      // Add remaining nodes to ppNode list
      for ( j=0; j<pMiterListCurForward->MatchedNodes; j++ ) {
	
	// If not, add the node to ppnode list
	if ( j == pMiterListCurForward->MatchedNodes-1 ) {
	  if (pMiterListCurForward->ppSatNode[j] != ppNode[k]) {
	    // See if we're actually generating a new key...
	    fNodePresent = 0;
	    for( m=0; m<pMiterListCurForward->NumKeys; m++ ) {
	      // Are we looking at a key input?
	      for( n = 0; n < CurKeyIdx; n++) {
		printf(" Considered key: %s Old Key Compared to: %s, curkeyidx %d, NumKeys: %d\n", CurKeysInferred[n], pMiterListCurForward->KeyNames[m], CurKeyIdx, pMiterListCurForward->NumKeys);
		if ( !strcmp(CurKeysInferred[n], pMiterListCurForward->KeyNames[m]) ) {
		      fNodePresent = 1;
		}
	      }

	      // We have a new key
	      if (!fNodePresent) {
		printf(" New Keys: %s\n", pMiterListCurForward->KeyNames[m]);
		strcpy(CurKeysInferred[NumKeysNew], pMiterListCurForward->KeyNames[m]);
		NumKeysNew++;
		SatStatus = 0;
	      }
	    }
	    ppNode[i] = pMiterListCurForward->ppSatNode[j];
	    i++;
	  }

	  break;
	}

	// Check if node is already present in ppnode list
	if ( pMiterListCurForward->ppSatNode[j] == ppNode[k] ) {
	  k++;
	} else {
	  fCurNodeDone = 1;
	  break;
	}
      }

      if (!SatStatus) {
	// Set miter that will be combined into.
	pNtkMiter = Abc_NtkDup( pMiterListCurForward->pMiter );

	
	// Merge miters
	if ( !Abc_NtkAppendSilentAnd( pNtkMiter, pNtkMiterBase, 0 ) ) {
	  Abc_Print( -1, "Appending the networks failed.\n" );
	  return;
	}

	printf("Running SAT on Combined Miter circuit.\n");

	// Is miter SAT?
	if ( !ClapAttack_RunSat(pNtkMiter) ) {
	  

	  NumKeysOld = CurKeyIdx;

	  // Calculate conservative estimate of identifiable keys
	  printf("Old: %d, New: %d\n\n\n", NumKeysOld, NumKeysNew);
	  
	  // We will always at least find as many keys as the best submiter...
	  IdentifiableKeysOld = ( pMiterListCurForward->IdentifiableKeyBits >  pMiterListCurBase->IdentifiableKeyBits ) ? pMiterListCurForward->IdentifiableKeyBits :  pMiterListCurBase->IdentifiableKeyBits;
	  NewMaxNodesConsidered = ( pMiterListCurForward->MatchedNodes >  pMiterListCurBase->MatchedNodes ) ? pMiterListCurForward->MatchedNodes+1 : pMiterListCurBase->MatchedNodes + 1;
	  
	  // We also may find additional keys
	  if (NumKeysNew > NumKeysOld) {
	    IdentifiableKeys = IdentifiableKeysOld + (1.0/(1<<(MaxKeysConsidered-1)));

	    // Update the miterlist with a new functional pairing
	    if ( MaxIdentifiableKeys <= IdentifiableKeys) {
	      ClapAttack_UpdateSatMiterList( ppSatMiterListNew, ppNode, pNtkMiter, NumKeysNew, CurKeysInferred, NewMaxNodesConsidered, IdentifiableKeys, pNtkMiter->pModel );
	      MaxIdentifiableKeys = IdentifiableKeys;
	    }
	  }
	}
	
	// Cleanup
	Abc_NtkDelete( pNtkMiter );
      } 
      if (fCurNodeDone && (*pMaxNodesConsidered > 2)) {
	break;
      }


      // Consider next forward node...
      pMiterListCurForward = pMiterListCurForward->pNext;
      
    }

    // Consider next base node
    pMiterListCurBase = pMiterListCurBase->pNext;
    Abc_NtkDelete( pNtkMiterBase );
    
  }

  // Cleanup
  for (i=0; i<MaxPiNum; i++) {
    free(CurKeysInferred[i]);
  }
  free(CurKeysInferred);
  
  free(ppNode);

}
*/ 


void ClapAttack_TraversalRecursiveHeuristic( Abc_Ntk_t * pNtk, Abc_Obj_t * pCurNode, struct BSI_KeyData_t * pGlobalBsiKeys, int MaxKeysConsidered, Abc_Ntk_t ** ppCurKeyCnf, struct SatMiterList ** ppSatMiterList, int *pNumProbes, int MaxProbes ) {

  int i, j, k, SatStatus, MiterStatus, NumKeys, NumKnownKeys, fCurKeyCnfAlloc;
  Abc_Ntk_t *pNtkCone, *pNtkMiter;
  Abc_Obj_t * pNode, * pPi, **ppNodeFreeList;
  char ** KeyNameTmp;




  // Initialize partial key info to NULL
  fCurKeyCnfAlloc = 0;
  NumKeys = 0;
  
  // Goal: For each PI that is a key, follow fanout until it intersects with unknown key.
  Abc_ObjForEachFanout( pCurNode, pNode, i ) {
  

    // Have we visited this node before?
    if (!pNode->fMarkC) {


      // Initialzie free list to the number of keys present...
      ppNodeFreeList = (Abc_Obj_t **)malloc( sizeof(Abc_Obj_t *) * pGlobalBsiKeys->NumKeys);
      
      KeyNameTmp = (char **)malloc( sizeof(char *) * MaxKeysConsidered);
      for (k=0; k<MaxKeysConsidered; k++) {
	KeyNameTmp[k] = (char *)malloc( sizeof(char) * 100);
      }
      
      // Check to make sure were not lookign at a PO. If so,
      // don't bother pursuing the fanout.
      if ( Abc_ObjIsCo(pNode) ) {
	///printf("This node is a primary output. Done with fanout.\n");       
	for(k=0; k<MaxKeysConsidered; k++)
	  free(KeyNameTmp[k]);
	free(KeyNameTmp);
	free(ppNodeFreeList);
	continue;
      }


      
      // Check supports ... if only one is an unknown key,
      // process it and continue fanout
      ClapAttack_IsolateCone(pNtk, &pNtkCone, pNode);
      //ClapAttack_WriteMiterVerilog(pNtkCone, "cone.v");
      
      // Loop over the miter generation phase until SAT fails
      NumKeys = 0;
      NumKnownKeys = 0;
      
      SatStatus = 1;
      MiterStatus = 1;
      
      Abc_NtkForEachPi( pNtkCone, pPi, j ) {
	// Are we looking at a key input?
	if( strstr(Abc_ObjName(pPi), "key") ) {
	  if ( !ClapAttack_SetKnownKeys( pNtkCone, pPi, pGlobalBsiKeys ) ) {
	    if( NumKeys < MaxKeysConsidered )
	      strcpy(KeyNameTmp[NumKeys], Abc_ObjName(pPi) );
	    NumKeys++;
	  } else {
	    ppNodeFreeList[NumKnownKeys] = pPi;
	    NumKnownKeys++;	    
	  }
	}
      }
      
      // Delete known keys from cone.
      if ( NumKnownKeys )
	ClapAttack_DelKnownKeys( ppNodeFreeList, NumKnownKeys );
            
      if ( (NumKeys == MaxKeysConsidered) && NumKeys ) {
	
	printf("Evaluating node %s\n", Abc_ObjName(pNode) );	  
	//printf("The number of keys is: %d\n", NumKeys);	  
	//printf("Generating miter circuit.\n");       
	
	MiterStatus = ClapAttack_MakeMiterHeuristic(pNtkCone, *ppCurKeyCnf, &pNtkMiter );

	if (!MiterStatus) {
	  
	  //printf("Running SAT on Miter circuit.\n");       
	  //ClapAttack_WriteMiterVerilog(pNtkMiter, Abc_ObjName(pNode));
	  SatStatus = ClapAttack_RunSat(pNtkMiter);
	  
	  if (!SatStatus) {

	    //ClapAttack_WriteMiterVerilog(pNtkCone, Abc_ObjName(pNode));
	  //ClapAttack_WriteMiterVerilog(pNtkMiter, "miter.v");

	    // Update SAT node list with new Sat miter
	    //printf(" Test: %f %d \n\n\n",  1.0/(1<<(MaxKeysConsidered-1)), MaxKeysConsidered);
	    pNode->fMarkC = 1;
	    (*pNumProbes)++;
	    ClapAttack_UpdateSatMiterList( ppSatMiterList, &pNode, pNtkMiter, NumKeys, KeyNameTmp, 1, 1.0/(1<<(MaxKeysConsidered-1)), pNtkMiter->pModel );
	  } else {
	    // Node is UNSAT, but has the right number of keys... It's useless so mark it
	    pNode->fMarkC = 1;
	  }
	} else {
	  printf("Mitering failed. Proceed.\n");
	}
	Abc_NtkDelete( pNtkMiter );
      }
      Abc_NtkDelete( pNtkCone );
      for(k=0; k<MaxKeysConsidered; k++)
	free(KeyNameTmp[k]);
      free(KeyNameTmp);
      free(ppNodeFreeList);


    }


  
    // Recurse
    if( (NumKeys <= MaxKeysConsidered) && (*pNumProbes <= MaxProbes) ) {
      ClapAttack_TraversalRecursiveHeuristic( pNtk, pNode, pGlobalBsiKeys, MaxKeysConsidered, ppCurKeyCnf, ppSatMiterList, pNumProbes, MaxProbes );
    }
  }

}

// Evaluate a known model for heuristic case
void ClapAttack_EvalMultinodeProbe ( struct SatMiterList *pSatMiter, Abc_Ntk_t *pNtk, struct BSI_KeyData_t * pGlobalBsiKeys, int *pOracleKey, Abc_Ntk_t ** ppCurKeyCnf, int MaxKeysConsidered ) {

  int *pFullDi;
  int i, j, k, m, NumKeys, NumKnownKeys, *KeyWithFreq, *KeyNoFreq, *WrongKeyValue, KeyValue, PartialKeySatStatus, fCurKeyCnfAlloc, fRerunInfer;
  Abc_Ntk_t *pNtkCone, *pInferPartialKeyMiter, *ntkTmp;
  Abc_Obj_t * pNode, * pPi, * pKey, **ppNodeFreeList;
  char ** KeyNameTmp;
  struct BSI_KeyData_t GlobalBsiKeysTmp;
  struct BSI_KeyData_t *pGlobalBsiKeysTmp;
  //struct SatMiterList {
  //Abc_Obj_t ** ppSatNode;
  //int MatchedNodes;
  //Abc_Ntk_t *pMiter;
  //float IdentifiableKeyBits;
  //struct SatMiterList * pNext;
  //struct SatMiterList * pPrev;
  //};

  ppNodeFreeList = (Abc_Obj_t **)malloc( sizeof(Abc_Obj_t *) * pGlobalBsiKeys->NumKeys);

  KeyNameTmp = (char **)malloc( sizeof(char *) * MaxKeysConsidered);
  for (j=0; j<MaxKeysConsidered; j++) {
    KeyNameTmp[j] = (char *)malloc( sizeof(char) * 100);
  }

  // Temporarily freeze off keystore
  pGlobalBsiKeysTmp = &GlobalBsiKeysTmp;
  ClapAttack_CopyKeyStore( pGlobalBsiKeys, pGlobalBsiKeysTmp );
  
  // Check supports ... if only one is an unknown key,
  // process it and continue fanout
  for ( i = 0; i < pSatMiter->MatchedNodes; i++ ) {
    pNode = pSatMiter->ppSatNode[i];
    ClapAttack_IsolateCone(pNtk, &pNtkCone, pSatMiter->ppSatNode[i]);
      
    // Loop over the miter generation phase until SAT fails
    NumKeys = 0;
    NumKnownKeys = 0;
    
    Abc_NtkForEachPi( pNtkCone, pPi, j ) {
      // Are we looking at a key input?
      if( strstr(Abc_ObjName(pPi), "key") ) {
	if ( !ClapAttack_SetKnownKeys( pNtkCone, pPi, pGlobalBsiKeysTmp ) ) {
	  strcpy(KeyNameTmp[NumKeys], Abc_ObjName(pPi) );
	  NumKeys++;
	  //printf("Keyname: %s, KeyNum: %d\n",  Abc_ObjName(pPi), NumKeys);
	} else {
	  ppNodeFreeList[NumKnownKeys] = pPi;
	  NumKnownKeys++;	    
	}
      }
    }
    
    // Delete known keys from cone.
    if ( NumKnownKeys )
      ClapAttack_DelKnownKeys( ppNodeFreeList, NumKnownKeys );

    // Mark node as evaluated.
    //pNode->fMarkC = 1;
	
    //if ( (NumKeys <= MaxKeysConsidered) && NumKeys ) {

    printf("Evaluating node %s\n", Abc_ObjName(pNode) );	  
    printf("The number of keys is: %d\n", NumKeys);	  
    // Remove unused logic and clean the PIs
    //ClapAttack_CleanCone(&pNtkCone);
    //ClapAttack_WriteMiterVerilog(pNtkCone, "cone.v");
    
    // We're processing the node. Mark it so we don't do this again.
    // If it fails here, there is no information we can ever gain
    //pNode->fMarkC = 1;
    
    //printf("Generating miter circuit.\n");       
    //ClapAttack_WriteMiterVerilog(pNtkCone, "cone_test1.v");
    //MiterStatus = ClapAttack_MakeMiter( pNtkCone, *ppCurKeyCnf, &pNtkMiter );
    //Abc_NtkPrintIo( stdout, pNtkMiter, 1 );
    //Abc_NtkPrintIo( stdout, pNtkCone, 1 );
    
    //if (!MiterStatus) {
    
    //ClapAttack_WriteMiterVerilog(pNtkCone, Abc_ObjName(pNode));
    
    // Malloc key values from SAT to infer from
    KeyWithFreq = (int *)malloc( sizeof(int) * NumKeys );
    KeyNoFreq = (int *)malloc( sizeof(int) * NumKeys );
    
    ClapAttack_InterpretDiHeuristic(pNtkCone, pSatMiter->pMiter, pSatMiter->pModel, NumKeys, KeyWithFreq, KeyNoFreq, &pFullDi);
    
    // Oracle testing. Comment out with real probe.
    ClapAttack_OracleSetConeKeys(pNtkCone, pFullDi, pOracleKey);
    
    WrongKeyValue = (int *)malloc( sizeof(int) * NumKeys);
    ClapAttack_OracleSimDi(pNtkCone, pFullDi, NumKeys, KeyWithFreq, KeyNoFreq, WrongKeyValue);

    // Cleanup
    free(pFullDi);
    
    printf("Wrong KeyValue: ");
    for (k=0; k<NumKeys; k++)
      printf(" %d", WrongKeyValue[k]);
    printf("\n\n");
    
    // Initialize partial key logic if it doesnt exist
    // Otherwise update it...
    if( NumKeys > 1 ) {
      if ( !(*ppCurKeyCnf) ) {
	ClapAttack_InitKeyCnf( ppCurKeyCnf, NumKeys, WrongKeyValue, KeyNameTmp );
	fCurKeyCnfAlloc = 1;
	//ppCurKeyCnf = &pCurKeyCnf;
      } else {
	ClapAttack_UpdateKeyCnf( ppCurKeyCnf, NumKeys, WrongKeyValue, KeyNameTmp );
      }
    }
    
    // Short term hack to handle case wehre key is of length 1
    if ( NumKeys == 1) {
      KeyValue = (WrongKeyValue[0] + 1) % 2;
      ClapAttack_UpdateKey(KeyNameTmp[0], KeyValue, pGlobalBsiKeys);
      //Abc_NtkDelete( pNtkMiter );
      //ABC_FREE(pNtkMiter);
      //break;
    } else {
      // Evaluate partial key logic for complete key info
      for (k=0; k<NumKeys; k++) {
	//printf("KeyName: %s \n\n\n\n\n", KeyNameTmp[k]);
	ClapAttack_PartialKeyInferenceMiter( *ppCurKeyCnf, &pInferPartialKeyMiter, KeyNameTmp[k] );
	PartialKeySatStatus = ClapAttack_RunSat(pInferPartialKeyMiter);
	Abc_NtkDelete( pInferPartialKeyMiter );
	
	if ( PartialKeySatStatus && (PartialKeySatStatus != -1) ) {		    
	  if ( !ClapAttack_RunSat(*ppCurKeyCnf) ){
	    
	    Abc_NtkForEachPi( *ppCurKeyCnf, pKey, m ) {
	      if ( !strcmp(Abc_ObjName(pKey), KeyNameTmp[k]) ) {
		KeyValue = (*ppCurKeyCnf)->pModel[m];
		ClapAttack_UpdateKey(KeyNameTmp[k], KeyValue, pGlobalBsiKeys);
		printf("We determined that key %s is %d\n", Abc_ObjName(pKey), KeyValue);
		//exit(0);
		// Either remove the key or set it to constant 1.
		break;
	      }
	    }
	    // Optimize out key from partial CNF
	    ntkTmp = Abc_NtkToLogic( *ppCurKeyCnf );
	    Abc_NtkDelete( *ppCurKeyCnf );
	    *ppCurKeyCnf = ntkTmp;
	    Abc_NtkForEachPi( *ppCurKeyCnf, pKey, m ) {
	      if ( !strcmp(Abc_ObjName(pKey), KeyNameTmp[k]) ) {
		Abc_ObjReplaceByConstant(pKey, KeyValue);
		Abc_NtkDeleteObj(pKey);
		break;
	      }
	    }
	    
	    // Check if we optimized out all the PIs but one.
	    // IF so, free the partial info
	    ntkTmp = Abc_NtkStrash( *ppCurKeyCnf, 0, 1, 0 );
	    Abc_NtkDelete( *ppCurKeyCnf );
	    *ppCurKeyCnf = ntkTmp;
	    
	    if ( (Abc_NtkPiNum(*ppCurKeyCnf) < 2) && fCurKeyCnfAlloc) {
	      //ABC_FREE(pCurKeyCnf);
	      Abc_NtkDelete( *ppCurKeyCnf );
	      *ppCurKeyCnf = NULL;
	      break;
	    } 
	    //printf("After\n");
	  } else {
	    printf("The key is somehow UNSAT... Exiting.\n\n");
	    exit(0);
	  }		      
	}
	//ABC_FREE(pInferPartialKeyMiter);
	
	//exit(0);
      }
    
    }
    free( WrongKeyValue );
    free( KeyWithFreq );
    free( KeyNoFreq );
    
    //printf("Key successfully simualted on oracle. The new inferred key is %s : %d \n", KeyNameTmp[0], KeyValue);
    
    // Print off updated keystore
    //printf("KeyStore is now: {");
    //for (k=0; k<pGlobalBsiKeys->NumKeys; k++)
    //  printf("%d, ", pGlobalBsiKeys->KeyValue[k]);
    //printf("}\n\n");
    //exit(0);
    // Key found!
    // TODO - Update Keystore
    //free(ppNodeFreeList);
    //for(k=0; k<MaxKeysConsidered; k++)
    //  free(KeyNameTmp[k]);
    //free(KeyNameTmp);
    //return 1;
    
    //ABC_FREE(pNtkMiter);
    //    } else {
    //ABC_FREE(pNtkMiter);
    //printf("Mitering failed. Proceed.\n");
    //}
    //Abc_NtkDelete( pNtkMiter );
    //if (!MiterStatus)
    //  Abc_NtkDelete( pNtkMiter );
    //} //else {
    //printf("This cone has %d key inputs. For now we only handle nodes with up to %d keys. Proceed.\n", NumKeys, MaxKeysConsidered);
    //}
    Abc_NtkDelete( pNtkCone );
  
    // Save off any key cnf info we found to the global key CNF store
    /*if ( (*ppCurKeyCnf) && fCurKeyCnfAlloc ) {
      if( ClapAttack_UpdateGlobalKeyCnf ( ppCurKeyCnf, pGlobalBsiKeys ) )
	printf("Global CNF Update Failed. \n");
      Abc_NtkDelete( *ppCurKeyCnf );
      }*/
  }



  // Update known keys in partial key cnf
  


  // Realloc keyname tmp so that we can infer all keys from partial cnf we generated
  for(k=0; k<MaxKeysConsidered; k++)
    free(KeyNameTmp[k]);
  free(KeyNameTmp);

  KeyNameTmp = (char **)malloc( sizeof(char *) * pGlobalBsiKeys->NumKeys);
  for (j=0; j<pGlobalBsiKeys->NumKeys; j++) {
    KeyNameTmp[j] = (char *)malloc( sizeof(char) * 100);
  }

  k=0;
  Abc_NtkForEachPi( pNtk, pPi, j ) {
    // Are we looking at a key input?
    if( strstr(Abc_ObjName(pPi), "key") ) {
      strcpy(KeyNameTmp[k], Abc_ObjName(pPi) );
      k++;
    }
  }

  fRerunInfer = 1;

  while (fRerunInfer) {

    // Clean up known keys in CNF
    ClapAttack_CleanKeyCnf( pGlobalBsiKeys );
    fRerunInfer = 0;
    
    // Evaluate partial key logic for complete key info
    for (k=0; k<pGlobalBsiKeys->NumKeys; k++) {

      if ( *ppCurKeyCnf ){
	ClapAttack_PartialKeyInferenceMiter( *ppCurKeyCnf, &pInferPartialKeyMiter, KeyNameTmp[k] );

	PartialKeySatStatus = ClapAttack_RunSat(pInferPartialKeyMiter);
	Abc_NtkDelete( pInferPartialKeyMiter );
	
	if ( PartialKeySatStatus && (PartialKeySatStatus != -1) ) {		    
	  if ( !ClapAttack_RunSat(*ppCurKeyCnf) ){
	    
	    Abc_NtkForEachPi( *ppCurKeyCnf, pKey, m ) {
	      if ( !strcmp(Abc_ObjName(pKey), KeyNameTmp[k]) ) {
		KeyValue = (*ppCurKeyCnf)->pModel[m];
		ClapAttack_UpdateKey(KeyNameTmp[k], KeyValue, pGlobalBsiKeys);
		printf("We determined that key %s is %d\n", Abc_ObjName(pKey), KeyValue);
		fRerunInfer = 1;
		//exit(0);
		// Either remove the key or set it to constant 1.
		break;
	      }
	    }
	    // Optimize out key from partial CNF
	    ntkTmp = Abc_NtkToLogic( *ppCurKeyCnf );
	    Abc_NtkDelete( *ppCurKeyCnf );
	    *ppCurKeyCnf = ntkTmp;
	    Abc_NtkForEachPi( *ppCurKeyCnf, pKey, m ) {
	      if ( !strcmp(Abc_ObjName(pKey), KeyNameTmp[k]) ) {
		Abc_ObjReplaceByConstant(pKey, KeyValue);
		Abc_NtkDeleteObj(pKey);
		break;
	      }
	    }
	    
	    // Check if we optimized out all the PIs but one.
	    // IF so, free the partial info
	    ntkTmp = Abc_NtkStrash( *ppCurKeyCnf, 0, 1, 0 );
	    Abc_NtkDelete( *ppCurKeyCnf );
	    *ppCurKeyCnf = ntkTmp;
	    
	    if ( (Abc_NtkPiNum(*ppCurKeyCnf) < 2) && fCurKeyCnfAlloc) {
	      //ABC_FREE(pCurKeyCnf);
	      Abc_NtkDelete( *ppCurKeyCnf );
	      *ppCurKeyCnf = NULL;
	      break;
	    } 
	    //printf("After\n");
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
  for (k=0; k<pGlobalBsiKeys->NumKeys; k++)
    printf("%d, ", pGlobalBsiKeys->KeyValue[k]);
  printf("}\n\n");

  free(pGlobalBsiKeysTmp->KeyValue);

  for(k=0; k<pGlobalBsiKeys->NumKeys; k++)
    free(KeyNameTmp[k]);
  free(KeyNameTmp);
  free(ppNodeFreeList);
  
}

void ClapAttack_UpdateSatMiterList( struct SatMiterList ** ppSatMiterList, Abc_Obj_t ** ppNode, Abc_Ntk_t * pMiter, int NumKeys, char **KeyNames, int MaxNodesConsidered, float IdentifiableKeys, int *pModel ) {

  int i;
  struct SatMiterList * pSatMiterListCur;
  struct SatMiterList * pSatMiterList;
  Abc_Obj_t *pPi;

  // If not allocated, initialize the data structure.
  if ( !(*ppSatMiterList) ) {
    *ppSatMiterList = (struct SatMiterList *) malloc( sizeof(struct SatMiterList) );
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
    pSatMiterListCur->pNext = (struct SatMiterList *) malloc( sizeof(struct SatMiterList) );

    // Set prev pointer
    (pSatMiterListCur->pNext)->pPrev = pSatMiterListCur;
    pSatMiterListCur = pSatMiterListCur->pNext;    
  }
  
  // Update data
  pSatMiterListCur->ppSatNode = (Abc_Obj_t **) malloc( sizeof(Abc_Obj_t *) * MaxNodesConsidered );

  pSatMiterListCur->pModel = (int *)malloc( sizeof(int) * Abc_NtkPiNum(pMiter) );
  Abc_NtkForEachPi( pMiter, pPi, i ) {
    pSatMiterListCur->pModel[i] = pMiter->pModel[i];
  }
  
  pSatMiterListCur->pMiter = Abc_NtkDup(pMiter);
  
  // Add each node to the list.
  for ( i=0; i<MaxNodesConsidered; i++ ) {
    pSatMiterListCur->ppSatNode[i] = ppNode[i];
  }
  
  pSatMiterListCur->MatchedNodes = MaxNodesConsidered;

  // Add keynames
  pSatMiterListCur->NumKeys = NumKeys;
  pSatMiterListCur->KeyNames = (char **)malloc( sizeof(char *) * NumKeys);
  for (i=0; i<NumKeys; i++) {
    pSatMiterListCur->KeyNames[i] = (char *)malloc( sizeof(char) * 100);
    strcpy(pSatMiterListCur->KeyNames[i], KeyNames[i]);
  }

  // Terminate list a new endpoint
  pSatMiterListCur->pNext = NULL;

  // Calculate number of key bits found by miter
  pSatMiterListCur->IdentifiableKeyBits = IdentifiableKeys;
  
}


void ClapAttack_FreeSatMiterList( struct SatMiterList ** ppSatMiterList ) {

  int i;
  struct SatMiterList * pSatMiterListCur;
  struct SatMiterList * pSatMiterList;

  pSatMiterList = *ppSatMiterList;
  
  // If not allocated, we're done...
  if ( !pSatMiterList )
    return;
  
  // Find end of list.
  pSatMiterListCur = pSatMiterList;
  while (pSatMiterListCur->pNext) {
    pSatMiterListCur = pSatMiterListCur->pNext;
  }


  //printf("Debug Here1\n\n");
	
  while ( pSatMiterListCur ) {

    // Update data
    //printf("Debug Here2\n");
    free(pSatMiterListCur->ppSatNode);

    // Clear miter ntk
    //printf("Debug Here3\n");
    Abc_NtkDelete( pSatMiterListCur->pMiter );

    // Clear model if it exists
    //printf("Debug Here4\n");
    free( pSatMiterListCur->pModel );

    // Free keyname list
    for ( i=0; i < pSatMiterListCur->NumKeys; i++ ) {
      free(pSatMiterListCur->KeyNames[i]);
    }
    free(pSatMiterListCur->KeyNames);
    
    // Go to previous node and free
    if ( pSatMiterListCur->pPrev ) {
      //printf("Debug Here5\n\n");
      pSatMiterListCur = pSatMiterListCur->pPrev;
      free(pSatMiterListCur->pNext);
    } else {
      //printf("Debug Here6\n\n");
      free(pSatMiterListCur);
      *ppSatMiterList = NULL;
      break;
    }
  }
  
}
//void ClapAttack_GlobalKeyCnfInfer ( struct BSI_KeyData_t * pGlobalBsiKeys ) {
//
//
//}

int ClapAttack_UpdateGlobalKeyCnf ( Abc_Ntk_t **ppCurKeyCnf, struct BSI_KeyData_t * pGlobalBsiKeys ) {
  
  // Handle Keystore partial CNF management
  if (*ppCurKeyCnf) {
    
    if (pGlobalBsiKeys->pKeyCnf) {
      if ( !Abc_NtkAppendSilent(pGlobalBsiKeys->pKeyCnf, *ppCurKeyCnf, 1 ) ) {
	Abc_NtkDelete( *ppCurKeyCnf );
	Abc_Print( -1, "Appending the networks failed.\n" );
	return 1;
      }
      
      // And the Pos
      extern int Abc_NtkCombinePos( Abc_Ntk_t * pNtk, int fAnd, int fXor );
      
      if ( !Abc_NtkCombinePos( pGlobalBsiKeys->pKeyCnf, 1, 0 ) ) {
	Abc_Print( -1, "ANDing the POs has failed.\n" );
	return 1;
      }
    } else {
      pGlobalBsiKeys->pKeyCnf = Abc_NtkDup(*ppCurKeyCnf);
    }
    
    // Rename Pos
    ClapAttack_RenamePo( pGlobalBsiKeys->pKeyCnf, 0, "not_key" );
    
    // Reset the cur partial key pointer to NULL
    Abc_NtkDelete( *ppCurKeyCnf );
    *ppCurKeyCnf = NULL;
    
    // Debug print
    //ClapAttack_WriteMiterVerilog(pGlobalBsiKeys->pKeyCnf, "complete_key_logic.v");
  }

  return 0;
}


int ClapAttack_IsolateCone(Abc_Ntk_t * pNtk, Abc_Ntk_t ** ppNtkCone, Abc_Obj_t * pProbe) {

  // Run cone command... and return the fanout cone as a separate network
  int fUseAllCis, i, j, k, l, ignoreFanin, ignoreFanin2, ignoreFanin3;
  Abc_Obj_t *pFanin, *pNode, *pHeadNode1, *pHeadNode2, *pPo, *pProbe2, *pHeadNode3, *pProbe3, *pHeadNode4, *pProbe4;
  char PoTmpName[25];
  Abc_Ntk_t *pNtkConeTmp, *pNtkTmp;
  // set defaults
  fUseAllCis = 0;
  
  extern Abc_Ntk_t * Abc_NtkMakeOnePo( Abc_Ntk_t * pNtk, int Output, int nRange );

  //Abc_FrameReplaceCurrentNetwork( pAbc, pNtkRes );

  *ppNtkCone = Abc_NtkCreateCone( pNtk, pProbe, Abc_ObjName(pProbe), fUseAllCis );
  //ClapAttack_WriteMiterVerilog(*ppNtkCone, "cone0.v");
  
  pPo = Abc_NtkPo(*ppNtkCone, 0);
  pHeadNode1 = Abc_ObjChild0( pPo );
  Abc_NtkDeleteObj( pPo );
  assert( Abc_NtkPoNum(*ppNtkCone) == 0 );

  // Re-sort network
  Abc_NtkOrderCisCos( *ppNtkCone );

  
  Abc_ObjForEachFanin( pHeadNode1, pFanin, j ) {
    pNode = Abc_NtkCreatePo( *ppNtkCone ); 
    Abc_ObjAddFanin( pNode, pFanin );
    sprintf(PoTmpName, "po%d", j);
    Abc_ObjAssignName( pNode, PoTmpName, NULL );
  }

  // Re-sort network
  Abc_NtkOrderCisCos( *ppNtkCone );

  // Cut off the head when we're done with it.
  // Unusued so unnecessary.
  Abc_NtkDeleteObj( pHeadNode1 );

  
  /* Probe Resolution Increase */
  if (Abc_ObjFanoutNum(pProbe)){
  
  pProbe2 = Abc_ObjFanout0( pProbe );

  
  if ( !Abc_ObjIsPo(pProbe2) ){
    // Figure out which fanin to skip
    ignoreFanin=-1;
    Abc_ObjForEachFanin( pProbe2, pFanin, i ) {
      if ( pFanin == pProbe ) {
	ignoreFanin = i;
      }
    }
    
    // Comment me
    pNtkConeTmp = Abc_NtkCreateCone( pNtk, pProbe2, Abc_ObjName(pProbe2), fUseAllCis );
    
    // comment me
    // remove the POs and their names
    pPo = Abc_NtkPo(pNtkConeTmp, 0);
    pHeadNode2 = Abc_ObjChild0( pPo );
    Abc_NtkDeleteObj( pPo );
    assert( Abc_NtkPoNum(pNtkConeTmp) == 0 );
    
    // comment me
    Abc_ObjForEachFanin( pHeadNode2, pFanin, i ) {
      if ( i != ignoreFanin) {
	pNode = Abc_NtkCreatePo( pNtkConeTmp ); 
	Abc_ObjAddFanin( pNode, pFanin );
	sprintf(PoTmpName, "po%d", j+i);
	Abc_ObjAssignName( pNode, PoTmpName, NULL );
      }
    }
    
    Abc_NtkDeleteObj( pHeadNode2 );
    
    // Re-sort network
    Abc_NtkOrderCisCos( pNtkConeTmp );
    


  
    pNtkTmp = Abc_NtkStrash( *ppNtkCone, 0, 1, 0 );
    Abc_NtkDelete( *ppNtkCone );
    *ppNtkCone = pNtkTmp;
    
    pNtkTmp = Abc_NtkStrash( pNtkConeTmp, 0, 1, 0 );
    Abc_NtkDelete( pNtkConeTmp );
    pNtkConeTmp = pNtkTmp;
    
  

    if ( !Abc_NtkAppendSilent( *ppNtkCone, pNtkConeTmp, 1 ) )
      {
	Abc_Print( -1, "Appending the networks failed 1.\n" );
	exit(0);
	return 1;
      }
    Abc_NtkDelete(pNtkConeTmp);
    //ClapAttack_WriteMiterVerilog(*ppNtkCone, "cone1.v");

    pNtkTmp = Abc_NtkToLogic( *ppNtkCone );
    Abc_NtkDelete( *ppNtkCone );
    *ppNtkCone = pNtkTmp;  
    



    



  if (Abc_ObjFanoutNum(pProbe2)){
  
  pProbe3 = Abc_ObjFanout0( pProbe2 );

  
  if ( !Abc_ObjIsPo(pProbe3) ){
    // Figure out which fanin to skip
    ignoreFanin=-1;
    ignoreFanin2=-1;
    Abc_ObjForEachFanin( pProbe3, pFanin, k ) {
      if ( pFanin == pProbe2 ) {
	ignoreFanin = k;
      }
      if ( pFanin == pProbe ) {
	ignoreFanin2 = k;
      }
    }
    
    // Comment me
    pNtkConeTmp = Abc_NtkCreateCone( pNtk, pProbe3, Abc_ObjName(pProbe3), fUseAllCis );
    
    // comment me
    // remove the POs and their names
    pPo = Abc_NtkPo(pNtkConeTmp, 0);
    pHeadNode3 = Abc_ObjChild0( pPo );
    Abc_NtkDeleteObj( pPo );
    assert( Abc_NtkPoNum(pNtkConeTmp) == 0 );
    
    // comment me
    Abc_ObjForEachFanin( pHeadNode3, pFanin, k ) {
      if ( k != ignoreFanin && k != ignoreFanin2) {
	pNode = Abc_NtkCreatePo( pNtkConeTmp ); 
	Abc_ObjAddFanin( pNode, pFanin );
	sprintf(PoTmpName, "po%d", j+i+k);
	Abc_ObjAssignName( pNode, PoTmpName, NULL );
      }
    }
    
    Abc_NtkDeleteObj( pHeadNode3 );
    
    // Re-sort network
    Abc_NtkOrderCisCos( pNtkConeTmp );
    


  
    pNtkTmp = Abc_NtkStrash( *ppNtkCone, 0, 1, 0 );
    Abc_NtkDelete( *ppNtkCone );
    *ppNtkCone = pNtkTmp;
    
    pNtkTmp = Abc_NtkStrash( pNtkConeTmp, 0, 1, 0 );
    Abc_NtkDelete( pNtkConeTmp );
    pNtkConeTmp = pNtkTmp;
    
  

    if ( !Abc_NtkAppendSilent( *ppNtkCone, pNtkConeTmp, 1 ) )
      {
	Abc_Print( -1, "Appending the networks failed 1.\n" );
	exit(0);
	return 1;
      }
    Abc_NtkDelete(pNtkConeTmp);
    //ClapAttack_WriteMiterVerilog(*ppNtkCone, "cone1.v");

    pNtkTmp = Abc_NtkToLogic( *ppNtkCone );
    Abc_NtkDelete( *ppNtkCone );
    *ppNtkCone = pNtkTmp;  







    /*  
  if (Abc_ObjFanoutNum(pProbe3)){
  
  pProbe4 = Abc_ObjFanout0( pProbe3 );

  
  if ( !Abc_ObjIsPo(pProbe4) ){
    // Figure out which fanin to skip
    ignoreFanin=-1;
    ignoreFanin2=-1;
    ignoreFanin3=-1;
    Abc_ObjForEachFanin( pProbe4, pFanin, l ) {
      if ( pFanin == pProbe2 ) {
	ignoreFanin = l;
      }
      if ( pFanin == pProbe ) {
	ignoreFanin2 = l;
      }
      if ( pFanin == pProbe3 ) {
	ignoreFanin3 = l;
      }
    }
    
    // Comment me
    pNtkConeTmp = Abc_NtkCreateCone( pNtk, pProbe4, Abc_ObjName(pProbe4), fUseAllCis );
    
    // comment me
    // remove the POs and their names
    pPo = Abc_NtkPo(pNtkConeTmp, 0);
    pHeadNode4 = Abc_ObjChild0( pPo );
    Abc_NtkDeleteObj( pPo );
    assert( Abc_NtkPoNum(pNtkConeTmp) == 0 );
    
    // comment me
    Abc_ObjForEachFanin( pHeadNode4, pFanin, l ) {
      if ( l != ignoreFanin && l != ignoreFanin2 && l != ignoreFanin3) {
	pNode = Abc_NtkCreatePo( pNtkConeTmp ); 
	Abc_ObjAddFanin( pNode, pFanin );
	sprintf(PoTmpName, "po%d", j+i+k+l);
	Abc_ObjAssignName( pNode, PoTmpName, NULL );
      }
    }
    
    Abc_NtkDeleteObj( pHeadNode4 );
    
    // Re-sort network
    Abc_NtkOrderCisCos( pNtkConeTmp );
    


  
    pNtkTmp = Abc_NtkStrash( *ppNtkCone, 0, 1, 0 );
    Abc_NtkDelete( *ppNtkCone );
    *ppNtkCone = pNtkTmp;
    
    pNtkTmp = Abc_NtkStrash( pNtkConeTmp, 0, 1, 0 );
    Abc_NtkDelete( pNtkConeTmp );
    pNtkConeTmp = pNtkTmp;
    
  

    if ( !Abc_NtkAppendSilent( *ppNtkCone, pNtkConeTmp, 1 ) )
      {
	Abc_Print( -1, "Appending the networks failed 1.\n" );
	exit(0);
	return 1;
      }
    Abc_NtkDelete(pNtkConeTmp);
    //ClapAttack_WriteMiterVerilog(*ppNtkCone, "cone1.v");

    pNtkTmp = Abc_NtkToLogic( *ppNtkCone );
    Abc_NtkDelete( *ppNtkCone );
    *ppNtkCone = pNtkTmp;  
    
  }}
    */

  }}

  }}








  
  /* End bigger resolution stuff... */


  
  // make sure that everything is okay
  if ( !Abc_NtkCheck( *ppNtkCone ) )
    {
      printf( "Abc_NtkOrPos: The network check has failed.\n" );
      return 1;
    }

  
  return 0;
}

void ClapAttack_CleanCone( Abc_Ntk_t ** ppNtk ) {

  extern Abc_Ntk_t * Abc_NtkDarCleanupAig( Abc_Ntk_t * pNtk, int fCleanupPis, int fCleanupPos, int fVerbose );
      
  // Strash the cone.
  *ppNtk = Abc_NtkStrash( *ppNtk, 0, 1, 0 );

  // Cleanup the unused logic.
  if ( Abc_NtkIsStrash(*ppNtk) )
    *ppNtk = Abc_NtkDarCleanupAig( *ppNtk, 1, 1, 0 );
}


void ClapAttack_InitKeyCnf( Abc_Ntk_t ** ppNtk, int NumKeys, int * WrongKeyValue, char ** KeyNames ) {

  int i;
  Abc_Ntk_t *ntkTmp;
  Abc_Obj_t ** pPi = (Abc_Obj_t **)malloc( sizeof(Abc_Obj_t *) * NumKeys );
  Abc_Obj_t * pPo;
  
  // Allocate the network
  *ppNtk = Abc_NtkAlloc( ABC_NTK_STRASH, ABC_FUNC_AIG, 1 ); 
  (*ppNtk)->pName = Extra_UtilStrsav("PartialKeyInfo");
  
  // Create a PI for each key
  for ( i=0; i<NumKeys; i++ ) {
    pPi[i] = Abc_NtkCreatePi( *ppNtk );
    Abc_ObjAssignName( pPi[i], KeyNames[i], NULL );
  }
    
  // Calculate logic based off wrong key value.
  for ( i=0; i<NumKeys; i++ ) {
    // Invert the key if necessary
    if ( !WrongKeyValue[i] ) {
      pPi[i] = Abc_ObjNot(pPi[i]);
    }

      
    if ( i )
      pPi[i] = Abc_AigAnd( (Abc_Aig_t *)(*ppNtk)->pManFunc, pPi[i-1],  pPi[i] );
  }

  // Final inversion ... this is not the key!
  pPi[NumKeys-1] = Abc_ObjNot(pPi[NumKeys-1]);

  // Now make this a PO
  pPo = Abc_NtkCreatePo( *ppNtk );
  Abc_ObjAssignName( pPo, "not_key_partial", NULL );
  Abc_ObjAddFanin( pPo, pPi[NumKeys-1] );


  ntkTmp = Abc_NtkStrash( *ppNtk, 0, 1, 0 );
  Abc_NtkDelete( *ppNtk );
  *ppNtk = ntkTmp;

  if ( !Abc_NtkCheck( *ppNtk ) )
    fprintf( stdout, "Abc_NtkCreateFromNode(): Network check has failed.\n" );
  
  //ClapAttack_WriteMiterVerilog(*ppNtk, "partial_key_logic.v");

  // Free the malloced array
  free(pPi);
      
}

void ClapAttack_RenamePo( Abc_Ntk_t * pNtk, int PoIdx, char *NewPoName) {

  Abc_Obj_t * pNode, *pPo;

  // Get fanin for PO
  pNode = Abc_ObjChild0(Abc_NtkPo(pNtk, PoIdx));
    
  // remove the POs and their names
  Abc_NtkDeleteObj( Abc_NtkPo(pNtk, PoIdx) );

  // create the new PO
  pPo = Abc_NtkCreatePo( pNtk );
  Abc_ObjAddFanin( pPo, pNode );
  Abc_ObjAssignName( pPo, NewPoName, NULL );
  Abc_NtkOrderCisCos( pNtk );
  
}

void ClapAttack_UpdateKeyCnf( Abc_Ntk_t ** ppNtk, int NumKeys, int * WrongKeyValue, char ** KeyNames ) {

  int i, j, fPiExists;
  Abc_Ntk_t *ntkTmp;
  Abc_Obj_t ** pPi = (Abc_Obj_t **)malloc( sizeof(Abc_Obj_t *) * NumKeys );
  Abc_Obj_t * pNode, * pMiter;
  Fraig_Params_t Params;
  
  // Create a PI for each key that doesnt exist
  for ( i=0; i<NumKeys; i++ ) {

    fPiExists=0;

    // Do we already have the PI?
    Abc_NtkForEachPi( *ppNtk, pNode, j ) {
      if ( !strcmp(Abc_ObjName(pNode), KeyNames[i]) ) {
	fPiExists = 1;
	pPi[i] = pNode;
      }
    }

    // If not, allocate it.
    if( !fPiExists ) {
      pPi[i] = Abc_NtkCreatePi( *ppNtk );
      Abc_ObjAssignName( pPi[i], KeyNames[i], NULL );
    }
  }

  // Calculate logic based off wrong key value.
  for ( i=0; i<NumKeys; i++ ) {
    // Invert the key if necessary
    if ( !WrongKeyValue[i] ) {
      pPi[i] = Abc_ObjNot(pPi[i]);
    }

      
    if ( i )
      pPi[i] = Abc_AigAnd( (Abc_Aig_t *)(*ppNtk)->pManFunc, pPi[i-1],  pPi[i] );
  }

  // Final inversion ... this is not the key!
  pPi[NumKeys-1] = Abc_ObjNot(pPi[NumKeys-1]);

  // And this node with the previous PO to create the miter
  pMiter = Abc_AigAnd( (Abc_Aig_t *)(*ppNtk)->pManFunc, pPi[NumKeys-1], Abc_ObjChild0(Abc_NtkPo(*ppNtk, 0)) );
  
  // remove the POs and their names
  Abc_NtkDeleteObj( Abc_NtkPo(*ppNtk, 0) );
  assert( Abc_NtkPoNum(*ppNtk) == 0 );

  // create the new PO
  pNode = Abc_NtkCreatePo( *ppNtk );
  Abc_ObjAddFanin( pNode, pMiter );
  Abc_ObjAssignName( pNode, "not_key_partial", NULL );
  Abc_NtkOrderCisCos( *ppNtk );

  Fraig_ParamsSetDefault( &Params );
  *ppNtk = Abc_NtkFraig( ntkTmp = *ppNtk, &Params, 0, 0 );
  Abc_NtkDelete( ntkTmp );

  // Strash the network again
  ntkTmp = Abc_NtkStrash( *ppNtk, 0, 1, 0 );
  Abc_NtkDelete( *ppNtk );
  *ppNtk = ntkTmp;
  
  if ( !Abc_NtkCheck( *ppNtk ) )
    fprintf( stdout, "Abc_NtkCreateFromNode(): Network check has failed.\n" );
  
  //ClapAttack_WriteMiterVerilog(*ppNtk, "updated_key_logic.v");
  //exit(0);
  // Free the malloced array
  free(pPi);
      
}


int ClapAttack_BuildPartialKeyMiter( Abc_Ntk_t ** ppNtk ) {

  int i, NumDelNode;
  Abc_Ntk_t *ntkTmp;
  Abc_Obj_t *pNode;
  Abc_Obj_t **DelNode = (Abc_Obj_t **)malloc( sizeof(Abc_Obj_t *) * (Abc_NtkPoNum(*ppNtk) + Abc_NtkPiNum(*ppNtk)) );

  // Delete all PIs that arent keys
  NumDelNode = 0;
  Abc_NtkForEachPi( *ppNtk, pNode, i ) {

    // Is it a key?
    if( !strstr(Abc_ObjName(pNode), "key") ) {
      DelNode[NumDelNode] = pNode;
      NumDelNode++;
    }
  }

  for (i=0; i<NumDelNode; i++) {
    Abc_NtkDeleteObj( DelNode[i] );
  }

  // Delete all Pos that arent the miter
  NumDelNode = 0;
  Abc_NtkForEachPo( *ppNtk, pNode, i ) {

    if( !strstr(Abc_ObjName(pNode), "not_key") ) {
      DelNode[NumDelNode] = pNode;
      NumDelNode++;
    }
  }

  for (i=0; i<NumDelNode; i++) {
    Abc_NtkDeleteObj( DelNode[i] );
  }

  // strash things again and check the final miter.
  ntkTmp = Abc_NtkStrash( *ppNtk, 0, 1, 0 ); 
  Abc_NtkDelete( *ppNtk );
  *ppNtk = ntkTmp;

  
  // Make the final miter
  extern int Abc_NtkCombinePos( Abc_Ntk_t * pNtk, int fAnd, int fXor );

  //ClapAttack_WriteMiterVerilog(*ppNtk, "not_key_premiter.v");
  if ( !Abc_NtkCombinePos( *ppNtk, 1, 0 ) )
    {
      Abc_Print( -1, "ANDing the POs has failed.\n" );
      return 1;
    }

  
  ClapAttack_RenamePo( *ppNtk, 0, "not_key_miter" );
  
  //ClapAttack_WriteMiterVerilog(pNtk, "test3.v");
  // make sure that everything is okay
  //printf("Pre-Check\n\n");
  if ( !Abc_NtkCheck( *ppNtk ) )
    {
      printf( "Abc_NtkOrPos: The network check has failed.\n" );
      return 1;
    }
  //printf("Post-Check\n\n");
  //ClapAttack_WriteMiterVerilog(*ppNtk, "not_key_miter.v");

  free(DelNode);
  
  return 0;
}


int ClapAttack_MakeMiterHeuristic(Abc_Ntk_t * pNtkCone, Abc_Ntk_t * pNtkPartialKey, Abc_Ntk_t ** ppNtkMiter) {
  Abc_Ntk_t *pNtkTmpMiter1, *pNtkMiter2, *pNtkTmpMiter2, *pKeyMiter, *pNtkPartialKeyMiter, *pNtkPartialKeyMiterTmp, *ntkTmp;
  Abc_Obj_t *pNode;
  Fraig_Params_t Params;
  int fCheck;
  int fComb;
  int fImplic;
  int fMulti;
  int nPartSize;
  int NumKeys;
  int i;
  //  int nDigits;
  
  // Named function params for clarity
  fComb  = 0;
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
  ntkTmp = Abc_NtkStrash( *ppNtkMiter, 0, 1, 0 );
  Abc_NtkDelete( *ppNtkMiter );
  *ppNtkMiter = ntkTmp;

  ntkTmp = Abc_NtkStrash( pNtkTmpMiter1, 0, 1, 0 );
  Abc_NtkDelete( pNtkTmpMiter1 );
  pNtkTmpMiter1 = ntkTmp;

  ntkTmp = Abc_NtkStrash( pNtkMiter2, 0, 1, 0 );
  Abc_NtkDelete( pNtkMiter2 );
  pNtkMiter2 = ntkTmp; 

  ntkTmp = Abc_NtkStrash( pNtkTmpMiter2, 0, 1, 0 );
  Abc_NtkDelete( pNtkTmpMiter2 );
  pNtkTmpMiter2 = ntkTmp;

  // Build partial key logic miter.
  pNtkPartialKeyMiter = NULL;
  if ( pNtkPartialKey ) {

    //ClapAttack_WriteMiterVerilog(pNtkPartialKey, "ntk3.v");
    pNtkPartialKeyMiter = Abc_NtkDup(pNtkPartialKey);

    ntkTmp = Abc_NtkStrash( pNtkPartialKeyMiter, 0, 1, 0 );
    Abc_NtkDelete( pNtkPartialKeyMiter );
    pNtkPartialKeyMiter = ntkTmp;

    //if ( !Abc_NtkAppendSilent( pNtkPartialKeyMiter, pNtkPartialKey, 1 ) )
    //  {
    //	Abc_Print( -1, "Appending the networks failed 1.\n" );
    //	exit(0);
    //	return 1;
    // }

    pNtkPartialKeyMiterTmp = Abc_NtkDup(pNtkPartialKeyMiter);
    //pNtkPartialKeyMiter = Abc_NtkStrash( pNtkPartialKeyMiter, 0, 1, 0 );
    ntkTmp = Abc_NtkStrash( pNtkPartialKeyMiterTmp, 0, 1, 0 );
    Abc_NtkDelete( pNtkPartialKeyMiterTmp );
    pNtkPartialKeyMiterTmp = ntkTmp;

    // Temporary hack so padding 0s will always match...
    //nDigits = Abc_Base10Log( Abc_NtkPiNum(pNtkCone) );
    ClapAttack_RenameLogic( pNtkPartialKeyMiter, "_1", "_1", "_cone1", 1 );
    ClapAttack_RenameLogic( pNtkPartialKeyMiterTmp, "_2", "_2", "_cone2", 1 );

    //ClapAttack_WriteMiterVerilog(pNtkPartialKeyMiter, "ntk6.v");
    
    //printf("Before append\n");
    if ( !Abc_NtkAppendSilent( pNtkPartialKeyMiter, pNtkPartialKeyMiterTmp, 1 ) )
      {
	//ClapAttack_WriteMiterVerilog(pNtkPartialKeyMiter, "ntk1.v");
	//ClapAttack_WriteMiterVerilog(pNtkPartialKeyMiterTmp, "ntk2.v");
	Abc_NtkDelete( pNtkPartialKeyMiterTmp );
	Abc_Print( -1, "Appending the networks failed 2.\n" );
	exit(0);
	return 1;
      }
    Abc_NtkDelete( pNtkPartialKeyMiterTmp );

    //ClapAttack_WriteMiterVerilog(pNtkPartialKeyMiter, "ntk7.v");
    
    // ToDo:
    // Delete all Pis that arent keys
    // Delete all POs that arent non_key
    // Add new anded PO of not_key
    ntkTmp = Abc_NtkStrash( pNtkPartialKeyMiter, 0, 1, 0 );
    Abc_NtkDelete( pNtkPartialKeyMiter );
    pNtkPartialKeyMiter = ntkTmp;
    //if ( ClapAttack_BuildPartialKeyMiter( &pNtkPartialKeyMiter ) ) {
    //  Abc_Print( -1, "Building partial key miter failed.\n");
    //  exit(0);
    // }
    //ClapAttack_WriteMiterVerilog(pNtkPartialKeyMiter, "ntk4.v");


    // Make the final miter
    extern int Abc_NtkCombinePos( Abc_Ntk_t * pNtk, int fAnd, int fXor );

    //ClapAttack_WriteMiterVerilog(*ppNtk, "not_key_premiter.v");
    if ( !Abc_NtkCombinePos( pNtkPartialKeyMiter, 1, 0 ) )
      {
	Abc_Print( -1, "ANDing the POs has failed.\n" );
	return 1;
      }

  
    ClapAttack_RenamePo( pNtkPartialKeyMiter, 0, "not_key_miter" );
  
    //ClapAttack_WriteMiterVerilog(pNtk, "test3.v");
    // make sure that everything is okay
    //printf("Pre-Check\n\n");
    if ( !Abc_NtkCheck( pNtkPartialKeyMiter ) )
      {
	printf( "Abc_NtkOrPos: The network check has failed.\n" );
	return 1;
      }

    
    //ClapAttack_WriteMiterVerilog(pNtkPartialKeyMiter, "not_key_miter.v");
  }


  


  
  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "pre_ntk.v");
  ClapAttack_RenameLogic( *ppNtkMiter, "_1", "_1", "_cone1", 1 );
  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "ntk1.v");
  ClapAttack_RenameLogic( pNtkTmpMiter1, "_2", "_1", "_cone2", 1 );
  //ClapAttack_WriteMiterVerilog(pNtkTmpMiter1, "ntk2.v");



  ClapAttack_RenameLogic( pNtkMiter2, "_1", "_2", "_cone3", 1 );
  //ClapAttack_WriteMiterVerilog(pNtkMiter2, "ntk3.v");
  ClapAttack_RenameLogic( pNtkTmpMiter2, "_2", "_2", "_cone4", 1 );
  //ClapAttack_WriteMiterVerilog(pNtkTmpMiter2, "ntk4.v");

  // Append the two networks we prepped for miter 1
  if ( !Abc_NtkAppendSilent( *ppNtkMiter, pNtkTmpMiter1, 1 ) )
    {
      Abc_NtkDelete( pNtkTmpMiter1 );
      Abc_Print( -1, "Appending the networks failed 3.\n" );
      exit(0);
      return 1;
    }

  Abc_NtkDelete( pNtkTmpMiter1 );

  // Append the two networks we prepped for miter 2
  if ( !Abc_NtkAppendSilent( pNtkMiter2, pNtkTmpMiter2, 1 ) )
    {
      Abc_NtkDelete( pNtkTmpMiter2 );
      Abc_Print( -1, "Appending the networks failed 4.\n" );
      exit(0);
      return 1;
    }
  Abc_NtkDelete( pNtkTmpMiter2 );

  // Generate the first miter cone
  if ( !ClapAttack_MiterPos( *ppNtkMiter, 1, 0 ) )
    {
      Abc_Print( -1, "XOR/ORing the POs has failed.\n" );
      exit(0);
      return 1;
    }

  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "miter_firstcone.v");
  
  // Generate the second miter cone
  if ( !ClapAttack_MiterPos( pNtkMiter2, 0, 1 ) )
    {
      Abc_Print( -1, "XNOR/ANDing the POs has failed.\n" );
      exit(0);
      return 1;
    }
  
  // Merge Miters
  ClapAttack_RenameOutput( *ppNtkMiter, "_1", 1 );
  ClapAttack_RenameOutput( pNtkMiter2, "_2", 1 );
  if ( !Abc_NtkAppendSilent( *ppNtkMiter, pNtkMiter2, 1 ) )
    {
      Abc_NtkDelete( pNtkMiter2 );
      Abc_Print( -1, "Appending the networks failed 5.\n" );
      exit(0);
      return 1;
    }

  Abc_NtkDelete( pNtkMiter2 );


  // check to see if there are any keys left in the optimized
  // circuit. If not, we don't need to bother checking anything
  Abc_NtkForEachPi( *ppNtkMiter, pNode, i ) {

    // Is it a key?
    if( strstr(Abc_ObjName(pNode), "key") )
      NumKeys++;
  }

  // No keys... don't bother continuing
  if (!NumKeys) {
    if ( pNtkPartialKey ) {
      Abc_NtkDelete( pNtkPartialKeyMiter );
    }
    //printf("We failed out because there are no keys for some reason....\n\n\n\n");
    return 1;
  }
      
  // Finally, we must make the key miter --
  // We will operate off of the final miter and
  // add a circuit XORing each key pair.
  // If we remove other POs, the optimization
  // engine will dump the remainder of the circuit.
  // It's a hack.... for now....
  pKeyMiter = Abc_NtkDup(*ppNtkMiter);
  if ( !ClapAttack_MiterKeys( pKeyMiter ) )
    {
      Abc_Print( -1, "XORing the keys has failed.\n" );
      exit(0);
      return 1;
    }

  // Merge in key miter
  if ( !Abc_NtkAppendSilent( *ppNtkMiter, pKeyMiter, 1 ) )
    {
      Abc_NtkDelete( pKeyMiter );
      Abc_Print( -1, "Appending the networks failed 6.\n" );
      exit(0);
      return 1;
    }

  Abc_NtkDelete( pKeyMiter );

  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "miter_premerge.v");

  // Merge in partial key logic miter
  if (pNtkPartialKeyMiter) {
    if ( !Abc_NtkAppendSilent( *ppNtkMiter, pNtkPartialKeyMiter, 1 ) )
      {
	Abc_NtkDelete( pNtkPartialKeyMiter );
	Abc_Print( -1, "Appending the networks failed 7.\n" );
	exit(0);
	return 1;
      }
    //printf("Partial Key append Done\n\n");
    Abc_NtkDelete( pNtkPartialKeyMiter );
    //ABC_FREE(pNtkPartialKeyMiter);
    //ABC_FREE(pNtkPartialKeyMiterTmp);

  }

  // Generate the final miter cone
  extern int Abc_NtkCombinePos( Abc_Ntk_t * pNtk, int fAnd, int fXor );

  if ( !Abc_NtkCombinePos( *ppNtkMiter, 1, 0 ) )//!ClapAttack_MiterPos( *ppNtkMiter, 0, 0, 1 ) )
    {
      Abc_Print( -1, "ANDing the POs has failed.\n" );
	exit(0);
      return 1;
    }

  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "miter_prefraig.v");


  Fraig_ParamsSetDefault( &Params );
  *ppNtkMiter = Abc_NtkFraig( ntkTmp = *ppNtkMiter, &Params, 0, 0 );
  Abc_NtkDelete( ntkTmp );


  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "final_miter.v");
  
  // Free all the networks
  //Abc_NtkDelete( pKeyMiter );
  //Abc_NtkDelete( pNtkTmpMiter1 );
  //Abc_NtkDelete( pNtkTmpMiter2 );
  //Abc_NtkDelete( pNtkMiter2 );
  //ABC_FREE(pKeyMiter);
  //ABC_FREE(pNtkTmpMiter2);
  //ABC_FREE(pNtkTmpMiter1);
  //ABC_FREE(pNtkMiter2);

  
  return 0;

}



int ClapAttack_MakeMiter(Abc_Ntk_t * pNtkCone, Abc_Ntk_t * pNtkPartialKey, Abc_Ntk_t ** ppNtkMiter) {
  Abc_Ntk_t *pNtkTmpMiter1, *pNtkMiter2, *pNtkTmpMiter2, *pKeyMiter, *pNtkPartialKeyMiter, *pNtkPartialKeyMiterTmp, *ntkTmp;
  Abc_Obj_t *pNode;
  Fraig_Params_t Params;
  int fCheck;
  int fComb;
  int fImplic;
  int fMulti;
  int nPartSize;
  int NumKeys;
  int i;
  int nDigits;
  
  // Named function params for clarity
  fComb  = 0;
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
  ntkTmp = Abc_NtkStrash( *ppNtkMiter, 0, 1, 0 );
  Abc_NtkDelete( *ppNtkMiter );
  *ppNtkMiter = ntkTmp;

  ntkTmp = Abc_NtkStrash( pNtkTmpMiter1, 0, 1, 0 );
  Abc_NtkDelete( pNtkTmpMiter1 );
  pNtkTmpMiter1 = ntkTmp;

  ntkTmp = Abc_NtkStrash( pNtkMiter2, 0, 1, 0 );
  Abc_NtkDelete( pNtkMiter2 );
  pNtkMiter2 = ntkTmp; 

  ntkTmp = Abc_NtkStrash( pNtkTmpMiter2, 0, 1, 0 );
  Abc_NtkDelete( pNtkTmpMiter2 );
  pNtkTmpMiter2 = ntkTmp;


  // Build partial key logic miter.
  pNtkPartialKeyMiter = NULL;
  if ( pNtkPartialKey ) {

    pNtkPartialKeyMiter = Abc_NtkDup(pNtkCone);

    ntkTmp = Abc_NtkStrash( pNtkPartialKeyMiter, 0, 1, 0 );
    Abc_NtkDelete( pNtkPartialKeyMiter );
    pNtkPartialKeyMiter = ntkTmp;

    if ( !Abc_NtkAppendSilent( pNtkPartialKeyMiter, pNtkPartialKey, 1 ) )
      {
	Abc_Print( -1, "Appending the networks failed 1.\n" );
	exit(0);
	return 1;
      }

    pNtkPartialKeyMiterTmp = Abc_NtkDup(pNtkPartialKeyMiter);
    //pNtkPartialKeyMiter = Abc_NtkStrash( pNtkPartialKeyMiter, 0, 1, 0 );
    ntkTmp = Abc_NtkStrash( pNtkPartialKeyMiterTmp, 0, 1, 0 );
    Abc_NtkDelete( pNtkPartialKeyMiterTmp );
    pNtkPartialKeyMiterTmp = ntkTmp;

    // Temporary hack so padding 0s will always match...
    nDigits = Abc_Base10Log( Abc_NtkPiNum(pNtkCone) );
    ClapAttack_RenameInput( pNtkPartialKeyMiter, "_1", "_1", "_1", nDigits );
    ClapAttack_RenameInput( pNtkPartialKeyMiterTmp, "_2", "_2", "_2", nDigits );

    //printf("Before append\n");
    if ( !Abc_NtkAppendSilent( pNtkPartialKeyMiter, pNtkPartialKeyMiterTmp, 1 ) )
      {
	Abc_NtkDelete( pNtkPartialKeyMiterTmp );
	Abc_Print( -1, "Appending the networks failed 2.\n" );
	return 1;
      }
    Abc_NtkDelete( pNtkPartialKeyMiterTmp );
    
    // ToDo:
    // Delete all Pis that arent keys
    // Delete all POs that arent non_key
    // Add new anded PO of not_key
    ntkTmp = Abc_NtkStrash( pNtkPartialKeyMiter, 0, 1, 0 );
    Abc_NtkDelete( pNtkPartialKeyMiter );
    pNtkPartialKeyMiter = ntkTmp;
    if ( ClapAttack_BuildPartialKeyMiter( &pNtkPartialKeyMiter ) )
      Abc_Print( -1, "Building partial key miter failed.\n");


    //ClapAttack_WriteMiterVerilog(pNtkPartialKeyMiter, "not_key_miter.v");
  }


  


  
  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "pre_ntk_noheur.v");
  
  ClapAttack_RenameLogic( *ppNtkMiter, "_1", "_1", "_cone1", 0 );
  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "ntk1_noheur.v");
  ClapAttack_RenameLogic( pNtkTmpMiter1, "_2", "_1", "_cone2", 0 );
  //ClapAttack_WriteMiterVerilog(pNtkTmpMiter1, "ntk2.v");



  ClapAttack_RenameLogic( pNtkMiter2, "_1", "_2", "_cone3", 0 );
  //ClapAttack_WriteMiterVerilog(pNtkMiter2, "ntk3.v");
  ClapAttack_RenameLogic( pNtkTmpMiter2, "_2", "_2", "_cone4", 0 );
  //ClapAttack_WriteMiterVerilog(pNtkTmpMiter2, "ntk4.v");

  // Append the two networks we prepped for miter 1
  if ( !Abc_NtkAppendSilent( *ppNtkMiter, pNtkTmpMiter1, 1 ) )
    {
      Abc_NtkDelete( pNtkTmpMiter1 );
      Abc_Print( -1, "Appending the networks failed 3.\n" );
      return 1;
    }

  Abc_NtkDelete( pNtkTmpMiter1 );

  // Append the two networks we prepped for miter 2
  if ( !Abc_NtkAppendSilent( pNtkMiter2, pNtkTmpMiter2, 1 ) )
    {
      Abc_NtkDelete( pNtkTmpMiter2 );
      Abc_Print( -1, "Appending the networks failed 4.\n" );
      return 1;
    }
  Abc_NtkDelete( pNtkTmpMiter2 );

  // Generate the first miter cone
  if ( !ClapAttack_MiterPos( *ppNtkMiter, 1, 0 ) )
    {
      Abc_Print( -1, "XOR/ORing the POs has failed.\n" );
      return 1;
    }

  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "miter_firstcone_noheur.v");
  
  // Generate the second miter cone
  if ( !ClapAttack_MiterPos( pNtkMiter2, 0, 1 ) )
    {
      Abc_Print( -1, "XNOR/ANDing the POs has failed.\n" );
      return 1;
    }
  
  // Merge Miters
  ClapAttack_RenameOutput( *ppNtkMiter, "_1", 0 );
  ClapAttack_RenameOutput( pNtkMiter2, "_2", 0 );
  if ( !Abc_NtkAppendSilent( *ppNtkMiter, pNtkMiter2, 1 ) )
    {
      Abc_NtkDelete( pNtkMiter2 );
      Abc_Print( -1, "Appending the networks failed 5.\n" );
      return 1;
    }

  Abc_NtkDelete( pNtkMiter2 );


  // check to see if there are any keys left in the optimized
  // circuit. If not, we don't need to bother checking anything
  Abc_NtkForEachPi( *ppNtkMiter, pNode, i ) {

    // Is it a key?
    if( strstr(Abc_ObjName(pNode), "key") )
      NumKeys++;
  }

  // No keys... don't bother continuing
  if (!NumKeys) {
    if ( pNtkPartialKey ) {
      Abc_NtkDelete( pNtkPartialKeyMiter );
    }
    //printf("We failed out because there are no keys for some reason....\n\n\n\n");
    return 1;
  }
      
  // Finally, we must make the key miter --
  // We will operate off of the final miter and
  // add a circuit XORing each key pair.
  // If we remove other POs, the optimization
  // engine will dump the remainder of the circuit.
  // It's a hack.... for now....
  pKeyMiter = Abc_NtkDup(*ppNtkMiter);
  if ( !ClapAttack_MiterKeys( pKeyMiter ) )
    {
      Abc_Print( -1, "XORing the keys has failed.\n" );
      return 1;
    }

  // Merge in key miter
  if ( !Abc_NtkAppendSilent( *ppNtkMiter, pKeyMiter, 1 ) )
    {
      Abc_NtkDelete( pKeyMiter );
      Abc_Print( -1, "Appending the networks failed 6.\n" );
      return 1;
    }

  Abc_NtkDelete( pKeyMiter );

  // Merge in partial key logic miter
  if (pNtkPartialKeyMiter) {
    if ( !Abc_NtkAppendSilent( *ppNtkMiter, pNtkPartialKeyMiter, 1 ) )
      {
	Abc_NtkDelete( pNtkPartialKeyMiter );
	Abc_Print( -1, "Appending the networks failed 7.\n" );
	return 1;
      }
    printf("Partial Key append Done\n\n");
    Abc_NtkDelete( pNtkPartialKeyMiter );
    //ABC_FREE(pNtkPartialKeyMiter);
    //ABC_FREE(pNtkPartialKeyMiterTmp);

  }

  // Generate the final miter cone
  extern int Abc_NtkCombinePos( Abc_Ntk_t * pNtk, int fAnd, int fXor );

  if ( !Abc_NtkCombinePos( *ppNtkMiter, 1, 0 ) )//!ClapAttack_MiterPos( *ppNtkMiter, 0, 0, 1 ) )
    {
      Abc_Print( -1, "ANDing the POs has failed.\n" );
      return 1;
    }


  Fraig_ParamsSetDefault( &Params );
  *ppNtkMiter = Abc_NtkFraig( ntkTmp = *ppNtkMiter, &Params, 0, 0 );
  Abc_NtkDelete( ntkTmp );

  // Free all the networks
  //Abc_NtkDelete( pKeyMiter );
  //Abc_NtkDelete( pNtkTmpMiter1 );
  //Abc_NtkDelete( pNtkTmpMiter2 );
  //Abc_NtkDelete( pNtkMiter2 );
  //ABC_FREE(pKeyMiter);
  //ABC_FREE(pNtkTmpMiter2);
  //ABC_FREE(pNtkTmpMiter1);
  //ABC_FREE(pNtkMiter2);

  
  return 0;
}

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
  
  extern int Abc_NtkDSat( Abc_Ntk_t * pNtk, ABC_INT64_T nConfLimit, ABC_INT64_T nInsLimit, int nLearnedStart, int nLearnedDelta, int nLearnedPerce, int fAlignPol, int fAndOuts, int fNewSolver, int fVerbose );
  // set defaults
  fAlignPol  = 0;
  fAndOuts   = 0;
  fNewSolver = 0;
  fSilent    = 0;
  fShowPattern = 1;
  fVerbose   = 0;
  nConfLimit = 0;
  nInsLimit  = 0;
  nLearnedStart = 0;
  nLearnedDelta = 0;
  nLearnedPerce = 0;

  //clk = Abc_Clock();
  RetValue = Abc_NtkDSat( pNtk, (ABC_INT64_T)nConfLimit, (ABC_INT64_T)nInsLimit, nLearnedStart, nLearnedDelta, nLearnedPerce, fAlignPol, fAndOuts, fNewSolver, fVerbose );
  // verify that the pattern is correct

  if ( RetValue == 0 && Abc_NtkPoNum(pNtk) == 1 )
    {
      int * pSimInfo = Abc_NtkVerifySimulatePattern( pNtk, pNtk->pModel );
      if ( pSimInfo[0] != 1 )
	Abc_Print( 1, "ERROR in Abc_NtkMiterSat(): Generated counter example is invalid.\n" );

      pCex = Abc_CexCreate( 0, Abc_NtkPiNum(pNtk), pNtk->pModel, 0, 0, 0 );

      // Print full input/output
      //ClapAttack_PrintInp( pNtk, pNtk->pModel );
      //ClapAttack_PrintOut( pNtk, pSimInfo );

      ABC_FREE( pSimInfo );
      ABC_FREE( pCex );
    }


  // print CEX with pcex->pData[0] it is of type unsigned -> line 40 of ./src/misc/util/utilCex.h

  /*if ( !fSilent )
    {
      if ( RetValue == -1 )
	Abc_Print( 1, "UNDECIDED      " );
      else if ( RetValue == 0 )
	Abc_Print( 1, "SATISFIABLE    " );
      else
	Abc_Print( 1, "UNSATISFIABLE  " );
      //Abc_PrintTime( 1, "Time", Abc_Clock() - clk );
      }*/
  
  
  return RetValue;
}

// Note that this fails on underscore characters for some reason...
void ClapAttack_InterpretDiHeuristic(Abc_Ntk_t *pNtk, Abc_Ntk_t *pNtkMiter, int *pModel, int NumKeys, int *KeyWithFreq, int *KeyNoFreq, int **ppDiFull) {

  int i, j, idx1, idx2, KeyIndex;
  Abc_Obj_t *pPi, *pPiTmp, *pPo;
  int *pInp1, *pInp2, *pDi1, *pDi2, *pKey1, *pKey2, *pDiFull;
  
  pInp1 = (int *)malloc( sizeof(int) * (Abc_NtkPiNum(pNtk)*2) );
  pInp2 = (int *)malloc( sizeof(int) * (Abc_NtkPiNum(pNtk)*2) );
  pDi1 = (int *)malloc( sizeof(int) * Abc_NtkPiNum(pNtk) );
  pDi2 = (int *)malloc( sizeof(int) * Abc_NtkPiNum(pNtk) );
  pKey1 = (int *)malloc( sizeof(int) * NumKeys );
  pKey2 = (int *)malloc( sizeof(int) * NumKeys );
  pDiFull = (int *)malloc( sizeof(int) * (Abc_NtkPiNum(pNtk)*2) );
  *ppDiFull = pDiFull;

  // Init.
  idx1 = 0;
  idx2 = Abc_NtkPiNum(pNtk);

  // Organize larger miter model into shorter cone model for oracle
  Abc_NtkForEachPi( pNtk, pPi, i ) {
    Abc_NtkForEachPi( pNtkMiter, pPiTmp, j ) {

      // Check if PI names match. If so, save off the model value
      if ( strstr(Abc_ObjName(pPiTmp), Abc_ObjName(pPi)) && (strlen(Abc_ObjName(pPiTmp)) == (strlen(Abc_ObjName(pPi)) + 2))) {

	// Part of first DI
	if ( strstr(Abc_ObjName(pPiTmp), "_1") ) {
	  pDiFull[idx1] = pModel[j];
	  idx1++;
	}
	
	// Part of second DI
	if ( strstr(Abc_ObjName(pPiTmp), "_2") ) {
	  pDiFull[idx2] = pModel[j];
	  idx2++;
	}
      }
    }
  }
  
  for(i=0; i<(Abc_NtkPiNum(pNtk)*2); i++) {
    pInp1[i] = pDiFull[i];
    pInp2[i] = pDiFull[i];
  }
  
  // Assign key->0
  
  // Goal: Iterate through each PI. Identify list of keys.
  KeyIndex = 0;
  Abc_NtkForEachPi( pNtk, pPi, i ) {
    
    // Are we looking at a key input?
    if( strstr(Abc_ObjName(pPi), "key") ) {

      pInp1[i] = pInp1[i+Abc_NtkPiNum(pNtk)];
      // pInp1[i+Abc_NtkPiNum(pNtk)] = 0;
      //pInp2[i] = 1;
      pInp2[i+Abc_NtkPiNum(pNtk)] = pInp2[i];
      //Abc_Print(1, "match!: %d=%s %d %d\n", i, Abc_ObjName(pPi), pInp[i], pOracleKey[keyindex] );

      // Save off each key value to return
      pKey1[KeyIndex] = pInp1[i];
      pKey2[KeyIndex] = pInp2[i];
      KeyIndex++;
    }
  }
  
  // Save off each input separtely
  for( i=0; i < Abc_NtkPiNum(pNtk); i++ ) {
    pDi1[i] = pInp1[i];
    pDi2[i] = pInp1[i+Abc_NtkPiNum(pNtk)];
  }

  // Simulate Pattern 1
  int * pSimInfo1 = Abc_NtkVerifySimulatePattern( pNtk, pDi1 );

  // Simulate Pattern 2
  int * pSimInfo2 = Abc_NtkVerifySimulatePattern( pNtk, pDi2 );

  // Print pattern 2 input/output
  //ClapAttack_PrintInp( pNtk, pDi1 );
  //ClapAttack_PrintOut( pNtk, pSimInfo1 );
  //ClapAttack_PrintInp( pNtk, pDi2 );
  //ClapAttack_PrintOut( pNtk, pSimInfo2 );

  Abc_NtkForEachPo( pNtk, pPo, i ) {
    if ( pSimInfo1[i] - pSimInfo2[i] ) {
      printf("Interpret: We had a frequency component for key=0!\n");
      for (KeyIndex=0; KeyIndex < NumKeys; KeyIndex++) {
	KeyWithFreq[KeyIndex] = pKey1[KeyIndex];
	KeyNoFreq[KeyIndex] = pKey2[KeyIndex];
      }
    }
  }

  ABC_FREE( pSimInfo1 );
  ABC_FREE( pSimInfo2 );

  // Test for Key -> 1
  // Save off each input separtely
  for( i=0; i < Abc_NtkPiNum(pNtk); i++ ) {
    pDi1[i] = pInp2[i];
    pDi2[i] = pInp2[i+Abc_NtkPiNum(pNtk)];
  }

  // Simulate Pattern 1
  pSimInfo1 = Abc_NtkVerifySimulatePattern( pNtk, pDi1 );

  // Simulate Pattern 2
  pSimInfo2 = Abc_NtkVerifySimulatePattern( pNtk, pDi2 );

  // Print pattern 2 input/output
  //ClapAttack_PrintInp( pNtk, pDi1 );
  //ClapAttack_PrintOut( pNtk, pSimInfo1 );
  //ClapAttack_PrintInp( pNtk, pDi2 );
  //ClapAttack_PrintOut( pNtk, pSimInfo2 );

  Abc_NtkForEachPo( pNtk, pPo, i ) {
    if ( pSimInfo1[i] - pSimInfo2[i] ) {
      printf("Interpret: We had a frequency component for key=1!\n");
      for (KeyIndex=0; KeyIndex < NumKeys; KeyIndex++) {
	KeyWithFreq[KeyIndex] = pKey2[KeyIndex];
	KeyNoFreq[KeyIndex] = pKey1[KeyIndex];
      }
    }
  }

  ABC_FREE( pSimInfo1 );
  ABC_FREE( pSimInfo2 );
  free( pDi1 );
  free( pDi2 );
  free( pInp1 );
  free( pInp2 );
  free( pKey1 );
  free( pKey2 );
}

void ClapAttack_InterpretDi(Abc_Ntk_t *pNtk, int *pDiFull, int NumKeys, int *KeyWithFreq, int *KeyNoFreq) {

  int i, KeyIndex;
  Abc_Obj_t *pPi, *pPo;
  int *pInp1, *pInp2, *pDi1, *pDi2, *pKey1, *pKey2;
  
  pInp1 = (int *)malloc( sizeof(int) * (Abc_NtkPiNum(pNtk)*2) );
  pInp2 = (int *)malloc( sizeof(int) * (Abc_NtkPiNum(pNtk)*2) );
  pDi1 = (int *)malloc( sizeof(int) * Abc_NtkPiNum(pNtk) );
  pDi2 = (int *)malloc( sizeof(int) * Abc_NtkPiNum(pNtk) );
  pKey1 = (int *)malloc( sizeof(int) * NumKeys );
  pKey2 = (int *)malloc( sizeof(int) * NumKeys );
  
  for(i=0; i<(Abc_NtkPiNum(pNtk)*2); i++) {
    pInp1[i] = pDiFull[i];
    pInp2[i] = pDiFull[i];
  }
  
  // Assign key->0
  
  // Goal: Iterate through each PI. Identify list of keys.
  KeyIndex = 0;
  Abc_NtkForEachPi( pNtk, pPi, i ) {
    
    // Are we looking at a key input?
    if( strstr(Abc_ObjName(pPi), "key") ) {

      pInp1[i] = pInp1[i+Abc_NtkPiNum(pNtk)];
      // pInp1[i+Abc_NtkPiNum(pNtk)] = 0;
      //pInp2[i] = 1;
      pInp2[i+Abc_NtkPiNum(pNtk)] = pInp2[i];
      //Abc_Print(1, "match!: %d=%s %d %d\n", i, Abc_ObjName(pPi), pInp[i], pOracleKey[keyindex] );

      // Save off each key value to return
      pKey1[KeyIndex] = pInp1[i];
      pKey2[KeyIndex] = pInp2[i];
      KeyIndex++;
    }
  }
  
  // Save off each input separtely
  for( i=0; i < Abc_NtkPiNum(pNtk); i++ ) {
    pDi1[i] = pInp1[i];
    pDi2[i] = pInp1[i+Abc_NtkPiNum(pNtk)];
  }

  // Simulate Pattern 1
  int * pSimInfo1 = Abc_NtkVerifySimulatePattern( pNtk, pDi1 );

  // Simulate Pattern 2
  int * pSimInfo2 = Abc_NtkVerifySimulatePattern( pNtk, pDi2 );

  // Print pattern 2 input/output
  ClapAttack_PrintInp( pNtk, pDi1 );
  ClapAttack_PrintOut( pNtk, pSimInfo1 );
  ClapAttack_PrintInp( pNtk, pDi2 );
  ClapAttack_PrintOut( pNtk, pSimInfo2 );

  Abc_NtkForEachPo( pNtk, pPo, i ) {
    if ( pSimInfo1[i] - pSimInfo2[i] ) {
      printf("Interpret: We had a frequency component for key=0!\n");
      for (KeyIndex=0; KeyIndex < NumKeys; KeyIndex++) {
	KeyWithFreq[KeyIndex] = pKey1[KeyIndex];
	KeyNoFreq[KeyIndex] = pKey2[KeyIndex];
      }
    }
  }

  ABC_FREE( pSimInfo1 );
  ABC_FREE( pSimInfo2 );

  // Test for Key -> 1
  // Save off each input separtely
  for( i=0; i < Abc_NtkPiNum(pNtk); i++ ) {
    pDi1[i] = pInp2[i];
    pDi2[i] = pInp2[i+Abc_NtkPiNum(pNtk)];
  }

  // Simulate Pattern 1
  pSimInfo1 = Abc_NtkVerifySimulatePattern( pNtk, pDi1 );

  // Simulate Pattern 2
  pSimInfo2 = Abc_NtkVerifySimulatePattern( pNtk, pDi2 );

  // Print pattern 2 input/output
  ClapAttack_PrintInp( pNtk, pDi1 );
  ClapAttack_PrintOut( pNtk, pSimInfo1 );
  ClapAttack_PrintInp( pNtk, pDi2 );
  ClapAttack_PrintOut( pNtk, pSimInfo2 );

  Abc_NtkForEachPo( pNtk, pPo, i ) {
    if ( pSimInfo1[i] - pSimInfo2[i] ) {
      printf("Interpret: We had a frequency component for key=1!\n");
      for (KeyIndex=0; KeyIndex < NumKeys; KeyIndex++) {
	KeyWithFreq[KeyIndex] = pKey2[KeyIndex];
	KeyNoFreq[KeyIndex] = pKey1[KeyIndex];
      }
    }
  }

  ABC_FREE( pSimInfo1 );
  ABC_FREE( pSimInfo2 );
  free( pDi1 );
  free( pDi2 );
  free( pInp1 );
  free( pInp2 );
  free( pKey1 );
  free( pKey2 );
}

int ClapAttack_OracleInferKey(Abc_Ntk_t *pNtk, int * pSimInfo1, int * pSimInfo2) {
  int i, fKeyFreq;
  Abc_Obj_t *pPo;

  // Initialize key to no frequency component value. If we see a frequency component...
  // Flip it!
  fKeyFreq = 0;
  Abc_NtkForEachPo( pNtk, pPo, i ) {
    if ( pSimInfo1[i] - pSimInfo2[i] ) {
      printf("Oracle: We observed a frequency component in our probe!\n");
      fKeyFreq = 1;
    }
  }

  return fKeyFreq;
}

void ClapAttack_OracleSimDi(Abc_Ntk_t *pNtk, int * pDi, int NumKeys, int *KeyWithFreq, int *KeyNoFreq, int *WrongKeyValue) {

  int i;
  int *pDi1, *pDi2;
  int fKeyFreq;

  // Malloc input arrays
  pDi1 = (int *)malloc( sizeof(int) * Abc_NtkPiNum(pNtk));
  pDi2 = (int *)malloc( sizeof(int) * Abc_NtkPiNum(pNtk));
  
  // Save off each input separtely
  for( i=0; i < Abc_NtkPiNum(pNtk); i++ ) {
    pDi1[i] = pDi[i];
    pDi2[i] = pDi[i+Abc_NtkPiNum(pNtk)];
  }

  // Simulate Pattern 1
  int * pSimInfo1 = Abc_NtkVerifySimulatePattern( pNtk, pDi1 );

  // Print pattern 2 input/output
  //ClapAttack_PrintInp( pNtk, pDi1 );
  //ClapAttack_PrintOut( pNtk, pSimInfo1 );

  // Simulate Pattern 2
  int * pSimInfo2 = Abc_NtkVerifySimulatePattern( pNtk, pDi2 );

  // Print pattern 1 input/output
  //ClapAttack_PrintInp( pNtk, pDi2 );
  //ClapAttack_PrintOut( pNtk, pSimInfo2 );

  // Interpret key value based on oracle simulation
  fKeyFreq = ClapAttack_OracleInferKey( pNtk, pSimInfo1, pSimInfo2);

  // Note that were storing the wrong key value as we can't infer the correct one
  // only eliminate the wrong one...
  for ( i=0; i<NumKeys; i++ )
    WrongKeyValue[i] = (fKeyFreq) ? KeyNoFreq[i] : KeyWithFreq[i];

  
  ABC_FREE( pSimInfo1 );
  ABC_FREE( pSimInfo2 );
  free( pDi1 );
  free( pDi2 );
  //ABC_FREE( pDi );
}

void ClapAttack_UpdateKey(char *KeyNameTmp, int KeyValue, struct BSI_KeyData_t *pGlobalBsiKeys) {

  int keyindex;

  // Isolate key index
  keyindex = (int) strtol(&(KeyNameTmp[8]), (char **)NULL, 10);
    
  // update key store with newly known key value
  pGlobalBsiKeys->KeyValue[keyindex] = KeyValue;
    
  // Mark that we changed the keystore.
  pGlobalBsiKeys->Updated = 1;  
}

void ClapAttack_CleanKeyCnf( struct BSI_KeyData_t *pGlobalBsiKeys ) {

  int i;
  Abc_Obj_t *pPi;
  Abc_Ntk_t *pNtkTmp;

  // Replace known keys in the partial key cnf by constants.
  // Convert to logic so we can use existing constant functions
  if (pGlobalBsiKeys->pKeyCnf) {
    pNtkTmp = Abc_NtkToLogic( pGlobalBsiKeys->pKeyCnf );
    Abc_NtkDelete( pGlobalBsiKeys->pKeyCnf );
    pGlobalBsiKeys->pKeyCnf = pNtkTmp;
    
    // Set known keys in circuit
    Abc_NtkForEachPi( pGlobalBsiKeys->pKeyCnf, pPi, i ) {
      // Are we looking at a key input?
      if( strstr(Abc_ObjName(pPi), "key") ) {
	if (ClapAttack_SetKnownKeys( pGlobalBsiKeys->pKeyCnf, pPi, pGlobalBsiKeys ))
	  Abc_NtkDeleteObj( pPi );
      }
    }
    
    pNtkTmp = Abc_NtkStrash( pGlobalBsiKeys->pKeyCnf, 0, 1, 0 );
    Abc_NtkDelete( pGlobalBsiKeys->pKeyCnf );
    pGlobalBsiKeys->pKeyCnf = pNtkTmp;
  }
}

void ClapAttack_DelKnownKeys(Abc_Obj_t **ppNodeList, int NumKnownKeys ) {

  int i;
  
  for(i=0;i<NumKnownKeys;i++) {
    // Remove the node.
    Abc_NtkDeleteObj( ppNodeList[i] );
  }
}

int ClapAttack_SetKnownKeys(Abc_Ntk_t *pNtkCone, Abc_Obj_t *pKey, struct BSI_KeyData_t *pGlobalBsiKeys ) {

  int keyindex;
  
  // Grab the key index
  keyindex = (int) strtol(&((Abc_ObjName(pKey))[8]), (char **)NULL, 10);

  // Check if we know this key index
  if( pGlobalBsiKeys->KeyValue[keyindex] != -1 ) {
    // We know it! -- Replace it with a constant of the proper value
    if( pGlobalBsiKeys->KeyValue[keyindex] )
      Abc_ObjReplaceByConstant( pKey, 1 );
    else
      Abc_ObjReplaceByConstant( pKey, 0 );

    return 1;
  }
  
  return 0;
}

void ClapAttack_OracleSetConeKeys(Abc_Ntk_t * pNtk, int * pInp, int * pOracleKey) {

  int keyindex, i;
  Abc_Obj_t * pPi;  
  
  // Goal: Iterate through each PI. Identify list of keys.
  Abc_NtkForEachPi( pNtk, pPi, i ) {
    
    // Are we looking at a key input?
    if( strstr(Abc_ObjName(pPi), "key") ) {

      // Grab the key index
      keyindex = (int) strtol(&((Abc_ObjName(pPi))[8]), (char **)NULL, 10);

      pInp[i] = pOracleKey[keyindex];
      pInp[i+Abc_NtkPiNum(pNtk)] = pOracleKey[keyindex];

      //Abc_Print(1, "match!: %d=%s %d %d\n", i, Abc_ObjName(pPi), pInp[i], pOracleKey[keyindex] );
    }
  }
}

void ClapAttack_PrintInp(Abc_Ntk_t *pNtk, int * pInp) {

  int i;
  
  for ( i=0 ; i<Abc_NtkPiNum(pNtk) ; i++ ) {
    Abc_Print( 1, "Input %d: %d\n", i, pInp[i] );
  }

}

void ClapAttack_PrintOut(Abc_Ntk_t *pNtk, int * pOut) {

  int i;
  
  for ( i=0 ; i<Abc_NtkPoNum(pNtk) ; i++ ) {
    Abc_Print( 1, "Output %d: %d\n", i, pOut[i] );
  }

}

int ClapAttack_WriteMiterBench(Abc_Ntk_t *pNtk, char *pFileName) {

   Abc_Ntk_t * pNtkTemp;
   pNtkTemp = Abc_NtkToNetlist( pNtk );
   Abc_NtkToAig( pNtkTemp );
   Io_WriteBenchLut( pNtkTemp, pFileName );
   Abc_NtkDelete( pNtkTemp );

  
   return 0;
}

int ClapAttack_WriteMiterVerilog(Abc_Ntk_t *pNtk, char *pFileName) {

   Io_Write( pNtk, pFileName, IO_FILE_VERILOG );
  
   return 0;
}

int ClapAttack_RenameLogic( Abc_Ntk_t * pNtk, char *PiSuffix, char *KeySuffix, char *PoSuffix, int KeepName )
{
  Abc_Obj_t * pObj;
  int nDigits, i, k, CountCur, CountMax = 0;
  char * pName, PrefLi[100], PrefLo[100], **NameTmp;


  // How many zeros od we need to prepend on pi names...
  nDigits = Abc_Base10Log( Abc_NtkPiNum(pNtk) );

  // Malloc name array
  NameTmp = (char **)malloc( sizeof(char *) * Abc_NtkPiNum(pNtk) );
  for ( i=0; i < Abc_NtkPiNum(pNtk); i++) {
    NameTmp[i] = (char *)malloc( sizeof(char) * 100);
  }

  // Generate Pi Name List
  if (KeepName) {
    Abc_NtkForEachPi( pNtk, pObj, i ) {
      if( strstr(Abc_ObjName(pObj), "key") )
	sprintf(NameTmp[i], "%s%s", Abc_ObjName(pObj), KeySuffix);
      else
	sprintf(NameTmp[i], "%s%s", Abc_ObjName(pObj), PiSuffix );
    }
  } else {
    Abc_NtkForEachPi( pNtk, pObj, i ) {
      if( strstr(Abc_ObjName(pObj), "key") )
	sprintf(NameTmp[i], "pi%s_key%s%s", Abc_ObjNameDummy("", i, nDigits), &((Abc_ObjName(pObj))[8]), KeySuffix);
      else
	sprintf(NameTmp[i], "pi%s%s",  Abc_ObjNameDummy("", i, nDigits), PiSuffix );
    }
  }

  // Delete all names
  Nm_ManFree( pNtk->pManName );
  pNtk->pManName = Nm_ManCreate( Abc_NtkCiNum(pNtk) + Abc_NtkCoNum(pNtk) + Abc_NtkBoxNum(pNtk) );

  // Pis
  Abc_NtkForEachPi( pNtk, pObj, i ) {
    Abc_ObjAssignName( pObj, NameTmp[i], NULL );
  }
	
  // Pos
  nDigits = Abc_Base10Log( Abc_NtkPoNum(pNtk) );

  // if (KeepName) {
  //  Abc_NtkForEachPo( pNtk, pObj, i )
  //    Abc_ObjAssignName( pObj, Abc_ObjName(pObj), PoSuffix );
  //} else {
    Abc_NtkForEachPo( pNtk, pObj, i )
      Abc_ObjAssignName( pObj, Abc_ObjNameDummy("po", i, nDigits), PoSuffix );
    //}
  
  // Other logic
  // if PIs/POs already have nodes with what looks like latch names
  // we need to add different prefix for the new latches
  Abc_NtkForEachPi( pNtk, pObj, i )
    {
      CountCur = 0;
      pName = Abc_ObjName(pObj);
      for ( k = 0; pName[k]; k++ )
	if ( pName[k] == 'l' )
	  CountCur++;
	else
	  break;
      CountMax = Abc_MaxInt( CountMax, CountCur );
    }
  Abc_NtkForEachPo( pNtk, pObj, i )
    {
      CountCur = 0;
      pName = Abc_ObjName(pObj);
      for ( k = 0; pName[k]; k++ )
	if ( pName[k] == 'l' )
	  CountCur++;
	else
	  break;
      CountMax = Abc_MaxInt( CountMax, CountCur );
    }
  //printf( "CountMax = %d\n", CountMax );
  assert( CountMax < 100-2 );
  for ( i = 0; i <= CountMax; i++ )
    PrefLi[i] = PrefLo[i] = 'l';
  PrefLi[i] = 'i';
  PrefLo[i] = 'o';
  PrefLi[i+1] = 0;
  PrefLo[i+1] = 0;
  // create latch names
  assert( !Abc_NtkIsNetlist(pNtk) );
  nDigits = Abc_Base10Log( Abc_NtkLatchNum(pNtk) );
  Abc_NtkForEachLatch( pNtk, pObj, i )
    {
      Abc_ObjAssignName( pObj, Abc_ObjNameDummy("l", i, nDigits), PoSuffix );
      Abc_ObjAssignName( Abc_ObjFanin0(pObj),  Abc_ObjNameDummy(PrefLi, i, nDigits), PoSuffix );
      Abc_ObjAssignName( Abc_ObjFanout0(pObj), Abc_ObjNameDummy(PrefLo, i, nDigits), PoSuffix );
    }


  for ( i=0; i < Abc_NtkPiNum(pNtk); i++) {
    free(NameTmp[i]);
   }
  free(NameTmp);
  
  return 0;
}

int ClapAttack_RenameInput( Abc_Ntk_t * pNtk, char *PiSuffix, char *KeySuffix, char *PoSuffix, int nDigits )
{
  Abc_Obj_t * pObj;
  int i, k, CountCur, CountMax = 0;
  char * pName, PrefLi[100], PrefLo[100], **NameTmpPo, **NameTmpPi;


  // Malloc name array
  NameTmpPo = (char **)malloc( sizeof(char *) * Abc_NtkPoNum(pNtk) );
  for ( i=0; i < Abc_NtkPoNum(pNtk); i++) {
    NameTmpPo[i] = (char *)malloc( sizeof(char) * 100 );
  }
  
  // Generate Po Name List
  Abc_NtkForEachPo( pNtk, pObj, i ) {
    sprintf(NameTmpPo[i], "%s%s", Abc_ObjName(pObj), PoSuffix);
  }

    // How many zeros od we need to prepend on pi names...
  //nDigits = Abc_Base10Log( Abc_NtkPiNum(pNtk) );

  // Malloc name array
  NameTmpPi = (char **)malloc( sizeof(char *) * Abc_NtkPiNum(pNtk) );
  for ( i=0; i < Abc_NtkPiNum(pNtk); i++) {
    NameTmpPi[i] = (char *)malloc( sizeof(char) * 100);
  }

  // Generate Pi Name List
  Abc_NtkForEachPi( pNtk, pObj, i ) {
    if( strstr(Abc_ObjName(pObj), "key") )
      sprintf(NameTmpPi[i], "pi%s_key%s%s", Abc_ObjNameDummy("", i, nDigits), &((Abc_ObjName(pObj))[8]), KeySuffix);
    else
      sprintf(NameTmpPi[i], "pi%s%s",  Abc_ObjNameDummy("", i, nDigits), PiSuffix );
   
  }

  // Delete all names
  Nm_ManFree( pNtk->pManName );
  pNtk->pManName = Nm_ManCreate( Abc_NtkCiNum(pNtk) + Abc_NtkCoNum(pNtk) + Abc_NtkBoxNum(pNtk) );

  // Pos
  Abc_NtkForEachPo( pNtk, pObj, i ) {
    Abc_ObjAssignName( pObj, NameTmpPo[i], NULL );
  }

  Abc_NtkForEachPi( pNtk, pObj, i ) {
    Abc_ObjAssignName( pObj, NameTmpPi[i], NULL );
  }

  // Other logic
  // if PIs/POs already have nodes with what looks like latch names
  // we need to add different prefix for the new latches
  Abc_NtkForEachPi( pNtk, pObj, i )
    {
      CountCur = 0;
      pName = Abc_ObjName(pObj);
      for ( k = 0; pName[k]; k++ )
	if ( pName[k] == 'l' )
	  CountCur++;
	else
	  break;
      CountMax = Abc_MaxInt( CountMax, CountCur );
    }
  Abc_NtkForEachPo( pNtk, pObj, i )
    {
      CountCur = 0;
      pName = Abc_ObjName(pObj);
      for ( k = 0; pName[k]; k++ )
	if ( pName[k] == 'l' )
	  CountCur++;
	else
	  break;
      CountMax = Abc_MaxInt( CountMax, CountCur );
    }
  //printf( "CountMax = %d\n", CountMax );
  assert( CountMax < 100-2 );
  for ( i = 0; i <= CountMax; i++ )
    PrefLi[i] = PrefLo[i] = 'l';
  PrefLi[i] = 'i';
  PrefLo[i] = 'o';
  PrefLi[i+1] = 0;
  PrefLo[i+1] = 0;
  // create latch names
  assert( !Abc_NtkIsNetlist(pNtk) );
  nDigits = Abc_Base10Log( Abc_NtkLatchNum(pNtk) );
  Abc_NtkForEachLatch( pNtk, pObj, i )
    {
      Abc_ObjAssignName( pObj, Abc_ObjNameDummy("l", i, nDigits), PoSuffix );
      Abc_ObjAssignName( Abc_ObjFanin0(pObj),  Abc_ObjNameDummy(PrefLi, i, nDigits), PoSuffix );
      Abc_ObjAssignName( Abc_ObjFanout0(pObj), Abc_ObjNameDummy(PrefLo, i, nDigits), PoSuffix );
    }


  for ( i=0; i < Abc_NtkPoNum(pNtk); i++) {
    free(NameTmpPo[i]);
  }
  free(NameTmpPo);

  for ( i=0; i < Abc_NtkPiNum(pNtk); i++) {
    free(NameTmpPi[i]);
  }
  free(NameTmpPi);

  return 0;
}

int ClapAttack_RenameOutput( Abc_Ntk_t * pNtk, char *PoSuffix, int KeepName )
{
  Abc_Obj_t * pObj;
  int nDigits, i, k, CountCur, CountMax = 0;
  char * pName, PrefLi[100], PrefLo[100], **NameTmp;


  // How many zeros od we need to prepend on pi names...
  nDigits = Abc_Base10Log( Abc_NtkPiNum(pNtk) );

  // Malloc name array
  NameTmp = (char **)malloc( sizeof(char *) * Abc_NtkPiNum(pNtk) );
  for ( i=0; i < Abc_NtkPiNum(pNtk); i++) {
    NameTmp[i] = (char *)malloc( sizeof(char) * 100);
  }

  // Generate Pi Name List
  Abc_NtkForEachPi( pNtk, pObj, i ) {
    sprintf(NameTmp[i], "%s", Abc_ObjName(pObj));
  }

  // Delete all names
  Nm_ManFree( pNtk->pManName );
  pNtk->pManName = Nm_ManCreate( Abc_NtkCiNum(pNtk) + Abc_NtkCoNum(pNtk) + Abc_NtkBoxNum(pNtk) );

  // Pis
  Abc_NtkForEachPi( pNtk, pObj, i ) {
    Abc_ObjAssignName( pObj, NameTmp[i], NULL );
  }
	
  // Pos
  //if (KeepName) {
  //  Abc_NtkForEachPo( pNtk, pObj, i )
  //   Abc_ObjAssignName( pObj, Abc_ObjName(pObj), PoSuffix );
  //} else {
    nDigits = Abc_Base10Log( Abc_NtkPoNum(pNtk) );
    Abc_NtkForEachPo( pNtk, pObj, i )
      Abc_ObjAssignName( pObj, Abc_ObjNameDummy("po", i, nDigits), PoSuffix );
    //}
    
  // Other logic
  // if PIs/POs already have nodes with what looks like latch names
  // we need to add different prefix for the new latches
  Abc_NtkForEachPi( pNtk, pObj, i )
    {
      CountCur = 0;
      pName = Abc_ObjName(pObj);
      for ( k = 0; pName[k]; k++ )
	if ( pName[k] == 'l' )
	  CountCur++;
	else
	  break;
      CountMax = Abc_MaxInt( CountMax, CountCur );
    }
  Abc_NtkForEachPo( pNtk, pObj, i )
    {
      CountCur = 0;
      pName = Abc_ObjName(pObj);
      for ( k = 0; pName[k]; k++ )
	if ( pName[k] == 'l' )
	  CountCur++;
	else
	  break;
      CountMax = Abc_MaxInt( CountMax, CountCur );
    }
  //printf( "CountMax = %d\n", CountMax );
  assert( CountMax < 100-2 );
  for ( i = 0; i <= CountMax; i++ )
    PrefLi[i] = PrefLo[i] = 'l';
  PrefLi[i] = 'i';
  PrefLo[i] = 'o';
  PrefLi[i+1] = 0;
  PrefLo[i+1] = 0;
  // create latch names
  assert( !Abc_NtkIsNetlist(pNtk) );
  nDigits = Abc_Base10Log( Abc_NtkLatchNum(pNtk) );
  Abc_NtkForEachLatch( pNtk, pObj, i )
    {
      Abc_ObjAssignName( pObj, Abc_ObjNameDummy("l", i, nDigits), PoSuffix );
      Abc_ObjAssignName( Abc_ObjFanin0(pObj),  Abc_ObjNameDummy(PrefLi, i, nDigits), PoSuffix );
      Abc_ObjAssignName( Abc_ObjFanout0(pObj), Abc_ObjNameDummy(PrefLo, i, nDigits), PoSuffix );
    }

  for ( i=0; i < Abc_NtkPiNum(pNtk); i++) {
    free(NameTmp[i]);
   }
  free(NameTmp);
  
  return 0;
}

int ClapAttack_GetNumKeys( Abc_Ntk_t *pNtk ) {
  int i, NumKeys = 0;
  Abc_Obj_t *pPi;
  
  Abc_NtkForEachPi( pNtk, pPi, i ) {
    // Are we looking at a key input?
    if( strstr(Abc_ObjName(pPi), "keyinput")) {
      //Abc_Print(1, "match!: %d=%s\n", i, Abc_ObjName(pPi) );
      NumKeys++;
    }
  }
  
  return NumKeys;
}

void ClapAttack_InitKeyStore( int NumKeys, struct BSI_KeyData_t *pGlobalBsiKeys ) {
  int i;
  
  pGlobalBsiKeys->NumKeys = NumKeys;
  pGlobalBsiKeys->Updated = 1;
  pGlobalBsiKeys->KeyValue = (int *) malloc( sizeof(int) * NumKeys );

  for( i=0 ; i<NumKeys ; i++ ) {
    pGlobalBsiKeys->KeyValue[i] = -1;
  }

  pGlobalBsiKeys->pKeyCnf = NULL;
}

void ClapAttack_CopyKeyStore( struct BSI_KeyData_t *pGlobalBsiKeys, struct BSI_KeyData_t *pGlobalBsiKeysNew) {
  int i;

  ClapAttack_InitKeyStore( pGlobalBsiKeys->NumKeys, pGlobalBsiKeysNew);
  
  pGlobalBsiKeysNew->Updated =  pGlobalBsiKeys->Updated;
  
  for( i=0 ; i<pGlobalBsiKeys->NumKeys ; i++ ) {
    pGlobalBsiKeysNew->KeyValue[i] = pGlobalBsiKeys->KeyValue[i];
  }
}

int ClapAttack_MiterPos( Abc_Ntk_t * pNtk, int fXorOr, int fXnorAnd)
{
  Abc_Obj_t * pNode, ** pMiterTmp, * pMiter;
  int i, NumPos;
  Abc_Obj_t ** pPos;
  char PoTmpName[100];

  // Check strash status
  assert( Abc_NtkIsStrash(pNtk) );
  
  // Check that we have an even number of Pos
  NumPos = Abc_NtkPoNum(pNtk);
  if ( NumPos == 1 )
    return 1;
  pPos = (Abc_Obj_t **)malloc( sizeof( Abc_Obj_t * ) * NumPos);
  pMiterTmp = (Abc_Obj_t **)malloc( sizeof( Abc_Obj_t * ) * (NumPos)/2);

  
  // start the result -- Unclear why I need this not gate... (Zuzak)
  //pMiter = Abc_ObjNot( Abc_AigConst1(pNtk) );

  
  // Generate PO list
  Abc_NtkForEachPo( pNtk, pNode, i ) {
    pPos[i] = pNode;
  }

  //ClapAttack_WriteMiterVerilog(pNtk, "test1.v");  
  for( i=0; i<(NumPos/2); i++ ) { 
    
    //Abc_AigConst1(pNtk)->pCopy = Abc_AigConst1(pNtkMiter);
    
    // Generate miter connections between oones
    if ( fXorOr )
      pMiterTmp[i] = Abc_AigXor( (Abc_Aig_t *)pNtk->pManFunc, Abc_ObjChild0(pPos[i]), Abc_ObjChild0(pPos[i+(NumPos/2)]) );
    else 
      pMiterTmp[i] =  Abc_ObjNot( Abc_AigXor( (Abc_Aig_t *)pNtk->pManFunc, Abc_ObjChild0(pPos[i]), Abc_ObjChild0(pPos[i+(NumPos/2)]) ));
  }

  //ClapAttack_WriteMiterVerilog(pNtk, "test2.v");
  // remove the POs and their names
  for ( i = Abc_NtkPoNum(pNtk) - 1; i >= 0; i-- )
    Abc_NtkDeleteObj( Abc_NtkPo(pNtk, i) );
  assert( Abc_NtkPoNum(pNtk) == 0 );


  // create new POs
  for( i=0; i<(NumPos/2); i++ ) { 
    pNode = Abc_NtkCreatePo( pNtk ); 
    Abc_ObjAddFanin( pNode, pMiterTmp[i] );
    if (NumPos/2 == 1) {
      Abc_ObjAssignName( pNode, "miter", NULL );
    } else {
      sprintf(PoTmpName, "miter_%d", i);
      Abc_ObjAssignName( pNode, PoTmpName, NULL );
    }
    Abc_NtkOrderCisCos( pNtk );
  }

  //ClapAttack_WriteMiterVerilog(pNtk, "test3.v");
  // make sure that everything is okay
  if ( !Abc_NtkCheck( pNtk ) )
    {
      printf( "Abc_NtkOrPos: The network check has failed.\n" );
      return 0;
    }
  


  //ClapAttack_WriteMiterVerilog(pNtk, "test4.v");
  // Do the last miter...
  // Check that we have an even number of Pos

  // start the result -- Unclear why I need this not gate... (Zuzak)
  //pMiter = Abc_ObjNot( Abc_AigConst1(pNtk) );
  //pMiter = Abc_AigConst1(pNtk);
  
  // Generate PO list
  Abc_NtkForEachPo( pNtk, pNode, i ) {

    // Generate miter connections between oones
    if (i == 0) {
      pMiter = Abc_ObjChild0(pNode);
    } else {
      if ( fXorOr ) {
	pMiter = Abc_AigOr( (Abc_Aig_t *)pNtk->pManFunc, pMiter, Abc_ObjChild0(pNode) );
      } else {
	pMiter = Abc_AigAnd( (Abc_Aig_t *)pNtk->pManFunc, pMiter,  Abc_ObjChild0(pNode) );
      }
    }
  }

  //ClapAttack_WriteMiterVerilog(pNtk, "test6.v");
  
  // remove the POs and their names
  for ( i = Abc_NtkPoNum(pNtk) - 1; i >= 0; i-- )
    Abc_NtkDeleteObj( Abc_NtkPo(pNtk, i) );
  assert( Abc_NtkPoNum(pNtk) == 0 );
  // create the new PO
  pNode = Abc_NtkCreatePo( pNtk );
  Abc_ObjAddFanin( pNode, pMiter );
  Abc_ObjAssignName( pNode, "miter", NULL );
  Abc_NtkOrderCisCos( pNtk );
  // make sure that everything is okay

  //ClapAttack_WriteMiterVerilog(pNtk, "test7.v");
  if ( !Abc_NtkCheck( pNtk ) )
    {
      printf( "Abc_NtkOrPos: The network check has failed.\n" );
      return 0;
    }


  free(pPos);
  free(pMiterTmp);
  
  return 1;
}

int ClapAttack_MiterKeys( Abc_Ntk_t * pNtk )
{
  Abc_Obj_t * pNode, ** pMiterTmp, * pMiter;
  int i, j, NumKeys=0, KeyIdx1=0, KeyIdx2=0;
  Abc_Obj_t ** pKey1, **pKey2;
  char PoTmpName[100];

  // Check strash status
  assert( Abc_NtkIsStrash(pNtk) );

  // Check that we have an even number of Keys
  Abc_NtkForEachPi( pNtk, pNode, i ) {

    // Is it a key?
    if( strstr(Abc_ObjName(pNode), "key") ) {
      NumKeys++;
    }

  }
  
  // Ensure we have an even number of keys
  if ( (NumKeys % 2) )
    return 1;
  pKey1 = (Abc_Obj_t **)malloc( sizeof( Abc_Obj_t * ) * (NumKeys/2));
  pKey2 = (Abc_Obj_t **)malloc( sizeof( Abc_Obj_t * ) * (NumKeys/2));
  pMiterTmp = (Abc_Obj_t **)malloc( sizeof( Abc_Obj_t * ) * (NumKeys/2));

  
  // Check that we have an even number of Keys
  Abc_NtkForEachPi( pNtk, pNode, i ) {

    // Is it a key?
    if( (strstr(Abc_ObjName(pNode), "key")) && (strstr(Abc_ObjName(pNode), "_1")) ) {
      pKey1[KeyIdx1] = pNode;
      KeyIdx1++;
    }
    if( (strstr(Abc_ObjName(pNode), "key")) && (strstr(Abc_ObjName(pNode), "_2")) ) {
      pKey2[KeyIdx2] = pNode;
      KeyIdx2++;
    }
  }


  for( i=0; i<KeyIdx1; i++ ) { 
    for( j=0; j<KeyIdx2; j++ ) { 

      // Generate miter connections between oones
      if ( !ClapAttack_CmpKeyName( Abc_ObjName(pKey1[i]), Abc_ObjName(pKey2[j]),  strlen(Abc_ObjName(pKey1[i])), strlen(Abc_ObjName(pKey2[j])) )) {
	pMiterTmp[i] = Abc_AigXor( (Abc_Aig_t *)pNtk->pManFunc, pKey1[i], pKey2[j] );
	break;
      }
    }
  }

  // remove the POs and their names
  for ( i = Abc_NtkPoNum(pNtk) - 1; i >= 0; i-- )
    Abc_NtkDeleteObj( Abc_NtkPo(pNtk, i) );
  assert( Abc_NtkPoNum(pNtk) == 0 );

// create new POs
  for( i=0; i<(NumKeys/2); i++ ) { 
    pNode = Abc_NtkCreatePo( pNtk ); 
    Abc_ObjAddFanin( pNode, pMiterTmp[i] );
    if (NumKeys/2 == 1) {
      Abc_ObjAssignName( pNode, "key_miter", NULL );
    } else {
      sprintf(PoTmpName, "key_miter_%d", i);
      Abc_ObjAssignName( pNode, PoTmpName, NULL );
    }
    Abc_NtkOrderCisCos( pNtk );
  }

  //ClapAttack_WriteMiterVerilog(pNtk, "test3.v");
  // make sure that everything is okay
  if ( !Abc_NtkCheck( pNtk ) )
    {
      printf( "Abc_NtkOrPos: The network check has failed.\n" );
      return 0;
    }
  

    

  //ClapAttack_WriteMiterVerilog(pNtk, "test4.v");
  // Do the last miter...
  // Check that we have an even number of Pos

  // start the result -- Unclear why I need this not gate... (Zuzak)
  //pMiter = Abc_ObjNot( Abc_AigConst1(pNtk) );
  //pMiter = Abc_AigConst1(pNtk);
  
  // Generate PO list
  Abc_NtkForEachPo( pNtk, pNode, i ) {

    // Generate miter connections between oones
    if (i == 0) {
      pMiter = Abc_ObjChild0(pNode);
    } else {
	pMiter = Abc_AigOr( (Abc_Aig_t *)pNtk->pManFunc, pMiter, Abc_ObjChild0(pNode) );
    }
  }

  //ClapAttack_WriteMiterVerilog(pNtk, "test6.v");
  
  // remove the POs and their names
  for ( i = Abc_NtkPoNum(pNtk) - 1; i >= 0; i-- )
    Abc_NtkDeleteObj( Abc_NtkPo(pNtk, i) );
  assert( Abc_NtkPoNum(pNtk) == 0 );
  // create the new PO
  pNode = Abc_NtkCreatePo( pNtk );
  Abc_ObjAddFanin( pNode, pMiter );
  Abc_ObjAssignName( pNode, "keymiter", NULL );
  Abc_NtkOrderCisCos( pNtk );
  // make sure that everything is okay

  //ClapAttack_WriteMiterVerilog(pNtk, "test7.v");
  if ( !Abc_NtkCheck( pNtk ) )
    {
      printf( "Abc_NtkOrPos: The network check has failed.\n" );
      return 0;
    }

  free(pKey1);
  free(pKey2);
  free(pMiterTmp);
  
  return 1;
}

int ClapAttack_PartialKeyInferenceMiter( Abc_Ntk_t * pNtk, Abc_Ntk_t ** ppNtkMiter, char * CurKeyName )
{
  Abc_Ntk_t *pNtkMiterTmp, *ntkTmp;
  Abc_Obj_t *pNode, **ppNodeFreeList;
  int i, numKeysRemoved;
  int nDigits;

  // Initialzie free list to the number of keys present...
  ppNodeFreeList = (Abc_Obj_t **)malloc( sizeof(Abc_Obj_t *)  * (Abc_NtkPiNum(pNtk)*2) );
  numKeysRemoved = 0;
  
  *ppNtkMiter = Abc_NtkDup(pNtk);
  pNtkMiterTmp = Abc_NtkDup(pNtk);

  // Convert to logic
  ntkTmp = Abc_NtkToLogic( *ppNtkMiter );
  Abc_NtkDelete( *ppNtkMiter );
  *ppNtkMiter = ntkTmp;

  ntkTmp = Abc_NtkToLogic( pNtkMiterTmp );
  Abc_NtkDelete( pNtkMiterTmp );
  pNtkMiterTmp = ntkTmp;

  
  // Replace key of interest by constants
  Abc_NtkForEachPi( *ppNtkMiter, pNode, i ) {
    if ( !strcmp(Abc_ObjName(pNode), CurKeyName) ) {
      Abc_ObjReplaceByConstant( pNode, 0 );
      ppNodeFreeList[numKeysRemoved] = pNode;
      numKeysRemoved++;
    }
  }
  
  Abc_NtkForEachPi( pNtkMiterTmp, pNode, i ) {
    if ( !strcmp(Abc_ObjName(pNode), CurKeyName) ) {
      Abc_ObjReplaceByConstant( pNode, 1 );
      ppNodeFreeList[numKeysRemoved] = pNode;
      numKeysRemoved++;
    }
  }

  if ( numKeysRemoved )
    ClapAttack_DelKnownKeys( ppNodeFreeList, numKeysRemoved );

  free(ppNodeFreeList);

  // Rename Po to avoid collision
  nDigits = Abc_Base10Log( Abc_NtkPiNum(*ppNtkMiter) );
  ClapAttack_RenameInput( *ppNtkMiter, "", "_1", "_1", nDigits );
  ClapAttack_RenameInput( pNtkMiterTmp, "", "_2", "_2", nDigits );
  //ClapAttack_RenamePo( pNtkMiterTmp, 0, "not_key_miter_inv" );

  
  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "partial_key_inference_miter1.v");
  //ClapAttack_WriteMiterVerilog(pNtkMiterTmp, "partial_key_inference_miter2.v");

  // Strash the networks
  ntkTmp = Abc_NtkStrash( *ppNtkMiter, 0, 1, 0 );
  Abc_NtkDelete( *ppNtkMiter );
  *ppNtkMiter = ntkTmp;

  ntkTmp = Abc_NtkStrash( pNtkMiterTmp, 0, 1, 0 );
  Abc_NtkDelete( pNtkMiterTmp );
  pNtkMiterTmp = ntkTmp;

  
  // Append the two networks we prepped for miter 1
  if ( !Abc_NtkAppendSilent( *ppNtkMiter, pNtkMiterTmp, 1 ) )
    {
      Abc_Print( -1, "Appending the networks failed 8.\n" );
      return 1;
    }  

  //ABC_FREE(pNtkMiterTmp);
  Abc_NtkDelete( pNtkMiterTmp );
  // Generate the final miter cone
  extern int Abc_NtkCombinePos( Abc_Ntk_t * pNtk, int fAnd, int fXor );

  if ( !Abc_NtkCombinePos( *ppNtkMiter, 1, 0 ) )
    {
      Abc_Print( -1, "ANDing the POs has failed.\n" );
      return 1;
    }

  //ClapAttack_WriteMiterVerilog(*ppNtkMiter, "partial_key_inference_miter.v");
  return 0;
}

int ClapAttack_CmpKeyName(char *KeyName1, char *KeyName2, int KeyLen1, int KeyLen2) {

  char *Key1Tmp = (char *) malloc( sizeof(char) * (KeyLen1+1) );
  char *Key2Tmp = (char *) malloc( sizeof(char) * (KeyLen2+1) );
  int tmpFlag;
  
  // Copy the strings to their temporary holders
  strcpy(Key1Tmp, KeyName1);
  strcpy(Key2Tmp, KeyName2);

  // Cut the suffix off (i.e. "_1")
  Key1Tmp[KeyLen1-2] = '\0';
  Key2Tmp[KeyLen2-2] = '\0';

  tmpFlag = strcmp(Key1Tmp, Key2Tmp);
  free(Key1Tmp);
  free(Key2Tmp);
  
  return tmpFlag;
}

void ClapAttack_GenSatAttackConfig( Abc_Ntk_t * pNtk, struct BSI_KeyData_t *pGlobalBsiKeys ) {

  Abc_Ntk_t *pNtkTmp, * pNtkTmpSwap, * pNtkFinal;
  Abc_Obj_t * pObj, * pPi, * pPo, * pNode, *pNode2, **ppKnownKeys, *pObj2, **ppPoVec;
  Fraig_Params_t Params;
  int i, j, numKnownKeys;
  char tmpName[100];

  ppPoVec = ( Abc_Obj_t ** ) malloc( sizeof( Abc_Obj_t *) * Abc_NtkPoNum(pNtk));
  
  // Miter in partial key info.
  pNtkFinal = Abc_NtkDup(pNtk);

  // Strash the networks
  pNtkTmpSwap = Abc_NtkStrash( pNtkFinal, 0, 1, 0 );
  Abc_NtkDelete( pNtkFinal );
  pNtkFinal = pNtkTmpSwap;
  
  // Convert to logic so we can use existing constant functions
  pNtkTmpSwap = Abc_NtkToLogic( pNtkFinal );
  Abc_NtkDelete( pNtkFinal );
  pNtkFinal = pNtkTmpSwap;

  numKnownKeys = 0;
  // Set known keys in circuit
  Abc_NtkForEachPi( pNtkFinal, pPi, i ) {
    // Are we looking at a key input?
    if( strstr(Abc_ObjName(pPi), "key") ) {
      if (ClapAttack_SetKnownKeys( pNtkFinal, pPi, pGlobalBsiKeys )) {
	printf("Deleted key: %s\n", Abc_ObjName(pPi));
	numKnownKeys++;
      }
    }
  }


  ppKnownKeys = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * numKnownKeys);
  j = 0;
  Abc_NtkForEachPi( pNtkFinal, pPi, i ) {
    // Are we looking at a key input?
    if( strstr(Abc_ObjName(pPi), "key") ) {
      if (ClapAttack_SetKnownKeys( pNtkFinal, pPi, pGlobalBsiKeys )) {
	ppKnownKeys[j] = pPi;
	j++;
      }
    }
  }

  for(j=0;j<numKnownKeys;j++)
    Abc_NtkDeleteObj( ppKnownKeys[j] );

  free(ppKnownKeys);
  
  // Strash the networks
  pNtkTmpSwap = Abc_NtkStrash( pNtkFinal, 0, 1, 0 );
  Abc_NtkDelete( pNtkFinal );
  pNtkFinal = pNtkTmpSwap;
  
  // Dump final SAT attack bench file
  //Io_Write( pNtkFinal, "test.bench", IO_FILE_BENCH );
  // Dump final SAT attack bench file
  //Io_Write( pNtkFinal, "final_sat_dump_premiter.v", IO_FILE_VERILOG );
  
  // Edge case - skip if we gained no partial key info...
  
  
  if (pGlobalBsiKeys->pKeyCnf) {






    Abc_NtkForEachCo( pNtkFinal, pObj2, j ) {
      ppPoVec[j] = pObj2;
    }
      
    pNtkTmp = Abc_NtkDup(pGlobalBsiKeys->pKeyCnf);

    // Strash the networks
    pNtkTmpSwap = Abc_NtkStrash( pNtkTmp, 0, 1, 0 );
    Abc_NtkDelete( pNtkTmp );
    pNtkTmp = pNtkTmpSwap;

    // Convert to logic so we can use existing constant functions
    pNtkTmpSwap = Abc_NtkToLogic( pNtkTmp );
    Abc_NtkDelete( pNtkTmp );
    pNtkTmp = pNtkTmpSwap;
    
    numKnownKeys = 0;
    // Set known keys in circuit
    Abc_NtkForEachPi( pNtkTmp, pPi, i ) {
      // Are we looking at a key input?
      if( strstr(Abc_ObjName(pPi), "key") ) {
	if (ClapAttack_SetKnownKeys( pNtkTmp, pPi, pGlobalBsiKeys )) {
	  printf("Deleted key: %s\n", Abc_ObjName(pPi));
	  numKnownKeys++;
	}
      }
    }


    ppKnownKeys = (Abc_Obj_t **)malloc(sizeof(Abc_Obj_t *) * numKnownKeys);
    j = 0;
    Abc_NtkForEachPi( pNtkTmp, pPi, i ) {
      // Are we looking at a key input?
      if( strstr(Abc_ObjName(pPi), "key") ) {
	if (ClapAttack_SetKnownKeys( pNtkTmp, pPi, pGlobalBsiKeys )) {
	  ppKnownKeys[j] = pPi;
	  j++;
	}
      }
    }

    for(j=0;j<numKnownKeys;j++)
      Abc_NtkDeleteObj( ppKnownKeys[j] );
  



    





    
    pNtkTmpSwap = Abc_NtkStrash( pNtkTmp, 0, 1, 0 );
    Abc_NtkDelete( pNtkTmp );
    pNtkTmp = pNtkTmpSwap;

    // Dump final SAT attack bench file
    Io_Write( pNtkTmp, "final_sat_dump_miter.v", IO_FILE_VERILOG );
    
    if ( !Abc_NtkAppendSilent( pNtkFinal, pNtkTmp, 1 ) ) {
      Abc_NtkDelete( pNtkTmp );
      Abc_Print( -1, "Appending the networks failed.\n" );
      return;
    }



    // Dump final SAT attack bench file
    //Io_Write( pNtkTmp, "pre_append.v", IO_FILE_VERILOG );

        // Dump final SAT attack bench file
    //Io_Write( pNtkFinal, "append.v", IO_FILE_VERILOG );

    
    // Get fanin for PO
    Abc_NtkForEachCo( pNtkFinal, pObj, i ) {
      if ( (!strcmp(Abc_ObjName(pObj), "not_key_partial" )) || (!strcmp(Abc_ObjName(pObj), "not_key" )) )
	break;
    }

    pNode = Abc_ObjChild0(Abc_NtkPo(pNtkFinal, i));

    // invert it.
    //pNode = Abc_ObjNot(pNode);
    
    // remove the PO and their names
    Abc_NtkDeleteObj( Abc_NtkPo(pNtkFinal, i) );

    

    for(j=0;j< Abc_NtkPoNum(pNtk); j++){

      pNode2 = Abc_ObjChild0(ppPoVec[j]);
	
      // invert it.
      //pNode = Abc_ObjNot(pNode);
      
      // remove the PO and their names
      strcpy (tmpName, Abc_ObjName(ppPoVec[j]));
      Abc_NtkDeleteObj( ppPoVec[j] );
      
      //pNode2 = Abc_AigAnd( pNtkFinal->pManFunc, pNode,  pNode2 );
      
      // create the new PO     
      pPo = Abc_NtkCreatePo( pNtkFinal );
      
      Abc_ObjAddFanin( pPo, Abc_AigAnd( (Abc_Aig_t *)(pNtkFinal)->pManFunc, pNode,  pNode2 ));
      Abc_ObjAssignName( pPo, tmpName, NULL );
      //ClapAttack_RenamePo( pNtkFinal, i,  Abc_ObjName(pObj));
      //Abc_ObjAddFanin( pObj2, Abc_AigAnd( (Abc_Aig_t *)(pNtkFinal)->pManFunc, pNode,  pNode2 ) );
      //Abc_ObjDeleteFanin( pObj2, pNode2 );
    }


    
    // Order the new POs added
    Abc_NtkOrderCisCos( pNtkFinal );


    /*
    exit(0);
    if (j == 5){
      // Append partial key info with the original benchfile
      if ( !Abc_NtkAppendSilentAnd( pNtkFinal, pNtkTmp, 0 ) )
	{
	  Abc_NtkDelete( pNtkFinal );
	  Abc_NtkDelete( pNtkTmp );
	  Abc_Print( -1, "Appending the networks failed.\n" );
	  return;
	}
    }
    */
    Abc_NtkDelete( pNtkTmp );


     

  }

  // Strash+fraig the networks
  pNtkTmpSwap = Abc_NtkStrash( pNtkFinal, 0, 1, 0 );
  Abc_NtkDelete( pNtkFinal );
  pNtkFinal = pNtkTmpSwap;

  // Dump final SAT attack bench file
  //Io_Write( pNtkFinal, "test2.bench", IO_FILE_BENCH );
  
  Fraig_ParamsSetDefault( &Params );
  pNtkTmpSwap = Abc_NtkFraig( pNtkFinal, &Params, 0, 0 );
  Abc_NtkDelete( pNtkFinal );
  pNtkFinal = pNtkTmpSwap;

  // Dump final SAT attack bench file
  Io_Write( pNtkFinal, "final_sat_dump.bench", IO_FILE_BENCH );

    // Dump final SAT attack bench file
  //Io_Write( pNtkFinal, "final_sat_dump.v", IO_FILE_VERILOG );

}



ABC_NAMESPACE_IMPL_END
