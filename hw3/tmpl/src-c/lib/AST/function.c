#include "AST/function.h"
#include "OoUtils.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __FunctionNode {
    ANONYMOUS_AST_NODE;

    // TODO: name, declarations, return type, compound statement
} FunctionNode;

const size_t kFunctionNodeSize = sizeof(FunctionNode);

static void destruct(FunctionNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: name, declarations, return type, compound statement

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(FunctionNode *self) {}

// static void visitChildNodes(FunctionNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(FunctionNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static FunctionNode *newFunctionNode(const uint32_t line, const uint32_t col) {
    FunctionNode *node = (FunctionNode *)myMalloc(sizeof(FunctionNode));
    construct(node, line, col);
    return node;
}

void freeFunctionNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IFunctionNode gFunctionNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newFunctionNode,
    .operatorDelete = (void (*)(FunctionNode *))freeFunctionNode,
    .print = (PrintFPtr)print};
