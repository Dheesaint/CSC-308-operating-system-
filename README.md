# CSC-308-operating-system-

# CSC 308 – Operating Systems | Week 8 Practical Sessions

**Student:** Asita Kelvin Chimoabi
**Reg No:** 2023934045
**Department:** Software Engineering
**Course:** Operating Systems
**Course Code:** CSC 308
**Institution:** Nnamdi Azikiwe University

## Overview

Four practical implementations covering IPC synchronization mechanisms in Linux using C.

-----

## Practical Sessions

|Session|Topic                       |Key Concepts                                        |
|-------|----------------------------|----------------------------------------------------|
|1      |Mutex Lock Demonstration    |`pthread_mutex_t`, mutual exclusion, race conditions|
|2      |Producer-Consumer Simulation|POSIX semaphores, circular buffer, synchronization  |
|3      |Semaphore Implementation    |Counting vs binary semaphore, performance comparison|
|4      |Shared Memory Programming   |`shmget`, `shmat`, `shmdt`, `shmctl`, IPC           |

-----

## How to Compile & Run

### Requirements

- Linux (Ubuntu/Debian recommended)
- GCC compiler
- POSIX thread library (`-lpthread`)

### Compile All

```bash
gcc -o session1_mutex          session1_mutex.c          -lpthread
gcc -o session2_producer_consumer session2_producer_consumer.c -lpthread
gcc -o session3_semaphore      session3_semaphore.c      -lpthread
gcc -o session4_shared_memory  session4_shared_memory.c  -lpthread
```

### Run Each

```bash
./session1_mutex
./session2_producer_consumer
./session3_semaphore
./session4_shared_memory
```

-----

## Expected Outputs

### Session 1 – Mutex

- Counter **WITH** mutex always equals `NUM_THREADS × INCREMENT_COUNT` ✓
- Counter **WITHOUT** mutex gives random lower values (race condition) ✗

### Session 2 – Producer-Consumer

- Producer and consumer alternate access to the circular buffer
- No buffer overflow or underflow thanks to semaphores

### Session 3 – Semaphore vs Mutex

- Counting semaphore lets 3 threads work concurrently → faster
- Mutex serializes all threads → slower

### Session 4 – Shared Memory

- Parent writes to shared memory; child reads from the same segment
- Semaphore ensures no simultaneous read/write corruption

-----

## Key Takeaways

- **Mutex**: Binary lock — only 1 thread at a time (best for simple exclusion)
- **Counting Semaphore**: Allows N concurrent threads (best for resource pools)
- **Shared Memory**: Fastest IPC method — no kernel copies needed
- **Shared Memory + Semaphores** = foundation of high-performance IPC in Linux

-----

## Course

CSC 308 – Operating Systems | Week 8 | Practical Submission
