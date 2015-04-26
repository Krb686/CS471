
typedef struct ll{
  struct ll *next, *prev;
  char name[9];
}llR, *llP;

int clientLogin();
void listItems();
void addItem(int item_number, char *pch);
void sellItem(int item_number);
void invalidCommand();
