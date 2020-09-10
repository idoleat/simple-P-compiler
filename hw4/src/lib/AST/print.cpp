#include "AST/print.hpp"
#include "visitor/AstNodeVisitor.hpp"

PrintNode::PrintNode(const uint32_t line, const uint32_t col,
                     ExpressionNode *p_expr)
    : AstNode{line, col}, target(p_expr) {}

void PrintNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    target->accept(p_visitor);
}
