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
  RR_Q1->qNum = 1;

  RR_Q2->tQ = rObj->tQ2;
  RR_Q2->head = NULL;
  RR_Q2->qNum = 2;

  SRTF_Q3->tQ = 0;
  SRTF_Q3->head = NULL;
  SRTF_Q3->qNum = 3;

  IO_Q1->tQ = 0;
  IO_Q1->head = NULL;
  IO_Q1->qNum = 4;

  IO_Q2->tQ = 0;
  IO_Q2->head = NULL;
  IO_Q2->qNum = 5;

  IO_Q3->tQ = 0;
  IO_Q3->head = NULL;
  IO_Q3->qNum = 6;

  IO_Q4->tQ = 0;
  IO_Q4->head = NULL;
  IO_Q4->qNum = 7;

  IO_Q5->tQ = 0;
  IO_Q5->head = NULL;
  IO_Q5->qNum = 8;

  //printOutput(rObj); 
  printf("tQ1: %d\ntQ2: %d\n", rObj->tQ1, rObj->tQ2);
  printf("Process count: %d\n", rObj->procCount);
  printP(rObj->PID0);

  int totalrTime = 0, totalwTime = 0;

  //While processes still remain 
  int remaining = rObj->procCount;
  while(remaining){
    if(nextProc != NULL && nextProc->aTime == clk){
      printf("\nTime = %d:\t",clk);
    }
    //First check arrivals
    checkArrivals(&RR_Q1, &nextProc, &clk);
    
    //check if execProc should be preempted
    if(execProc!=NULL){
      int runningTime = clk - execProc->aTime - execProc->wTime;
      if(execProc->status == RUNQ1){
        if(runningTime == execProc->CPU->length){//CPU burst complete
          execProc->rTime = runningTime;
          execProc->CPU = removeCPUburst(execProc->CPU);
          printf("\nTime = %d:\t",clk);
          printf("CPU burst of Process %d completes\n",execProc->pid);
          printf("\t\tProcess %d requests I/O",execProc->pid);
          printf(" on Device %d\n",execProc->IO->devId);
          printf("\t\tDevice %d Queue = ",execProc->IO->devId);
          execProc->status = WAIT;
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
        else if(clk - execProc->rTime - execProc->wTime - execProc->aTime == execProc->CPU->length){
          execProc->rTime += execProc->CPU->length;
          execProc->CPU = removeCPUburst(execProc->CPU);
          printf("\nTime = %d:\t",clk);
          printf("CPU burst of Process %d completes\n",execProc->pid);
          printf("\t\tProcess %d requests I/O",execProc->pid);
          printf(" on Device %d\n",execProc->IO->devId);
          printf("\t\tDevice %d Queue = ",execProc->IO->devId);
          execProc->status = WAIT;
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
        else if(clk - execProc->rTime - execProc->wTime - execProc->aTime == RR_Q1->tQ){//demote to q2
          execProc->rTime += RR_Q1->tQ;
          execProc->CPU->length -= RR_Q1->tQ;
          printf("\nTime = %d:\tProcess %d is preempted\n",clk,execProc->pid);
          printf("\t\tProcess %d is demoted to Ready Queue Q2\n",execProc->pid);
          insertQ(RR_Q2,execProc);
          execProc->status = READY;
          printf("\t\tReady Queue Q2 = ");
          printQueue(RR_Q2);//prints [Process x, Process x+1,...]\n
          execProc = NULL;
        }
        else if(runningTime == RR_Q1->tQ){//demote to q2
          execProc->rTime = runningTime;
          execProc->CPU->length -= RR_Q1->tQ;
          printf("\nTime = %d:\tProcess %d is preempted\n",clk,execProc->pid);
          printf("\t\tProcess %d is demoted to Ready Queue Q2\n",execProc->pid);
          insertQ(RR_Q2,execProc);
          execProc->status = READY;
          printf("\t\tReady Queue Q2 = ");
          printQueue(RR_Q2);//prints [Process x, Process x+1,...]\n
          execProc = NULL;
        }
      }
      else if(execProc->status == RUNQ2){
        if(runningTime == execProc->CPU->length + RR_Q1->tQ){//CPU burst complete
          execProc->rTime = runningTime;
          execProc->CPU = removeCPUburst(execProc->CPU);
          printf("\nTime = %d:\t",clk);
          printf("CPU burst of Process %d completes\n",execProc->pid);
          printf("\t\tProcess %d requests I/O",execProc->pid);
          printf(" on Device %d\n",execProc->IO->devId);
          printf("\t\tDevice %d Queue = ",execProc->IO->devId);
          execProc->status = WAIT;
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
        else if(clk - execProc->rTime - execProc->wTime - execProc->aTime == execProc->CPU->length){
          execProc->rTime += execProc->CPU->length;
          execProc->CPU = removeCPUburst(execProc->CPU);
          printf("\nTime = %d:\t",clk);
          printf("CPU burst of Process %d completes\n",execProc->pid);
          printf("\t\tProcess %d requests I/O",execProc->pid);
          printf(" on Device %d\n",execProc->IO->devId);
          printf("\t\tDevice %d Queue = ",execProc->IO->devId);
          execProc->status = WAIT;
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
        else if(clk - execProc->rTime - execProc->wTime - execProc->aTime == RR_Q2->tQ){//demote to q2
          execProc->rTime += RR_Q2->tQ;
          execProc->CPU->length -= RR_Q2->tQ;
          printf("\nTime = %d:\tProcess %d is preempted\n",clk,execProc->pid);
          printf("\t\tProcess %d is demoted to Ready Queue Q3\n",execProc->pid);
          insertQ(SRTF_Q3,execProc);
          execProc->status = READY;
          printf("\t\tReady Queue Q3 = ");
          printQueue(SRTF_Q3);//prints [Process x, Process x+1,...]\n
          execProc = NULL;
        }
        else if(runningTime == RR_Q1->tQ + RR_Q2->tQ){
          execProc->rTime = runningTime;
          execProc->CPU->length -= RR_Q2->tQ;
          printf("\nTime = %d:\tProcess %d is preempted\n",clk,execProc->pid);
          printf("\t\tProcess %d is demoted to Ready Queue Q3\n",execProc->pid);
          insertSRTF(SRTF_Q3,execProc);
          execProc->status = READY;
          printf("\t\tReady Queue Q3 = ");
          printQueue(SRTF_Q3);//prints [Process x, Process x+1,...]\n
          execProc = NULL;
        }
      }
      else if(execProc->status == RUNQ3 &&
         runningTime == execProc->CPU->length + RR_Q1->tQ + RR_Q2->tQ){
        execProc->rTime = runningTime;
        execProc->CPU = removeCPUburst(execProc->CPU);
        printf("\nTime = %d:\t",clk);
        printf("CPU burst of Process %d completes\n",execProc->pid);
        printf("\t\tProcess %d requests I/O",execProc->pid);
        printf(" on Device %d\n",execProc->IO->devId);
        printf("\t\tDevice %d Queue = ",execProc->IO->devId);
        execProc->status = WAIT;
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
    }
    //Place code to run IO devices
    if(IO_Q1->head!=NULL){
      processP IOp = IO_Q1->head;
      int IOtime = clk - IOp->wTime - IOp->rTime - IOp->aTime;
      if(IOtime == IOp->IO->length && IOp->status == IOEX){
        IOp->rTime += IOtime-IOp->aTime;
        IOp->IO = removeIOburst(IOp->IO);
        printf("\nTime = %d:\t",clk);
        printf("I/O burst of Process %d completes\n",IOp->pid);
        IOp = dispatch(IO_Q1);
        if(IOp->IO == NULL){
          printf("\t\tProcess %d completes\n",IOp->pid);
          IOp->status = DONE;
          totalrTime += IOp->rTime;
          totalwTime += IOp->wTime;
          free(IOp);
          remaining--;
          printf("\t\tDevice 1 Queue = "); printQueue(IO_Q1);
        }
        else{
          insertQ(RR_Q1,IOp);//***check if done
          IOp->status = READY;
          printf("\t\tDevice 1 Queue = "); printQueue(IO_Q1);
          printf("\t\tProcess %d requests CPU\n",IOp->pid);
          printf("\t\tReady Queue Q1 = "); printQueue(RR_Q1);
        }
        IOp = IO_Q1->head;
      }
      if(IOp != NULL && IOp->status==WAIT){
        IOp->status = IOEX;
        IOp->wTime = clk - IOp->aTime - IOp->rTime;
        printf("\t\tI/O Device 1 now in use by Process %d\n",IOp->pid);
      }
    }
    if(IO_Q2->head!=NULL){
      processP IOp = IO_Q2->head;
      int IOtime = clk - IOp->wTime - IOp->rTime - IOp->aTime;
      if(IOtime == IOp->IO->length && IOp->status == IOEX){
        IOp->rTime += IOtime-IOp->aTime;
        IOp->IO = removeIOburst(IOp->IO);
        printf("\nTime = %d:\t",clk);
        printf("I/O burst of Process %d completes\n",IOp->pid);
        IOp = dispatch(IO_Q2);
        if(IOp->IO == NULL){
          printf("\t\tProcess %d completes\n",IOp->pid);
          IOp->status = DONE;
          totalrTime += IOp->rTime;
          totalwTime += IOp->wTime;
          free(IOp);
          remaining--;
          printf("\t\tDevice 1 Queue = "); printQueue(IO_Q1);
        }
        else{
          insertQ(RR_Q1,IOp);//***check if done
          IOp->status = READY;
          printf("\t\tDevice 1 Queue = "); printQueue(IO_Q1);
          printf("\t\tProcess %d requests CPU\n",IOp->pid);
          printf("\t\tReady Queue Q1 = "); printQueue(RR_Q1);
        }
        IOp = IO_Q2->head;
      }
      if(IOp != NULL && IOp->status==WAIT){
        IOp->status = IOEX;
        IOp->wTime = clk - IOp->aTime - IOp->rTime;
        printf("\t\tI/O Device 2 now in use by Process %d\n",IOp->pid);
      }
    }
    if(IO_Q3->head!=NULL){

    }
    if(IO_Q4->head!=NULL){

    }
    if(IO_Q5->head!=NULL){

    }
    
    //Dispatch processes from their queues to the processor
    if(execProc == NULL){
      if(RR_Q1->head != NULL){
        dispatchProcsFromQueue(&execProc, &RR_Q1, &clk);
      } else if(RR_Q2 != NULL){
        dispatchProcsFromQueue(&execProc, &RR_Q2, &clk);
      } else if(SRTF_Q3 != NULL){
        dispatchProcsFromQueue(&execProc, &SRTF_Q3, &clk);
      }
    }
 
    clk++;
    if(clk==1000){printf("Timeout\n");remaining = 0;}//will delete
  }
  free(RR_Q1); free(RR_Q2); free(SRTF_Q3);
  free(IO_Q1); free(IO_Q2); free(IO_Q3); free(IO_Q4); free(IO_Q5);
  printf("\n\nAverage Waiting Time: %d\n",totalwTime/rObj->procCount);
  printf("Average Turnaround Time: %d\n",totalrTime/rObj->procCount);
}

void dispatchProcsFromQueue(processP *execProcP, QueueP *queueP, int *clkP){
  //Next dispatch process to running state
    if((*queueP)->head!=NULL){
      
      *execProcP = dispatch(*queueP);

      if((*queueP)->qNum = 1){
        (*execProcP)->status = RUNQ1;
      } else if((*queueP)->qNum = 2){
        (*execProcP)->status = RUNQ2;
      } else if((*queueP)->qNum = 3){
        (*execProcP)->status = RUNQ3;
      }

      (*execProcP)->wTime = *clkP - (*execProcP)->aTime - (*execProcP)->rTime;
      printf("\t\tDispatcher moves Process %d to Running State\n", (*execProcP)->pid);
      printf("\t\tReady Queue Q1 = ");
      printQueue(*queueP);
      printf("\t\tProcess %d starts running\n", (*execProcP)->pid);
    }
}

void checkArrivals(QueueP *RR_Q1P, processP *nextProcP, int *clkP){
  while(*nextProcP != NULL && (*nextProcP)->aTime == *clkP){
    printf("Process %d arrives\n\t\t", (*nextProcP)->pid);
    printf("Process %d requests CPU\n\t\t", (*nextProcP)->pid);
    
    insertQ(*RR_Q1P, *nextProcP);
    *nextProcP = (*nextProcP)->next;// nextProc = nP // nP->prev = P
      
    if(*nextProcP!=NULL){
      (*nextProcP)->prev->next = NULL;// q: ...->|P|->NULL
      (*nextProcP)->prev = NULL;// nP->prev = NULL
    }
      
    printf("Ready Queue Q1 = ");
    printQueue(*RR_Q1P);//prints [Process x, Process x+1,...]\n
      
    if(*nextProcP != NULL && (*nextProcP)->aTime == *clkP){
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

processP dispatch(QueueP q){//q->head should never be null
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
/*
void printOutput(ParserObject rObj){

  printf("tQ1: %d\ntQ2: %d\n", rObj->tQ1, rObj->tQ2);


}
*/

//int count(processP){
//  
//}
