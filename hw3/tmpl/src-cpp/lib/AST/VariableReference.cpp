#include "AST/VariableReference.hpp"

// TODO
VariableReferenceNode::VariableReferenceNode(const uint32_t line,
                                             const uint32_t col)
    : ExpressionNode{line, col} {}

// TODO
// VariableReferenceNode::VariableReferenceNode(const uint32_t line,
//                                              const uint32_t col)
//     : ExpressionNode{line, col} {}

// TODO: You may use code snippets in AstDumper.cpp
void VariableReferenceNode::print() {}

// void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
