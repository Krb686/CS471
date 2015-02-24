#include "parser.h"
#include "process.h"
#include "queue.h"

#define READY	1
#define	RUN	2
#define IO	3
#define WAIT	4
#define DONE	5

int main(int, char*[]);
void insertRR(QueueP, processP);
void printQueue(QueueP);
processP dispatch(QueueP);
