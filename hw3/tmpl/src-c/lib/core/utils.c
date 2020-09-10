#include "core/utils.h"

#include <assert.h>
#include <stdlib.h>

void *myMalloc(size_t size) {
    void *ptr = malloc(size);
    assert(ptr != NULL);
    return ptr;
}

void *myCalloc(size_t nmemb, size_t size) {
    void *ptr = calloc(nmemb, size);
    assert(ptr != NULL);
    return ptr;
}

void *myRealloc(void *ptr, size_t size) {
    void *NewPtr = realloc(ptr, size);
    assert(ptr != NULL);
    return NewPtr;
}
