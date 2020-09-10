#include "AST/read.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __ReadNode {
    ANONYMOUS_AST_NODE;

    // TODO: variable reference
} ReadNode;

const size_t kReadNodeSize = sizeof(ReadNode);

static void destruct(ReadNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: variable reference

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(ReadNode *self) {}

// static void visitChildNodes(ReadNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(ReadNode *self, const uint32_t line, const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static ReadNode *newReadNode(const uint32_t line, const uint32_t col) {
    ReadNode *node = (ReadNode *)myMalloc(sizeof(ReadNode));
    construct(node, line, col);
    return node;
}

void freeReadNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IReadNode gReadNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newReadNode,
    .operatorDelete = (void (*)(ReadNode *))freeReadNode,
    .print = (PrintFPtr)print};
