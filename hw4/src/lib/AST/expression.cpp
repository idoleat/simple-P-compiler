#include "AST/expression.hpp"

ExpressionNode::ExpressionNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
