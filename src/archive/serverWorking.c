#include<errno.h>
#include<stdlib.h>
//#include<unistd.h>
//#include<sys/types.h>
//#include<sys/wait.h>
//#include<strings.h>
#include<fcntl.h>
#include<string.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/mman.h>
#include<netinet/in.h>
#include<time.h>

int server_port = 5372;
int backlog = 10;

typedef struct items{
  int item_number, id;
  char item_name[32];
}itemR, *itemP;

typedef struct itemList{
  struct items *next, *prev;
  itemP data;
}itemListR,*itemListP;

int channel[2];

void main() {
  int sockfd, newsockfd, clilen;
  int pid, timeout, ret;
  struct sockaddr_in my_addr, comm_addr;
  struct timeval t;
  t.tv_usec = 10000;
  char data[99];
  itemP t1 = malloc(sizeof(itemR));
  t1->item_number = 1;
  t1->id = 0;
  strcpy(t1->item_name,"blah");
  itemP t2 = malloc(sizeof(itemR));
  t2->item_number = 5;
  t2->id = 1;
  strcpy(t2->item_name,"bleh");

  ret = pipe2(channel,O_NONBLOCK);
  printf("pipe:%d\n",ret);
  ret = write(channel[1], &t1, sizeof(itemR));
  printf("wr1:%d\n",ret);
  ret = write(channel[1], &t2, sizeof(itemR));
  printf("wr2:%d\n",ret);
  

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char *)&my_addr, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  my_addr.sin_port = htons(server_port);

  bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
  printf("TCP Server Initialized...\n");
  listen(sockfd, backlog);
  clilen = sizeof(comm_addr);
  printf("Waiting for client connections...\n");

  int flags;
//  if(-1 == (flags = fcntl(sockfd, F_GETFL, 0))) flags = 0;
//  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

  while(1){
    newsockfd = accept(sockfd, (struct sockaddr *)&comm_addr,
                (socklen_t *)&clilen);
    if((pid = fork()) != 0){
      close(newsockfd);
      continue;
    }
    else{
      setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO,&t,
                 sizeof(struct timeval));
      printf("TCP Client Connected...\n");
      itemP item;
      ret = (int)read(channel[0], &item, sizeof(itemR));
      printf("ret:%d\n",ret);
      if(ret>0){
	printf("item_number:%d, item_name:%s\n\n",item->item_number, item->item_name);
      }
      
      //item->next = NULL;
      //printf("[%d]\n",*TEST);

      while(1){
        timeout = (int)recv(newsockfd, data, sizeof(data), 0);
	if(timeout>0){
          printf("data recvd:"); printf("%s",data); printf("\n");
          if(strcmp(data,"exit")==0){
            close(newsockfd);
            //(*TEST)--;
            //printf("[%d]\n",*TEST);
            printf("disconnected\n");
            exit(0);
          }
	}
        //else printf("%d_err:%s%p\n",timeout,strerror(errno),data);
      }
    }
  }
}
