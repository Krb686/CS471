#include<stdio.h>
#include <stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<netinet/in.h>
#include<string.h>
#include<strings.h>
#include<errno.h>

#define ADD 1
#define SUBTRACT 2
#define EXIT 3

int server_port_no = 5372, a;
char ip_addr[20] = "127.0.0.1";

main() {
   int sockfd, result = 0;
   struct sockaddr_in my_addr, comm_addr;
   char *data = "data";
  
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
    
   printf("%d_",(int)connect(sockfd, (struct sockaddr *)&comm_addr, sizeof(comm_addr)));
   printf("err:%s\n",strerror(errno));
   printf("Connected to TCP Server...\n");

   send(sockfd, data, sizeof(data), 0);
   printf("Sent %s to TCP Server\n",data);
  
   sleep(1);
//   strcat(data,"2");
   send(sockfd, data, sizeof(data), 0);
   printf("Sent %s to TCP Server\n",data);
  
 while(1){
   sleep(1);
   printf("Exit?\n");
   scanf("%d",&a);
   if(a == 3){
     send(sockfd, "exit",sizeof(data), 0);
     printf("exit\n");
     close(sockfd);
     exit(0);
   }
 }
}
