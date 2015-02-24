#include "main.h"
#include "parser.h"
#include "queue.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

  //Call the parser 
  if(argc!=2){printf("Usage: %s filename\n",argv[0]); exit(2);}
  ParserOutputP rObj = parser(argv[1]);
  processP i = rObj->PID0;
  while(i!=NULL){
    i->rTime = 0; i->wTime = 0;
    i = i->next;
  }
 
  //Currently executing process
  processP execProc = NULL;

  //Link to next un-queued process
  processP nextProc = rObj->PID0;

  //Current time
  int clk = 0;

  //Instantiate queues
  QueueP RR_Q1 = malloc(sizeof(QueueR));
  QueueP RR_Q2 = malloc(sizeof(QueueR));
  QueueP SRTF_Q3 = malloc(sizeof(QueueR));

  //Setup queues
  RR_Q1->tQ = rObj->tQ1;
  RR_Q1->head = NULL;
  RR_Q2->tQ = rObj->tQ2;
  RR_Q2->head = NULL;
  SRTF_Q3->tQ = 0;
  SRTF_Q3->head = NULL;

  //printOutput(rObj); 
  printf("tQ1: %d\ntQ2: %d\n", rObj->tQ1, rObj->tQ2);
  printf("Process count: %d\n", rObj->procCount);
  printP(rObj->PID0);


  //While processes still remain 
  int remaining = 3;//rObj->procCount;
  while(remaining){
    if(nextProc != NULL && nextProc->aTime == clk){
      printf("\nTime = %d:\t",clk);
    }
    //First, check arrivals
    while(nextProc != NULL && nextProc->aTime == clk){
      printf("Process %d arrives\n\t\t",nextProc->pid);
      printf("Process %d requests CPU\n\t\t",nextProc->pid);
      insertRR(RR_Q1, nextProc);
      nextProc = nextProc->next;// nextProc = nP // nP->prev = P
      if(nextProc!=NULL){
        nextProc->prev->next = NULL;// q: ...->|P|->NULL
        nextProc->prev = NULL;// nP->prev = NULL
      }
      printf("Ready Queue Q1 = ");
      printQueue(RR_Q1);//prints [Process x, Process x+1,...]\n
      if(nextProc != NULL && nextProc->aTime == clk){
        printf("\t\t");
      }
      remaining--;//will delete
    }
    //check if execProc should be preempted
    if(execProc!=NULL &&
       clk - execProc->aTime - execProc->wTime == RR_Q1->tQ){
      execProc->rTime = RR_Q1->tQ;
      printf("\nTime = %d:\tProcess %d is preempted\n",clk,execProc->pid);
      printf("\t\tProcess %d is demoted to Ready Queue Q2\n",execProc->pid);
      insertRR(RR_Q2,execProc);
      printf("\t\tReady Queue Q2 = ");
      printQueue(RR_Q2);//prints [Process x, Process x+1,...]\n
      execProc = NULL;
    }
    if(clk!=0 && clk % RR_Q2->tQ == 0){
      printf("bump3");
    }
    //Next dispatch process to running state
    if(execProc==NULL){
      if(RR_Q1->head!=NULL){
        execProc = dispatch(RR_Q1);
        execProc->wTime = clk - execProc->aTime - execProc->rTime;
        printf("\t\tDispatcher moves Process %d to Running State\n",execProc->pid);
        printf("\t\tReady Queue Q1 = ");
        printQueue(RR_Q1);
        printf("\t\tProcess %d starts running\n",execProc->pid);
      }
      else if(RR_Q2->head!=NULL){
        printf("|");
      }
      else if(SRTF_Q3->head!=NULL){
        printf("bump2");
      }
    }
    clk++;
    if(clk==500) remaining--;//will delete
  }
}

void insertRR(QueueP RR_Q, processP p){
  if(RR_Q->head==NULL){
    RR_Q->head = p;// q: |P|->|nP|->... // nextProc = P
  }
  else{ // q: ...->|P|->... // nextProc = P
    processP qTail = lastP(RR_Q->head);// last = lP // q: ...->|lP|->NULL
    qTail->next = p;// q: ...->|lP|->|P|->|nP|->... // nextProc = P
    p->prev = qTail;
  }
}

void printQueue(QueueP RR_Q){
  processP i = RR_Q->head;
  printf("[");
  while(i!=NULL){
    printf("Process %d",i->pid);
    if(i->next!=NULL){
      printf(", ");
    }
    i = i->next;
  }
  printf("]\n");
}

processP dispatch(QueueP q){//q->head should never be null
  processP t = q->head;//t->prev should always be null
  if(t->next!=NULL){
    t->next->prev = NULL;//|qh|<->|q2| to |qh|->|q2|
  }
  q->head = q->head->next;//q2=newqh
  t->next = NULL;//oldqh->next != newqh
  t->status = RUN;
  return t;
}

demote(QueueP nextQ, processP p){
  nextQ->head = p;
}
/*
void printOutput(ParserObject rObj){

  printf("tQ1: %d\ntQ2: %d\n", rObj->tQ1, rObj->tQ2);


}
*/

//int count(processP){
//  
//}
