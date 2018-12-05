#include <stdio.h>
#include "gc.h"

stack_t *stack = NULL;

int main(int argc, char *argv) {
  gc_init();

  // Alocate two ints
  obj_t *obj1 = gc_malloc(sizeof(int));
  obj_t *obj2 = gc_malloc(sizeof(int));

  // Now we can use the addr as a regular location in memory
  *((int *)obj1->addr) = 40;
  *((int *)obj2->addr) = 2;

  // Now we can use these values as regular variables
  int result = *((int *)obj1->addr) + *((int *)obj2->addr);
  printf("%d\n", result);

  printf("Before mem leak\n");
  __dump_gc();

  // These should no longer be reachable
  obj1->addr = NULL;
  obj2->addr = NULL;

  printf("After mem leak\n");

  gc();
  __dump_gc();

  gc_destroy();
  return 0;
}
