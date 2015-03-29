#include "parser.h"
#include "mm.h"

#define ARRIVAL	1
#define ALLOC	2
#define DONE	3

int main(int, char*[]);
memP sweepMem(long, int*, memP);
void removeProc(memP*);
void popQ2Q(processP*, processP*);
void popN(processP*);
memP initMem(int,int,int);
int allocMem(memP*, processP, int);
int allocMemVSP(memP*, processP, int);
//void allocMemPAG(memP*, processP, int);
//void allocMemSEG(memP*, processP, int);
void subdivideHeap(memP*, processP, int, int);
void printHeap(memP);
void printStatus(long,int*,int,processP);
void printMM(memP);
void printQueue(processP);
