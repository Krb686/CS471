#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/mman.h>
#include <netinet/in.h>
#include <time.h>
#include "buyer.h"

#define MAX_INPUT_SIZE 512


int BUYER_PORT = 5373;
char AUCTIONSERVER_IP_ADDRESS[20] = "127.0.0.1";

void main(){
  int sockfd, ret, pid, login=1, logout=0, item_number;
  struct sockaddr_in my_addr, comm_addr;
  char username[16];
  char command[MAX_INPUT_SIZE];
  char response[32] = "";
  int bytesSent = 0;

  char data[MAX_INPUT_SIZE];
  char responseStr[32];
  char *pch, *p;
  struct timeval t;

  int bidNum;
  int bidAmt;

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
  comm_addr.sin_port = htons(BUYER_PORT);

  //Send the connection request
  ret = (int)connect(sockfd, (struct sockaddr *)&comm_addr, sizeof(comm_addr));  

  if(ret < 0){
    printf("%d_err:%s\n", ret, strerror(errno));
    exit(0);
  } else {
    printf("Buyer connected successfully!\n");
  }
  
  printf("Buyer initialized\n\n");

  while(login){
    printf("Please enter 'login <username>'\n>>");

    fgets(command, MAX_INPUT_SIZE, stdin);


    //Send the username data 
    bytesSent = send(sockfd, command, strlen(command), 0);

    //printf("Sent %d bytes\n", bytesSent);
    
    //Wait for the response
    ret = (int)recv(sockfd, response, sizeof(response), 0);
    if(ret < 0) {
      printf("%d_err:%s\n", ret, strerror(errno));
      exit(0);
    }

    if(strcmp(response, "Login successful!\n") == 0){
      login = 0;
    } else {
      printf("The server rejected the login request.  Please ensure the username is correct\n");
    }
  }

  //Loop and print output responses from the server
  if((pid = fork()) != 0){
    while(1){
      memset(data, 0, strlen(data));
      ret = (int)recv(sockfd, data, sizeof(data), 0);
      if(ret > 0){
        printf("%s\n>>", data);
      }
    }
  } else {
      printf("\t\t~~~~MENU~~~~\n1) List\n2) Bid <Item Number> <Bid Amount>\n3) Logout\n~~~~~~~~~~~~~~~~\n");
    while(!logout){
      printf(">>");
      strcpy(command, "");
      fgets(command, MAX_INPUT_SIZE, stdin);

      p = command;
      for(;*p;++p) *p = tolower(*p);
      pch = strtok(command, "\n");
      ret = send(sockfd,pch,strlen(pch)+1,0);
      if(0) logout = 1;
     
    }

  }
}


void bid(int sockfd, char *p){
  send(sockfd, p, strlen(p), 0);
}
