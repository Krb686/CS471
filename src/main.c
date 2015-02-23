#include "main.h"
#include "parser.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  //Call the parser 
  if(argc!=2){printf("Usage: %s filename\n",argv[0]); exit(2);}
  ParserOutput *rObj = parser(argv[1]);
 
  //Currently executing process
  processP *execProc;

  //Current time
  int currTime = 0;

  //Instantiate queues
  QueueP RR_Q1 = malloc(sizeof(QueueP));
  QueueP RR_Q2 = malloc(sizeof(QueueP));
  QueueP SRTF_Q3 = malloc(sizeof(QueueP));

  //Setup queues
  RR_Q1->tQ = rObj->tQ1;
  RR_Q2->tQ = rObj->tQ2;
  SRTF_Q3->tQ = 0;

  //printOutput(rObj); 
  printf("tQ1: %d\ntQ2: %d\n", rObj->tQ1, rObj->tQ2);
  printf("Device count: %d\n", rObj->deviceCount);
  printf("Process count: %d\n", rObj->procCount);


  //While processes still remain 
  int remaining = 0;
  while(remaining){

    //First, check arrivals
    processP tempProc = rObj->PID0;
    while(tempProc != NULL){
      if(tempProc->aTime = currTime){
        insertQ1(RR_Q1, tempProc);
      }
    } 
  }
}

void insertQ1(QueueP RR_Q1, processP tempProc){
  if(!RR_Q1->head){
    RR_Q1->head = tempProc;
  } else {
    RR_Q1->head->next = tempProc;
  }
}
/*
void printOutput(ParserObject rObj){

  printf("tQ1: %d\ntQ2: %d\n", rObj->tQ1, rObj->tQ2);


}
*/

//int count(processP){
//  
//}
