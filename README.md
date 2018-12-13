# gc
> A mark &amp; sweep garbage collector

## Basic Algorithm

1. Initialize all objects with their marked bit cleared
2. First pass: Traverse and mark all objects reachable from the root set and mark every 
3. Second pass: Traverse the global list and remove all objects that are not marked

## To build
1. Simply clone the repo and use the `Makefile` to build

``$ git clone https://github.com/tazzaoui/gc.git && cd gc && make``

2. Run the resulting executable
```
$ ./gc-test
42
==============================
Count = 2, Size = 8
Global List:
Addr = 0x55a9bac99340, Size = 4, Tag = 1, Marked = 0
Addr = 0x55a9bac992c0, Size = 4, Tag = 0, Marked = 0
Local List:
Reachable = 1, Addr = 0x55a9bac99340, Size = 4, Tag = 1, Marked = 0
Reachable = 1, Addr = 0x55a9bac992c0, Size = 4, Tag = 0, Marked = 0
==============================
Garbage Collector Awake!
Elapsed time: 0.000028s
Garbage Collector Awake!
Elapsed time: 0.000014s
```

## Testing with Valgrind
1. Use the `Makefile` to invoke valgrind in order to check for memory leaks
```
$ make check
==14060==
==14060== HEAP SUMMARY:
==14060==     in use at exit: 0 bytes in 0 blocks
==14060==   total heap usage: 8 allocs, 8 frees, 1,224 bytes allocated
==14060==
==14060== All heap blocks were freed -- no leaks are possible
==14060==
==14060== For counts of detected and suppressed errors, rerun with: -v
==14060== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
