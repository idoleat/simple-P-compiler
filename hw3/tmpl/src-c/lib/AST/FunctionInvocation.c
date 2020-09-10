#include "AST/FunctionInvocation.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __FunctionInvocationNode {
    ANONYMOUS_EXPRESSION_NODE;

    // TODO: name, expressions
} FunctionInvocation;

static void destruct(FunctionInvocationNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: name, expressions

    gExpressionNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(FunctionInvocationNode *self) {}

// static void visitChildNodes(FunctionInvocation *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(FunctionInvocationNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gExpressionNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static FunctionInvocationNode *
newFunctionInvocationNode(const uint32_t line, const uint32_t col) {
    FunctionInvocationNode *node =
        (FunctionInvocationNode *)myMalloc(sizeof(FunctionInvocationNode));
    construct(node, line, col);
    return node;
}

void freeFunctionInvocationNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IFunctionInvocationNode gFunctionInvocationNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newFunctionInvocationNode,
    .operatorDelete =
        (void (*)(FunctionInvocationNode *))freeFunctionInvocationNode,
    .print = (PrintFPtr)print};
