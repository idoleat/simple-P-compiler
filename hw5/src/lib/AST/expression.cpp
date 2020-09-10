#include "AST/expression.hpp"

ExpressionNode::ExpressionNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}

const PType *ExpressionNode::getInferredType() const { return type.get(); }

void ExpressionNode::setInferredType(PType *p_type) { type.reset(p_type); }
