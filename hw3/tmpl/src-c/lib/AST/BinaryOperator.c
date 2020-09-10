#include "AST/BinaryOperator.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __BinaryOperatorNode {
    ANONYMOUS_EXPRESSION_NODE;

    // TODO: operator, expressions
} BinaryOperatorNode;

const size_t kBinaryOperatorNodeSize = sizeof(BinaryOperatorNode);

static void destruct(BinaryOperatorNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: operator, expressions

    gExpressionNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(BinaryOperatorNode *self) {}

// static void visitChildNodes(BinaryOperatorNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
//
//     // TODO: operator, expressions
// }

// TODO
static void construct(BinaryOperatorNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gExpressionNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static BinaryOperatorNode *
newBinaryOperatorNode(const uint32_t line, const uint32_t col) {
    BinaryOperatorNode *node =
        (BinaryOperatorNode *)myMalloc(sizeof(BinaryOperatorNode));
    construct(node, line, col);
    return node;
}

void freeBinaryOperatorNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IBinaryOperatorNode gBinaryOperatorNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newBinaryOperatorNode,
    .operatorDelete = (void (*)(BinaryOperatorNode *))freeBinaryOperatorNode,
    .print = (PrintFPtr)print};
