#include "AST/assignment.hpp"

AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
AssignmentNode::AssignmentNode(const uint32_t line, const uint32_t col, VariableReferenceNode *var, ExpressionNode *exp, uint32_t *indent)
    : AstNode{line, col}, variable(var), expression(exp), indent(indent) {}

void AssignmentNode::print() {
    outputIndentationSpace(*indent);
    std::printf("assignment statement <line: %u, col: %u>\n",location.line, location.col);
    
    *indent = *indent + 1;
    if(variable != NULL) variable->print();
    if(expression != NULL) expression->print();
    *indent = *indent - 1;
}

// void AssignmentNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
