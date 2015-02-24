#include "parser.h"
#include "process.h"
#include "queue.h"

int main(int, char*[]);
void insertRR(QueueP, processP);
void insertSRTF(QueueP, processP);
void printQueue(QueueP);
processP dispatch(QueueP);
