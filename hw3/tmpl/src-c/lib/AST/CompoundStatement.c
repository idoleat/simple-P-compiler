#include "AST/CompoundStatement.h"
#include "OoUtils.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __CompoundStatementNode {
    ANONYMOUS_AST_NODE;

    // TODO: declarations, statements
} CompoundStatementNode;

const size_t kCompoundStatementNodeSize = sizeof(CompoundStatementNode);

static void destruct(CompoundStatementNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: declarations, statements

    gAstNode.destructImpl(self);
}

// TODO: You may use code snippets in AstDumper.c
static void print(CompoundStatementNode *self) {}

// static void visitChildNodes(CompoundStatementNode *self,
//                             AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO: declarations, statements
// }

// TODO
static void construct(CompoundStatementNode *self, const uint32_t line,
                      const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
    self->print = (PrintFPtr)print;
}

// TODO
static CompoundStatementNode *
newCompoundStatementNode(const uint32_t line, const uint32_t col) {
    CompoundStatementNode *node =
        (CompoundStatementNode *)myMalloc(sizeof(CompoundStatementNode));
    construct(node, line, col);
    return node;
}

void freeCompoundStatementNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const ICompoundStatementNode gCompoundStatementNode = {
    .construct = construct,
    .destruct = (DestructFPtr)destruct,
    .operatorNew = newCompoundStatementNode,
    .operatorDelete =
        (void (*)(CompoundStatementNode *))freeCompoundStatementNode,
    .print = (PrintFPtr)print};
