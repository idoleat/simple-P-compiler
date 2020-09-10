#include "AST/for.hpp"

ForNode::ForNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
ForNode::ForNode(const uint32_t line, const uint32_t col, DeclNode *decl, AssignmentNode *ass, ExpressionNode *exp, CompoundStatementNode *body, uint32_t *indent)
    : AstNode{line, col}, declration(decl), assignment(ass), expression(exp), body(body), indent(indent) {}

void ForNode::print() {
    outputIndentationSpace(*indent);
    std::printf("function invocation <line: %u, col: %u>\n",location.line, location.col);

    *indent = *indent + 1;
    if(declration != NULL) declration->print();
    if(assignment != NULL) assignment->print();
    if(expression != NULL) expression->print();
    if(body != NULL) body->print();
    *indent = *indent - 1;
}

// void ForNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
