#define SELLER_LOGIN	0
#define BUYER_LOGIN	1
#define LIST		2
#define ADD		3
#define SELL		4
#define BID		5
#define INVALID		-1

#define LOGINSUCCESS	0
#define LOGINFAILED	1

#define ITEM_EXISTS	1
#define ITEM_UPDATE	0
#define ITEM_ADD	1
#define ITEM_DEL	2
#define ITEM_BID	3

#define SUCCESS		0
#define FAIL		-1

typedef struct items{
  int item_number, id, bid;
  char item_name[32], bidder[9];
}itemR, *itemP;

typedef struct itemList{
  struct itemList *next, *prev;
  itemP data;
}itemListR, *itemListP;

int clientLogin();
int listItems();
int addItem(int item_number, char *pch);
int sellItem(int item_number);
int invalidCommand();
void sig_handler(int signum);
