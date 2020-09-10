#ifndef __VISITOR_H
#define __VISITOR_H

// Forward declarations of AST nodes
struct __ProgramNode;
struct __DeclNode;
struct __VariableNode;
struct __ConstantValueNode;
struct __FunctionNode;
struct __CompoundStatementNode;
struct __PrintNode;
struct __BinaryOperatorNode;
struct __UnaryOperatorNode;
struct __FunctionInvocationNode;
struct __VariableReferenceNode;
struct __AssignmentNode;
struct __ReadNode;
struct __IfNode;
struct __WhileNode;
struct __ForNode;
struct __ReturnNode;

#define AST_NODE_VISITOR_VIRTUAL_FUNCTIONS                                     \
    VisitProgramNodeFPtr visitProgramNode;                                     \
    VisitDeclNodeFPtr visitDeclNode;                                           \
    VisitVariableNodeFPtr visitVariableNode;                                   \
    VisitConstantValueNodeFPtr visitConstantValueNode;                         \
    VisitFunctionNodeFPtr visitFunctionNode;                                   \
    VisitCompoundStatementNodeFPtr visitCompoundStatementNode;                 \
    VisitPrintNodeFPtr visitPrintNode;                                         \
    VisitBinaryOperatorNodeFPtr visitBinaryOperatorNode;                       \
    VisitUnaryOperatorNodeFPtr visitUnaryOperatorNode;                         \
    VisitFunctionInvocationNodeFPtr visitFunctionInvocationNode;               \
    VisitVariableReferenceNodeFPtr visitVariableReferenceNode;                 \
    VisitAssignmentNodeFPtr visitAssignmentNode;                               \
    VisitReadNodeFPtr visitReadNode;                                           \
    VisitIfNodeFPtr visitIfNode;                                               \
    VisitWhileNodeFPtr visitWhileNode;                                         \
    VisitForNodeFPtr visitForNode;                                             \
    VisitReturnNodeFPtr visitReturnNode

#define ANONYMOUS_AST_NODE_VISITOR                                             \
    struct {                                                                   \
        AST_NODE_VISITOR_VIRTUAL_FUNCTIONS;                                    \
    }

// clang-format off
#define CONSTRUCT_VISITOR(visitor, action)                                     \
    visitor->visitProgramNode             = (VisitProgramNodeFPtr)               \
                                            action ## ProgramNode;               \
    visitor->visitDeclNode                = (VisitDeclNodeFPtr)                  \
                                            action ## DeclNode;                  \
    visitor->visitVariableNode            = (VisitVariableNodeFPtr)              \
                                            action ## VariableNode;              \
    visitor->visitConstantValueNode       = (VisitConstantValueNodeFPtr)         \
                                            action ## ConstantValueNode;         \
    visitor->visitFunctionNode            = (VisitFunctionNodeFPtr)              \
                                            action ## FunctionNode;              \
    visitor->visitCompoundStatementNode   = (VisitCompoundStatementNodeFPtr)     \
                                            action ## CompoundStatementNode;     \
    visitor->visitPrintNode               = (VisitPrintNodeFPtr)                 \
                                            action ## PrintNode;                 \
    visitor->visitBinaryOperatorNode      = (VisitBinaryOperatorNodeFPtr)        \
                                            action ## BinaryOperatorNode;        \
    visitor->visitUnaryOperatorNode       = (VisitUnaryOperatorNodeFPtr)         \
                                            action ## UnaryOperatorNode;         \
    visitor->visitFunctionInvocationNode  = (VisitFunctionInvocationNodeFPtr)    \
                                            action ## FunctionInvocationNode;    \
    visitor->visitVariableReferenceNode   = (VisitVariableReferenceNodeFPtr)     \
                                            action ## VariableReferenceNode;     \
    visitor->visitAssignmentNode          = (VisitAssignmentNodeFPtr)            \
                                            action ## AssignmentNode;            \
    visitor->visitReadNode                = (VisitReadNodeFPtr)                  \
                                            action ## ReadNode;                  \
    visitor->visitIfNode                  = (VisitIfNodeFPtr)                    \
                                            action ## IfNode;                    \
    visitor->visitWhileNode               = (VisitWhileNodeFPtr)                 \
                                            action ## WhileNode;                 \
    visitor->visitForNode                 = (VisitForNodeFPtr)                   \
                                            action ## ForNode;                   \
    visitor->visitReturnNode              = (VisitReturnNodeFPtr)                \
                                            action ## ReturnNode
// clang-format on

/******************************************************************************
 * > typedef
 *   - xxxFPtr is defined for derived class to perform casting
 *     - void * for achieving upcasting
 ******************************************************************************/
typedef void (*VisitProgramNodeFPtr)(void *self,
                                     struct __ProgramNode *program_node);
typedef void (*VisitDeclNodeFPtr)(void *self, struct __DeclNode *decl_node);
typedef void (*VisitVariableNodeFPtr)(void *self,
                                      struct __VariableNode *variable_node);
typedef void (*VisitConstantValueNodeFPtr)(
    void *self, struct __ConstantValueNode *constant_value_node);
typedef void (*VisitFunctionNodeFPtr)(void *self,
                                      struct __FunctionNode *func_node);
typedef void (*VisitCompoundStatementNodeFPtr)(
    void *self, struct __CompoundStatementNode *compound_statement_node);
typedef void (*VisitPrintNodeFPtr)(void *self, struct __PrintNode *print_node);
typedef void (*VisitBinaryOperatorNodeFPtr)(
    void *self, struct __BinaryOperatorNode *bin_op_node);
typedef void (*VisitUnaryOperatorNodeFPtr)(
    void *self, struct __UnaryOperatorNode *un_op_node);
typedef void (*VisitFunctionInvocationNodeFPtr)(
    void *self, struct __FunctionInvocationNode *func_invocation_node);
typedef void (*VisitVariableReferenceNodeFPtr)(
    void *self, struct __VariableReferenceNode *var_ref_node);
typedef void (*VisitAssignmentNodeFPtr)(
    void *self, struct __AssignmentNode *assignment_node);
typedef void (*VisitReadNodeFPtr)(void *self, struct __ReadNode *read_node);
typedef void (*VisitIfNodeFPtr)(void *self, struct __IfNode *if_node);
typedef void (*VisitWhileNodeFPtr)(void *self, struct __WhileNode *while_node);
typedef void (*VisitForNodeFPtr)(void *self, struct __ForNode *for_node);
typedef void (*VisitReturnNodeFPtr)(void *self,
                                    struct __ReturnNode *return_node);

typedef struct __AstNodeVisitor AstNodeVisitor;

/******************************************************************************
 * > Interface related
 *   - gAstNodeVisitor will populate these function pointers with corresponding
 *     dispatcher for clients to invoke
 ******************************************************************************/
typedef struct __IAstNodeVisitor {
    AST_NODE_VISITOR_VIRTUAL_FUNCTIONS;
} IAstNodeVisitor;

extern const IAstNodeVisitor gAstNodeVisitor;

#endif
