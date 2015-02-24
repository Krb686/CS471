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
  struct process *next, *prev;
  CPUburstP CPU; IOburstP IO;
  int pid, aTime, status, wTime, rTime;
}processR, *processP;
#endif

//Function prototypes
void printP(processP);
