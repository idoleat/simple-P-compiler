#include "AST/return.hpp"
#include "visitor/AstNodeVisitor.hpp"

ReturnNode::ReturnNode(const uint32_t line, const uint32_t col,
                       ExpressionNode *p_retval)
    : AstNode{line, col}, retval(p_retval) {}

const ExpressionNode *ReturnNode::getRetval() const { return retval.get(); }

void ReturnNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    retval->accept(p_visitor);
}
