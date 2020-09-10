#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.h"

#include <stdint.h>

typedef struct __VariableReferenceNode VariableReferenceNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IVariableReferenceNode {
    void (*construct)(VariableReferenceNode *self, const uint32_t line,
                      const uint32_t col
                      /* TODO: name, expressions (optional) */);
    DestructFPtr destruct;

    VariableReferenceNode *(*operatorNew)(const uint32_t line,
                                          const uint32_t col
                                          /* TODO: name, expressions (optional) */);
    void (*operatorDelete)(VariableReferenceNode *var_ref_node);

    PrintFPtr print;
} IVariableReferenceNode;

extern const IVariableReferenceNode gVariableReferenceNode;

extern const size_t kVariableReferenceNodeSize;

void freeVariableReferenceNode(void *node);

#endif
