#define SELLER_LOGIN	0
#define BUYER_LOGIN	1
#define LIST		2
#define ADD		3
#define SELL		4
#define INVALID		5
#define LOGINSUCCESS	0
#define LOGINFAILED	1

typedef struct ll{
  struct ll *next, *prev;
  char name[9];
}llR, *llP;

int clientLogin();
int listItems();
int addItem(int item_number, char *pch);
int sellItem(int item_number);
int invalidCommand();
