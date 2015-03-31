#include "mm.h"
#include <stdio.h>
#include <stdlib.h>

memP sweepMem(long clk, int *count, int *tprint, memP heap){
  if(heap==NULL) return heap;
  if(heap->proc != NULL && heap->proc->deadline == clk){
    printStatus(clk, tprint, DONE, heap->proc);
    (*count)--;
    removeProc(&heap);
    memP t = heap;
    while(t->prev!=NULL) t = t->prev;
    printMM(t);
    t = sweepMem(clk, count, tprint, heap->next);
    if(t!=heap) heap->next = t;
    return heap;
  }
  memP t = sweepMem(clk, count, tprint, heap->next);
  if(t!=heap) heap->next = t;
  return heap;
}

//heap->proc should never be null
void removeProc(memP *heap){
  memP t = *heap;
  memP *tempHeap = heap;
  int policy = t->policy;
  tempHeapP pageIter = t->proc->blocks;

  free(t->proc->space);//fix for SEG
  free(t->proc);
  if(policy==PAG){
    do{
      t = pageIter->memBlock;
      t->proc = NULL;
      pageIter = pageIter->next;
    }while(pageIter!=NULL);
  }
  else{
    do{
      if(policy==SEG){
        t = pageIter->memBlock;
        memP *tempHeap = &((*heap)->prev);
      }
      t->proc = NULL;
      if(t->prev!=NULL && t->prev->proc==NULL){
        t = t->prev;
        t->size += t->next->size;
        if(t->next->next!=NULL && t->next->next->proc==NULL){
          t->size += t->next->next->size;
          t->next = t->next->next->next;
          free((*tempHeap)->next);
        }
        else if(t->next->next==NULL) t->next = NULL;
        else t->next = t->next->next;
        free(*tempHeap);
        *tempHeap = t;
      }
      else if(t->next!=NULL && t->next->proc==NULL){
        t = t->next;
        (*tempHeap)->size += t->size;
        (*tempHeap)->next = t->next;
        free(t);
        t = *tempHeap;
      }
      if(t->next!=NULL) t->next->prev = t;
      if(pageIter!=NULL) pageIter = pageIter->next;
    }while(pageIter!=NULL);
  }
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
  if(policy==PAG){
    int i, pagesRequired = (size / param);
    memP old = NULL, head = NULL;

    for(i=0; i<pagesRequired; i++){
      memP t = malloc(sizeof(memR));
      t->size = param;
      t->policy = policy;
      t->param = param;
      t->next = NULL;
      t->prev = old;
      t->proc = NULL;
      t->addr = 0;
      if(old!=NULL){
        old->next = t;
        t->addr = old->size + old->addr;
      }
      else head = t;
      old = t;
    }
    return head;
  }
  else{
    memP t = malloc(sizeof(memR));
    t->size = size;
    t->addr = 0;
    t->policy=policy;
    t->param= param;
    t->next = NULL;
    t->prev = NULL;
    t->proc = NULL;
    return t;
  }
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
  int pagesRequired = (proc->space->x / pageSize);
  if(proc->space->x%pageSize != 0) pagesRequired++;


  int pagesFound = 0;
  while((pagesFound < pagesRequired) && (t != NULL)){
    if(t->proc == NULL){
      pagesFound++;


      if(pagesFound == 1){
        tempPageIndex->memBlock = t;
        tempPageIndex->pNum = pagesFound;
      } else if(pagesFound > 1) {
        tempHeapP tempPage = malloc(sizeof(tempHeapR));
        tempPage->memBlock = t;
        tempPage->pNum = pagesFound;
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
    proc->blocks = tempPageHead;
    return tempPageHead;

  } else {
    while(tempPageIndex->next != NULL){
      tempPageIndex = tempPageIndex->next;
      free(tempPageIndex->prev);
    }

    free(tempPageIndex);

    return NULL;
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

    //While not enough segments have been found and there are more heap blocks to check 
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

        subdivideHeap(&localHeap, proc, requiredSize);
        //suppressing error. I'm guessing you don't need getFrontMem anymore
        localHeap = getFrontMem(localHeap);

        currentSpace = currentSpace->next;

        //If all the proc spaces have been found, everything is done
        if(currentSpace == NULL){
          allLocsFound = 1;
        }
      } else {
        //This needs to be an else only condition because if a block is stored and 
        //the heap gets subdivided, scanning for the next block needs to start
        //from the beginning 
        localHeap = localHeap->next;
      }
    }

    tempSegIndex = tempSegHead;
    if(allLocsFound == 1){
      while(tempSegIndex != NULL){
        tempSegIndex->memBlock->proc = proc;
        tempSegIndex = tempSegIndex->next;
      }

      proc->blocks = tempSegHead;

      return tempSegHead;
    } else {
      while(tempSegIndex->next != NULL){
        tempSegIndex = tempSegIndex->next;
        free(tempSegIndex->prev);
      }

      free(tempSegIndex);

      return NULL;
    }

  //Best fit
  } else if(param == 1){

    int blockFound;
    int smallestSize;
    memP smallestBlock;
    //For each space assigned to the process
    do {
      blockFound = 0;

      smallestSize = memSize + 1;
      smallestBlock = NULL;

      //Reset localHeap to the head of the heap
      localHeap = *heap;

      //Update the required size
      requiredSize = currentSpace->x;

      //For each heap block
      while(localHeap != NULL){
        heapSize = localHeap->size;

        //If the heap block is free, large enough, and the smallest currently seen
        if(localHeap->proc == NULL && heapSize >= requiredSize && heapSize < smallestSize){
          numSegsFound++;
          blockFound = 1;
            
          smallestSize = heapSize;
          smallestBlock = localHeap;
 
          
        }

        localHeap = localHeap->next;
      }

      if(blockFound == 1){

        subdivideHeap(smallestBlock, proc, smallestSize);     
 
        if(numSegsFound == 1){
          tempSegIndex->memBlock = smallestBlock;
        } else if(numSegsFound > 1){
          tempHeapP tempSeg = malloc(sizeof(tempHeapR));
          
          tempSeg->next = NULL;
          tempSeg->prev = tempSegIndex;
          tempSeg->memBlock = smallestBlock;

          tempSegIndex->next = tempSeg;
          tempSegIndex = tempSegIndex->next;
        }  
      } else {
        //This will occur if the process block could not be allocated.
        //De-allocate temp stuff, the proc cannot be loaded

        tempSegIndex = tempSegHead;


        //Loop through tempSeg blocks and free them
        while(tempSegIndex->next != NULL){

          tempSegIndex->memBlock->proc = NULL;

          tempSegIndex = tempSegIndex->next;
          free(tempSegIndex->prev);
        }

        tempSegIndex->memBlock->proc = NULL;       
        //Free the last one 
        free(tempSegIndex);

        cleanHeap(*heap);

        return NULL;
      }
      currentSpace = currentSpace->next;
    } while(currentSpace != NULL && blockFound == 1); 
   
    //All blocks could be allocated. Success!

    
  //Worst fit
  } else if(param == 2){

  }

  //If successfully found enough blocks
  if(currentSpace == NULL){

  } else {

  }

}

void cleanHeap(memP *heap){

  memP *temp;
  while(heap->next !=  NULL){

    if(heap->proc == NULL && heap->next->proc == NULL){

      temp = heap->next;        

      heap->size = heap->size + heap->next->size;
      heap->next = heap->next->next;
      heap->next->prev = heap;

      free(temp);
      temp = NULL;
    } else {
      heap = heap->next;
    }
  }

  heap = getFrontMem(heap);

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
    if(heap->proc!=NULL){
      printf("%d-%d: ",heap->addr,heap->addr+heap->size-1);
      printf("Process %d",heap->proc->pid);
    }
    if(heap->policy == VSP){
      if(heap->proc==NULL){
        printf("%d-%d: ",heap->addr,heap->addr+heap->size-1);
        printf("Hole");
      }
    }
    else if(heap->policy == PAG){
      if(heap->proc==NULL){
        int startMem = heap->addr;
        while(heap->next!=NULL&&heap->next->proc==NULL) heap = heap->next;
        printf("%d-%d: ",startMem,heap->addr+heap->size-1);
        printf("Free frame(s)");
      }
      else printf(", Page %d",heap->proc->blocks->pNum);
    }
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

memP getFrontMem(memP p){
  while(p->prev != NULL){
    p = p->prev;
  }
  return p;
}


spaceP getFrontSpace(spaceP p){
  while(p->prev != NULL){
    p = p->prev;
  }
  return p;
}
