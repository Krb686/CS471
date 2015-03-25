/*#define READY	0
#define	RUNQ1	1
#define RUNQ2	2
#define RUNQ3	3
#define IOEX	4
#define WAIT	5
#define DONE	6
*/
#define VSP	0
#define PAG	1
#define SEG	2

#ifndef PROCESS_STRUCT
#define PROCESS_STRUCT
typedef struct process{
  struct process *next, *prev;
  int pid, aTime, lifetime, space;
}processR, *processP;
#endif

//Function prototypes
void printP(processP);
