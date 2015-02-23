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
  Queue *Q1 = malloc(sizeof(Queue));
  Queue *Q2 = malloc(sizeof(Queue));
  Queue *Q3 = malloc(sizeof(Queue));

  //Setup queues
  Q1->tQ = *tQ1;
  Q2->tQ = *tQ2;
  Q3->tQ = 0;

  Q1->head = test;  

  //While processes still remain 
  int remaining = 0;
  while(remaining){
    
  }
}


//int count(processP){
//  
//}
