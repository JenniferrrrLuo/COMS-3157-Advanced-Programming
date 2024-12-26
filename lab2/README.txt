This file should contain:

  - your name
  - your UNI
  - lab assignment number
  - description for each part

- Jennifer Luo
- dl3572
- lab2

- part1: My code works exactly as specified in the lab. 
- part2: My code works exactly as specified in the lab. 

The description should indicate whether your solution for the part is
working or not.  You may also want to include anything else you would
like to communicate to the grader such as extra functionalities you
implemented or how you tried to fix your non-working code.

// You must include the output of the valgrind run for each part in the
README.txt

==951953== Memcheck, a memory error detector
==951953== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==951953== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==951953== Command: ./isort
==951953== 
    original: 89 78 75 55 95 91 34 41 39 92 51 52 55 0 50 51 62 6 45 13
    ascending: 0 6 13 34 39 41 45 50 51 51 52 55 55 62 75 78 89 91 92 95
    descending: 95 92 91 89 78 75 62 55 55 52 51 51 50 45 41 39 34 13 6 0
==951953== 
==951953== HEAP SUMMARY:
==951953==     in use at exit: 0 bytes in 0 blocks
==951953==   total heap usage: 5 allocs, 5 frees, 2,288 bytes allocated
==951953== 
==951953== All heap blocks were freed -- no leaks are possible
==951953== 
==951953== For lists of detected and suppressed errors, rerun with: -s
==951953== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

==957696== Memcheck, a memory error detector
==957696== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==957696== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==957696== Command: ./twecho hello world dude
==957696== 
hello HELLO
world WORLD
dude DUDE
==957696== 
==957696== HEAP SUMMARY:
==957696==     in use at exit: 0 bytes in 0 blocks
==957696==   total heap usage: 6 allocs, 6 frees, 2,114 bytes allocated
==957696== 
==957696== All heap blocks were freed -- no leaks are possible
==957696== 
==957696== For lists of detected and suppressed errors, rerun with: -s
==957696== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
