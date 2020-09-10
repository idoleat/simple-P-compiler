#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __WhileNode WhileNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IWhileNode {
    void (*construct)(WhileNode *self, const uint32_t line, const uint32_t col
                      /* TODO: expression, compound statement */);
    DestructFPtr destruct;

    WhileNode *(*operatorNew)(const uint32_t line, const uint32_t col
                              /* TODO: expression, compound statement */);
    void (*operatorDelete)(WhileNode *while_node);

    PrintFPtr print;
} IWhileNode;

extern const IWhileNode gWhileNode;

extern const size_t kWhileNodeSize;

void freeWhileNode(void *node);

#endif
