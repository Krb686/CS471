#include "parser.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  //Create time quantum variables to be returned from the parser
  int *tQ1 = malloc(sizeof(int));
  int *tQ2 = malloc(sizeof(int));

  //Call the parser 
  if(argc!=2){printf("Usage: %s filename\n",argv[0]); exit(2);}
  processP test = parser(argv[1], tQ1, tQ2);
 
  //Instantiate queues
  Queue *RRq1 = malloc(sizeof(Queue));
  Queue *RRq2 = malloc(sizeof(Queue));
  Queue *SRTFq = malloc(sizeof(Queue));

  //Setup queues
  RRq1->tQ = *tQ1;
  RRq1->head = test;

  RRq2->tQ = *tQ2;

  SRTFq->tQ = 0;  

  //While processes still remain 
  int remaining = 0;
  while(remaining){
    
  }
}

//int count(processP){
//  
//}
