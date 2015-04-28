#include<errno.h>
#include<ctype.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/mman.h>
#include<netinet/in.h>
#include<time.h>
#include "seller.h"

#define MAX_INPUT_SIZE 40

int SELLER_PORT = 5372;
char AUCTIONSERVER_IP_ADDRESS[20] = "127.0.0.1";

void main(){
  int sockfd, ret, pid, login=1, logout=0, item_number;
  struct sockaddr_in my_addr, comm_addr;
  char command[MAX_INPUT_SIZE];
  char response[32];
  int bytesSent = 0;

  char data[MAX_INPUT_SIZE];
  char responseStr[32];
  char *pch, *p;
  struct timeval t;

  t.tv_usec = 100000;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero((char *)&my_addr, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  my_addr.sin_port = htons(0);
  bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));

  bzero((char *)&comm_addr, sizeof(comm_addr));
  comm_addr.sin_family = AF_INET;
  comm_addr.sin_addr.s_addr = inet_addr(AUCTIONSERVER_IP_ADDRESS);
  comm_addr.sin_port = htons(SELLER_PORT);

/*  ret = (int)connect(sockfd, (struct sockaddr *)&comm_addr,
        sizeof(comm_addr));
  if(ret < 0){
    printf("%d_err:%s\n",ret,strerror(errno));
    exit(0);
  }*/
  printf("Seller Initialized\n\n");
  while(login){
    printf("Please enter 'login <username>'\n>>");
    fgets(command, MAX_INPUT_SIZE, stdin);
    ret = (int)connect(sockfd, (struct sockaddr *)&comm_addr, sizeof(comm_addr));
    if(ret < 0){
      printf("%d_err:%s\n",ret,strerror(errno));
      exit(0);
    }
    else{
      printf("Seller connected successfully!\n");
    }
    bytesSent = send(sockfd, command, sizeof(char)*MAX_INPUT_SIZE, 0);
    ret = (int)recv(sockfd, response, sizeof(char)*MAX_INPUT_SIZE, 0);
    if(ret < 0){
      printf("%d_err:%s\n", ret, strerror(errno));
      exit(0);
    }
    printf("%s\n",response);
    if(strcmp(response, "1")==0){
      login = 0;
    }
    else{
      printf("The server rejected the login request. Please ensure the usernmae is correct\n");
    }
  }
  if((pid = fork()) != 0){
    while(1){
      ret = (int)recv(sockfd, data, sizeof(data), 0);
      if(ret>0){
        printf("<%s>\n",data);
      }
    }
  }
  else{
    printf("\t\t~~~~MENU~~~~\n1) List\n2) Add <Item Number> <Item Name>\n3) Sell <Item Number>\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    while(!logout){
      printf(">>");
      scanf("%s",data);
      p = data;
      for(;*p;++p) *p = tolower(*p);
      pch = strtok(data, " ");
      if(strcmp(pch,"list")==0){
	list();
      }
      else if(strcmp(pch,"add")==0){
	pch = strtok(NULL, " ");
	item_number = atoi(pch);
	pch = strtok(NULL, " ");
	add(item_number,pch);
      }
      else if(strcmp(pch,"sell")==0){
	pch = strtok(NULL, " ");
	item_number = atoi(pch);
	sell(item_number);
      }
      else{
        printf("ERR!\nInvalid command!");
        printf("\t\t~~~~MENU~~~~\n1) List\n2) Add <Item Number> <Item Name>\n3) Sell <Item Number>\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
      }
    }
  }
}


void list(){


}


void add(){


}


void sell(){


}



