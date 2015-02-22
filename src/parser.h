#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CPUburst{
  int length; struct CPUburst *next;
}CPUburstR, *CPUburstP;

typedef struct IOburst{
  int length; int devId; struct IOburst *next;
}IOburstR, *IOburstP;

typedef struct process{
  int pid; int aTime; CPUburstP CPU; IOburstP IO;
  int status; struct process *next; struct process *prev;
}processR, *processP;

processP parser(char*);
processP insertP(processP,int);
CPUburstP insertCPU(CPUburstP,int);
IOburstP insertIO(IOburstP,int);
processP lastP(processP);
CPUburstP lastCPU(CPUburstP);
IOburstP lastIO(IOburstP);
void printP(processP);
