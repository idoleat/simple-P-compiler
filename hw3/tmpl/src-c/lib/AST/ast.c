#include "AST/ast.h"
#include "OoUtils.h"
#include "core/utils.h"

#include <stdio.h>
#include <stdlib.h>

struct __AstNodeVisitor;

typedef struct __AstNode {
    AST_NODE_MEMBERS;
} AstNode;

static void construct(AstNode *self, const uint32_t line, const uint32_t col) {
    MEMBER_FUNCTION_PROLOGUE;

    Location *location = (Location *)&self->location;
    location->line = line;
    location->col = col;
}

static void destruct(AstNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    self->destruct(self);
}

static void destructImpl(AstNode *self) {}

static const Location *getLocation(AstNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    return &self->location;
}

// static void visitChildNodes(AstNode *self, struct __AstNodeVisitor *visitor) {
//     MEMBER_FUNCTION_PROLOGUE;
// 
//     self->visitChildNodes(self, visitor);
// }

void freeAstNode(void *node) {
    if (node == NULL) {
        return;
    }

    AstNode *n = (AstNode *)node;
    n->destruct(n);
    free(n);
}

static void print(AstNode *self) {
    MEMBER_FUNCTION_PROLOGUE;

    self->print(self);
}

const IAstNode gAstNode = {
    .construct = (void (*)(void *, const uint32_t, const uint32_t))construct,
    .destruct = (DestructFPtr)destruct,
    .destructImpl = (DestructFPtr)destructImpl,
    .getLocation = (const Location *(*)(void *))getLocation,
    .print = (PrintFPtr)print
    };
