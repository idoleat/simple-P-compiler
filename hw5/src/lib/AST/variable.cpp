#include "AST/variable.hpp"
#include "visitor/AstNodeVisitor.hpp"

VariableNode::VariableNode(const uint32_t line, const uint32_t col,
                           const std::string &name,
                           const PTypeSharedPtr &p_type,
                           const std::shared_ptr<ConstantValueNode> &p_constant)
    : AstNode{line, col}, name(std::move(name)), type(p_type),
      constant(p_constant) {}

const std::string &VariableNode::getName() const { return name; }

const char *VariableNode::getNameCString() const { return name.c_str(); }

const PType *VariableNode::getTypePtr() const { return type.get(); }

const char *VariableNode::getTypeCString() const {
    return type->getPTypeCString();
}

const Constant *VariableNode::getConstantPtr() const {
    if (!constant) {
        return nullptr;
    }
    return constant->getConstantPtr();
}

void VariableNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    if (constant) {
        constant->accept(p_visitor);
    }
}
