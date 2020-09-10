#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.h"

#include <stdint.h>

typedef struct __FunctionInvocationNode FunctionInvocationNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IFunctionInvocationNode {
    void (*construct)(FunctionInvocationNode *self, const uint32_t line,
                      const uint32_t col
                      /* TODO: function name, expressions */);
    DestructFPtr destruct;

    FunctionInvocationNode *(*operatorNew)(const uint32_t line,
                                           const uint32_t col
                                           /* TODO: function name, expressions */);
    void (*operatorDelete)(FunctionInvocationNode *func_invocation_node);

    PrintFPtr print;
} IFunctionInvocationNode;

extern const IFunctionInvocationNode gFunctionInvocationNode;

extern const size_t kFunctionInvocationNodeSize;

void freeFunctionInvocationNode(void *node);

#endif
