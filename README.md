#Thread Library
## About
This program implements a basic user-level thread library for Linux. The library provides an interface for applications to create and run independent threads concurrently.
## Features
This thread library contains the following core features that are seen in similar existing lightweight user-level thread libraries:
1.  Creation of new threads
2.  Scheduling the execution of threads in a round-robin fashion
3.  Synchronization mechanism for threads to join other threads
4.  Preemption through an interrupt-based scheduler
## How To Use
The use of this thread library requires that GCC be installed into your computer (link: https://gcc.gnu.org/install/), and that Linux is the operating system of your computer. 

To utilize the thread library within your C application, you must make sure that:
1. Your C application's file(s) is located in `../Thread_Library/apps`.
2. Include the `uthread.h` header file in your C application's file(s).
```
#include <uthread.h>
```
3. In ``../Thread_Library/apps/Makefile``, include your C application's file(s) to the target programs at the top of the Makefile.
```
# Target programs
programs := new_application.x
```
If you become unsure about how to correctly utilize the library, please look at `../Test_Library/apps/test_preempt.c`, `../Test_Library/apps/uthread_hello.c`, `../Test_Library/apps/uthread_yield.c`, and `../Test_Library/apps/Makefile` in reference to the 3 instruction steps stated above.

To utilize the library's thread commands, please refer to the library's Public API located in `../Test_Library/libuthread/uthread.h`

To run your C application's file(s) using the thread library: 
1. Enter the `../Test_Library/libuthread` directory in your computer's terminal. 
2. `make` the Makefile in `../Test_Library/libuthread`.
3. Run the executable of your C application.
## Testing
Test files demonstrating the correctness of the thread library along with examples of how to utilize the library have already been included in this repository. The test files are located in `../Test_Library/apps`. The test descriptions are included as comments at the top of each test file. To see the output of a test file, please follow the steps described in the "**How To Use**" section.
## Lessons Learned
This project taught me about threads, and about how they can be scheduled. Threads can execute instructions concurrently and may share resources amongst themselves such memory with their concurrent execution. I learned about the chosen round-robin scheduling method of threads, which alternates between threads through a "first-come, first serve" scheduling basis. I also learned how to forcefully yield a thread's execution through a process called preemption, which interrupts a thread after it operates for a certain length of time. I also learned how to recover all of a thread's original execution process after it has yielded, which is fundamental to allowing threads to run concurrently.

