/*
Let’s actually start looking at how we can solve this. In the lectures, we have talked about the principle of mutual exclusion,
locks and barriers, but let’s see how we can actually apply these to solve the problem at hand.

To ensure that consumer and producer can run concurrently, the simplest thing we can do is to use Mutex.
A Mutex (or lock) can be in one of two states, locked and unlocked. When locked, no other thread can lock it as well.
This provides mutual exclusion (hence the name). Only when one thread unlocks the lock can another lock it.

Use a Mutex (pthread_mutex_t) to fix the consumer and producer.
*/

#include "library.c"
#include <stdbool.h>
#include <pthread.h>

Queue queue;
pthread_mutex_t lock;

void init() {
    queue_init(&queue);
}

/// producer gets a message to add to the queue
void producer(Message message) {
    pthread_mutex_lock(&lock);  // acquire lock
    enqueue(&queue, message);
    pthread_mutex_unlock(&lock);  // release lock
}

/// called continuously. Return false when no message could be
/// consumed from the queue. Otherwise, writes the message to
/// `message`
bool consumer(Message *message) {
    if (queue_size(&queue) != 0) {
        pthread_mutex_lock(&lock);  // acquire lock
        dequeue(&queue, message);
        pthread_mutex_unlock(&lock);  // release lock
        return true;
    }
    return false;
}