#include "AST/program.h"
#include "OoUtils.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __ProgramNode {
    ANONYMOUS_AST_NODE;

    const char *name;
    // TODO: return type, declarations, functions, compound statement
} ProgramNode;

const size_t kProgramNodeSize = sizeof(ProgramNode);

static void destruct(ProgramNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    free((void *)self->name);
    // TODO: return type, declarations, functions, compound statement

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(ProgramNode *self) {
    // TODO
    // outputIndentationSpace();

    fprintf(stdout, "program <line: %u, col: %u> %s %s\n",
            self->location.line,
            self->location.col,
            self->name, "void");

    // TODO
    // incrementIndentation();
    // visitChildNodes();
    // decrementIndentation();

}

// static void visitChildNodes(ProgramNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(ProgramNode *self, const uint32_t line,
                      const uint32_t col, char *name) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;

    self->name = name;
}

// TODO
static ProgramNode *newProgramNode(const uint32_t line, const uint32_t col,
                                   char *name) {
    ProgramNode *node = (ProgramNode *)myMalloc(sizeof(ProgramNode));
    construct(node, line, col, name);
    return node;
}

void freeProgramNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IProgramNode gProgramNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newProgramNode,
    .operatorDelete = (void (*)(ProgramNode *))freeProgramNode,
    .print = (PrintFPtr)print};
