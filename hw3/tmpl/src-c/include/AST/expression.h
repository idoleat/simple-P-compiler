#ifndef __AST_EXPRESSION_NODE_H
#define __AST_EXPRESSION_NODE_H

#include "AST/ast.h"

#define EXPRESSION_NODE_MEMBERS                                                \
    ANONYMOUS_AST_NODE

#define ANONYMOUS_EXPRESSION_NODE                                              \
    struct {                                                                   \
        EXPRESSION_NODE_MEMBERS;                                               \
    }

struct __ExpressionNode;

typedef struct __ExpressionNode ExpressionNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 *   - this struct will also be inherited, prepare destructImpl as real
 *     destructor
 ******************************************************************************/
typedef struct __IExpressionNode {
    void (*construct)(void *self, const uint32_t line, const uint32_t col);
    DestructFPtr destruct;
    DestructFPtr destructImpl;

    void (*operatorDelete)(ExpressionNode *node);
    PrintFPtr print;
} IExpressionNode;

extern const IExpressionNode gExpressionNode;

void freeExpressionNode(void *node);

#endif
