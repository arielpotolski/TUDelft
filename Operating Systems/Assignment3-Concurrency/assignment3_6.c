/*
In this assignment we will switch things up a bit. Instead of using a queue to store our messages,
we will now continue with a database. To be honest, it’s not really a database, just a very simple analogue.
The database supports 4 actions: db_init, db_contains, db_put and db_get (see the library tab).

This assignment serves to get some familiarity with the database interface, we will get to using it in the next ones.
Make sure that, in a thread-safe manner, the producer function puts the requested elements in the database,
and the reader function retrieves them from the database again (in contrast to the consumer, it does not remove values).
*/

#include "library.c"
#include <stdbool.h>
#include <pthread.h>

Database db;
pthread_mutex_t lock;

void init() {
    db_init(&db);
}

/// producer gets a message to add to the database at key k
void producer(Key k, Message message) {
    pthread_mutex_lock(&lock);
    db_put(&db, k, message);
    pthread_mutex_unlock(&lock);
}

/// Tries to get values from the database. The same key
/// may be requested multiple times, and concurrently.
/// Should return false when the message is not found in the 
/// database (use `db_contains`) and should return true and 
/// write the corresponding message to `message` when the message
/// is found in the database.
bool reader(Key k, Message *message) {
    if (db_contains(&db, k)) {
        pthread_mutex_lock(&lock);
        *message = db_get(&db, k);
        pthread_mutex_unlock(&lock);
        return true;
    }
    return false;
}