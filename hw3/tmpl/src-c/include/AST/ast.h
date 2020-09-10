#ifndef __AST_AST_NODE_H
#define __AST_AST_NODE_H

#include "OoUtils.h" // for macro MEMBER_FUNCTION_PROLOGUE

#include <stddef.h> // for declaring size_t in each derived module
#include <stdint.h>

// these function pointers are left for derived class to populate
#define AST_NODE_VIRTUAL_FUNCTIONS                                             \
    DestructFPtr destruct;                                                     \
    PrintFPtr print

#define AST_NODE_DATA_MEMBERS const Location location

#define AST_NODE_MEMBERS                                                       \
    AST_NODE_VIRTUAL_FUNCTIONS;                                                \
    AST_NODE_DATA_MEMBERS

// use anonymous struct (introduced in C11) to implement inheritance
#define ANONYMOUS_AST_NODE                                                     \
    struct {                                                                   \
        AST_NODE_MEMBERS;                                                      \
    }

/******************************************************************************
 * > typedef
 *   - xxxFPtr is defined for derived class to perform casting
 *     - void * for achieving upcasting
 ******************************************************************************/
typedef void (*DestructFPtr)(void *self);
typedef void (*PrintFPtr)(void *self);

typedef struct __Location {
    uint32_t line;
    uint32_t col;
} Location;

typedef struct __AstNode AstNode;

/******************************************************************************
 * > Interface related
 *   - destruct is used for dispatching to the real destructor
 *   - destructImpl is the real destructor of AstNode
 *   - gXXXXX is the interface variable, used for invoking member functions
 *   - freeXXXX is opened so that some containers can release the memory
 *     correctly
 *   - kXXXSize is for containers to copy (no need for abstract struct AstNode)
 ******************************************************************************/
typedef struct __IAstNode {
    void (*construct)(void *self, const uint32_t line, const uint32_t col);
    DestructFPtr destruct;
    DestructFPtr destructImpl;

    const Location *(*getLocation)(void *self) __attribute__((const));
    PrintFPtr print;
} IAstNode;

extern const IAstNode gAstNode;

void freeAstNode(void *node);

#endif
