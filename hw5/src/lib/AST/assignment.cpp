#include "AST/assignment.hpp"
#include "visitor/AstNodeVisitor.hpp"

AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col,
                               VariableReferenceNode *p_lvalue,
                               ExpressionNode *p_expr)
    : AstNode{line, col}, lvalue(p_lvalue), expr(p_expr) {}

const VariableReferenceNode *AssignmentNode::getLvalue() const {
    return lvalue.get();
}

const ExpressionNode *AssignmentNode::getExpr() const { return expr.get(); }

void AssignmentNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    lvalue->accept(p_visitor);
    expr->accept(p_visitor);
}
