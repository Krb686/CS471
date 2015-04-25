#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<string.h>
#include<strings.h>

#define ADD 1
#define SUBTRACT 2
#define EXIT 3

int server_port_no = 6088, a;
char ip_addr[20] = "127.0.0.1";
struct parameters {
   int operand1;
   int operand2;
   int operation;
};
  
main() {
   int sockfd, result = 0;
   struct sockaddr_in my_addr, comm_addr;
   struct parameters data;
  
   sockfd = socket(AF_INET, SOCK_STREAM, 0); /* create socket*/
  
   bzero((char *)&my_addr, sizeof(my_addr));
   my_addr.sin_family = AF_INET;
   my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   my_addr.sin_port = htons(0); /* Specifing 0 means the OS takes any available port number */
   bind(sockfd, (struct sockaddr *)&my_addr, sizeof(my_addr));
    
   bzero((char *)&comm_addr, sizeof(comm_addr));
   comm_addr.sin_family = AF_INET;
   comm_addr.sin_addr.s_addr = inet_addr(ip_addr); /*specify IP of server */
   comm_addr.sin_port = htons(server_port_no); /*specify server port */
    
   connect(sockfd, (struct sockaddr *)&comm_addr, sizeof(comm_addr));
   printf("Connected to TCP Server...\n");
  
   data.operand1 = 20;
   data.operand2 = 10;

   data.operation = ADD;
   send(sockfd, (char*)&data, sizeof(data), 0);
   printf("Sent request to ADD %d and %d to TCP Server\n",data.operand1, data.operand2);
   recv(sockfd, (char *)&result, sizeof(result), 0);
   printf("Received result of adding %d and %d = %d from TCP Server\n", data.operand1, data.operand2, result);
  
   sleep(1);
   data.operation = SUBTRACT;
   send(sockfd, (char*)&data, sizeof(data), 0);
   printf("Sent request to SUBTRACT %d from %d to TCP Server\n",data.operand2, data.operand1);
   recv(sockfd, (char *)&result, sizeof(result), 0);
   printf("Received result of subtracting %d from %d = %d from TCP Server\n", data.operand2, data.operand1, result);
  
 while(1){
   sleep(1);
   printf("Exit?\n");
   scanf("%d",&a);
   if(a == 3){
     data.operation = EXIT;
     send(sockfd, (char*)&data, sizeof(data), 0);
     printf("Sent request to EXIT to TCP Server\n");
     close(sockfd);
     printf("exit\n");
     exit(0);
   }
 }
}
