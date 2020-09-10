#include "AST/expression.hpp"

ExpressionNode::ExpressionNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
ExpressionNode::ExpressionNode(const uint32_t line, const uint32_t col, const char *type)
    : AstNode{line, col}, type(std::string(type)) {}
std::string ExpressionNode::getType(){
    return type;
}
