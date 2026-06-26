/*

- Practical Session 1: Mutex Lock Demonstration
- CSC 308 - Operating Systems | Week 8
- 
- Objective: Demonstrate mutual exclusion using pthread mutex locks
- Compile: gcc -o session1_mutex session1_mutex.c -lpthread
  */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define INCREMENT_COUNT 100000

// Shared counter (the resource to protect)
long counter_with_mutex = 0;
long counter_without_mutex = 0;

// Step 1: Initialize a pthread_mutex_t variable
pthread_mutex_t mutex;

// Thread function WITH mutex protection
void *increment_with_mutex(void *arg) {
for (int i = 0; i < INCREMENT_COUNT; i++) {
pthread_mutex_lock(&mutex);     // Step 3: Lock before accessing shared resource
counter_with_mutex++;
pthread_mutex_unlock(&mutex);   // Step 3: Unlock after access
}
return NULL;
}

// Thread function WITHOUT mutex protection (race condition demo)
void *increment_without_mutex(void *arg) {
for (int i = 0; i < INCREMENT_COUNT; i++) {
counter_without_mutex++;        // Unsafe - race condition!
}
return NULL;
}

int main() {
pthread_t threads[NUM_THREADS];

```
// Initialize the mutex
pthread_mutex_init(&mutex, NULL);

printf("=== Practical Session 1: Mutex Lock Demonstration ===\n\n");
printf("Number of threads: %d\n", NUM_THREADS);
printf("Increments per thread: %d\n", INCREMENT_COUNT);
printf("Expected counter value: %d\n\n", NUM_THREADS * INCREMENT_COUNT);

// --- Step 2: Create multiple threads WITH mutex ---
printf("--- Test 1: WITH Mutex ---\n");
for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, increment_with_mutex, NULL);
}
// Wait for all threads to finish
for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
}
// Step 4: Compare results WITH mutex
printf("Counter WITH mutex:    %ld (Correct? %s)\n\n",
       counter_with_mutex,
       counter_with_mutex == NUM_THREADS * INCREMENT_COUNT ? "YES ✓" : "NO ✗");

// --- Step 2: Create multiple threads WITHOUT mutex ---
printf("--- Test 2: WITHOUT Mutex ---\n");
for (int i = 0; i < NUM_THREADS; i++) {
    pthread_create(&threads[i], NULL, increment_without_mutex, NULL);
}
for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
}
// Step 5: Observe that without mutex, final counter value is incorrect
printf("Counter WITHOUT mutex: %ld (Correct? %s)\n",
       counter_without_mutex,
       counter_without_mutex == NUM_THREADS * INCREMENT_COUNT ? "YES ✓" : "NO ✗ (Race condition!)");

printf("\nNote: Run this program multiple times without mutex.\n");
printf("You'll see different results each time (non-deterministic).\n");

// Destroy the mutex (cleanup)
pthread_mutex_destroy(&mutex);

return 0;
```

}
