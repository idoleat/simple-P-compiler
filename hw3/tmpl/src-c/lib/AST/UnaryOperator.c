#include "AST/UnaryOperator.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __UnaryOperatorNode {
    ANONYMOUS_EXPRESSION_NODE;

    // TODO: operator, expression
} UnaryOperatorNode;

const size_t kUnaryOperatorNodeSize = sizeof(UnaryOperatorNode);

static void destruct(UnaryOperatorNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: operator, expression

    gExpressionNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(UnaryOperatorNode *self) {}

// static void visitChildNodes(UnaryOperatorNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(UnaryOperatorNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gExpressionNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static UnaryOperatorNode *newUnaryOperatorNode(const uint32_t line,
                                               const uint32_t col) {
    UnaryOperatorNode *node =
        (UnaryOperatorNode *)myMalloc(sizeof(UnaryOperatorNode));
    construct(node, line, col);
    return node;
}

void freeUnaryOperatorNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IUnaryOperatorNode gUnaryOperatorNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newUnaryOperatorNode,
    .operatorDelete = (void (*)(UnaryOperatorNode *))freeUnaryOperatorNode,
    .print = (PrintFPtr)print};
