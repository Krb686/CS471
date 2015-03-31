#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  //Call the parser 
  if(argc!=2){printf("Usage: %s filename\n",argv[0]); exit(2);}
  ParserOutputP ret = parser(argv[1]);
  processP unQed = ret->PID0;
  
  processP inQ = NULL;
  processP i = NULL;
  long clk = 0;
  int tprint=0, avgLife=0, count=ret->count;

  //this function will initialize the heap according to size and policy
  memP heap = initMem(ret->memSize,ret->policy,ret->param);

  while(count>0){
    tprint = 0;
    //traverse mem checking all processes for end of life
    heap = sweepMem(clk,&count,&tprint,heap);
    while(unQed!=NULL && unQed->aTime == clk){
      popQ2Q(&unQed,&inQ);
      printStatus(clk, &tprint, ARRIVAL, inQ);//print formatted status
    }

    i = inQ;
    int update = 1;
    while(i!=NULL){
      tempHeapP tempHeap = NULL;
      int success = 0;
      if(heap->policy == VSP){
        success = allocMemVSP(&heap, i, ret->memSize);
      } else if(heap->policy == PAG){
        tempHeap = allocMemPAG(&heap, i, ret->memSize);
      } else if(heap->policy == SEG){
        tempHeap = allocMemSEG(&heap, i, ret->memSize);
      }
      if(tempHeap!=NULL|success>0){//need to add tempHeap to proc
        printf("\t\tMM moves Process %d to memory\n",i->pid);
        i->deadline += clk;
        avgLife += i->deadline - i->aTime;
        popN(&i);
        if(update) inQ = i;
        printStatus(clk, &tprint, ALLOC, inQ);
        printMM(heap);
      }
      else{
        update = 0;
        i = i->next;
      }
    }
    clk++;
  }
  printf("\nAverage Turnaround Time: %4.2f\n",
        (float)avgLife/ret->count);
}

void orderHighLow(processP proc){
  spaceP spaceIndex    = proc->space;
  spaceP spaceIterator = proc->space;
  spaceP largestSpace  = NULL;
  spaceP swapSpace     = NULL;

  int highestSize; 

  while(spaceIndex != NULL){
    
    spaceIterator = spaceIndex;
    highestSize = 0;
    while(spaceIterator != NULL){
      if(spaceIterator->x > highestSize){
        highestSize = spaceIterator->x;
        largestSpace = spaceIterator;
      }

      spaceIterator = spaceIterator->next;
    }

    swapSpace = spaceIndex;
    if(spaceIndex->prev != NULL){
      spaceIndex->prev->next = largestSpace;
    }
    
    if(spaceIndex->next != NULL){
      spaceIndex->next->prev = largestSpace;
    }

    if(largestSpace->prev != NULL){
      largestSpace->prev->next = spaceIndex;
    }

    if(largestSpace->next != NULL){
      largestSpace->next->prev = spaceIndex;
    }

    spaceIndex->prev = largestSpace->prev;
    spaceIndex->next = largestSpace->next;

    largestSpace->prev = swapSpace->prev;
    largestSpace->next = swapSpace->next; 
    
    spaceIndex = largestSpace->next;
  }

  spaceP head = getFrontSpace(largestSpace);

  proc->space = head;
}


