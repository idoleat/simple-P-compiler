#include "AST/VariableReference.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __VariableReferenceNode {
    ANONYMOUS_EXPRESSION_NODE;

    // TODO: name, expressions (optional)
} VariableReferenceNode;

const size_t kVariableReferenceNodeSize = sizeof(VariableReferenceNode);

static void destruct(VariableReferenceNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: name, expressions (optional)

    gExpressionNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(VariableReferenceNode *self) {}

// static void visitChildNodes(VariableReferenceNode *self,
//                             AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(VariableReferenceNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gExpressionNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static VariableReferenceNode *
newVariableReferenceNode(const uint32_t line, const uint32_t col) {
    VariableReferenceNode *node =
        (VariableReferenceNode *)myMalloc(sizeof(VariableReferenceNode));
    construct(node, line, col);
    return node;
}

void freeVariableReferenceNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IVariableReferenceNode gVariableReferenceNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newVariableReferenceNode,
    .operatorDelete =
        (void (*)(VariableReferenceNode *))freeVariableReferenceNode,
    .print = (PrintFPtr)print};
