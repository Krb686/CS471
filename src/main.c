#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  //Call the parser 
  if(argc!=2){printf("Usage: %s filename\n",argv[0]); exit(2);}
  ParserOutputP ret = parser(argv[1]);
  processP unQed = ret->PID0;
  
  printf("Memory Size: %d\n",ret->memSize);
  printf("Memory Management Policy: %d\n",ret->policy);
  printf("Policy Parameter: %d\n\n",ret->param);
  //printP(unQed);

  processP inQ = NULL;
  long clk = 0;
  int done = 0;
  int pass= 0;

  //this function will initialize the heap according to size and policy
  memP heap = initMem(ret->memSize,ret->policy, ret->param);

  while(!done){
    //traverse mem checking all processes for end of life
    heap = sweepMem(clk,inQ,heap)//mem_ptr should have processP field

    while(unQed!=NULL && unQed->aTime == clk){
      pop(&unQed,&inQ);//finished
      printStatus(ARRIVAL,inQ)//print formatted status
    }

    i = inQ;
    while(i!=NULL){
      //can prob make one func that calls memcheck first
      allocMem(heap, i, ret->memSize);
      printStatus(ADMIT,inQ)//print formatted status
      i = i->next;
    }

    clk++;
    done = 1;//temp. Can prob use (inQ==NULL && unQed==NULL) instead
  }
}

memP sweepMem(int clk, memP heap){
  if(heap==NULL) return heap;
  if(heap->proc != NULL && heap->proc->deadline == clk){
    removeProc(&heap);
    heap->next = sweepMem(clk, heap->next);
    return heap;
  }
  heap->next = sweepMem(clk, heap->next);
  return heap;
}

//heap->proc should never be null
void removeProc(memP *heap){
  memP t = (*heap);
  free(t->proc->space);
  free(t->proc);
  t->proc = NULL;
  if(t->prev!=NULL){
    t = t->prev;
    t->size += t->next->size
    if(t->next->next!=NULL){
      t->size += t->next->next->size;
      t->next = t->next->next->next;
      free((*heap)->next);
    }
    else t->next = NULL;
    free(*heap);
  }
  else if(t->next!=NULL){
    t = t->next;
    (*heap)->size += t->size;
    (*heap)->next = t->next;
    free(t)
    t = (*heap);
  }
  if(t->next!=NULL) t->next->prev = t;
  *heap = t;
}

void pop(processP *unQ, processP *Q){
  processP t = *unQ;
  processP lQ = lastP(*Q);
  if(t->next!=NULL) t->next->prev = NULL;
  *unQ = t->next;
  t->next = NULL;
  t->prev = lQ;
  if(lQ==NULL) *Q=t;
  else lQ->next = t;
}

//this function will initialize the heap according to size and policy
memP initMem(int size, int policy, int param){
  memP t = malloc(sizeof(memP));
  t->size = size;
  t->addr = 0;
  t->policy = policy;
  t->param= param;
  t->next = NULL;
  t->prev = NULL;
  t->proc = NULL;
  return t;
}



void allocMem(memP *heap, processP *proc, int memSize){
  if((*heap)->policy == 0){
    allocMemVSP(heap, proc, memSize);
  } else if((*heap)->policy == 1){
    allocMemPAG(heap, proc, memSize);
  } else if((*heap)->policy == 2){
    allocMemSEG(heap, proc, memSize);
  }
}

void allocMemVSP(memP *heap, processP *proc, int memSize){
  int param = (*heap)->param;
  int requiredSize = proc->space->x;

  
  //First fit
  if(param == 0){
    int locFound = 0;

    while((locFound == 0) && (heap != NULL)){
      int heapSize = 0;

      if(heap->next != NULL){
        heapSize = heap->next->address - heap->address;
      } else {
        heapSize = memSize; 
      }


      if(heap->status == 0 && (heapSize >= requiredSize)){
        locFound = 1;
        subdivideHeap(heap, heapSize, requiredSize);
      } else {
        heap = heap->next;
      }
      
    }

  //Best fit
  } else if(param == 1){
    int smallestSize = memSize + 1;
    memP smallestBlock = heap;

    while(heap != NULL){
      if(heap->status == 0){
        int heapSize = 0;
        
        if(heap->next != NULL){
          heapSize = heap->next->address - heap->address;
        } else {
          heapSize = memSize;
        }

        if(heapSize >= requiredSize && heapSize < smallestSize){
          smallestSize = heapSize;
          smallestBlock = heap;
        } 
      }

      heap = heap->next;
    }

    subdivideHeap(heap, heapSize, requiredSize);


  //Worst fit
  } else if(param == 2){
    int largestSize = 0;
    memP largestBlock = heap;

    while(heap!=NULL){
      if(heap->status == 0){
        int heapSize = 0;

        if(heap->next != NULL){
          heapSize = heap->next->address - heap->address;
        } else {
          heapSize = memSize;
        }

        if(heapSize >= requiredSize && heapSize > largestSize){
          largestSize = heapSize;
          largestBlock = heap;
        }
      }

      heap = heap->next;
    }

    subdivideHeap(heap, heapSize, requiredSize);

  }

}

void allocMemPAG(memP *heap, processP *proc){



}

void allocMemSEG(memP *heap, processP *proc){


}

void subdivideHeap(memP *heap, int heapSize, int requiredSize){
  if(heapSize != requiredSize){
    memP newHeapBlock = malloc(sizeof(memP));
    newHeapBlock->status = 0;
    newHeapBlock->address = heap->address + requiredSize;
    newHeapBlock->param = heap->param;
    newHeapBlock->next = heap->next;
    newHeapBlock->prev = heap;

    heap->status = 1;
    heap->next = newHeapBlock;
  } else {
    heap->status = 1;
  }
}
