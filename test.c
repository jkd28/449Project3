#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

int main(){
  void *new_brk = my_bestfit_malloc(1);
  void *try_again = my_bestfit_malloc(30);
  printf("%p\n", new_brk);
  my_free(new_brk);
  printf("Successful Free\n");
}
