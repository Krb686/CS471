#include "process.h"

typedef struct m{
  processP proc;
  struct m *next, *prev;
  int size, addr, policy, param;
}memR, *memP;


typedef struct tempHeap{
  struct tempHeap *next, *prev;
  struct m *memBlock;
} tempHeapR, *tempHeapP;
