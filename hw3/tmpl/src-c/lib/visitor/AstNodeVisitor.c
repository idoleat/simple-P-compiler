#include "visitor/AstNodeVisitor.h"
#include "AST/ast.h"
#include "OoUtils.h"

typedef struct __AstNodeVisitor {
    AST_NODE_VISITOR_VIRTUAL_FUNCTIONS;
} AstNodeVisitor;

static void visitProgramNode(AstNodeVisitor *self,
                             struct __ProgramNode *program_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitProgramNode(self, program_node);
}

static void visitDeclNode(AstNodeVisitor *self, struct __DeclNode *decl_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitDeclNode(self, decl_node);
}

static void visitVariableNode(AstNodeVisitor *self,
                              struct __VariableNode *variable_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitVariableNode(self, variable_node);
}

static void
visitConstantValueNode(AstNodeVisitor *self,
                       struct __ConstantValueNode *constant_value_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitConstantValueNode(self, constant_value_node);
}

static void visitFunctionNode(AstNodeVisitor *self,
                              struct __FunctionNode *func_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitFunctionNode(self, func_node);
}

static void visitCompoundStatementNode(
    AstNodeVisitor *self,
    struct __CompoundStatementNode *compound_statement_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitCompoundStatementNode(self, compound_statement_node);
}

static void visitPrintNode(AstNodeVisitor *self,
                           struct __PrintNode *print_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitPrintNode(self, print_node);
}

static void visitBinaryOperatorNode(AstNodeVisitor *self,
                                    struct __BinaryOperatorNode *bin_op_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitBinaryOperatorNode(self, bin_op_node);
}

static void visitUnaryOperatorNode(AstNodeVisitor *self,
                                   struct __UnaryOperatorNode *un_op_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitUnaryOperatorNode(self, un_op_node);
}

static void visitFunctionInvocationNode(
    AstNodeVisitor *self,
    struct __FunctionInvocationNode *func_invocation_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitFunctionInvocationNode(self, func_invocation_node);
}

static void
visitVariableReferenceNode(AstNodeVisitor *self,
                           struct __VariableReferenceNode *var_ref_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitVariableReferenceNode(self, var_ref_node);
}

static void visitAssignmentNode(AstNodeVisitor *self,
                                struct __AssignmentNode *assignment_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitAssignmentNode(self, assignment_node);
}

static void visitReadNode(AstNodeVisitor *self, struct __ReadNode *read_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitReadNode(self, read_node);
}

static void visitIfNode(AstNodeVisitor *self, struct __IfNode *if_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitIfNode(self, if_node);
}

static void visitWhileNode(AstNodeVisitor *self,
                           struct __WhileNode *while_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitWhileNode(self, while_node);
}

static void visitForNode(AstNodeVisitor *self, struct __ForNode *for_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitForNode(self, for_node);
}

static void visitReturnNode(AstNodeVisitor *self,
                            struct __ReturnNode *return_node) {
    MEMBER_FUNCTION_PROLOGUE;

    self->visitReturnNode(self, return_node);
}

const IAstNodeVisitor gAstNodeVisitor = {
    .visitProgramNode = (VisitProgramNodeFPtr)visitProgramNode,
    .visitDeclNode = (VisitDeclNodeFPtr)visitDeclNode,
    .visitVariableNode = (VisitVariableNodeFPtr)visitVariableNode,
    .visitConstantValueNode =
        (VisitConstantValueNodeFPtr)visitConstantValueNode,
    .visitFunctionNode = (VisitFunctionNodeFPtr)visitFunctionNode,
    .visitCompoundStatementNode =
        (VisitCompoundStatementNodeFPtr)visitCompoundStatementNode,
    .visitPrintNode = (VisitPrintNodeFPtr)visitPrintNode,
    .visitBinaryOperatorNode =
        (VisitBinaryOperatorNodeFPtr)visitBinaryOperatorNode,
    .visitUnaryOperatorNode =
        (VisitUnaryOperatorNodeFPtr)visitUnaryOperatorNode,
    .visitFunctionInvocationNode =
        (VisitFunctionInvocationNodeFPtr)visitFunctionInvocationNode,
    .visitVariableReferenceNode =
        (VisitVariableReferenceNodeFPtr)visitVariableReferenceNode,
    .visitAssignmentNode = (VisitAssignmentNodeFPtr)visitAssignmentNode,
    .visitReadNode = (VisitReadNodeFPtr)visitReadNode,
    .visitIfNode = (VisitIfNodeFPtr)visitIfNode,
    .visitWhileNode = (VisitWhileNodeFPtr)visitWhileNode,
    .visitForNode = (VisitForNodeFPtr)visitForNode,
    .visitReturnNode = (VisitReturnNodeFPtr)visitReturnNode};
