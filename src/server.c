#include<errno.h>
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
#include<semaphore.h>
#include<signal.h>

#define MAX_INPUT_SIZE 40

int SELLER_PORT = 5372;
int BUYER_PORT  = 5373;
int backlog = 10;
int item_count = 0;
int channel[2];
char outbid_msg[512] = "";
char name[9] = "";
char *names[6] = {"alice","bob","dave","pam","susan","tom"};
itemListP itemlist;
sem_t mutex;
sem_t outbid;
int selsockfd, buysockfd, newsockfd;


void main() {
  int clilen;
  int pid, ret, item_number, login=1;
  struct sockaddr_in sel_addr, buy_addr, comm_addr;
  struct timeval t;
  t.tv_usec = 10000;
  char data[MAX_INPUT_SIZE]="";
  char *pch, *p, j;
  struct sigaction sa;


  sa.sa_handler = sig_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);

  sem_init(&mutex, 1, 1);
  sem_init(&outbid, 1, 0);
  ret = pipe2(channel, O_NONBLOCK);
  if(ret<0) printf("DEBUG pipe->%d_%s",ret,strerror(errno));

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

  if((pid = fork()) == 0){
    newsockfd = accept(selsockfd, (struct sockaddr *)&comm_addr,
                (socklen_t *)&clilen);
    while(login){
      ret = (int)recv(newsockfd, data, sizeof(data), 0);
      if(ret>0){
	p = data;
	printf("Seller sends the command: %s\n",data);
	for(;*p;++p) *p = tolower(*p);
	pch = strtok(data, " ");
	if(strcmp(pch,"login")==0){
	  pch = strtok(NULL, " \n");
	  if(strcmp(pch,"seller")==0) login = 0;
	  sendResponse(newsockfd, SELLER_LOGIN, login);
	}//pch==login
      }//ret>0
    }//login==1
    printf("exit login\n");
    while(1){
      strcpy(data,"");
      ret = (int)recv(newsockfd, data, sizeof(data), 0);
      if(ret>0){
	p = data;
	printf("Seller sends the command: %s,ret:%d\n",data,ret);
	for(;*p;++p) *p = tolower(*p);
	pch = strtok(data, " ");
        printf("pch: %s\n", pch);
	if(strcmp(pch,"list")==0){
	  ret = listItems(newsockfd);
	  //sendResponse(newsockfd, LIST, ret);
        }
	else if(strcmp(pch,"add")==0){
	  pch = strtok(NULL, " ");
	  item_number = atoi(pch);
          printf("item num: %d\n", item_number);
          if(item_number>0){
	    pch = strtok(NULL, " ");
            printf("pch: %s\n", pch);
	    ret = addItem(item_number, pch);
	    sendResponse(newsockfd, ADD, ret);
	  }
	  else sendResponse(newsockfd, INVALID, 1);
	}
	else if(strcmp(pch,"sell")==0){
	  pch = strtok(NULL, " ");
	  item_number = atoi(pch);
	  ret = sellItem(item_number);
	  sendResponse(newsockfd, SELL, ret);
        }
	else sendResponse(newsockfd, INVALID, 0);
      }//ret>0
      //else printf("%d_err:%s",ret,strerror(errno));
    }//True
  }//end child
  else{//parent
    // ===============================================================
    // ===============================================================
    // BUYER code
    while(1){
      //make nonblocking
      newsockfd = accept(buysockfd, (struct sockaddr *)&comm_addr,
                  (socklen_t *)&clilen);
      if((pid = fork()) == 0){
        close(newsockfd);
      }
      else{
        while(login){
          ret = (int)recv(newsockfd, data, sizeof(data), 0);
          if(ret>0){
            p = data;
            printf("Buyer sends the command: %s",data);
            for(;*p;++p) *p = tolower(*p);
            pch = strtok(data, " ");
            printf("%s\n", pch);
            if(strcmp(pch,"login")==0){
              pch = strtok(NULL, " \n");
              strcpy(name, pch);
              printf("here\n");
              login=clientLogin(pch);//returns 0 on success
              printf("login val: %d\n", login);
              sendResponse(newsockfd, BUYER_LOGIN, login);
            }
          } else {
            printf("ret was < 0\n");
          }
        }
	setsockopt(newsockfd, SOL_SOCKET, SO_RCVTIMEO,&t,
		sizeof(struct timeval));
        while(1){
          ret = sem_trywait(&outbid);
          if(ret == 0){
            ret=updateItemList(ITEM_OUTBID, NULL);
            if(ret != OUTBID) sem_post(&outbid);
            else{
	      ret = (int)send(newsockfd, outbid_msg,strlen(outbid_msg),0);
	      printf("outbidmsg:%s ret: %d\n",outbid_msg, ret);
	      strcpy(outbid_msg,"");
            }
          }
          ret = (int)recv(newsockfd, data, sizeof(data), 0);
          if(ret>0){
            p = data;
            printf("%s sends the command: %s,ret:%d\n",name,data,ret);
            for(;*p;++p) *p = tolower(*p);
            pch = strtok(data, " ");
            if(strcmp(pch,"list")==0){
              printf("sending the items...\n");
              listItems(newsockfd);
              //sendResponse(newsockfd, LIST, ret);
            }
            else if(strcmp(pch,"bid")==0){
              pch = strtok(NULL, " ");
              item_number = atoi(pch);
              pch = strtok(NULL, " ");
              ret = bidOnItem(item_number, atoi(pch));
              sendResponse(newsockfd, BID, ret);
            }
            else {
              printf("going thru the else...\n");
              sendResponse(newsockfd, INVALID, 0);
            }
          }//ret>0
        }//True
      }//childn
    }//True
  }//end parent
}//main

int clientLogin(){
  int i = 0;
  int code = 0;
  while(i<6){
    if(names[i]!=NULL){
      code = strcmp(names[i], name);
      if(code == 0){
        names[i] = NULL;
        return LOGINSUCCESS;
      }
    }
    i++;
  }
  return LOGINFAILED;
}

int listItems(int sockfd){
  char response[512] = "\t~~~~ITEMS~~~~\n";
  char line[50] = "";
  int ret = 0;
  updateItemList(ITEM_UPDATE,NULL);
  itemListP iter = itemlist;
  if(iter!=NULL){
    while(iter!=NULL){
      sprintf(line,"%d) %s %d %s\n",iter->data->item_number,
	     iter->data->item_name,iter->data->bid,iter->data->bidder);
      strcat(response,line);
      iter = iter->next;
    }
    ret = (int)send(sockfd, response, strlen(response)+1, 0);
    if(ret>0) return SUCCESS;
    else printf("DEBUG4_ret:%d\n",ret);
  } else {
    sprintf(response, "No items in the item list!\n");
    ret = (int)send(sockfd, response, strlen(response)+1, 0);
    return FAIL;
  }
}

int addItem(int item_number, char *pch){
  int ret;
  itemP item = malloc(sizeof(itemR));
  item->item_number = item_number;
  item->id = item_count++;
  item->bid = 0;
  strcpy(item->item_name,pch);
  strcpy(item->bidder,"");
  ret = updateItemList(ITEM_ADD, item);
  if(ret!=SUCCESS) free(item);
  return ret;
}

int sellItem(int item_number){
  itemListP iter = itemlist;
  while(iter!=NULL){
    if(iter->data->item_number == item_number){
      return updateItemList(ITEM_DEL, iter->data);
    }
    iter = iter->next;
  }
  return FAIL;
}

int bidOnItem(int item_number, int bid_amount){
  int ret;
  itemP item = malloc(sizeof(itemR));
  item->item_number = item_number;
  item->bid = bid_amount;
  strcpy(item->bidder,name);
  ret = updateItemList(ITEM_BID, item);
  free(item);
  return ret;
/*
  updateItemList(ITEM_UPDATE, NULL);
  itemListP iter = itemlist;
  printf("DEBUG_iter:%p\n",iter);
  while(iter!=NULL){
    printf("inum:%d\n",iter->data->item_number);
    if(iter->data->item_number == item_number){
      printf("ibid:%d\n",iter->data->bid);
      if(iter->data->bid < bid_amount){
	strcpy(iter->data->bidder,name);
	iter->data->bid = bid_amount;
	return updateItemList(ITEM_BID, iter);
      }//bid_amnt < data->bid
    }//item_number != data->item_number
    iter = iter->next;
  }//iter == NULL
  return FAIL;
*/
}

int invalidCommand(){
  return 0;
}

int sendResponse(int newsockfd, int from, int code){

  int ret = 0;

  char responseStr[MAX_INPUT_SIZE];

  sprintf(responseStr, "%d", code);  

  if(from == BUYER_LOGIN){
    ret = (int)send(newsockfd, responseStr, strlen(responseStr), 0);
    printf("response:%s ret: %d\n",responseStr, ret);
  }
  else if(from == SELLER_LOGIN){
    ret = (int)send(newsockfd, responseStr, strlen(responseStr), 0);
    printf("response ret: %d\n", ret);
  }

  return 0;
}

int updateItemList(int flag, itemP newitem){
  int ret;
  int stat = 0;
  char temp[64] = "";
  itemP item = malloc(sizeof(itemR));
  itemListP previtem = NULL;
  itemListP head = itemlist;
  if(flag == ITEM_BID) printf("DEBUG_inum:%d\n",newitem->item_number);

  sem_wait(&mutex);
//////////////////////////////////////////////////////////////////////////
  ret = (int)read(channel[0], item, sizeof(itemR));
  while(ret>0){
    if(flag!=ITEM_UPDATE && item->id==newitem->id) stat = EXISTS;
    if(flag==ITEM_ADD && item->item_number==newitem->item_number){
      stat = EXISTS;
    }
    if(flag==ITEM_BID && item->item_number==newitem->item_number &&
	item->bid < newitem->bid){
      item->bid = newitem->bid;
      strcpy(item->bidder,newitem->bidder);
      sem_post(&outbid);
      printf("outbid detected\n");
    }
    if(itemlist==NULL){
      itemlist = malloc(sizeof(itemListR));
      itemlist->next = NULL;
      itemlist->prev = previtem;
      if(previtem!=NULL) previtem->next = itemlist;
      if(head == NULL) head = itemlist;
      itemlist->data = item;
    }
    else if(itemlist->data->id < item->id){
      if(itemlist->prev!=NULL) itemlist->prev->next = itemlist->next;
      if(itemlist->next!=NULL) itemlist->next->prev = itemlist->prev;
      if(itemlist==head) head = itemlist->next;
      itemListP save = itemlist;
      previtem = itemlist->prev;
      itemlist = itemlist->next;
      free(save->data);
      free(save);
      continue;
    }
    else if(itemlist->data->id > item->id){
      itemlist->prev = malloc(sizeof(itemListR));
      itemlist->prev->next = itemlist;
      itemlist->prev->prev = previtem;
      itemlist->prev->data = item;
      if(previtem!=NULL) previtem->next = itemlist->prev;
      if(itemlist==head) head = itemlist->prev;
      previtem = itemlist->prev;
      ret = (int)read(channel[0], item, sizeof(itemR));
      continue;
    }
    else{
      if(flag==ITEM_OUTBID && strcmp(itemlist->data->bidder,name)==0
	 && strcmp(itemlist->data->bidder,item->bidder)!=0){
	sprintf(temp,"%s has outbid you on %s\n",
		item->bidder,item->item_name);
	strcat(outbid_msg,temp);
	printf("%s",temp);
	stat = OUTBID;
      }
      itemlist->data->bid = item->bid;
      strcpy(itemlist->data->bidder,item->bidder);
      free(item);
    }
    previtem = itemlist;
    itemlist = itemlist->next;
    item = malloc(sizeof(itemR));
    ret = (int)read(channel[0], item, sizeof(itemR));
  }//while(ret>0)
  free(item);

  while(itemlist!=NULL){
    if(itemlist->prev!=NULL) itemlist->prev->next = itemlist->next;
    if(itemlist->next!=NULL) itemlist->next->prev = itemlist->prev;
    if(itemlist==head) head = itemlist->next;
    itemListP save = itemlist;
    itemlist = itemlist->next;
    free(save->data);
    free(save);
  }

  ret = SUCCESS;
  if(flag == ITEM_ADD && stat != EXISTS){
    itemlist = malloc(sizeof(itemListR));
    itemlist->next = NULL;
    itemlist->prev = previtem;
    if(previtem!=NULL) previtem->next = itemlist;
    if(head == NULL) head = itemlist;
    itemlist->data = newitem;
  }
  else if(flag == ITEM_ADD) ret = EXISTS;
  else if(flag == ITEM_DEL && stat == EXISTS){
    itemListP iter = head;
    while(iter->data->id!=newitem->id){
      if(iter==NULL){printf("DEBUG3\n"); exit(0);}//shouldn't happen
      iter = iter->next;
    }
    if(iter->prev!=NULL) iter->prev->next = iter->next;
    if(iter->next!=NULL) iter->next->prev = iter->prev;
    if(iter==head) head = iter->next;
    free(iter->data);
    free(iter);
  }
  else if(flag == ITEM_DEL) ret = FAIL;
  else if(stat == OUTBID) ret = OUTBID;

  itemlist = head;
  while(itemlist!=NULL){
    item = itemlist->data;
    write(channel[1], item, sizeof(itemR));
    itemlist = itemlist->next;
  }
//////////////////////////////////////////////////////////////////////////
  sem_post(&mutex);
  itemlist = head;
  return ret;
}

void sig_handler(int signum){
  printf("Caught CTRL + C\n");
  printf("Closing the server...\n");
  close(selsockfd);
  close(buysockfd);
  close(newsockfd);

  
  exit(0);
}
