#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __PrintNode PrintNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IPrintNode {
    void (*construct)(PrintNode *self, const uint32_t line, const uint32_t col
                      /* TODO: expression */);
    DestructFPtr destruct;

    PrintNode *(*operatorNew)(const uint32_t line, const uint32_t col
                              /* TODO: expression */);
    void (*operatorDelete)(PrintNode *print_node);

    PrintFPtr print;
} IPrintNode;

extern const IPrintNode gPrintNode;

extern const size_t kPrintNodeSize;

void freePrintNode(void *node);

#endif
