#ifndef GC_H
#define GC_H

#include <stddef.h>

#define SIZE 1 << 20
typedef struct obj_t obj_t;
struct obj_t {
  void* addr;      // Mem. addr of the payload
  size_t size;     // Size of payload
  size_t tag;      // Unique identifier of the object
  int marked : 1;  // Is marked
  obj_t* next;     // Next element in the list
};

typedef struct stack_t {
  obj_t* g_head;  // Global list of elements
  obj_t* l_head;  // Local list of elements
  size_t count;   // Number of objects on the global stack
  size_t size;    // Current size of the stack
} stack_t;

// Global stack object
extern stack_t* stack;

void gc_init(void);
void gc_destroy(void);
void gc(void);

obj_t* gc_malloc(size_t size);
obj_t* __deep_cpy(obj_t*);

void __mark(void);
void __sweep(void);
void __remove_local(size_t tag);
void __dump_gc(void);
int __is_reachable(obj_t*);
int __in_use(obj_t*);

#endif /* GC_H */
