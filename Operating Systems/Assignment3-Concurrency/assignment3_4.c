/*
In the previous three assignments, we’ve seen the need for mutual exclusion. However, a mutex is not the only way to achieve exclusivity.

In the lectures, a number of other options are discussed. In this assignment we will take a look at the semaphore in particular.
You could say that a semaphore is a more general version of a mutex. Semaphores are variables that, in an atomic manner,
can be incremented and decremented. However, when a semaphore’s value goes negative, no more decrement operations are possible,
and processes that want to decrement the semaphore will need to wait.

Your assignment is to make sure that all the messages are transferred from the producer to the consumer without using a mutex.
Instead, ensure thread-safety with a semaphore (sem_t). sem_t is already included through semaphore.h
*/

#include "library.c"
#include <stdbool.h>
#include <semaphore.h>

Queue queue;
sem_t sem;

void init() {
    queue_init(&queue);
    sem_init(&sem, 0, 1);  // initializes a semaphore with value 1.
                           // The zero indicates that the semaphore is shared between threads.
}

/// producer gets a message to add to the queue
void producer(Message message) {
    sem_wait(&sem); // if semaphore value is negative, wait. Else, decrement.
    enqueue(&queue, message);
    sem_post(&sem); //increments and wakes up any threads waiting on the semaphore
}

/// called continuously. Return false when no message could be
/// consumed from the queue. Otherwise, writes the message to
/// `message`
bool consumer(Message *message) {
    if (queue_size(&queue) != 0) {  //check if the queue contains any messages
        sem_wait(&sem); // if semaphore value is negative, wait. Else, decrement.
        dequeue(&queue, message);
        sem_post(&sem); //increments and wakes up any threads waiting on the semaphore
        return true;
    }
    return false;
}