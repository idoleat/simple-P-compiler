#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "AST/expression.h"

typedef struct __ConstantValueNode ConstantValueNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IConstantValueNode {
    void (*construct)(ConstantValueNode *self, const uint32_t line,
                      const uint32_t col /* TODO: constant value */);
    DestructFPtr destruct;

    ConstantValueNode *(*operatorNew)(const uint32_t line, const uint32_t col
                                      /* TODO: constant value */);
    void (*operatorDelete)(ConstantValueNode *constant_value_node);

    PrintFPtr print;
} IConstantValueNode;

extern const IConstantValueNode gConstantValueNode;

extern const size_t kConstantValueNodeSize;

void freeConstantValueNode(void *node);

#endif
