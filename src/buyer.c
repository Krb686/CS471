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

int BUYER_PORT = 5373;
char AUCTIONSERVER_IP_ADDRESS[20] = "127.0.0.1";

void main(){
  int sockfd, ret, pid, login=1, logout=0, item_number;
  struct sockaddr_in my_addr, comm_addr;
  char username[16];
  char data[99];
  char responseStr[32];
  char *pch;
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
  comm_addr.sin_port = htons(BUYER_PORT);

  

  
  printf("Buyer initialized\n\n");

  while(login){
    printf("Please enter 'login <username>'\n>>");
    scanf("%s", username);
    pch = strtok(username, " ");
    if(strcasecmp(pch, "login")){

      ret = (int)connect(sockfd, (struct sockaddr *)&comm_addr, sizeof(comm_addr));

      if(ret < 0){
        printf("%d_err:%s\n", ret, strerror(errno));
        exit(0);
      }

      //Send the connection request
      send(sockfd, username, sizeof(data), 0);

      //Wait for the response
      ret = (int)recv(sockfd, data, sizeof(data), 0);

      if(ret < 0) {
        printf("%d_err:%s\n", ret, strerror(errno));
        exit(0);
      }


      if(strcmp(data, "1") == 0){
        login = 0;
      } else {
        printf("The server rejected the login request.  Please ensure the username is correct\n");
      }
    }
  }
}
