#ifndef __VISITOR_H
#define __VISITOR_H

// Forward declaration of AST nodes
class ProgramNode;
class DeclNode;
class VariableNode;
class ConstantValueNode;
class FunctionNode;
class CompoundStatementNode;
class PrintNode;
class BinaryOperatorNode;
class UnaryOperatorNode;
class FunctionInvocationNode;
class VariableReferenceNode;
class AssignmentNode;
class ReadNode;
class IfNode;
class WhileNode;
class ForNode;
class ReturnNode;

class AstNodeVisitor {
  public:
    virtual ~AstNodeVisitor() = 0;

    virtual void visit(ProgramNode &p_program) {}
    virtual void visit(DeclNode &p_decl) {}
    virtual void visit(VariableNode &p_variable) {}
    virtual void visit(ConstantValueNode &p_constant_value) {}
    virtual void visit(FunctionNode &p_function) {}
    virtual void visit(CompoundStatementNode &p_compound_statement) {}
    virtual void visit(PrintNode &p_print) {}
    virtual void visit(BinaryOperatorNode &p_bin_op) {}
    virtual void visit(UnaryOperatorNode &p_un_op) {}
    virtual void visit(FunctionInvocationNode &p_func_invocation) {}
    virtual void visit(VariableReferenceNode &p_variable_ref) {}
    virtual void visit(AssignmentNode &p_assignment) {}
    virtual void visit(ReadNode &p_read) {}
    virtual void visit(IfNode &p_if) {}
    virtual void visit(WhileNode &p_while) {}
    virtual void visit(ForNode &p_for) {}
    virtual void visit(ReturnNode &p_return) {}
};

#endif
