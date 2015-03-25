#include "parser.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/////COMMENTS
///// Alot of the information in here needs to be available in main
///// such as device count, process count, etc
///// We could either return a bunch of information by reference,
///// or just return a nicely packaged struct.  I decided option 2.
///// Compile and run the test cases and you'll see it still works.
///// This way, we can return anything else we might need just by 
///// adding it into the "ParserOutput" struct

ParserOutputP parser(char* fName){

  ParserOutputP ret = (ParserOutputP)malloc(sizeof(ParserOutputR));
  ret->count = 0;
  FILE *inFileP;
  char* line;
  char* lines[100];
  int i=0,j=0;
  processP pid0=NULL,pid9=NULL;
  
  line = (char*)malloc(sizeof(char)*100);
  inFileP = fopen(fName, "r");
  while(fscanf(inFileP,"%c",line)!=EOF){
    if(*line=='\r') *line='\n';
    if(*line=='\n'){
      line++; *line = '\0'; line--;
      line -= sizeof(char)*i;
      if(strstr(line,"Memory Size: ")!=NULL){
        ret->memSize=atoi(line+13);
      }
      else if(strstr(line,"Memory Management Policy: ")!=NULL){
        if(strstr(line+26,"VSP")!=NULL){
          ret->policy=0;
        }
        else if(strstr(line+26,"PAG")!=NULL){
          ret->policy=1;
        }
        if(strstr(line+26,"SEG")!=NULL){
          ret->policy=2;
        }
      }
      else if(strstr(line,"Policy Parameter: ")!=NULL){
        ret->param=atoi(line+18);
      }
      else if(strstr(line,"Process Id: ")!=NULL){
        pid0 = insertP(pid0,atoi(line+12));
        pid9 = lastP(pid0);
        ret->count++;
        ret->PID0 = pid0;
      }
      else if(strstr(line,"Arrival time: ")!=NULL){
        pid9->aTime=atoi(line+14);
      }
      else if(strstr(line,"Lifetime in Memory: ")!=NULL){
        pid9->lifetime=atoi(line+20);
      }
      else if(strstr(line,"Address Space: ")!=NULL){
        pid9->space=atoi(line+15);
      }
      i = 0;
    }
    else{
      line += sizeof(char);
      i++;
    }
  }
  if(i!=0){
    line++; *line = '\0'; line--;
    line -= sizeof(char)*i;
    if(strstr(line,"Address Space: ")!=NULL){
      pid9->space=atoi(line+15);
    }
    i = 0;
  }
  free(line);
  fclose(inFileP);

  return ret;
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

processP lastP(processP p){
  if(p==NULL||p->next==NULL) return p;
  lastP(p->next);
}
