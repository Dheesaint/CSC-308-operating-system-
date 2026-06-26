/*

- Practical Session 3: Semaphore Implementation in C
- CSC 308 - Operating Systems | Week 8
- 
- Objective: Compare mutex locks and semaphores for protecting shared resources
- Compile: gcc -o session3_semaphore session3_semaphore.c -lpthread
  */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 6
#define MAX_CONCURRENT 3   // Step 4: Allow 3 threads simultaneously (counting semaphore)
#define WORK_DURATION 1

long counter_mutex = 0;
long counter_semaphore = 0;

// Step 1: Create semaphores
sem_t counting_semaphore;  // Counting semaphore - allows MAX_CONCURRENT threads
pthread_mutex_t mutex;     // Binary mutex for comparison

// — SEMAPHORE APPROACH —
// Step 2: Protect a shared counter with sem_wait()/sem_post()
void *semaphore_worker(void *arg) {
int tid = *(int *)arg;

```
sem_wait(&counting_semaphore);  // Decrement: block if 0

// Critical section: up to MAX_CONCURRENT threads can be here
printf("[Semaphore] Thread %d ENTERED critical section\n", tid);
sleep(WORK_DURATION);  // Simulate work
counter_semaphore++;
printf("[Semaphore] Thread %d EXITING critical section\n", tid);

sem_post(&counting_semaphore);  // Increment: release slot

return NULL;
```

}

// — MUTEX APPROACH —
// Step 3: Compare with pthread_mutex_lock/unlock approach
void *mutex_worker(void *arg) {
int tid = *(int *)arg;

```
pthread_mutex_lock(&mutex);   // Block ALL other threads

// Critical section: ONLY 1 thread can be here at a time
printf("[Mutex]     Thread %d ENTERED critical section\n", tid);
sleep(WORK_DURATION);  // Simulate work
counter_mutex++;
printf("[Mutex]     Thread %d EXITING critical section\n", tid);

pthread_mutex_unlock(&mutex); // Release

return NULL;
```

}

int main() {
pthread_t threads[NUM_THREADS];
int thread_ids[NUM_THREADS];
struct timespec start, end;

```
printf("=== Practical Session 3: Semaphore vs Mutex Comparison ===\n");
printf("Threads: %d | Max concurrent (semaphore): %d\n\n", NUM_THREADS, MAX_CONCURRENT);

// --- Step 1: Initialize semaphore and mutex ---
sem_init(&counting_semaphore, 0, MAX_CONCURRENT);  // Counting semaphore: allows 3
pthread_mutex_init(&mutex, NULL);

// ---- Test 1: COUNTING SEMAPHORE ----
printf("--- Test 1: Counting Semaphore (allows %d concurrent) ---\n", MAX_CONCURRENT);
clock_gettime(CLOCK_MONOTONIC, &start);

for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i + 1;
    pthread_create(&threads[i], NULL, semaphore_worker, &thread_ids[i]);
}
for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
}

clock_gettime(CLOCK_MONOTONIC, &end);
double semaphore_time = (end.tv_sec - start.tv_sec) +
                        (end.tv_nsec - start.tv_nsec) / 1e9;

printf("Semaphore counter: %ld | Time: %.2f seconds\n\n", counter_semaphore, semaphore_time);

// ---- Test 2: BINARY MUTEX ----
printf("--- Test 2: Binary Mutex (allows 1 at a time) ---\n");
clock_gettime(CLOCK_MONOTONIC, &start);

for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i + 1;
    pthread_create(&threads[i], NULL, mutex_worker, &thread_ids[i]);
}
for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
}

clock_gettime(CLOCK_MONOTONIC, &end);
double mutex_time = (end.tv_sec - start.tv_sec) +
                    (end.tv_nsec - start.tv_nsec) / 1e9;

printf("Mutex counter:     %ld | Time: %.2f seconds\n\n", counter_mutex, mutex_time);

// Step 5: Measure performance differences
printf("=== Performance Comparison ===\n");
printf("Semaphore (concurrent=%d): %.2f seconds\n", MAX_CONCURRENT, semaphore_time);
printf("Mutex      (concurrent=1): %.2f seconds\n", mutex_time);
printf("Speedup: %.1fx\n\n", mutex_time / semaphore_time);

// Key Takeaway
printf("KEY TAKEAWAY:\n");
printf("- Mutex: binary (0 or 1) — simplest, only 1 thread at a time\n");
printf("- Counting Semaphore: allows N threads — ideal for resource pools\n");
printf("DISCUSSION: Use counting semaphore when you have N identical resources\n");
printf("(e.g., 3 database connections, 5 printers, a thread pool)\n");

// Cleanup
sem_destroy(&counting_semaphore);
pthread_mutex_destroy(&mutex);

return 0;
```

}
