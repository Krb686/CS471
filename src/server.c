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
#include "server.h"

int SELLER_PORT = 5372;
int BUYER_PORT  = 5373;
int backlog = 10;
static int *seller_on;

void main() {
  int selsockfd, buysockfd, newsockfd, clilen;
  int pid, ret, item_number, login=1;
  struct sockaddr_in sel_addr, buy_addr, comm_addr;
  struct timeval t;
  t.tv_usec = 10000;
  char data[99];
  char *pch, *p;
  int *TEST;

  TEST = mmap(NULL, sizeof *TEST, PROT_READ | PROT_WRITE,
              MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *TEST = 0;

  selsockfd = socket(AF_INET, SOCK_STREAM, 0);
  buysockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char *)&sel_addr, sizeof(sel_addr));
  bzero((char *)&buy_addr, sizeof(buy_addr));
  sel_addr.sin_family = AF_INET;
  sel_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  sel_addr.sin_port = htons(SELLER_PORT);
  buy_addr.sin_family = AF_INET;
  buy_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  buy_addr.sin_port = htons(BUYER_PORT);

  bind(selsockfd, (struct sockaddr *)&sel_addr, sizeof(sel_addr));
  bind(buysockfd, (struct sockaddr *)&buy_addr, sizeof(buy_addr));
  printf("TCP Server Initialized...\n");
  listen(selsockfd, 1);
  listen(buysockfd, backlog);
  clilen = sizeof(comm_addr);
  printf("Waiting for client connections...\n");

//  int flags;
//  if(-1 == (flags = fcntl(sockfd, F_GETFL, 0))) flags = 0;
//  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
  if((pid = fork()) == 0){
    newsockfd = accept(selsockfd, (struct sockaddr *)&comm_addr,
                (socklen_t *)&clilen);
    while(login){
      ret = (int)recv(newsockfd, data, sizeof(data), 0);
      if(ret>0){
	p = data;
	printf("Seller sends the command: %s",data);
	for(;*p;++p) *p = tolower(*p);
	pch = strtok(data, " ");
	if(strcmp(pch,"login")==0){
	  pch = strtok(NULL, " ");
	  login=clientLogin(pch);//returns 0 on success
	}
      }
    }
    while(1){
      ret = (int)recv(newsockfd, data, sizeof(data), 0);
      if(ret>0){
	p = data;
	printf("Seller sends the command: %s",data);
	for(;*p;++p) *p = tolower(*p);
	pch = strtok(data, " ");
	if(strcmp(pch,"list")==0){
	  listItems();
        }
	else if(strcmp(pch,"add")==0){
	  pch = strtok(NULL, " ");
	  item_number = atoi(pch);
	  pch = strtok(NULL, " ");
	  addItem(item_number, pch);
	}
	else if(strcmp(pch,"sell")==0){
	  pch = strtok(NULL, " ");
	  item_number = atoi(pch);
	  sellItem(item_number);
        }
	else invalidCommand();
      }
    }
  }
  while(1){
    newsockfd = accept(buysockfd, (struct sockaddr *)&comm_addr,
                (socklen_t *)&clilen);
    if((pid = fork()) != 0){
      close(newsockfd);
      continue;
    }
    else{
//      setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO,&t,
//                 sizeof(struct timeval));
      printf("TCP Client Connected...\n");
      (*TEST)++;
      printf("[%d]\n",*TEST);

      while(1){
        ret = (int)recv(newsockfd, data, sizeof(data), 0);
	if(ret>0){
          printf("data recvd:"); printf("%s",data); printf("\n");
          if(strcmp(data,"exit")==0){
            close(newsockfd);
            (*TEST)--;
            printf("[%d]\n",*TEST);
            printf("disconnected\n");
            exit(0);
          }
	}
      }
    }
  }
}

int clientLogin(){
  return 0;
}

void listItems(){

}

void addItem(int item_number, char *pch){

}

void sellItem(int item_number){

}

void invalidCommand(){

}
