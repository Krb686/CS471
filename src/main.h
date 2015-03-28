#include "parser.h"
#include "mm.h"
#include "memory.h"

int main(int, char*[]);
void pop(processP*, processP*);

void allocMem(memP, processP, int);
void allocMemVSP(memP, processP, int);
void allocMemPAG(memP, processP, int);
void allocMemSEG(memP, processP, int);
void subdivideHeap(memP, int, int);
//void checkArrivals(QueueP*, processP*, int);
//void dispatch(processP*, QueueP*,int*, int);
//void CPUcomplete(processP*,int,int*,int);
//void demote(processP*,QueueP*,QueueP*,int*,int);
//void insertQ(QueueP, processP);
//void insertSRTF(QueueP, processP);
//CPUburstP removeCPUburst(CPUburstP);
//IOburstP removeIOburst(IOburstP);
//void printQueue(QueueP);
//processP popQ(QueueP);
//void processIO(QueueP*, QueueP*, int*, int*, int*, int);
