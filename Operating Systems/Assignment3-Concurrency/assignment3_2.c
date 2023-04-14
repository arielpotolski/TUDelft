/*
Let’s start with having a look at what happens when we have ’consumer’ running concurrently.

Copy your solution from assignment 1. Click on Spec Tests to run the multi threaded test (or write your own if you like,
using the provided library code). The test will spawn a lot of threads that add messages to the queue, and then dequeues all of them.
*/
#include "library.c"
#include <stdbool.h>
#include <pthread.h>

Queue queue;

void init() {
    queue_init(&queue);
}

/// producer gets a message to add to the queue
void producer(Message message) {
    enqueue(&queue, message);
}

/// called continuously. Return false when no message could be
/// consumed from the queue. Otherwise, writes the message to
/// `message`
bool consumer(Message *message) {
    if (queue_size(&queue) != 0) {
        dequeue(&queue, message);
        return true;
    }
    return false;
}