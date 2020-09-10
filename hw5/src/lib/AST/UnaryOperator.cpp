#include "AST/UnaryOperator.hpp"
#include "visitor/AstNodeVisitor.hpp"

UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col,
                                     Operator op, ExpressionNode *p_operand)
    : ExpressionNode{line, col}, op(op), operand(p_operand) {}

Operator UnaryOperatorNode::getOp() const { return op; }

const char *UnaryOperatorNode::getOpCString() const {
    return kOpString[static_cast<size_t>(op)];
}

const ExpressionNode *UnaryOperatorNode::getOperand() const {
    return operand.get();
}

void UnaryOperatorNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    operand->accept(p_visitor);
}
