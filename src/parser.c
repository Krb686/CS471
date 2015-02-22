#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

processP parser(char* fName){
  FILE *inFileP;
  char* line;
  char* seek;
  char* lines[100];
  int i=0,j=0,tQ1=0,tQ2=0,pidI=0;
  processP pid0=NULL,pid9=NULL;
  CPUburstP CPU0=NULL,CPU9=NULL;
  IOburstP IO0=NULL,IO9=NULL;
  
  line = (char*)malloc(sizeof(char)*100);
  inFileP = fopen(fName, "r");
  while(fscanf(inFileP,"%c",line)!=EOF){
    if(*line=='\r') *line='\n';
    if(*line=='\n'){
      line++; *line = '\0'; line--;
      line -= sizeof(char)*i;
      if(strstr(line,"Time Quantum 1: ")!=NULL){
        tQ1=atoi(line+16);
      }
      else if(strstr(line,"Time Quantum 2: ")!=NULL){
        tQ2=atoi(line+16);
      }
      else if(strstr(line,"Process ID: ")!=NULL){
        pid0 = insertP(pid0,atoi(line+12));
        pid9 = lastP(pid0);
        CPU0 = NULL;
        IO0 = NULL;
      }
      else if(strstr(line,"Arrival time: ")!=NULL){
        pid9->aTime=atoi(line+14);
      }
      else if(strstr(line,"CPU burst: ")!=NULL){
        CPU0 = insertCPU(CPU0,atoi(line+11));
        pid9->CPU = CPU0;
      }
      else if(strstr(line,"I/O burst: ")!=NULL){
        IO0 = insertIO(IO0,atoi(line+11));
        IO9 = lastIO(IO0);
        pid9->IO = IO0;
      }
      else if(strstr(line,"I/O device id: ")!=NULL){
        IO9->devId=atoi(line+15);
      }
      i = 0;
    }
    else{
      line += sizeof(char);
      i++;
    }
  }
  printf("tq1:%d,tq2:%d\n\n",tQ1,tQ2);
  free(line);
  fclose(inFileP);
  return pid0;
}

processP insertP(processP p, int pid){
  processP t=(processP)malloc(sizeof(processR));
  processP end = lastP(p);
  t->prev = end;
  t->pid = pid;
  t->next = NULL;
  if(end!=NULL){
    end->next = t;
    return p;
  }
  return t;
}

CPUburstP insertCPU(CPUburstP p, int l){
  CPUburstP t=(CPUburstP)malloc(sizeof(CPUburstR));
  CPUburstP end = lastCPU(p);
  t->length = l;
  t->next = NULL;
  if(end!=NULL){
    end->next = t;
    return p;
  }
  return t;
}

IOburstP insertIO(IOburstP p, int l){
  IOburstP t=(IOburstP)malloc(sizeof(IOburstR));
  IOburstP end = lastIO(p);
  t->length = l;
  t->next = NULL;
  if(end!=NULL){
    end->next = t;
    return p;
  }
  return t;
}

processP lastP(processP p){
  if(p==NULL||p->next==NULL) return p;
  lastP(p->next);
}

CPUburstP lastCPU(CPUburstP p){
  if(p==NULL||p->next==NULL) return p;
  lastCPU(p->next);
}

IOburstP lastIO(IOburstP p){
  if(p==NULL||p->next==NULL) return p;
  lastIO(p->next);
}

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
 }
}
