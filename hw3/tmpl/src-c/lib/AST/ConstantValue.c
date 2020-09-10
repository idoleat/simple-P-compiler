#include "AST/ConstantValue.h"
#include "OoUtils.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __ConstantValueNode {
    ANONYMOUS_EXPRESSION_NODE;

    // TODO: constant value
} ConstantValueNode;

extern const size_t kConstantValueNodeSize;

static void destruct(ConstantValueNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: constant value

    gExpressionNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(ConstantValueNode *self) {}

// TODO
static void construct(ConstantValueNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gExpressionNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static ConstantValueNode *newConstantValueNode(const uint32_t line,
                                               const uint32_t col) {
    ConstantValueNode *node =
        (ConstantValueNode *)myMalloc(sizeof(ConstantValueNode));
    construct(node, line, col);
    return node;
}

void freeConstantValueNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IConstantValueNode gConstantValueNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newConstantValueNode,
    .operatorDelete = (void (*)(ConstantValueNode *))freeConstantValueNode,
    .print = (PrintFPtr)print};
