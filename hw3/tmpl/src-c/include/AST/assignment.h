#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __AssignmentNode AssignmentNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IAssignmentNode {
    void (*construct)(AssignmentNode *self, const uint32_t line,
                      const uint32_t col
                      /* TODO: variable reference, expression */);
    DestructFPtr destruct;

    AssignmentNode *(*operatorNew)(const uint32_t line, const uint32_t col
                                   /* TODO: variable reference, expression */);
                      
    void (*operatorDelete)(AssignmentNode *assignment_node);

    PrintFPtr print;
} IAssignmentNode;

extern const IAssignmentNode gAssignmentNode;

extern const size_t kAssignmentNodeSize;

void freeAssignmentNode(void *node);

#endif
