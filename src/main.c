#include "main.h"
#include "parser.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  //Call the parser 
  if(argc!=2){printf("Usage: %s filename\n",argv[0]); exit(2);}
  ParserOutput *rObj = parser(argv[1]);
 
  //Instantiate queues
  Queue *RR_Q1 = malloc(sizeof(Queue));
  Queue *RR_Q2 = malloc(sizeof(Queue));
  Queue *SRTF_Q3 = malloc(sizeof(Queue));

  //Setup queues
  RR_Q1->tQ = rObj->tQ1;
  RR_Q2->tQ = rObj->tQ2;
  SRTF_Q3->tQ = 0;

  RR_Q1->head = rObj->PID0; 

  //printOutput(rObj); 
  printf("tQ1: %d\ntQ2: %d\n", rObj->tQ1, rObj->tQ2);
  printf("Device count: %d\n", rObj->deviceCount);
  printf("Process count: %d\n", rObj->procCount);


  //While processes still remain 
  int remaining = 0;
  while(remaining){
    
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
