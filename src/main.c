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
  QueueP IO_Q1 = malloc(sizeof(QueueR));
  QueueP IO_Q2 = malloc(sizeof(QueueR));
  QueueP IO_Q3 = malloc(sizeof(QueueR));
  QueueP IO_Q4 = malloc(sizeof(QueueR));
  QueueP IO_Q5 = malloc(sizeof(QueueR));

  //Setup queues
  RR_Q1->tQ = rObj->tQ1;
  RR_Q1->head = NULL;
  RR_Q1->status = RUNQ1;

  RR_Q2->tQ = rObj->tQ2;
  RR_Q2->head = NULL;
  RR_Q2->status = RUNQ2;

  SRTF_Q3->tQ = 0;
  SRTF_Q3->head = NULL;
  SRTF_Q3->status = RUNQ3;

  IO_Q1->head = NULL;
  IO_Q2->head = NULL;
  IO_Q3->head = NULL;
  IO_Q4->head = NULL;
  IO_Q5->head = NULL;

  //printOutput(rObj); 
  printf("tQ1: %d\ntQ2: %d\n", rObj->tQ1, rObj->tQ2);
  printf("Process count: %d\n", rObj->procCount);
  printP(rObj->PID0);

  int waitTime = 0, turnTime=0;

  //While processes still remain 
  int remaining = rObj->procCount;
  while(remaining){
    if(nextProc != NULL && nextProc->aTime == clk){
      printf("\nTime = %d:\t",clk);
    }
    //First check arrivals
    checkArrivals(&RR_Q1, &nextProc, clk);
    
    //check if execProc should be preempted
    if(execProc!=NULL){
      int CPUrun = clk-execProc->rTime-execProc->wTime-execProc->aTime;
      if(CPUrun == execProc->CPU->length){
        CPUcomplete(&execProc, CPUrun, clk);
        switch(execProc->IO->devId){
          case 1: insertQ(IO_Q1, execProc); printQueue(IO_Q1);  break;
          case 2: insertQ(IO_Q2, execProc); printQueue(IO_Q2);  break;
          case 3: insertQ(IO_Q3, execProc); printQueue(IO_Q3);  break;
          case 4: insertQ(IO_Q4, execProc); printQueue(IO_Q4);  break;
          case 5: insertQ(IO_Q5, execProc); printQueue(IO_Q5);  break;
          default: printf("invalid devId!\n");
        }
        execProc = NULL;
      }
      else if(execProc->status == RUNQ1 && CPUrun == RR_Q1->tQ){//demote to q2
        demote(&execProc,&RR_Q1,&RR_Q2,clk);
        execProc = NULL;
      }
      else if(execProc->status == RUNQ2 && CPUrun == RR_Q2->tQ){//demote to q3
        demote(&execProc,&RR_Q2,&SRTF_Q3,clk);
        execProc = NULL;
      }
    }


    //Handle IO
    if(IO_Q1->head!=NULL){
        processIO(&IO_Q1, &RR_Q1,  &turnTime, &remaining, clk);
    }    

    if(IO_Q2->head!=NULL){
        processIO(&IO_Q2, &RR_Q1,  &turnTime, &remaining, clk);
    }
    
    if(IO_Q3->head!=NULL){
        processIO(&IO_Q3, &RR_Q1,  &turnTime, &remaining, clk);
    }

    if(IO_Q4->head!=NULL){
        processIO(&IO_Q4, &RR_Q1,  &turnTime, &remaining, clk);
    }

    if(IO_Q5->head!=NULL){
        processIO(&IO_Q5, &RR_Q1,  &turnTime, &remaining, clk);
    }
                    
    
    //Dispatch processes from their queues to the processor
    if(execProc == NULL){
      if(RR_Q1->head != NULL){
        dispatch(&execProc, &RR_Q1, &waitTime, clk);
      } else if(RR_Q2->head != NULL){
        dispatch(&execProc, &RR_Q2, &waitTime, clk);
      } else if(SRTF_Q3->head != NULL){
        dispatch(&execProc, &SRTF_Q3, &waitTime, clk);
      }
    }
    clk++;
  }
  free(RR_Q1); free(RR_Q2); free(SRTF_Q3);
  free(IO_Q1); free(IO_Q2); free(IO_Q3); free(IO_Q4); free(IO_Q5);
  printf("\n\nAverage Waiting Time: %d\n",waitTime/rObj->procCount);
  printf("Average Turnaround Time: %d\n",turnTime/rObj->procCount);
  free(rObj);
}

void CPUcomplete(processP *execProc, int CPUrun, int clk){
  (*execProc)->rTime += CPUrun;
  (*execProc)->CPU = removeCPUburst((*execProc)->CPU);
  printf("\nTime = %d:\t",clk);
  printf("CPU burst of Process %d completes\n",(*execProc)->pid);
  printf("\t\tProcess %d requests I/O",(*execProc)->pid);
  printf(" on Device %d\n",(*execProc)->IO->devId);
  printf("\t\tDevice %d Queue = ",(*execProc)->IO->devId);
  (*execProc)->status = WAIT;
}

void demote(processP *execProc, QueueP *curQ, QueueP *destQ,int clk){
  (*execProc)->rTime += (*curQ)->tQ;
  (*execProc)->CPU->length -= (*curQ)->tQ;
  printf("\nTime = %d:\tProcess %d is preempted\n",clk,(*execProc)->pid);
  printf("\t\tProcess %d is demoted to ",(*execProc)->pid); 
  printf("Ready Queue Q%d\n",(*destQ)->status);
  insertQ(*destQ,(*execProc));
  (*execProc)->status = READY;
  printf("\t\tReady Queue Q%d = ",(*destQ)->status);
  printQueue(*destQ);//prints [Process x, Process x+1,...]\n
}

void dispatch(processP *execProcP, QueueP *queueP,int *waitTime, int clk){
  //Next popQ process to running state
  *execProcP = popQ(*queueP);
  (*execProcP)->status = (*queueP)->status;
  *waitTime += clk-(*execProcP)->aTime-(*execProcP)->rTime-(*execProcP)->wTime;
  (*execProcP)->wTime += clk - (*execProcP)->aTime -
                         (*execProcP)->rTime - (*execProcP)->wTime;
  printf("\t\tDispatcher moves Process %d to Running State\n", (*execProcP)->pid);
  printf("\t\tReady Queue Q%d = ",(*queueP)->status);
  printQueue(*queueP);
  printf("\t\tProcess %d starts running\n", (*execProcP)->pid);
}

void checkArrivals(QueueP *RR_Q1P, processP *nextProcP, int clk){
  while(*nextProcP != NULL && (*nextProcP)->aTime == clk){
    printf("Process %d arrives\n\t\t", (*nextProcP)->pid);
    printf("Process %d requests CPU\n\t\t", (*nextProcP)->pid);
    (*nextProcP)->status = READY;
    insertQ(*RR_Q1P, *nextProcP);
    *nextProcP = (*nextProcP)->next;// nextProc = nP // nP->prev = P
      
    if(*nextProcP!=NULL){
      (*nextProcP)->prev->next = NULL;// q: ...->|P|->NULL
      (*nextProcP)->prev = NULL;// nP->prev = NULL
    }
      
    printf("Ready Queue Q1 = ");
    printQueue(*RR_Q1P);//prints [Process x, Process x+1,...]\n
      
    if(*nextProcP != NULL && (*nextProcP)->aTime == clk){
      printf("\t\t");
    }
  }
}

void insertQ(QueueP RR_Q, processP p){
  if(RR_Q->head==NULL){
    RR_Q->head = p;// q: |P|->|nP|->... // nextProc = P
  }
  else{ // q: ...->|P|->... // nextProc = P
    processP qTail = lastP(RR_Q->head);// last = lP // q: ...->|lP|->NULL
    qTail->next = p;// q: ...->|lP|->|P|->|nP|->... // nextProc = P
    p->prev = qTail;
  }
}

void insertSRTF(QueueP SRTF_Q, processP p){
  if(SRTF_Q->head==NULL){
    SRTF_Q->head = p;
  }
  else{
    processP i = lastP(SRTF_Q->head);
    while(i!=NULL && p->CPU->length < i->CPU->length){
      i = i->prev;
    }
    if(i==NULL){
      p->next = SRTF_Q->head;
      SRTF_Q->head = p;
      p->next->prev = p;
    }
    else{
      p->next = i->next;
      if(i->next!=NULL){
        i->next->prev = p;
      }
      i->next = p;
      p->prev = i;
    }
  }
}

CPUburstP removeCPUburst(CPUburstP CPU){
  CPUburstP t = CPU->next;
  free(CPU);
  return t;
}

IOburstP removeIOburst(IOburstP IO){
  IOburstP t = IO->next;
  free(IO);
  return t;
}

processP popQ(QueueP q){//q->head should never be null
  processP t = q->head;//t->prev should always be null
  if(t->next!=NULL){
    t->next->prev = NULL;//|qh|<->|q2| to |qh|->|q2|
  }
  q->head = q->head->next;//q2=newqh
  t->next = NULL;//oldqh->next != newqh
  return t;
}

void printQueue(QueueP RR_Q){
  processP i = RR_Q->head;
  printf("[");
  while(i!=NULL){
    printf("%d: rT=%d wT=%d s=%d",i->pid,i->rTime,i->wTime,i->status);
    if(i->next!=NULL){
      printf(" | ");
    }
    i = i->next;
  }
  printf("]\n");
}

void processIO(QueueP *queueP, QueueP *RR_Q1, int *turnTime, int *remaining, int clk){

    //Save the devId
    int devId = (*queueP)->head->IO->devId;

    
    processP head = (*queueP)->head;
    int IOtime = clk - head->wTime - head->rTime - head->aTime;

    //If the IOburst is done
    if(IOtime == head->IO->length && head->status == IOEX){
        head->rTime += IOtime;

        

        //Pop the IOburst    
        head->IO = removeIOburst(head->IO);

        printf("\nTime = %d:\t",clk);
        printf("I/O burst of Process %d completes\n",head->pid);

        //Pop the proc from the IO queue
        head = popQ(*queueP);

        //If there are no more IObursts
        if(head->IO == NULL){
            printf("\t\tProcess %d completes\n",head->pid);
            head->status = DONE;
            (*turnTime) += clk-head->aTime;
            printf("\t\tDevice %d Queue = ", devId); printQueue(*queueP);
            free(head);
            (*remaining)--;
        //Otherwise, re-add to queue 1
        } else {
            insertQ(*RR_Q1, head);
            head->status = READY;
            printf("\t\tDevice %d Queue = ", devId); printQueue(*queueP);
            printf("\t\tProcess %d requests CPU\n",head->pid);
            printf("\t\tReady Queue Q1 = "); printQueue(*RR_Q1);
        }
        
        head = (*queueP)->head;
    }

    //If the next proc has an IOburst, set it to execute
    if(head != NULL && head->status==WAIT){
        head->status = IOEX;
        head->wTime += clk - head->aTime - head->rTime - head->wTime;
        printf("\t\tI/O Device %d now in use by Process %d\n",devId, head->pid);
    }
}
