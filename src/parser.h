#include "process.h"

/*#ifndef PROCESS_STRUCT
#define PROCESS_STRUCT
typedef struct process{
  int pid; int aTime; CPUburstP CPU; IOburstP IO;
  int status; struct process *next; struct process *prev;
}processR, *processP;
#endif*/

#ifndef PARSEROUTPUT_STRUCT
#define PARSEROUTPUT_STRUCT
typedef struct parserOutput{
  int tQ1, tQ2;
  int procCount;

  processP PID0;

} ParserOutputR, *ParserOutputP;
#endif

ParserOutputP parser(char*);
processP insertP(processP,int);
CPUburstP insertCPU(CPUburstP,int);
IOburstP insertIO(IOburstP,int);
processP lastP(processP);
CPUburstP lastCPU(CPUburstP);
IOburstP lastIO(IOburstP);
void printP(processP);
