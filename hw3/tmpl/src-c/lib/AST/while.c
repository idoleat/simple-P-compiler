#include "AST/while.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __WhileNode {
    ANONYMOUS_AST_NODE;

    // TODO: expression, compound statement
} WhileNode;

const size_t kWhileNodeSize = sizeof(WhileNode);

static void destruct(WhileNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: expression, compound statement

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(WhileNode *self) {}

// static void visitChildNodes(WhileNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(WhileNode *self, const uint32_t line, const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static WhileNode *newWhileNode(const uint32_t line, const uint32_t col) {
    WhileNode *node = (WhileNode *)myMalloc(sizeof(WhileNode));
    construct(node, line, col);
    return node;
}

void freeWhileNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IWhileNode gWhileNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newWhileNode,
    .operatorDelete = (void (*)(WhileNode *))freeWhileNode,
    .print = (PrintFPtr)print};
