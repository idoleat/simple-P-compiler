#include "AST/print.h"
#include "OoUtils.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __PrintNode {
    ANONYMOUS_AST_NODE;

    // TODO: expression
} PrintNode;

const size_t kPrintNodeSize = sizeof(PrintNode);

static void destruct(PrintNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: expression

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(PrintNode *self) {}

// static void visitChildNodes(PrintNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(PrintNode *self, const uint32_t line, const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static PrintNode *newPrintNode(const uint32_t line, const uint32_t col) {
    PrintNode *node = (PrintNode *)myMalloc(sizeof(PrintNode));
    construct(node, line, col);
    return node;
}

void freePrintNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IPrintNode gPrintNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newPrintNode,
    .operatorDelete = (void (*)(PrintNode *))freePrintNode,
    .print = (PrintFPtr)print};
