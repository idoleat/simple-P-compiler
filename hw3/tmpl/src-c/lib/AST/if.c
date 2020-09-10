#include "AST/if.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __IfNode {
    ANONYMOUS_AST_NODE;

    // TODO: expression, compound statement, compound statement (optional)
} IfNode;

const size_t kIfNodeSize = sizeof(IfNode);

static void destruct(IfNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: expression, compound statement, compound statement (optional)

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(IfNode *self) {}

// static void visitChildNodes(IfNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(IfNode *self, const uint32_t line, const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static IfNode *newIfNode(const uint32_t line, const uint32_t col) {
    IfNode *node = (IfNode *)myMalloc(sizeof(IfNode));
    construct(node, line, col);
    return node;
}

void freeIfNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IIfNode gIfNode = {.construct = construct,
                         .destruct = (DestructFPtr)destruct,
                         .operatorNew = newIfNode,
                         .operatorDelete = (void (*)(IfNode *))freeIfNode,
                         .print = (PrintFPtr)print};
