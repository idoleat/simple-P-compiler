#include "AST/assignment.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __AssignmentNode {
    ANONYMOUS_AST_NODE;

    // TODO: variable reference, expression
} AssignmentNode;

const size_t kAssignmentNodeSize = sizeof(AssignmentNode);

static void destruct(AssignmentNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: variable reference, expression

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(AssignmentNode *self) {}

// static void visitChildNodes(AssignmentNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(AssignmentNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static AssignmentNode *newAssignmentNode(const uint32_t line,
                                         const uint32_t col) {
    AssignmentNode *node = (AssignmentNode *)myMalloc(sizeof(AssignmentNode));
    construct(node, line, col);
    return node;
}

void freeAssignmentNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IAssignmentNode gAssignmentNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newAssignmentNode,
    .operatorDelete = (void (*)(AssignmentNode *))freeAssignmentNode,
    .print = (PrintFPtr)print};
