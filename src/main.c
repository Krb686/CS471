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
  heap = initMem(ret->memSize,ret->policy);

  while(!done){
    //traverse mem checking all processes for end of life
    sweepMem(clk,i)//mem_ptr should have processP field

    while(unQed!=NULL && unQed->aTime == clk){
      pop(&unQed,&inQ);//finished
      printStatus(ARRIVAL,inQ)//print formatted status
    }

    i = inQ;
    while(i!=NULL){
      //can prob make one func that calls memcheck first
      pass = memCheck(i,heap);//check if enough memory
      if(pass){
        inMem(i,&heap);//place in mem remove from inQ
        printStatus(ADMIT,inQ)//print formatted status
      }
      i = i->next;
    }

    clk++;
    done = 1;//temp. Can prob use (inQ==NULL && unQed==NULL) instead
  }
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
mem_ptr initMem(int size, int policy){

}
