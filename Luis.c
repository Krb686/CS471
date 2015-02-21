#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct process{
  int pid; int aTime; CPUburstP CPU; IOburstP IO;
  int status; struct process *next; struct process *prev;
}processR, *processP;

typedef struct CPUburst{
  int length; struct CPUburst *next;
}CPUburstR, *CPUburstP;

typedef struct IOburst{
  int length; int devId; struct IOburst *next;
}IOburstR, *IOburstP;

main(int argc, char* argv[]){
  if (argc != 2) {printf("Usage: %s <filename>\n",argv[0]);exit(2);}
  FILE *inFileP;
  char fName[32];
  char* line;
  char* seek;
  char* lines[100];
  int i=0,j=0,tQ1=0,tQ2=0,pidI=0;
  
  line = (char*)malloc(sizeof(char)*100);
  strcpy(fName,argv[1]);
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
        pid[pidI]=(processP)malloc(sizeof(processR));
        pid[pidI]->pid=atoi(line+12);
      }
      else if(strstr(line,"Arrival time: ")!=NULL){
        pid[pidI]->aTime=atoi(line+14);
        pidI++;
      }
      else if(strstr(line,"CPU burst: ")!=NULL){
        
        pid[--pidI]->=atoi(line+11);
      }
      /*if(i!= 0){
        lines[j] = (char*)malloc(sizeof(char)*100);
        strcpy(lines[j++],line);
      }*/
      i = 0;
    }
    else{
      line += sizeof(char);
      i++;
    }
  }
  printf("tq1:%d,tq2:%d,pid1:%d,aT1:%d",tQ1,tQ2,pid[0],aTime[0]);
  free(line);
  fclose(inFileP);
//  int x = j;
//  for(j;j>0;j--){
//    printf("%s",lines[x-j]);
//    free(lines[x-j]);
//  }
}

processP insertP(processP p){
  dubLLP t=(dubLLP)malloc(sizeof(dubLLR));
  if(p!=NULL) p->next = t;
  t->prev = p;
  t->next = NULL;
  return t;
}
