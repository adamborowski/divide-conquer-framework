#include "OmpLock.h"

OmpLock::OmpLock() {
    owner = -1;
    omp_init_lock(&_lock);
}

OmpLock::~OmpLock() {
    omp_destroy_lock(&_lock);
}

void OmpLock::lock() {
    omp_set_lock(&_lock);
    owner = omp_get_thread_num();
}

void OmpLock::unlock() {
    if (owner == omp_get_thread_num()) {
        owner = -1;
        omp_unset_lock(&_lock);
    }
    else if (owner == -1) {
        throw "cannot unlock: lock is free";
    }
    else {
        throw "cannot unlock from another thread";
    }
}

bool OmpLock::isLocked() {
    return owner >= 0;
}
