#include <stdio.h>
#include <stdlib.h>

#include "memory.h"

extern mem_ptr Heap;


mem_ptr mm_malloc(int size) {
  int new_size = ALIGN(size);
  int best_fit = HEAPSIZE-new_size;
  int is_free;
  int fit;
  mem_ptr loc = Heap; mem_ptr best = Heap;
  do{
    is_free = !GET_ALLOC(loc->status);
    fit = GET_SIZE(loc->status)-new_size;
    if(is_free && fit>=0 && fit<best_fit){
      best_fit = fit; best = loc; best->previous = loc->previous;
    }
    loc = loc->next;
  }while(loc!=NULL);
  if(GET_ALLOC(best->status)){
    error_msg(1);
    return NULL;
  }
  best->status = PACK(new_size,1);
  if(best_fit!=0){
    mem_ptr next = (mem_ptr)malloc(sizeof(mem_rec));
    next->status = PACK(best_fit,0);
    next->address = new_size+best->address;
    next->previous = best;
    next->next = best->next;
    if(next->next!=NULL)
      next->next->previous = next;
    best->next = next;
  }
  return best;
}

void mm_free(mem_ptr m) {
  if(m!=NULL){
    if(!GET_ALLOC(m->status)){
      error_msg(3);
      return;
    }
    int prev_free=0; int next_free=0; int size=0;
    if(m->previous!=NULL)
      prev_free = !GET_ALLOC(m->previous->status);
    if(m->next!=NULL)
      next_free = !GET_ALLOC(m->next->status);
    if(prev_free && next_free){
      mem_ptr t = m->previous;
      size = GET_SIZE(m->previous->status) +
                 GET_SIZE(m->status) + GET_SIZE(m->next->status);
      m->previous->status = PACK(size,1);
      m->previous->next = m->next->next;
      free(m->next);
      free(m);
      m = m->previous;
    }
    else if(prev_free){
      size = GET_SIZE(m->previous->status) + GET_SIZE(m->status);
      m->previous->status = PACK(size,1);
      m->previous->next = m->next;
      free(m);
      m = m->previous;
    }
    else if(next_free){
      size = GET_SIZE(m->status) + GET_SIZE(m->next->status);
      m->status = PACK(size,1);
      free(m->next);
      m->next = m->next->next;
    }
    else{
      m->status = PACK(GET_SIZE(m->status),0);
      return;
    }
    if(m->next!=NULL)
      m->next->previous = m;
    mm_free(m);
  }
  else{
  error_msg(2);
  return;
  }
}
