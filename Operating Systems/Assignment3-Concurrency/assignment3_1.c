/*In session 4 of the lab, you will explore multiple methods of ensuring thread safety.
You are given a (non thread-safe) implementation of a FIFO queue, into which a single producer process wants to write messages,
and from which multiple consumer processes want to read messages.

In your solution you can find three functions (producer, consumer and init) and a global variable (queue).
The provided library will call these functions while running tests on your code. The init function is called at the start of every
test and is already complete (i.e. you don’t need to change it).

For now, let’s stick to the basics and just get things working for a single-threaded (non-concurrent) application.
The producer function is called by tests with a message which you are supposed to put in the queue. consumer is then called by the
tests to retrieve a message. You are supposed to return the message at the front of the queue.

You can find the implementation of the Queue in the Library tab.

Task: Implement both the consumer and producer functions and make sure the single-threaded tests succeed.
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