#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.h"

#include <stdint.h>

typedef struct __UnaryOperatorNode UnaryOperatorNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IUnaryOperatorNode {
    void (*construct)(UnaryOperatorNode *self, const uint32_t line,
                      const uint32_t col
                      /* TODO: operator, expression */);
    DestructFPtr destruct;

    UnaryOperatorNode *(*operatorNew)(const uint32_t line, const uint32_t col
                                      /* TODO: operator, expression */);
    void (*operatorDelete)(UnaryOperatorNode *un_op_node);

    PrintFPtr print;
} IUnaryOperatorNode;

extern const IUnaryOperatorNode gUnaryOperatorNode;

extern const size_t kUnaryOperatorNodeSize;

void freeUnaryOperatorNode(void *node);

#endif
