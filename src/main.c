#include "main.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  //Call the parser 
  if(argc!=2){printf("Usage: %s filename\n",argv[0]); exit(2);}
  ParserOutputP ret = parser(argv[1]);
  processP unQed = ret->PID0;
  
  /*printf("Memory Size: %d\n",ret->memSize);
  printf("Memory Management Policy: %d\n",ret->policy);
  printf("Policy Parameter: %d\n\n",ret->param);
  printP(unQed);*/

  processP inQ = NULL;
  processP i = NULL;
  long clk = 0;
  int done = 0, tprint=0, avgLife=0;

  //this function will initialize the heap according to size and policy
  memP heap = initMem(ret->memSize,ret->policy,ret->param);

  while(!done){
    tprint = 0;
    //traverse mem checking all processes for end of life
    heap = sweepMem(clk,&tprint,heap);
    while(unQed!=NULL && unQed->aTime == clk){
      popQ2Q(&unQed,&inQ);
      printStatus(clk, &tprint, ARRIVAL, inQ);//print formatted status
    }

    i = inQ;
    int update = 1;
    while(i!=NULL){
      tempHeapP tempHeap = NULL;
      int success = 0;
      if(heap->policy == 0){
        success = allocMemVSP(&heap, i, ret->memSize);
      } else if(heap->policy == 1){
        tempHeap = allocMemPAG(&heap, i, ret->memSize);
      } else if(heap->policy == 2){
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
    if(inQ==NULL&&unQed==NULL&&heap->size==ret->memSize&&
       heap->proc==NULL) done = 1;
    clk++;
  }
  printf("\nAverage Turnaround Time: %4.2f\n",
        (float)avgLife/ret->count);
}

memP sweepMem(long clk, int *tprint, memP heap){
  if(heap==NULL) return heap;
  if(heap->proc != NULL && heap->proc->deadline == clk){
    printStatus(clk, tprint, DONE, heap->proc);
    removeProc(&heap);
    memP t = heap;
    while(t->prev!=NULL) t = t->prev;
    printMM(t);
    t = sweepMem(clk, tprint, heap->next);
    if(t!=heap) heap->next = t;
    return heap;
  }
  memP t = sweepMem(clk, tprint, heap->next);
  if(t!=heap) heap->next = t;
  return heap;
}

//heap->proc should never be null
void removeProc(memP *heap){
  memP t = (*heap);
  free(t->proc->space);
  free(t->proc);
  t->proc = NULL;
  if(t->prev!=NULL && t->prev->proc==NULL){
    t = t->prev;
    t->size += t->next->size;
    if(t->next->next!=NULL && t->next->next->proc==NULL){
      t->size += t->next->next->size;
      t->next = t->next->next->next;
      free((*heap)->next);
    }
    else if(t->next->next==NULL) t->next = NULL;
    else t->next = t->next->next;
    free(*heap);
  }
  else if(t->next!=NULL && t->next->proc==NULL){
    t = t->next;
    (*heap)->size += t->size;
    (*heap)->next = t->next;
    free(t);
    t = (*heap);
  }
  if(t->next!=NULL) t->next->prev = t;
  *heap = t;
}

void popQ2Q(processP *unQ, processP *Q){
  processP t = *unQ;
  processP lQ = lastP(*Q);
  if(t->next!=NULL) t->next->prev = NULL;
  *unQ = t->next;
  t->next = NULL;
  t->prev = lQ;
  if(lQ==NULL) *Q=t;
  else lQ->next = t;
}

//p should never be null
void popN(processP *p){
  processP t = *p;
  *p = t->next;
  if(t->next!=NULL) t->next->prev = t->prev;
  if(t->prev!=NULL) t->prev->next = t->next;
  t->next = NULL;
  t->prev = NULL;
}

//this function will initialize the heap according to size and policy
memP initMem(int size, int policy, int param){
  memP t = malloc(sizeof(memR));
  t->size = size;
  t->addr = 0;
  t->policy = policy;
  t->param= param;
  t->next = NULL;
  t->prev = NULL;
  t->proc = NULL;
  return t;
}

int allocMemVSP(memP *heap, processP proc, int memSize){
  int param = (*heap)->param;
  int requiredSize = proc->space->x;
  memP t = *heap;
  int heapSize = 0;

  //First fit
  if(param == 0){
    int locFound = 0;

    while((locFound == 0) && (t != NULL)){
      heapSize = t->size;
      if(t->proc == NULL && (heapSize >= requiredSize)){
        locFound = 1;
        subdivideHeap(&t, proc, requiredSize);
      } else {
        t = t->next;
      }
      
    }
  return locFound;

  //Best fit
  } else if(param == 1){
    int smallestSize = memSize + 1;
    memP smallestBlock = t;

    while(t != NULL){
      heapSize = t->size;
      if(t->proc == NULL){
        
        if(heapSize >= requiredSize && heapSize < smallestSize){
          smallestSize = heapSize;
          smallestBlock = t;
        } 
      }

      t = t->next;
    }
    if(smallestSize != memSize+1){
      subdivideHeap(&smallestBlock, proc, requiredSize);
      return 1;
    }
    else return 0;

  //Worst fit
  } else if(param == 2){
    int largestSize = 0;
    memP largestBlock = t;

    while(t!=NULL){
      heapSize = t->size;
      if(t->proc == NULL){

        if(heapSize >= requiredSize && heapSize > largestSize){
          largestSize = heapSize;
          largestBlock = t;
        }
      }

      t = t->next;
    }
    if(largestSize != 0){
      subdivideHeap(&largestBlock, proc, requiredSize);
      return 1;
    }
    else return 0;
  }

}

tempHeapP allocMemPAG(memP *heap, processP proc, int memSize){
  memP t = *heap;
  tempHeapP tempPageIndex = malloc(sizeof(tempHeapR));
  tempPageIndex->next = NULL;
  tempPageIndex->prev = NULL;
  tempPageIndex->memBlock = NULL;

  tempHeapP tempPageHead = tempPageIndex;

  int pageSize = t->param;
  int pagesRequired = (proc->space->x / pageSize) + 1;
  

  int pagesFound = 0;
  while((pagesFound < pagesRequired) && (t != NULL)){
    if(t->proc == NULL){
      pagesFound++;


      if(pagesFound == 1){
        tempPageIndex->memBlock = t;
      } else if(pagesFound > 1) {
        tempHeapP tempPage = malloc(sizeof(tempHeapR));
        tempPage->memBlock = t;
        tempPage->next = NULL;
        tempPage->prev = tempPageIndex;

        tempPageIndex->next = tempPage;
        tempPageIndex = tempPageIndex->next;
      }
    }

    t = t->next;

  }  



  tempPageIndex = tempPageHead;
  if(pagesFound == pagesRequired){
    while(tempPageIndex != NULL){
      tempPageIndex->memBlock->proc = proc;
      tempPageIndex = tempPageIndex->next;
    }

    return tempPageHead;

  } else {
    if(tempPageIndex->next == NULL){
      free(tempPageIndex);
    } else {
      while(tempPageIndex->next != NULL){
        tempPageIndex = tempPageIndex->next;
        free(tempPageIndex->prev);
      }

      free(tempPageIndex);

      return NULL;
    }
  } 

}

tempHeapP allocMemSEG(memP *heap, processP proc, int memSize){
  int param = (*heap)->param;
  int requiredSize;
  memP localHeap = *heap;
  spaceP currentSpace = proc->space;

  tempHeapP tempSegIndex = malloc(sizeof(tempHeapR));
  tempSegIndex->next = NULL;
  tempSegIndex->prev = NULL;
  tempSegIndex->memBlock = NULL;

  tempHeapP tempSegHead = tempSegIndex;

  int heapSize = 0;
  int allLocsFound = 0;
  int numSegsFound = 0;
  
  //First fit
  if(param == 0){
  
    while((allLocsFound == 0) && (localHeap != NULL)){
      requiredSize = currentSpace->x;
      heapSize = localHeap->size;

      //If the current heap block is free and is large enough
      if(localHeap->proc == NULL && heapSize >= requiredSize){
        numSegsFound++;

        if(numSegsFound == 1){
          tempSegIndex->memBlock = localHeap;
        } else if(numSegsFound > 1){
          tempHeapP tempSeg = malloc(sizeof(tempHeapR));
          tempSeg->memBlock = localHeap;
          tempSeg->next = NULL;
          tempSeg->prev = tempSegIndex;
          tempSegIndex->next = tempSeg;
          tempSegIndex = tempSegIndex->next;
        }
        currentSpace = currentSpace->next;

        if(currentSpace == NULL){
          allLocsFound = 1;
        }
      }
      localHeap = localHeap->next;
    }

  //Best fit
  } else if(param == 1){

  //Worst fit
  } else if(param == 2){

  }

  //If successfully found enough blocks
  if(currentSpace == NULL){

  } else {

  }

}


void subdivideHeap(memP *heap, processP proc, int requiredSize){
  int heapSize = (*heap)->size;
  if(heapSize != requiredSize){
    memP newHeapBlock = malloc(sizeof(memR));
    newHeapBlock->proc = NULL;
    newHeapBlock->addr = (*heap)->addr + requiredSize;
    newHeapBlock->param = (*heap)->param;
    newHeapBlock->policy= (*heap)->policy;
    newHeapBlock->next = (*heap)->next;
    newHeapBlock->prev = *heap;
    newHeapBlock->size = heapSize - requiredSize;
    if(newHeapBlock->next != NULL){
      newHeapBlock->next->prev = newHeapBlock;
    }
    (*heap)->proc = proc;
    (*heap)->next = newHeapBlock;
    (*heap)->size = requiredSize;
  } else {
    (*heap)->proc = proc;
  }
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

  spaceP head = getFront(largestSpace);

  proc->space = head;
}

spaceP getFront(spaceP p){
  while(p->prev != NULL){
    p = p->prev;
  }

  return p;
}

void printHeap(memP heap){
  printf("[");
  while(heap!=NULL){
    if(heap->proc!=NULL) printf("X");
    printf("%d",heap->size);
    heap = heap->next;
    if(heap!=NULL) printf("|");
    else printf("]\n");
  }
}

void printStatus(long clk, int *tprint, int status, processP p){
  if(!(*tprint)){
    printf("\nt = %ld:\t",clk);
    if(clk<100) printf("\t");
    *tprint = 1;
  }
  else printf("\t\t");
  if(status == ARRIVAL){
    printf("Process %d arrives\n",lastP(p)->pid);
    printf("\t\tInput Queue: ");
    printQueue(p);
  }
//  else if(status == ALLOC1){
//    printf("MM moves Process %d to memory\n", p->pid);
//  }
  else if(status == ALLOC){
    printf("Input Queue: ");
    printQueue(p);
  }
  else if(status == DONE){
    printf("Process %d completes\n",p->pid);
  }
}

void printMM(memP heap){
  printf("\t\tMemory Map:\t");
  while(heap!=NULL){
    printf("%d-%d: ",heap->addr,heap->addr+heap->size-1);
    if(heap->proc==NULL) printf("Hole");
    else printf("Process %d",heap->proc->pid);
    heap = heap->next;
    printf("\n");
    if(heap!=NULL) printf("\t\t\t\t");
  }
}

void printQueue(processP Q){
  printf("[");
  while(Q!=NULL){
    printf("Process %d",Q->pid);
    if(Q->next!=NULL){
      printf(", ");
    }
    Q = Q->next;
  }
  printf("]\n");
}
