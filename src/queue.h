#include "parser.h"

#ifndef QUEUE_STRUCT
#define QUEUE_STRUCT
typedef struct queue{
  int tQ; processP head;
}Queue;
#endif


void demoteProcess(processP*);
/*typedef struct RRqueue{
  int tQ; processP head;
}

typedef struct SRTFqueue{
  processP head;
}

typedef struct FCFSqueue{
  processP head;
}
*/
