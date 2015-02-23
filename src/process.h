#include "parser.h"

#ifndef PROCESS_STRUCT
#define PROCESS_STRUCT
typedef struct process{
  int pid; int aTime; CPUburstP CPU; IOburstP IO;
  int status; struct process *next; struct process *prev;
}processR, *processP;
#endif

//Function prototypes
void printP(processP);
