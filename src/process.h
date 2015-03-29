#define VSP	0
#define PAG	1
#define SEG	2

#ifndef SPACE_STRUCT
#define SPACE_STRUCT
typedef struct space{
  struct space *next, *prev;
  int x;
}spaceR, *spaceP;
#endif

#ifndef PROCESS_STRUCT
#define PROCESS_STRUCT
typedef struct process{
  struct process *next, *prev;
  spaceP space;
  int pid, aTime, deadline;
}processR, *processP;
#endif

#ifndef MEM_STRUCT
#define MEM_STRUCT
typedef struct m{
  processP proc;
  struct m *next, *prev;
  int size, addr, policy, param;
}memR, *memP;
#endif

#ifndef TEMP_HEAP_STRUCT
#define TEMP_HEAP_STRUCT
typedef struct tempHeap{
  struct tempHeap *next, *prev;
  struct m *memBlock;
} tempHeapR, *tempHeapP;
#endif

//Function prototypes
void printP(processP);
