#define HEAPSIZE 10000

#define ALIGNMENT 8

#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7) 

#define PACK(size, alloc)  ((size) | (alloc))
#define GET_SIZE(s)   (s & ~0x7)
#define GET_ALLOC(s)  (s & 0x1)

typedef enum{FREE, ALLOCATED} mem_status;

/* Do not delete any declarations above this comment */

/* You may ADD fields to the struct below if you need to but do
   not delete any fields or your submission will probably not compile */
/* The stuct (and related drivers, etc.) are set up so you can use a
   singly linked list (just next ptrs) or a doubly linked list */
/* You may add data structures if you need them.   Remember no arrays 
   (except char * for input purposes */

 
typedef struct m{
    int status;
    int address;
    int param;
    struct m *next;
    struct m *previous;
} mem_rec, *mem_ptr;



