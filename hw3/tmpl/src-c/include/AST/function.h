#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/ast.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct __FunctionNode FunctionNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IFunctionNode {
    void (*construct)(FunctionNode *self, const uint32_t line,
                      const uint32_t col
                      /* TODO: name, declarations, return type,
                       *       compound statement */);
    DestructFPtr destruct;

    FunctionNode *(*operatorNew)(const uint32_t line, const uint32_t col
                                 /* TODO: name, declarations, return type,
                                  *       compound statement (optional) */);
    void (*operatorDelete)(FunctionNode *func_node);

    PrintFPtr print;
} IFunctionNode;

extern const IFunctionNode gFunctionNode;

extern const size_t kFunctionNodeSize;

void freeFunctionNode(void *node);

#endif
