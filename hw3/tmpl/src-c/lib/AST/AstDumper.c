#include "AST/AstDumper.h"
#include "AST/BinaryOperator.h"
#include "AST/CompoundStatement.h"
#include "AST/ConstantValue.h"
#include "AST/FunctionInvocation.h"
#include "AST/UnaryOperator.h"
#include "AST/VariableReference.h"
#include "AST/assignment.h"
#include "AST/declaration.h"
#include "AST/for.h"
#include "AST/function.h"
#include "AST/if.h"
#include "AST/print.h"
#include "AST/program.h"
#include "AST/read.h"
#include "AST/return.h"
#include "AST/variable.h"
#include "AST/while.h"
#include "OoUtils.h"
#include "core/utils.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct __AstDumper {
    ANONYMOUS_AST_NODE_VISITOR;

    uint32_t indentation;
} AstDumper;

// FIXME: remove this line if you choose to use visitor pattern with this template
#ifdef I_WANT_TO_USE_VISITOR_PATTERN

static const uint32_t kIndentationStride = 2u;

static void outputIndentationSpace(const uint32_t indentation) {
    fprintf(stdout, "%*s", indentation, "");
}

static void incrementIndentation(AstDumper *ad) {
    ad->indentation += kIndentationStride;
}

static void decrementIndentation(AstDumper *ad) {
    ad->indentation -= kIndentationStride;
}

static void dumpProgramNode(AstDumper *self, ProgramNode *program_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "program <line: %u, col: %u> %s %s\n",
            gAstNode.getLocation(program_node)->line,
            gAstNode.getLocation(program_node)->col,
            gProgramNode.getName(program_node),
            "void");

    incrementIndentation(self);
    gProgramNode.visitChildNodes(program_node, self);
    decrementIndentation(self);
}

static void dumpDeclNode(AstDumper *self, DeclNode *decl_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "declaration <line: %u, col: %u>\n",
            gAstNode.getLocation(decl_node)->line,
            gAstNode.getLocation(decl_node)->col);

    incrementIndentation(self);
    gDeclNode.visitChildNodes(decl_node, self);
    decrementIndentation(self);
}

static void dumpVariableNode(AstDumper *self, VariableNode *var_node) {
    outputIndentationSpace(self->indentation);

    // TODO: name, type
    fprintf(stdout, "variable <line: %u, col: %u> %s %s\n",
            gAstNode.getLocation(var_node)->line,
            gAstNode.getLocation(var_node)->col,
            "TODO",
            "TODO");

    incrementIndentation(self);
    gVariableNode.visitChildNodes(var_node, self);
    decrementIndentation(self);
}

static void dumpConstantValueNode(AstDumper *self,
                                  ConstantValueNode *constant_value_node) {
    outputIndentationSpace(self->indentation);

    // TODO: string of constant value
    fprintf(stdout, "constant <line: %u, col: %u> %s\n",
            gAstNode.getLocation(constant_value_node)->line,
            gAstNode.getLocation(constant_value_node)->col,
            "TODO");
}

static void dumpFunctionNode(AstDumper *self, FunctionNode *func_node) {
    outputIndentationSpace(self->indentation);

    // TODO: name, prototype string
    fprintf(stdout, "function declaration <line: %u, col: %u> %s %s\n",
            gAstNode.getLocation(func_node)->line,
            gAstNode.getLocation(func_node)->col,
            "TODO",
            "TODO");

    incrementIndentation(self);
    gFunctionNode.visitChildNodes(func_node, self);
    decrementIndentation(self);
}

static void
dumpCompoundStatementNode(AstDumper *self,
                          CompoundStatementNode *compound_statement_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "compound statement <line: %u, col: %u>\n",
            gAstNode.getLocation(compound_statement_node)->line,
            gAstNode.getLocation(compound_statement_node)->col);

    incrementIndentation(self);
    gCompoundStatementNode.visitChildNodes(compound_statement_node, self);
    decrementIndentation(self);
}

static void dumpPrintNode(AstDumper *self, PrintNode *print_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "print statement <line: %u, col: %u>\n",
            gAstNode.getLocation(print_node)->line,
            gAstNode.getLocation(print_node)->col);

    incrementIndentation(self);
    gPrintNode.visitChildNodes(print_node, self);
    decrementIndentation(self);
}

static void dumpBinaryOperatorNode(AstDumper *self,
                                   BinaryOperatorNode *bin_op_node) {
    outputIndentationSpace(self->indentation);

    // TODO: operator string
    fprintf(stdout, "binary operator <line: %u, col: %u> %s\n",
            gAstNode.getLocation(bin_op_node)->line,
            gAstNode.getLocation(bin_op_node)->col,
            "TODO");

    incrementIndentation(self);
    gBinaryOperatorNode.visitChildNodes(bin_op_node, self);
    decrementIndentation(self);
}

static void dumpUnaryOperatorNode(AstDumper *self,
                                  UnaryOperatorNode *un_op_node) {
    outputIndentationSpace(self->indentation);

    // TODO: operator string
    fprintf(stdout, "unary operator <line: %u, col: %u> %s\n",
            gAstNode.getLocation(un_op_node)->line,
            gAstNode.getLocation(un_op_node)->col,
            "TODO");

    incrementIndentation(self);
    gUnaryOperatorNode.visitChildNodes(un_op_node, self);
    decrementIndentation(self);
}

static void
dumpFunctionInvocationNode(AstDumper *self,
                           FunctionInvocationNode *func_invocation_node) {
    outputIndentationSpace(self->indentation);

    // TODO: function name
    fprintf(stdout, "function invocation <line: %u, col: %u> %s\n",
            gAstNode.getLocation(func_invocation_node)->line,
            gAstNode.getLocation(func_invocation_node)->col,
            "TODO");

    incrementIndentation(self);
    gFunctionInvocationNode.visitChildNodes(func_invocation_node, self);
    decrementIndentation(self);
}

static void dumpVariableReferenceNode(AstDumper *self,
                                      VariableReferenceNode *var_ref_node) {
    outputIndentationSpace(self->indentation);

    // TODO: variable name
    fprintf(stdout, "variable reference <line: %u, col: %u> %s\n",
            gAstNode.getLocation(var_ref_node)->line,
            gAstNode.getLocation(var_ref_node)->col,
            "TODO");

    incrementIndentation(self);
    gVariableReferenceNode.visitChildNodes(var_ref_node, self);
    decrementIndentation(self);
}

static void dumpAssignmentNode(AstDumper *self,
                               AssignmentNode *assignment_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "assignment statement <line: %u, col: %u>\n",
            gAstNode.getLocation(assignment_node)->line,
            gAstNode.getLocation(assignment_node)->col);

    incrementIndentation(self);
    gAssignmentNode.visitChildNodes(assignment_node, self);
    decrementIndentation(self);
}

static void dumpReadNode(AstDumper *self, ReadNode *read_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "read statement <line: %u, col: %u>\n",
            gAstNode.getLocation(read_node)->line,
            gAstNode.getLocation(read_node)->col);

    incrementIndentation(self);
    gReadNode.visitChildNodes(read_node, self);
    decrementIndentation(self);
}

static void dumpIfNode(AstDumper *self, IfNode *if_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "if statement <line: %u, col: %u>\n",
            gAstNode.getLocation(if_node)->line,
            gAstNode.getLocation(if_node)->col);

    incrementIndentation(self);
    gIfNode.visitChildNodes(if_node, self);
    decrementIndentation(self);
}

static void dumpWhileNode(AstDumper *self, WhileNode *while_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "while statement <line: %u, col: %u>\n",
            gAstNode.getLocation(while_node)->line,
            gAstNode.getLocation(while_node)->col);

    incrementIndentation(self);
    gWhileNode.visitChildNodes(while_node, self);
    decrementIndentation(self);
}

static void dumpForNode(AstDumper *self, ForNode *for_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "for statement <line: %u, col: %u>\n",
            gAstNode.getLocation(for_node)->line,
            gAstNode.getLocation(for_node)->col);

    incrementIndentation(self);
    gForNode.visitChildNodes(for_node, self);
    decrementIndentation(self);
}

static void dumpReturnNode(AstDumper *self, ReturnNode *return_node) {
    outputIndentationSpace(self->indentation);

    fprintf(stdout, "return statement <line: %u, col: %u>\n",
            gAstNode.getLocation(return_node)->line,
            gAstNode.getLocation(return_node)->col);

    incrementIndentation(self);
    gReturnNode.visitChildNodes(return_node, self);
    decrementIndentation(self);
}

static void construct(AstDumper *self) {
    MEMBER_FUNCTION_PROLOGUE;

    CONSTRUCT_VISITOR(self, dump);

    self->indentation = 0u;
}

static void destruct(AstDumper *self) {}

static AstDumper *newAstDumper(void) {
    AstDumper *visitor = (AstDumper *)myMalloc(sizeof(AstDumper));
    gAstDumper.construct(visitor);
    return visitor;
}

static void freeAstDumper(AstDumper *self) { free(self); }

const IAstDumper gAstDumper = {
    .construct = construct,
    .destruct = destruct,
    .operatorNew = newAstDumper,
    .operatorDelete = freeAstDumper,
    .visitProgramNode = (VisitProgramNodeFPtr)dumpProgramNode,
    .visitDeclNode = (VisitDeclNodeFPtr)dumpDeclNode,
    .visitVariableNode = (VisitVariableNodeFPtr)dumpVariableNode,
    .visitConstantValueNode = (VisitConstantValueNodeFPtr)dumpConstantValueNode,
    .visitFunctionNode = (VisitFunctionNodeFPtr)dumpFunctionNode,
    .visitCompoundStatementNode =
        (VisitCompoundStatementNodeFPtr)dumpCompoundStatementNode,
    .visitPrintNode = (VisitPrintNodeFPtr)dumpPrintNode,
    .visitBinaryOperatorNode =
        (VisitBinaryOperatorNodeFPtr)dumpBinaryOperatorNode,
    .visitUnaryOperatorNode = (VisitUnaryOperatorNodeFPtr)dumpUnaryOperatorNode,
    .visitFunctionInvocationNode =
        (VisitFunctionInvocationNodeFPtr)dumpFunctionInvocationNode,
    .visitVariableReferenceNode =
        (VisitVariableReferenceNodeFPtr)dumpVariableReferenceNode,
    .visitAssignmentNode = (VisitAssignmentNodeFPtr)dumpAssignmentNode,
    .visitReadNode = (VisitReadNodeFPtr)dumpReadNode,
    .visitIfNode = (VisitIfNodeFPtr)dumpIfNode,
    .visitWhileNode = (VisitWhileNodeFPtr)dumpWhileNode,
    .visitForNode = (VisitForNodeFPtr)dumpForNode,
    .visitReturnNode = (VisitReturnNodeFPtr)dumpReturnNode};

// FIXME: remove this line if you choose to use visitor pattern with this template
#endif
