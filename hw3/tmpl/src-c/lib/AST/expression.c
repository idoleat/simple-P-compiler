#include "AST/expression.h"
#include "OoUtils.h"

#include <stddef.h>
#include <stdlib.h>

struct __AstNodeVisitor;

typedef struct __ExpressionNode {
    EXPRESSION_NODE_MEMBERS;
} ExpressionNode;

static void construct(ExpressionNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
}

static void destruct(ExpressionNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    self->destruct(self);
}

static void destructImpl(ExpressionNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.destructImpl(self);
}

void freeExpressionNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

static void print(ExpressionNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    self->print(self);
}

// static void visitChildNodes(ExpressionNode *self,
//                             struct __AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     self->visitChildNodes(self, visitor);
// }

const IExpressionNode gExpressionNode = {
    .construct = (void (*)(void *, const uint32_t, const uint32_t))construct,
    .destruct = (DestructFPtr)destruct,
    .destructImpl = (DestructFPtr)destructImpl,
    .operatorDelete = (void (*)(ExpressionNode *))freeExpressionNode,
    .print = (PrintFPtr)print};
