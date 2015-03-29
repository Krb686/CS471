#include "parser.h"
#include "mm.h"

int main(int, char*[]);
memP sweepMem(int, memP);
void removeProc(memP*);
void popQ2Q(processP*, processP*);
void popN(processP*);
memP initMem(int,int,int);
int allocMem(memP*, processP, int);
int allocMemVSP(memP*, processP, int);
void allocMemPAG(memP*, processP, int);
void allocMemSEG(memP*, processP, int);
void orderHighLow(processP);
spaceP getFront(spaceP);
void subdivideHeap(memP*, processP, int, int);
void printHeap(memP);
