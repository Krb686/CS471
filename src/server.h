#define SELLER_LOGIN	0
#define BUYER_LOGIN	1
#define LIST		2
#define ADD		3
#define SELL		4
#define INVALID		5

#define LOGINSUCCESS	0
#define LOGINFAILED	1

#define EXISTS		1
#define UPDATE		0
#define ADD		1
#define DEL		2

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
