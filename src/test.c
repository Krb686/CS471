#include<errno.h>
//#include<unistd.h>
//#include<sys/types.h>
//#include<sys/wait.h>
//#include<strings.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/mman.h>
#include<netinet/in.h>
#include<time.h>

void main() {
  char *names[9] = {"seller","alice","bob","dave","pam","susan","tom"};
  int i = 0;
  while(i<7){ printf("%d:%s\n",i+1,names[i]); i++;}
}
