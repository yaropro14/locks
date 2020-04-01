#include <atomic>
#include <cstdlib>
#include <unistd.h>

#include "lock.h"

struct lock
{
        std::atomic_size_t newServing;
        std::atomic_size_t nextTicket;
};


lock_t* lock_alloc(long unsigned n_threads)
{
        lock_t* lock;
        if(! (lock = (lock_t*) calloc(1, sizeof(lock_t))))
        {
                return NULL;
        }
        lock -> newServing.store(0, std::memory_order_release);
        lock -> nextTicket.store(0, std::memory_order_release);
        return lock;
}

int lock_free(lock_t* lock)
{
        free(lock);
        return 0;
}


int lock_acquire(lock_t * lock)
{
    unsigned i = 0;
    const auto ticket = lock -> nextTicket.fetch_add(1);
    while(lock -> newServing.load(std::memory_order_acquire) != ticket)
    {
        usleep((i + 100 / i) * 10);
        i++;
    }
    return 0;
}


int lock_release(lock_t * lock)
{
        const auto successor = lock -> newServing.load(std::memory_order_acquire) + 1;
        lock -> newServing.store(successor, std::memory_order_release);
        return 0;
}
