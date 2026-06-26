/*

- Practical Session 4: Shared Memory Programming
- CSC 308 - Operating Systems | Week 8
- 
- Objective: Implement inter-process communication using shared memory
- Compile: gcc -o session4_shared_memory session4_shared_memory.c -lpthread
- 
- Applications: Database servers, Web servers, Scientific computing, Real-time systems
- Note: Shared memory + semaphores = foundation of high-performance IPC in Linux
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>    // shmget, shmat, shmdt, shmctl
#include <sys/sem.h>    // semget, semop, semctl (System V semaphores)
#include <sys/wait.h>

#define SHM_SIZE 1024   // Shared memory size in bytes
#define SEM_KEY  0x1234
#define SHM_KEY  0x5678

// Semaphore union required for semctl
union semun {
int val;
struct semid_ds *buf;
unsigned short *array;
};

// Helper: Lock semaphore (P operation / sem_wait)
void sem_lock(int semid) {
struct sembuf op = {0, -1, 0};  // Decrement by 1
semop(semid, &op, 1);
}

// Helper: Unlock semaphore (V operation / sem_post)
void sem_unlock(int semid) {
struct sembuf op = {0, 1, 0};   // Increment by 1
semop(semid, &op, 1);
}

int main() {
printf(”=== Practical Session 4: Shared Memory Programming ===\n”);
printf(“Note: Shared memory + semaphores = foundation of high-performance IPC in Linux\n\n”);

```
// -------------------------------------------------------
// Step 1: Use shmget() to create a shared memory segment
// -------------------------------------------------------
int shmid = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
if (shmid == -1) {
    perror("shmget failed");
    exit(1);
}
printf("[Main] Shared memory created. ID: %d\n", shmid);

// -------------------------------------------------------
// Step 4: Use semaphores to synchronize access to shared memory
// -------------------------------------------------------
int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666);
if (semid == -1) {
    perror("semget failed");
    exit(1);
}

// Initialize semaphore to 1 (unlocked)
union semun arg;
arg.val = 1;
semctl(semid, 0, SETVAL, arg);
printf("[Main] Semaphore initialized.\n\n");

// Fork: create child process
pid_t pid = fork();

if (pid < 0) {
    perror("fork failed");
    exit(1);
}

// ============================================================
// PARENT PROCESS: WRITER
// ============================================================
if (pid > 0) {
    // Step 2: Use shmat() to attach memory in parent process
    char *shm_ptr = (char *)shmat(shmid, NULL, 0);
    if (shm_ptr == (char *)-1) {
        perror("shmat failed (parent)");
        exit(1);
    }
    printf("[Parent] Attached to shared memory at address: %p\n", (void *)shm_ptr);

    // Step 3: Write data from parent process
    sem_lock(semid);  // Acquire lock before writing
    printf("[Parent] Writing to shared memory...\n");
    snprintf(shm_ptr, SHM_SIZE,
             "Hello from Parent (PID=%d)! "
             "Shared memory IPC is fast because NO kernel copy is needed!", getpid());
    printf("[Parent] Written: \"%s\"\n", shm_ptr);
    sem_unlock(semid);  // Release lock after writing

    // Wait for child to finish reading
    wait(NULL);

    // Step 5: Clean up with shmdt()
    shmdt(shm_ptr);
    printf("[Parent] Detached from shared memory.\n");

    // Step 5: Clean up with shmctl(IPC_RMID)
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);
    printf("[Parent] Shared memory and semaphore destroyed. Cleanup complete.\n");

// ============================================================
// CHILD PROCESS: READER
// ============================================================
} else {
    sleep(1);  // Give parent time to write first

    // Step 2: Use shmat() to attach memory in child process
    char *shm_ptr = (char *)shmat(shmid, NULL, SHM_RDONLY);
    if (shm_ptr == (char *)-1) {
        perror("shmat failed (child)");
        exit(1);
    }
    printf("[Child]  Attached to shared memory at address: %p\n", (void *)shm_ptr);

    // Step 3: Read data from shared memory written by parent
    sem_lock(semid);  // Acquire lock before reading
    printf("[Child]  Reading from shared memory...\n");
    printf("[Child]  Read: \"%s\"\n", shm_ptr);
    sem_unlock(semid);  // Release lock after reading

    // Step 5: Detach (child does NOT destroy — parent handles that)
    shmdt(shm_ptr);
    printf("[Child]  Detached from shared memory.\n");
}

printf("\n=== Applications ===\n");
printf("Database servers  : Multiple processes share a buffer pool\n");
printf("Web servers       : Shared cache between worker processes\n");
printf("Scientific computing: Large data sets shared across processes\n");
printf("Real-time systems : Low-latency IPC between processes\n");

return 0;
```

}
