/*
Now that you have implemented a mutex using a semaphore, let’s have a look at how you can implement a semaphore using a mutex.

Recall that a semaphore is a method to ensure that multiple processes have access to a critical section.

In the solution template you can already find a struct that should give you an idea about how to get started.
Use the mutex to protect the counter from concurrent access. You should not need to add any fields to the struct.
*/

#include "library.c"
#include <stdbool.h>
#include <pthread.h>

Queue queue;

typedef struct {
    pthread_mutex_t mut;
    int count;
} sem_t;

// Initializes the semaphore count to n
void sem_init(sem_t *semaphore, int n){
    pthread_mutex_init(&semaphore->mut, NULL);
    semaphore->count = n;
}

// like sem_post
void sem_release(sem_t *semaphore){
    pthread_mutex_lock(&semaphore->mut);
    (semaphore->count)++; //increments semaphore
    pthread_mutex_unlock(&(semaphore->mut));
}

// like sem_wait
void sem_acquire(sem_t *semaphore){
    pthread_mutex_lock(&(semaphore->mut));

    while (semaphore->count <= 0) {
        pthread_mutex_unlock(&semaphore->mut);
        pthread_mutex_lock(&semaphore->mut);
    }
    (semaphore->count)--;
    pthread_mutex_unlock(&semaphore->mut);
}

// NOTE: The part below this line you can copy from the previous exercise.
sem_t sem;

void init() {
    queue_init(&queue);
    sem_init(&sem, 1);  // initializes a semaphore with value 1.
}

/// producer gets a message to add to the queue
void producer(Message message) {
    sem_acquire(&sem); // if semaphore value is negative, wait. Else, decrement.
    enqueue(&queue, message);
    sem_release(&sem); //increments and wakes up any threads waiting on the semaphore
}

/// called continuously. Return false when no message could be
/// consumed from the queue. Otherwise, writes the message to
/// `message`
bool consumer(Message *message) {
    sem_acquire(&sem); // if semaphore value is negative, wait. Else, decrement.
    if (queue_size(&queue) != 0) {  //check if the queue contains any messages
        dequeue(&queue, message);
        sem_release(&sem); //increments and wakes up any threads waiting on the semaphore
        return true;
    }
    sem_release(&sem); //increments and wakes up any threads waiting on the semaphore
    return false;
}