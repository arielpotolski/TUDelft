/*
Now that you’ve familiarized yourself with the structure of the database and made it thread safe,
let us consider some usecases of the database.

Quite often, a database is read much more often than it is written to. In fact, the test suite performs 8x
as many reads as writes (and calls both the consumer and producer functions in parallel).

In such a case, a so-called Read-Write lock may prove more performant. Repeat the previous assignment,
but use read-write locks instead of mutexes for thread safety.
*/

#include "library.c"
#include <stdbool.h>
#include <pthread.h>

Database db;
pthread_rwlock_t lock;

void init() {
    db_init(&db);
}

/// producer gets a message to add to the database at key k
void producer(Key k, Message message) {
    pthread_rwlock_wrlock(&lock);
    db_put(&db, k, message);
    pthread_rwlock_unlock(&lock);
}

/// Tries to get values from the database. The same key
/// may be requested multiple times, and concurrently.
/// Should return false when the message is not found in the 
/// database (use `db_contains`) and should return true and 
/// write the corresponding message to `message` when the message
/// is found in the database.
bool reader(Key k, Message *message) {
    if (db_contains(&db, k)) {
        pthread_rwlock_rdlock(&lock);
        *message = db_get(&db, k);
        pthread_rwlock_unlock(&lock);
        return true;
    }
    return false;
}