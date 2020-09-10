#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include "AST/ast.h"

#include <stdint.h>

typedef struct __ProgramNode ProgramNode;

/******************************************************************************
 * > Interface related
 *   - check ast.h for more info
 ******************************************************************************/
typedef struct __IProgramNode {
    void (*construct)(ProgramNode *self, const uint32_t line,
                      const uint32_t col, char *name
                      /* TODO: return type, declarations, functions,
                       *       compound statement */);
    DestructFPtr destruct;

    struct __ProgramNode *(*operatorNew)(const uint32_t line,
                                         const uint32_t col,
                                         char *name
                                         /* TODO: return type, declarations, functions,
                                          *       compound statement */);
    void (*operatorDelete)(ProgramNode *program_node);

    PrintFPtr print;
} IProgramNode;

extern const IProgramNode gProgramNode;

extern const size_t kProgramNodeSize;

void freeProgramNode(void *node);

#endif
