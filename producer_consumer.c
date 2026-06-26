/*

- Practical Session 2: Producer-Consumer Simulation
- CSC 308 - Operating Systems | Week 8
- 
- Objective: Implement the Producer-Consumer problem using POSIX semaphores
- Compile: gcc -o session2_producer_consumer session2_producer_consumer.c -lpthread
  */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Step 1: Create a circular buffer with fixed size
#define BUFFER_SIZE 5
#define ITEMS_TO_PRODUCE 10

int buffer[BUFFER_SIZE];
int in = 0;   // Producer inserts at index ‘in’
int out = 0;  // Consumer reads from index ‘out’

// Step 4: Use semaphores: mutex(1), empty(N), full(0)
sem_t mutex;           // Binary semaphore for mutual exclusion (init = 1)
sem_t empty_slots;     // Counts empty buffer slots (init = N = BUFFER_SIZE)
sem_t full_slots;      // Counts full buffer slots (init = 0)

// Step 2: Create producer thread that generates items
void *producer(void *arg) {
for (int item = 1; item <= ITEMS_TO_PRODUCE; item++) {
// Simulate production time (use sleep to see buffer filling)
sleep(1);  // Note: Adjust speed to observe buffer behavior

```
    // Wait if buffer is full
    sem_wait(&empty_slots);   // Decrement empty count

    // Enter critical section
    sem_wait(&mutex);

    // Add item to circular buffer
    buffer[in] = item;
    printf("[Producer] Produced item: %d | Buffer slot: %d\n", item, in);
    in = (in + 1) % BUFFER_SIZE;

    // Print current buffer status
    printf("[Buffer Status] in=%d, out=%d\n", in, out);

    // Exit critical section
    sem_post(&mutex);

    // Signal that a new item is available
    sem_post(&full_slots);    // Increment full count
}

printf("[Producer] Done producing.\n");
return NULL;
```

}

// Step 3: Create consumer thread that consumes items
void *consumer(void *arg) {
for (int i = 0; i < ITEMS_TO_PRODUCE; i++) {
// Wait if buffer is empty
sem_wait(&full_slots);    // Decrement full count

```
    // Enter critical section
    sem_wait(&mutex);

    // Remove item from circular buffer
    int item = buffer[out];
    printf("[Consumer] Consumed item: %d | Buffer slot: %d\n", item, out);
    out = (out + 1) % BUFFER_SIZE;

    // Exit critical section
    sem_post(&mutex);

    // Signal that a slot is now empty
    sem_post(&empty_slots);   // Increment empty count

    // Simulate consumption time (slower consumer to observe buffer filling)
    sleep(2);  // Note: Consumer is slower — watch the buffer fill up!
}

printf("[Consumer] Done consuming.\n");
return NULL;
```

}

int main() {
pthread_t prod_thread, cons_thread;

```
printf("=== Practical Session 2: Producer-Consumer Simulation ===\n");
printf("Buffer size: %d | Items to produce: %d\n\n", BUFFER_SIZE, ITEMS_TO_PRODUCE);

// Step 4: Initialize semaphores
sem_init(&mutex, 0, 1);                  // mutex = 1 (binary)
sem_init(&empty_slots, 0, BUFFER_SIZE);  // empty = N (full buffer available)
sem_init(&full_slots, 0, 0);             // full = 0 (nothing produced yet)

// Step 5: Observe synchronization behavior
pthread_create(&prod_thread, NULL, producer, NULL);
pthread_create(&cons_thread, NULL, consumer, NULL);

pthread_join(prod_thread, NULL);
pthread_join(cons_thread, NULL);

// Step 6: Monitor - no deadlock since we reached here!
printf("\n[Main] All items produced and consumed. No deadlock detected!\n");

// Cleanup
sem_destroy(&mutex);
sem_destroy(&empty_slots);
sem_destroy(&full_slots);

return 0;
```

}
