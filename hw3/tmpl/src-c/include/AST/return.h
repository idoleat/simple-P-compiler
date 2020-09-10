#ifndef __AST_RETURN_NODE_H
#define __AST_RETURN_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __ReturnNode ReturnNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IReturnNode {
    void (*construct)(ReturnNode *self, const uint32_t line, const uint32_t col
                      /* TODO: expression */);
    DestructFPtr destruct;

    ReturnNode *(*operatorNew)(const uint32_t line, const uint32_t col
                               /* TODO: expression */);
    void (*operatorDelete)(ReturnNode *return_node);

    PrintFPtr print;
} IReturnNode;

extern const IReturnNode gReturnNode;

extern const size_t kReturnNodeSize;

void freeReturnNode(void *node);

#endif
