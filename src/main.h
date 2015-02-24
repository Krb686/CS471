#include "parser.h"
#include "process.h"
#include "queue.h"

int main(int, char*[]);
void checkArrivals(QueueP*, processP*, int*);
void insertQ(QueueP, processP);
void insertSRTF(QueueP, processP);
CPUburstP removeCPUburst(CPUburstP);
IOburstP removeIOburst(IOburstP);
void printQueue(QueueP);
processP dispatch(QueueP);
