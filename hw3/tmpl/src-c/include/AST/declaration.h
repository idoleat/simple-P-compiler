#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.h"

typedef struct __DeclNode DeclNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IDeclNode {
    void (*construct)(DeclNode *self, const uint32_t line, const uint32_t col
                      /* TODO: identifiers, type, constant (optional)*/);
    DestructFPtr destruct;

    DeclNode *(*operatorNew)(const uint32_t line, const uint32_t col
                             /* TODO: identifiers, type, constant (optional)*/);
    void (*operatorDelete)(DeclNode *decl_node);

    PrintFPtr print;
} IDeclNode;

extern const IDeclNode gDeclNode;

extern const size_t kDeclNodeSize;

void freeDeclNode(void *node);

#endif
