#include "AST/VariableReference.hpp"
#include "visitor/AstNodeVisitor.hpp"

VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *p_name)
    : ExpressionNode{line, col}, name(p_name) {}

VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col,
                                             const char *p_name,
                                             Exprs *p_indices)
    : ExpressionNode{line, col}, name(p_name), indices(std::move(*p_indices)) {}

const std::string &VariableReferenceNode::getName() const { return name; }

const char *VariableReferenceNode::getNameCString() const {
    return name.c_str();
}

const VariableReferenceNode::Exprs &VariableReferenceNode::getIndices() const {
    return indices;
}

void VariableReferenceNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &index : indices) {
        index->accept(p_visitor);
    }
}
