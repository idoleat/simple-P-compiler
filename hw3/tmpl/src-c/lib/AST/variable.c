#include "AST/variable.h"
#include "OoUtils.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __VariableNode {
    ANONYMOUS_AST_NODE;

    // TODO: name, type, constant value
} VariableNode;

const size_t kVariableNodeSize = sizeof(VariableNode);

static void destruct(VariableNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: name, type, constant value

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(VariableNode *self) {}

// static void visitChildNodes(VariableNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(VariableNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static VariableNode *newVariableNode(const uint32_t line, const uint32_t col) {
    VariableNode *node = (VariableNode *)myMalloc(sizeof(VariableNode));
    construct(node, line, col);
    return node;
}

void freeVariableNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IVariableNode gVariableNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newVariableNode,
    .operatorDelete = (void (*)(VariableNode *))freeVariableNode,
    .print = (PrintFPtr)print};
