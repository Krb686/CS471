#include "process.h"
#include "parser.h"
#include <stdlib.h>
#include <stdio.h>

void printP(processP p){
  if(p!=NULL){
    printf("PID:\t%d\naTime:\t%d\n",p->pid,p->aTime);
    printf("Life:\t%d\nSpace:\t%d\n\n",p->lifetime,p->space);
    printP(p->next);
  }
}
