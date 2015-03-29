#define VSP	0
#define PAG	1
#define SEG	2

#ifndef SPACE_STRUCT
#define SPACE_STRUCT
typedef struct space{
  struct space *next;
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

//Function prototypes
void printP(processP);
