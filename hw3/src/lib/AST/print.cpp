#include "AST/print.hpp"

PrintNode::PrintNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
PrintNode::PrintNode(const uint32_t line, const uint32_t col, ExpressionNode *target, uint32_t *indent)
    : AstNode{line, col}, target(target), indent(indent) {}

void PrintNode::print() {
    outputIndentationSpace(*indent);

    std::printf("print statement <line: %u, col: %u>\n",location.line, location.col);

    if(target != NULL){
        *indent = *indent + 1;
        target->print();
        *indent = *indent - 1;
    }
}

// void PrintNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
