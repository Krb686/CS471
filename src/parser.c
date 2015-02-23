#include "parser.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//COMMENTS
// Alot of the information in here needs to be available in main
// such as device count, process count, etc
// We could either return a bunch of information by reference,
// or just return a nicely packaged struct.  I decided option 2.
// Compile and run the test cases and you'll see it still works.
// This way, we can return anything else we might need just by 
// adding it into the "ParserOutput" struct

ParserOutput* parser(char* fName){

  ParserOutput *rObj = malloc(sizeof(ParserOutput));

  FILE *inFileP;
  char* line;
  char* seek;
  char* lines[100];
  int i=0,j=0,pidI=0;
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
        rObj->tQ1=atoi(line+16);
      }
      else if(strstr(line,"Time Quantum 2: ")!=NULL){
        rObj->tQ2=atoi(line+16);
      }
      else if(strstr(line,"Process ID: ")!=NULL){
        pid0 = insertP(pid0,atoi(line+12));
        pid9 = lastP(pid0);
        CPU0 = NULL;
        IO0 = NULL;
        rObj->procCount++;
        rObj->PID0 = pid0;
      }
      else if(strstr(line,"Arrival time: ")!=NULL){
        pid9->aTime=atoi(line+14);
      }
      else if(strstr(line,"CPU burst: ")!=NULL){
        CPU0 = insertCPU(CPU0,atoi(line+11));
        pid9->CPU = CPU0;
        rObj->CPU0 = CPU0;
      }
      else if(strstr(line,"I/O burst: ")!=NULL){
        IO0 = insertIO(IO0,atoi(line+11));
        IO9 = lastIO(IO0);
        pid9->IO = IO0;
        rObj->IO0 = IO0;
      }
      else if(strstr(line,"I/O device id: ")!=NULL){
        IO9->devId=atoi(line+15);
        rObj->deviceCount++;
      }
      i = 0;
    }
    else{
      line += sizeof(char);
      i++;
    }
  }
  free(line);
  fclose(inFileP);

  return rObj;
  //return pid0;
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


