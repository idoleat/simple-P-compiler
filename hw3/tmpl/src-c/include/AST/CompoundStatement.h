#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __CompoundStatementNode CompoundStatementNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __ICompoundStatementNode {
    void (*construct)(CompoundStatementNode *self, const uint32_t line,
                      const uint32_t col
                      /* TODO: declarations, statements */);
    DestructFPtr destruct;

    CompoundStatementNode *(*operatorNew)(const uint32_t line,
                                          const uint32_t col
                                          /* TODO: declarations, statements */);
    void (*operatorDelete)(CompoundStatementNode *compound_statement_node);

    PrintFPtr print;
} ICompoundStatementNode;

extern const ICompoundStatementNode gCompoundStatementNode;

extern const size_t kCompoundStatementNodeSize;

void freeCompoundStatementNode(void *node);

#endif
