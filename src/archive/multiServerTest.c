#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<string.h> 
#include<strings.h> 
#include<time.h>
#define TRUE 1
#define ADD 1
#define SUBTRACT 2
#define EXIT 3

int server_port_no = 6088;
int backlog = 5;
struct parameters {
   int operand1;
   int operand2;
   int operation;
};

main() {
   int i, sockfd, newsockfd, clilen, result, pid, timeout;
   struct sockaddr_in my_addr, comm_addr;
   struct parameters data;
   struct timeval t;
   t.tv_usec = 10000;

   sockfd = socket(AF_INET, SOCK_STREAM, 0); /*Create a Socket*/
   bzero((char *)&my_addr, sizeof(my_addr));
   my_addr.sin_family = AF_INET;
   my_addr.sin_addr.s_addr = htonl(INADDR_ANY); /*Puts the IP address of the machine on which the code runs*/
   my_addr.sin_port = htons(server_port_no);
   /* Binds to IP address and port number specified in my_addr */
        bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr)); 
        printf("TCP Server Initialized...\n");

        listen(sockfd, backlog); /* listen queue */
   clilen = sizeof(comm_addr);
   printf("Waiting for client connections...\n"); 
   while(1){
   newsockfd = accept(sockfd, (struct sockaddr *)&comm_addr, (socklen_t *)&clilen); 
   if((pid = fork()) != 0){
     close(newsockfd);
     exit(0);
   }
   else{
     setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO,&t,sizeof(struct timeval));
     printf("TCP Client Connected...\n"); 

     while(TRUE) {
       timeout = (int)recv(newsockfd, (char *)&data, sizeof(data), 0);
       if(timeout!=-1){
         switch(data.operation) {
         case ADD     :result = data.operand1 + data.operand2;
                      printf("Received request to ADD %d and %d from TCP Client\n", data.operand1, data.operand2); 
                      break; 
         case SUBTRACT:result = data.operand1 - data.operand2;
                      printf("Received request to SUBTRACT %d from %d from TCP Client\n", data.operand2, data.operand1); 
                      break;
         case EXIT    :printf("Received request to EXIT from TCP Client\n");
                                                  close(newsockfd);
                      close(sockfd);
                      printf("TCP Client connection closed.\n");
                      break; 
         } 

         send(newsockfd, (char*)&result, sizeof(result), 0);
         printf("Sent result to TCP Client\n");
       }
    }
   }
  }
}

