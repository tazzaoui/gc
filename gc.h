#ifndef GC_H
#define GC_H

#include <stddef.h>

#define SIZE 1 << 20

/* Node in the free list*/
typedef struct obj_t obj_t;
struct obj_t {
  void* addr;      // Mem. addr of the payload
  size_t size;     // Size of payload
  size_t tag;      // Unique identifier of the object
  int marked : 1;  // Is marked
  obj_t* next;     // Next element in the list
};

/* Global memory*/
typedef struct stack_t {
  obj_t* g_head;  // Global list of elements
  obj_t* l_head;  // Local list of elements
  size_t count;   // Number of objects on the global stack
  size_t size;    // Current size of the stack
} stack_t;

// Global stack object
extern stack_t* stack;

// Init the stack & garbage collector
void gc_init(void);

// Destory the stack & clean up
void gc_destroy(void);

// Awaken the garbage collector (mark followed by sweep)
void gc(void);

// Dynamically allocate size bytes in global memory
obj_t* gc_malloc(size_t size);

// Return ptr to a deep copy of an object
obj_t* __deep_cpy(obj_t*);

// Walk the global list and mark elements that are still reachable
void __mark(void);

// Walk the local list and free elements that are unmarked
void __sweep(void);

// Remove an element with id=tag from the local list
void __remove_local(size_t tag);

// Output the gc's internals
void __dump_gc(void);

// Check if the object is reachable from any of the objects in the local scope
int __is_reachable(obj_t*);

#endif /* GC_H */
