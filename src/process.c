#include "process.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

void printP(processP p){
  if(p!=NULL){
    printf("PID:\t\t%d\nArrival Time:\t%d\n",p->pid,p->aTime);
    CPUburstP t1 = p->CPU;
    int i=0;
    while(t1!=NULL){
      printf("CPU burst%d:\t%d\n",++i,t1->length);
      t1 = t1->next;
    }
    IOburstP t2 = p->IO;
    while(t2!=NULL){
      printf("IO dev%d:\t%d\n",t2->devId,t2->length);
      t2 = t2->next;
    }
    printf("\n");
    printP(p->next);
 } else {
   printf("\nERROR\n");
 }
}
