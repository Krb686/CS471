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

int SELLER_PORT = 5372;
int BUYER_PORT  = 5373;
int backlog = 10;
static int *seller_on;

void main() {
  int sockfd, newsockfd, clilen;
  int pid, timeout;
  struct sockaddr_in my_addr, comm_addr;
  struct timeval t;
  t.tv_usec = 10000;
  char data[99];
  int *TEST;
  TEST = mmap(NULL, sizeof *TEST, PROT_READ | PROT_WRITE,
              MAP_SHARED | MAP_ANONYMOUS, -1, 0);
  *TEST = 0;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  bzero((char *)&my_addr, sizeof(my_addr));
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  my_addr.sin_port = htons(SELLER_PORT);

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
      (*TEST)++;
      printf("[%d]\n",*TEST);

      while(1){
        timeout = (int)recv(newsockfd, data, sizeof(data), 0);
	if(timeout>0){
          printf("data recvd:"); printf("%s",data); printf("\n");
          if(strcmp(data,"exit")==0){
            close(newsockfd);
            (*TEST)--;
            printf("[%d]\n",*TEST);
            printf("disconnected\n");
            exit(0);
          }
	}
        //else printf("%d_err:%s%p\n",timeout,strerror(errno),data);
      }
    }
  }
}
