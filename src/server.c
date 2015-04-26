#include<errno.h>
//#include<unistd.h>
//#include<sys/types.h>
//#include<sys/wait.h>
//#include<strings.h>
#include<semaphore.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<sys/socket.h>
#include<sys/mman.h>
#include<netinet/in.h>
#include<time.h>
#include"server.h"
#include<fcntl.h>

int SELLER_PORT = 5372;
int BUYER_PORT  = 5373;
int backlog = 10;
int channel[2];
char *names[6] = {"alice","bob","dave","pam","susan","tom"};
itemListP itemlist;
sem_t mutex;

void main() {
  int selsockfd, buysockfd, newsockfd, clilen;
  int pid, ret, item_number, login=1;
  struct sockaddr_in sel_addr, buy_addr, comm_addr;
  struct timeval t;
  t.tv_usec = 10000;
  char data[99];
  char *pch, *p, j, *name;

  sem_init(&mutex, 1, 1);
  ret = pipe2(channel, O_NONBLOCK);
  if(ret<0) printf("DEBUG pipe->%d_%s",ret,strerror(errno));

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
	  sendResponse(newsockfd, LIST, ret);
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
          printf("Buyer sends the command: %s",data);
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
	    listItems(newsockfd);
	    //sendResponse(newsockfd, LIST, ret);
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

int clientLogin(char *name){
  int i = 0;
  while(i<6){
    if(names[i]!=NULL && strcmp(names[i],name)==0){
      names[i] = NULL;
      return LOGINSUCCESS;
    }
  }
  return LOGINFAILED;
}

int listItems(int sockfd){
  itemListP iter = updateItemList();
  if(iter!=NULL) printf("\t\t~~~~ITEMS~~~~\n");
  while(iter!=NULL){
    printf("%d) %s %d %s\n",iter->item_number,iter->item_name,
			    iter->bid,iter->bidder);
    iter = iter->next;
  }
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
  return 0;
}

int addItem(int item_number, char *pch){
  updateItemList();
  return 0;
}

int sellItem(int item_number){
  return 0;
}

int invalidCommand(){
  return 0;
}

int sendResponse(int newsockfd, int CODE, int ret){
  return 0;
}

itemListP updateItemList(){
  int ret = 1;
  itemP = item = NULL;
  itemListP head = itemlist;

  sem_wait(&mutex);
  while(ret>0){
    if(item != NULL && itemlist->data->id == item->id){
      itemlist = itemlist->next
    }
    ret = (int)read(channel[0], &item, sizeof(itemR))
    while(itemlist!=NULL){
      if(itemlist->data->id == item->id) break;
      if(itemlist->next == NULL){
        itemlist->next = malloc(sizeof(itemListR));
        itemlist->next->next = NULL
        itemlist->next->prev = itemlist;
        itemlist->next->data = item;
      }
      if(itemlist->data->id < item->id){
	itemlist->prev->next = itemlist->next;
	itemlist->next->prev = itemlist->prev;
	free(itemlist->data);
	free(itemlist);
      }
      itemlist = itemlist->next;
    }
  }
  sem_post(&mutex);

  return itemlist;
}
