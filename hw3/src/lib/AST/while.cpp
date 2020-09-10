#include "AST/while.hpp"

WhileNode::WhileNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
WhileNode::WhileNode(const uint32_t line, const uint32_t col, ExpressionNode *exp, CompoundStatementNode *body, uint32_t *indent)
    : AstNode{line, col},expression(exp), body(body), indent(indent) {}

void WhileNode::print() {
    outputIndentationSpace(*indent);
    std::printf("while statement <line: %u, col: %u>\n",location.line, location.col);

    *indent = *indent + 1;
    if(expression != NULL) expression->print();
    if(body != NULL) body->print();
    *indent = *indent - 1;
}

// void WhileNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
