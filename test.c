/*
John Dott
COE 0449: Project 3
3/19/17
*/
#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

int main(){
  printf("%p\n", sbrk(0));
  //allocate some space
  void *new_brk = my_bestfit_malloc(1);
  void *try_again = my_bestfit_malloc(30);
  //print new value of brk
  printf("%p\n", sbrk(0));
  my_free(new_brk);
  printf("Successful Free\n");
  my_free(try_again);
}
