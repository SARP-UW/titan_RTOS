# RTOS

## Core functionality

- We want threadng support on a dual core
  - start_thread(), exit()
- Concurrency primitives
  - Mutex, Semaphore, event/barrier/gate
- maybe allocator
- Time utilities
  - `get_time()` (systime since restart)
  - `sleep()`

- system utilities
  - `restart()` MCU
  - shutdown MCU

## Implementation stuff

### Concurrency/Atomics

- Peterson's algo for locks, can't use ARM exclusive write instrs
  - you spin while you hold this, so only hold it when you are doing something simple (only one Peterson's lock)
  - some global lock that exclusive access to all atomic memory
    - anytime someone wants to update state of concurrency primitive (e.g. mutex), they acquire global peterson lock, and then come back
  - maybe wite atomic "instr" functions w/ the global lock
  - only use peterson lock for ensuring no race conditions / atomicity
- OR semaphore peripheral (hadrware semaphore)
  - probaly do peterson's to abstract away from the stm32
- start from scratch

### Multithreading

- main challenge is dual core
- How to enforce cross-core exclusive access
  - No interrupts (CRITICAL_SECTION macro). cpsid stores some mask of priority, interrupts have priority, you disable interrupts up until a certain priority
  - enter/exit critical just see what you're blocking

- No execution/exclusion on both cores
  - May want cases where code only runs on one core
  - maybe EXCULUSIVE_SECTION that blocks interrupts and stuff on the other core
  - please don't make scheduler so it has to block execution across cores
    - sync primitive don't work in interrupts (stuff like mutex don't because you can just get stuck, you must disable interrupts)
    - only way to prevent it is to disable interrupts when you acquire mutex

### Scheduler

- FreeRTOS is good inspo
- in systick interrupts you initiate pendSV interrupt or when yield() or sleep are called(), initiate pendSV interrupt
  - push all regs to stack
  - call func that picks next thread to run
  - moves stack ptr to location of thread's stack
  - pull saved regs to CPU, clear that part of the stack
- scheduler runs inside pendSV interrupt
- [context switching](https://interrupt.memfault.com/blog/cortex-m-rtos-context-switching)

### CPU stuff

- each core has a vtable with stack pointer, IP, pointer to interrupts
- you only have a single interrupt
- registers taht tells us which core to run on

- first section of vtable is processor, below that is microcontroller
- application does the interrupt part of vtable (like spi, i2c, etc)
- youd want to initialize vtable pointers and links to kernel interrupts
- weak reference means you can update vtable form another library

- shutdown - wait for next interrupt, clear memory and state
- restart is completely restart - close all threads, go to main
- some specific functions in armv7m (aircr reg?)

- implement time
- dont do sleep as just simple while loop
- you want scheduler to know a thread is sleeping and not reschedule it
- breakpoint will use debug stuff
- os printf would use semihosting (???) in debug console
