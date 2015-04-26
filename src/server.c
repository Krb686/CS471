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
#include"server.h"

int SELLER_PORT = 5372;
int BUYER_PORT  = 5373;
int backlog = 10;

void main() {
  int selsockfd, buysockfd, newsockfd, clilen;
  int pid, ret, item_number, login=1;
  struct sockaddr_in sel_addr, buy_addr, comm_addr;
  struct timeval t;
  t.tv_usec = 10000;
  char data[99];
  char *pch, *p,j *name;
  char *names[6] = {"alice","bob","dave","pam","susan","tom"}

//  TEST = mmap(NULL, sizeof *TEST, PROT_READ | PROT_WRITE,
//              MAP_SHARED | MAP_ANONYMOUS, -1, 0);

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
  printf("Server Initialized...\n");
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
	  if(strcmp(pch,"seller")==0) login = 0;
	  sendResponse(newsockfd, SELLER_LOGIN, login);
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
	  ret = listItems();
	  sendResponse(newsockfd, LIST, ret)
        }
	else if(strcmp(pch,"add")==0){
	  pch = strtok(NULL, " ");
	  item_number = atoi(pch);
	  pch = strtok(NULL, " ");
	  ret = addItem(item_number, pch);
	  sendResponse(newsockfd, ADD, ret);
	}
	else if(strcmp(pch,"sell")==0){
	  pch = strtok(NULL, " ");
	  item_number = atoi(pch);
	  ret = sellItem(item_number);
	  sendResponse(newsockfd, SELL, ret);
        }
	else sendResponse(newsockfd, INVALID, 0);
      }
    }
  }
  while(1){
    newsockfd = accept(buysockfd, (struct sockaddr *)&comm_addr,
                (socklen_t *)&clilen);
    if((pid = fork()) != 0){
      close(newsockfd);
    }
    else{
//      setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO,&t,
//                 sizeof(struct timeval));
      printf("DEBUG2 Connection established\n");
      while(login){
        ret = (int)recv(newsockfd, data, sizeof(data), 0);
        if(ret>0){
          p = data;
          printf("Buyer sends the command: %s",pch,data);
          for(;*p;++p) *p = tolower(*p);
          pch = strtok(data, " ");
          if(strcmp(pch,"login")==0){
            pch = strtok(NULL, " ");
            login=clientLogin(pch);//returns 0 on success
	    sendResponse(newsockfd, BUYER_LOGIN, login);
	    name = pch;
          }
        }
      }
      while(1){
        ret = (int)recv(newsockfd, data, sizeof(data), 0);
	if(ret>0){
	  p = data;
	  printf("%s sends the command: %s",name,data);
	  for(;*p;++p) *p = tolower(*p);
	  pch = strtok(data, " ");
	  if(strcmp(pch,"list")==0){
	    ret = listItems();
	    sendResponse(newsockfd, LIST, ret)
          }
	  else if(strcmp(pch,"add")==0){
	    pch = strtok(NULL, " ");
	    item_number = atoi(pch);
	    pch = strtok(NULL, " ");
	    ret = addItem(item_number, pch);
	    sendResponse(newsockfd, ADD, ret);
	  }
	  else if(strcmp(pch,"sell")==0){
	    pch = strtok(NULL, " ");
	    item_number = atoi(pch);
	    ret = sellItem(item_number);
	    sendResponse(newsockfd, SELL, ret);
          }
	  else sendResponse(newsockfd, INVALID, 0);
	}
      }
    }
  }
}

int clientLogin(){
  return 0;
}

int listItems(){

}

int addItem(int item_number, char *pch){

}

int sellItem(int item_number){

}

int invalidCommand(){

}
