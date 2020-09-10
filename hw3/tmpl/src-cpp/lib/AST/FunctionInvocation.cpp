#include "AST/FunctionInvocation.hpp"

// TODO
FunctionInvocationNode::FunctionInvocationNode(const uint32_t line,
                                               const uint32_t col)
    : ExpressionNode{line, col} {}

// TODO: You may use code snippets in AstDumper.cpp
void FunctionInvocationNode::print() {}

// void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
