#ifndef CPUBURST_STRUCT
#define CPUBURST_STRUCT
typedef struct CPUburst{
  int length; struct CPUburst *next;
}CPUburstR, *CPUburstP;
#endif

#ifndef IOBURST_STRUCT
#define IOBURST_STRUCT
typedef struct IOburst{
  int length; int devId; struct IOburst *next;
}IOburstR, *IOburstP;
#endif

#ifndef PROCESS_STRUCT
#define PROCESS_STRUCT
typedef struct process{
  int pid; int aTime; CPUburstP CPU; IOburstP IO;
  int status; struct process *next; struct process *prev;
}processR, *processP;
#endif

processP parser(char*, int*, int*);
processP insertP(processP,int);
CPUburstP insertCPU(CPUburstP,int);
IOburstP insertIO(IOburstP,int);
processP lastP(processP);
CPUburstP lastCPU(CPUburstP);
IOburstP lastIO(IOburstP);
void printP(processP);
