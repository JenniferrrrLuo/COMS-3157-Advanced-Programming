This file should contain:

  - your name
  - your UNI
  - lab assignment number
  - description for each part

- Jennifer Luo
- dl3572
- lab3 
- part1: My code works exactly as specified. 
  part2: My code works exactly as specified. 

The description should indicate whether your solution for the part is
working or not.  You may also want to include anything else you would
like to communicate to the grader such as extra functionalities you
implemented or how you tried to fix your non-working code.


valgrind --leak-check=yes ./mylist-test
==537289== Memcheck, a memory error detector
==537289== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==537289== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==537289== Command: ./mylist-test
==537289==
testing addFront(): 9.0 8.0 7.0 6.0 5.0 4.0 3.0 2.0 1.0
testing flipSignDouble(): -9.0 -8.0 -7.0 -6.0 -5.0 -4.0 -3.0 -2.0 -1.0
testing flipSignDouble() again: 9.0 8.0 7.0 6.0 5.0 4.0 3.0 2.0 1.0
testing findNode(): OK
popped 9.0, the rest is: [ 8.0 7.0 6.0 5.0 4.0 3.0 2.0 1.0 ]
popped 8.0, the rest is: [ 7.0 6.0 5.0 4.0 3.0 2.0 1.0 ]
popped 7.0, the rest is: [ 6.0 5.0 4.0 3.0 2.0 1.0 ]
popped 6.0, the rest is: [ 5.0 4.0 3.0 2.0 1.0 ]
popped 5.0, the rest is: [ 4.0 3.0 2.0 1.0 ]
popped 4.0, the rest is: [ 3.0 2.0 1.0 ]
popped 3.0, the rest is: [ 2.0 1.0 ]
popped 2.0, the rest is: [ 1.0 ]
popped 1.0, the rest is: [ ]
testing addAfter(): 1.0 2.0 3.0 4.0 5.0 6.0 7.0 8.0 9.0
popped 1.0, and reversed the rest: [ 9.0 8.0 7.0 6.0 5.0 4.0 3.0 2.0 ]
popped 9.0, and reversed the rest: [ 2.0 3.0 4.0 5.0 6.0 7.0 8.0 ]
popped 2.0, and reversed the rest: [ 8.0 7.0 6.0 5.0 4.0 3.0 ]
popped 8.0, and reversed the rest: [ 3.0 4.0 5.0 6.0 7.0 ]
popped 3.0, and reversed the rest: [ 7.0 6.0 5.0 4.0 ]
popped 7.0, and reversed the rest: [ 4.0 5.0 6.0 ]
popped 4.0, and reversed the rest: [ 6.0 5.0 ]
popped 6.0, and reversed the rest: [ 5.0 ]
popped 5.0, and reversed the rest: [ ]
==537289==
==537289== HEAP SUMMARY:
==537289==     in use at exit: 0 bytes in 0 blocks
==537289==   total heap usage: 19 allocs, 19 frees, 1,312 bytes allocated
==537289==
==537289== All heap blocks were freed -- no leaks are possible
==537289==
==537289== For lists of detected and suppressed errors, rerun with: -s
==537289== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)




valgrind --leak-check=yes ./revecho hello world dude
==538565== Memcheck, a memory error detector
==538565== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==538565== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==538565== Command: ./revecho hello world dude
==538565== 
dude
world
hello

dude found
==538565== 
==538565== HEAP SUMMARY:
==538565==     in use at exit: 0 bytes in 0 blocks
==538565==   total heap usage: 4 allocs, 4 frees, 1,072 bytes allocated
==538565== 
==538565== All heap blocks were freed -- no leaks are possible
==538565== 
==538565== For lists of detected and suppressed errors, rerun with: -s
==538565== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
