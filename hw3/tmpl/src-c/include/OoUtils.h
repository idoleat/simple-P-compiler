#ifndef __OO_UTILS_H
#define __OO_UTILS_H

#include <assert.h>

#define MEMBER_FUNCTION_PROLOGUE                                               \
    assert(self != NULL && "Cannot call a member function on a NULL object")

#endif
