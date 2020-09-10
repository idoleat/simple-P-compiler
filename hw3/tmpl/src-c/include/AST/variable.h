#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ast.h"

typedef struct __VariableNode VariableNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IVariableNode {
    void (*construct)(VariableNode *self, const uint32_t line,
                      const uint32_t col
                      /* TODO: variable name, type, constant value */);
    DestructFPtr destruct;

    VariableNode *(*operatorNew)(const uint32_t line, const uint32_t col
                                 /* TODO: variable name, type, constant value */);
    void (*operatorDelete)(VariableNode *var_node);

    PrintFPtr print;
} IVariableNode;

extern const IVariableNode gVariableNode;

extern const size_t kVariableNodeSize;

void freeVariableNode(void *node);

#endif
