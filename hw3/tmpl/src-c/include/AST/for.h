#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __ForNode ForNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IForNode {
    void (*construct)(ForNode *self, const uint32_t line, const uint32_t col
                      /* TODO: declaration, assignment, expression,
                       *       compound statement */);
    DestructFPtr destruct;

    ForNode *(*operatorNew)(const uint32_t line, const uint32_t col
                            /* TODO: declaration, assignment, expression,
                             *       compound statement */);
    void (*operatorDelete)(ForNode *for_node);

    PrintFPtr print;
} IForNode;

extern const IForNode gForNode;

extern const size_t kForNodeSize;

void freeForNode(void *node);

#endif
