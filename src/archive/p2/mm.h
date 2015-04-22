#include "parser.h"

#define ARRIVAL 1
#define ALLOC   2
#define DONE    3

memP sweepMem(long, int*, int*, memP);
void removeProc(memP*);
void popQ2Q(processP*, processP*);
void popN(processP*);
memP initMem(int,int,int);
int allocMemVSP(memP*, processP, int);
tempHeapP allocMemPAG(memP*, processP, int);
tempHeapP allocMemSEG(memP*, processP, int);
void subdivideHeap(memP*, processP, int);
void printHeap(memP);
void printStatus(long,int*,int,processP);
void printMM(memP);
void printQueue(processP);
spaceP getFrontSpace(spaceP);
memP getFrontMem(memP);
void cleanHeap(memP*);
