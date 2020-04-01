#include <atomic>
#include <cstdlib>
#include <unistd.h>

#include  "lock.h"

struct lock
{
    std::atomic<unsigned> val;
};


lock_t* lock_alloc(long unsigned n_threads)
{
    lock_t* lock;
    if( ! ( lock = (lock_t*) calloc(1, sizeof(lock_t))))
    {
            return NULL;
    }
    lock -> val.store(0);
    return lock;
}


int lock_acquire(lock_t * lock)
{
    unsigned expected = 0;
    int i = 0;
    do
    {
        usleep((i + 100 / i) * 10);
        i++;
        while(lock -> val.load(std::memory_order_acquire));
        expected = 0;
    } while( !lock -> val.compare_exchange_weak(expected, 1, std::memory_order_release));
    return 0;
}


int lock_release(lock_t * lock)
{
    lock -> val.store(0, std::memory_order_release);
    return 0;
}


int lock_free(lock_t* lock)
{
    free(lock);
    return 0;
}
