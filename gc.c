#include "gc.h"

#define DEBUG 1

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void gc_init(void) {
  stack = malloc(sizeof(stack_t));
  if (stack) {
    stack->g_head = stack->l_head = NULL;
    stack->size = stack->count = 0;
  }
}

void gc_destroy(void) {
  obj_t *tmp, *ptr = stack->l_head;
  while (ptr != NULL) {
    tmp = ptr;
    ptr = ptr->next;
    free(tmp->addr);
    free(tmp);
  }

  ptr = stack->l_head;
  while (ptr != NULL) {
    tmp = ptr;
    ptr = ptr->next;
    free(tmp->addr);
    free(tmp);
  }
  free(stack);
}

void gc() {
  clock_t start = clock();
  if (DEBUG) printf("Garbage Collector Awake!\n");
  __mark();
  __sweep();
  if (DEBUG)
    printf("Elapsed time: %lfs\n", (double)(clock() - start) / CLOCKS_PER_SEC);
}

obj_t *gc_malloc(size_t size) {
  // Awaken the reaper
  if (stack->size + size >= SIZE) gc();

  // Allocate new object
  obj_t *g_obj, *l_obj = malloc(sizeof(obj_t));
  assert(l_obj != NULL);

  // Allocate the payload itself
  l_obj->addr = malloc(size);
  l_obj->size = size;
  l_obj->marked = 0;
  l_obj->tag = stack->count;

  // Insert new obj at the head of the local list
  g_obj = __deep_cpy(l_obj);
  l_obj->next = stack->l_head;
  g_obj->next = stack->g_head;

  stack->l_head = l_obj;
  stack->g_head = g_obj;
  stack->size += size;
  ++(stack->count);

  // Return a ptr to object
  return stack->l_head;
}

void __mark() {
  obj_t *obj = stack->g_head;
  while (obj != NULL) {
    if (__is_reachable(obj)) obj->marked = 1;
    obj = obj->next;
  }
}

obj_t *__deep_cpy(obj_t *obj) {
  obj_t *cpy = malloc(sizeof(obj_t));
  if (cpy) {
    cpy->addr = obj->addr;
    cpy->size = obj->size;
    cpy->marked = obj->marked;
    cpy->tag = obj->tag;
    cpy->next = NULL;
  }
  return cpy;
}

void __sweep() {
  obj_t *temp, **obj = &(stack->g_head);
  while (*obj) {
    if (!(*obj)->marked) {
      __remove_local((*obj)->tag);
      temp = *obj;
      *obj = (*obj)->next;
      stack->size -= temp->size;
      free(temp);
      --(stack->count);
    } else {
      (*obj)->marked = 0;
      obj = &((*obj)->next);
    }
  }
}

void __remove_local(size_t tag) {
  obj_t *tmp, *obj = stack->l_head;

  if (obj && obj->tag == tag) {
    tmp = obj;
    stack->l_head = obj->next;
    free(tmp);
    return;
  }

  while (obj && obj->tag != tag) {
    tmp = obj;
    obj = obj->next;
  }

  if (obj == NULL) return;
  tmp->next = obj->next;
  free(obj);
}

int __is_reachable(obj_t *obj) {
  obj_t *ptr = stack->l_head;
  while (ptr != NULL) {
    if (ptr->addr == obj->addr) return 1;
    ptr = ptr->next;
  }
  return 0;
}

void __dump_gc() {
  obj_t *ptr;
  printf("==============================\n");
  printf("Count = %zu, Size = %zu\n", stack->count, stack->size);
  printf("Global List:\n");
  ptr = stack->g_head;
  while (ptr != NULL) {
    printf("Addr = %p, Size = %zu, Tag = %zu, Marked = %d\n", ptr->addr,
           ptr->size, ptr->tag, ptr->marked);
    ptr = ptr->next;
  }
  printf("Local List:\n");
  ptr = stack->g_head;
  while (ptr != NULL) {
    printf("Reachable = %d, Addr = %p, Size = %zu, Tag = %zu, Marked = %d\n",
           __is_reachable(ptr), ptr->addr, ptr->size, ptr->tag, ptr->marked);
    ptr = ptr->next;
  }
  printf("==============================\n");
}
