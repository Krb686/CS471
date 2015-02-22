#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(int argc, char* argv[]){
  if (argc != 2) {printf("Usage: %s <filename>\n",argv[0]);exit(2);}
  FILE *inFileP;
  char fName[32];
  char* line;
  char* lines[100];
  int i=0, j=0;
  
  line = (char*)malloc(sizeof(char)*100);
  strcpy(fName,argv[1]);
  inFileP = fopen(fName, "r");
  while(fscanf(inFileP,"%c",line)!=EOF){
    if(*line=='\r') *line='\n';
    if(*line=='\n'){
      line++; *line = '\0'; line--;
      line -= sizeof(char)*i;
      if(i!= 0){
        lines[j] = (char*)malloc(sizeof(char)*100);
        strcpy(lines[j++],line);
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
  int x = j;
  for(j;j>0;j--){
    printf("%s",lines[x-j]);
    free(lines[x-j]);
  }
}
