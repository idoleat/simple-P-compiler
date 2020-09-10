#include "AST/if.hpp"

IfNode::IfNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
IfNode::IfNode(const uint32_t line, const uint32_t col, ExpressionNode *exp, CompoundStatementNode *body, uint32_t *indent)
    : AstNode{line, col}, expression(exp), body(body), indent(indent) {}
IfNode::IfNode(const uint32_t line, const uint32_t col, ExpressionNode *exp, CompoundStatementNode *body, CompoundStatementNode *elsee, uint32_t *indent)
    : AstNode{line, col}, expression(exp), body(body), elsee(elsee), indent(indent) {}

void IfNode::print() {
    outputIndentationSpace(*indent);
    std::printf("if statement <line: %u, col: %u>\n",location.line, location.col);

    *indent  = *indent + 1;
    if(expression != NULL) expression->print();
    if(body != NULL) body->print();
    if(elsee != NULL) elsee->print();
    *indent  = *indent - 1;
}

// void IfNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
