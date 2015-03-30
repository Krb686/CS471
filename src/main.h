#include "parser.h"
#include "mm.h"

#define ARRIVAL	1
#define ALLOC	2
#define DONE	3

int main(int, char*[]);
void orderHighLow(processP);
spaceP getFrontSpace(spaceP);
memP getFrontMem(memP);
