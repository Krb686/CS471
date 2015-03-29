#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/////COMMENTS

ParserOutputP parser(char* fName){

  ParserOutputP ret = (ParserOutputP)malloc(sizeof(ParserOutputR));
  ret->count = 0;
  FILE *inFileP;
  char* line;
  char* tmp1;
  char* tmp2;
  char* lines[100];
  int i=0,j=0;
  intptr_t x=0;
  processP pid0=NULL,pid9=NULL;
  spaceP space0=NULL;
  
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
        space0 = NULL;
        ret->count++;
        ret->PID0 = pid0;
      }
      else if(strstr(line,"Arrival Time: ")!=NULL){
        pid9->aTime=atoi(line+14);
      }
      else if(strstr(line,"Lifetime in Memory: ")!=NULL){
        pid9->deadline=atoi(line+20);
      }
      else if(strstr(line,"Address Space: ")!=NULL){
        if(ret->policy==2){
          tmp2 = line+15;
          tmp1 = strchr(line+15,' ');
          while(tmp1!=NULL){
            *tmp1 = '\0'; tmp1++;
            space0 = insertSpace(space0,atoi(tmp2));
            tmp2 = tmp1;
            tmp1 = strchr(tmp1,' ');
          }
          tmp1 = strchr(tmp2,'\n');
          *tmp1='\0';
          space0 = insertSpace(space0,atoi(tmp2));
        }
        else{
          space0 = insertSpace(space0,atoi(line+15));
        }
        pid9->space = space0;
      }
      i = 0;
    }
    else{
      line += sizeof(char);
      i++;
    }
  }
  if(i!=0){
    if(ret->policy==2){
      tmp2 = line+15;
      tmp1 = strchr(line+15,' ');
      while(tmp1!=NULL){
        *tmp1 = '\0'; tmp1++;
        space0 = insertSpace(space0,atoi(tmp2));
        tmp2 = tmp1;
        tmp1 = strchr(tmp1,' ');
      }
      tmp1 = strchr(tmp2,EOF);
      *tmp1='\0';
      space0 = insertSpace(space0,atoi(tmp2));
    }
    else{
      space0 = insertSpace(space0,atoi(line+15));
    }
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

spaceP insertSpace(spaceP p, int x){
  if(x==0) return p;
  spaceP t=(spaceP)malloc(sizeof(spaceR));
  spaceP end = lastSpace(p);
  t->x = x;
  t->next = NULL;
  if(end!=NULL){
    end->next = t;
    t->prev = end;
    return p;
  }
  t->prev = NULL;
  return t;
}

processP lastP(processP p){
  if(p==NULL||p->next==NULL) return p;
  lastP(p->next);
}

spaceP lastSpace(spaceP p){
  if(p==NULL||p->next==NULL) return p;
  lastSpace(p->next);
}
