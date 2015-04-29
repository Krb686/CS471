#define SELLER_LOGIN	0
#define BUYER_LOGIN	1
#define LIST		2
#define ADD		3
#define SELL		4
#define BID		5
#define INVALID		-1

#define LOGINSUCCESS	0
#define LOGINFAILED	1

#define ITEM_UPDATE	0
#define ITEM_ADD	1
#define ITEM_DEL	2
#define ITEM_BID	3
#define ITEM_OUTBID	4

#define EXISTS		1
#define OUTBID		2

#define SUCCESS		0
#define FAIL		-1
#define INSUFFICIENT_BID 8

#define SELL_NO_BID	1

#define BIT0		1
#define BIT1		2
#define BIT2		4
#define BIT3		8
#define BIT4		16
#define BIT5		32
#define BIT6		64
#define BIT7		128

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
