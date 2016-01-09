#include "OmpLock.h"

OmpLock::OmpLock() {
    owner = false;
    omp_init_lock(&_lock);
}

OmpLock::~OmpLock() {
    omp_destroy_lock(&_lock);
}

void OmpLock::lock() {
    omp_set_lock(&_lock);
    owner = true;
}

void OmpLock::unlock() {
    if (owner) {
        owner = false;
        omp_unset_lock(&_lock);
    }
    else {
        throw "cannot unlock from another thread or lock is free";
    }
}
