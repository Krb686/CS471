#include "process.h"
#include <stdlib.h>
#include <stdio.h>

//Not used
void printP(processP p){
  if(p!=NULL){
    printf("PID:\t%d\naTime:\t%d\n",p->pid,p->aTime);
    printf("Life:\t%d\nSpace:",p->deadline);
    spaceP t = p->space;
    while(t!=NULL){
      printf("\t%d",t->x);
      t = t->next;
    }
    printf("\n\n");
    printP(p->next);
  }
}
