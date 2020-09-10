#include "AST/return.hpp"

ReturnNode::ReturnNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
ReturnNode::ReturnNode(const uint32_t line, const uint32_t col, ExpressionNode *exp, uint32_t *indent)
    : AstNode{line, col}, expression(exp), indent(indent) {}

void ReturnNode::print() {
    outputIndentationSpace(*indent);

    std::printf("return statement <line: %u, col: %u>\n", location.line, location.col);

    if(expression != NULL){
        *indent = *indent + 1;
        expression->print();
        *indent = *indent - 1;
    }
}

// void ReturnNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
