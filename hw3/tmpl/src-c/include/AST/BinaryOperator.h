#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.h"

#include <stdint.h>

typedef struct __BinaryOperatorNode BinaryOperatorNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IBinaryOperatorNode {
    void (*construct)(BinaryOperatorNode *self, const uint32_t line,
                      const uint32_t col
                      /* TODO: operator, expressions */);
    DestructFPtr destruct;

    BinaryOperatorNode *(*operatorNew)(const uint32_t line, const uint32_t col
                                       /* TODO: operator, expressions */);
    void (*operatorDelete)(BinaryOperatorNode *bin_op_node);

    PrintFPtr print;
} IBinaryOperatorNode;

extern const IBinaryOperatorNode gBinaryOperatorNode;

extern const size_t kBinaryOperatorNodeSize;

void freeBinaryOperatorNode(void *node);

#endif
