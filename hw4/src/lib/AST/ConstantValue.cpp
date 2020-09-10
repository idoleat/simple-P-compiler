#include "AST/ConstantValue.hpp"
#include "visitor/AstNodeVisitor.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col,
                                     const Constant *p_constant)
    : ExpressionNode{line, col}, constant(p_constant) {}

const PTypeSharedPtr &ConstantValueNode::getTypePtr() const {
    return constant->getTypePtr();
}

const char *ConstantValueNode::getConstantValueCString() const {
    return constant->getConstantValueCString();
}

void ConstantValueNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}
