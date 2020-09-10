#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __IfNode IfNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IIfNode {
    void (*construct)(IfNode *self, const uint32_t line, const uint32_t col
                      /* TODO: expression, compound statement, compound statement */);
    DestructFPtr destruct;

    IfNode *(*operatorNew)(const uint32_t line, const uint32_t col
                           /* TODO: expression, compound statement,
                            *       compound statement */);
    void (*operatorDelete)(IfNode *if_node);

    PrintFPtr print;
} IIfNode;

extern const IIfNode gIfNode;

extern const size_t kIfNodeSize;

void freeIfNode(void *node);

#endif
