#include "AST/return.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __ReturnNode {
    ANONYMOUS_AST_NODE;

    // TODO: expression
} ReturnNode;

const size_t kReturnNodeSize;

static void destruct(ReturnNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: expression

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(ReturnNode *self) {}

// static void visitChildNodes(ReturnNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(ReturnNode *self, const uint32_t line, const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static ReturnNode *newReturnNode(const uint32_t line, const uint32_t col) {
    ReturnNode *node = (ReturnNode *)myMalloc(sizeof(ReturnNode));
    construct(node, line, col);
    return node;
}

void freeReturnNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IReturnNode gReturnNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newReturnNode,
    .operatorDelete = (void (*)(ReturnNode *))freeReturnNode,
    .print = (PrintFPtr)print};
