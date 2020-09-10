#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __ReadNode ReadNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IReadNode {
    void (*construct)(ReadNode *self, const uint32_t line, const uint32_t col
                      /* TODO: variable reference */);
    DestructFPtr destruct;

    ReadNode *(*operatorNew)(const uint32_t line, const uint32_t col
                             /* TODO: variable reference */);
    void (*operatorDelete)(ReadNode *read_node);

    PrintFPtr print;
} IReadNode;

extern const IReadNode gReadNode;

extern const size_t kReadNodeSize;

void freeReadNode(void *node);

#endif
