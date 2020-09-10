#include "AST/for.h"
#include "core/utils.h"
#include "visitor/AstNodeVisitor.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __ForNode {
    ANONYMOUS_AST_NODE;

    // TODO: declaration, assignment, expression, compound statement
} ForNode;

const size_t kForNodeSize = sizeof(ForNode);

static void destruct(ForNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    // TODO: declaration, assignment, expression, compound statement

    gAstNode.destructImpl(self);
}

// static void visitChildNodes(ForNode *self, AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     // TODO
// }

// TODO
static void construct(ForNode *self, const uint32_t line, const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    gAstNode.construct(self, line, col);
    self->destruct = (DestructFPtr)destruct;
}

// TODO
static ForNode *newForNode(const uint32_t line, const uint32_t col) {
    ForNode *node = (ForNode *)myMalloc(sizeof(ForNode));
    construct(node, line, col);
    return node;
}

void freeForNode(void *node) {
    if (!node) {
        return;
    }

    destruct(node);
    free(node);
}

const IForNode gForNode = {.construct = construct,
                           .destruct = (DestructFPtr)destruct,
                           .operatorNew = newForNode,
                           .operatorDelete = (void (*)(ForNode *))freeForNode};
