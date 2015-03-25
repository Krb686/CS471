#include "parser.h"

#ifndef QUEUE_STRUCT
#define QUEUE_STRUCT
typedef struct queue{
  processP head; int tQ, status;
}QueueR, *QueueP;
#endif


void demoteProcess(processP);
