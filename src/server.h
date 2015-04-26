#define SELLER_LOGIN 0
#define LIST 0
#define ADD 0
#define SELL 0
#define INVALID 0
#define BUYER_LOGIN 0


typedef struct ll{
  struct ll *next, *prev;
  char name[9];
}llR, *llP;

int clientLogin();
int listItems();
int addItem(int item_number, char *pch);
int sellItem(int item_number);
int invalidCommand();
