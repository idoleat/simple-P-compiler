#include "AST/declaration.h"
#include "OoUtils.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct __DeclNode {
    ANONYMOUS_AST_NODE;

    // TODO: variable nodes;
} DeclNode;

const size_t kDeclNodeSize = sizeof(DeclNode);

static void destruct(DeclNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: variable nodes;

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(DeclNode *self) {}

// static void visitChildNodes(DeclNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(DeclNode *self, const uint32_t line, const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static DeclNode *newDeclNode(const uint32_t line, const uint32_t col) {
    DeclNode *node = (DeclNode *)myMalloc(sizeof(DeclNode));
    construct(node, line, col);
    return node;
}

void freeDeclNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IDeclNode gDeclNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newDeclNode,
    .operatorDelete = (void (*)(DeclNode *))freeDeclNode,
    .print = (PrintFPtr)print};
