#include "AST/read.hpp"

ReadNode::ReadNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
ReadNode::ReadNode(const uint32_t line, const uint32_t col, VariableReferenceNode *var, uint32_t *indent)
    : AstNode{line, col}, variable(var), indent(indent) {}

void ReadNode::print() {
    outputIndentationSpace(*indent);
    std::printf("read statement <line: %u, col: %u>\n",location.line, location.col);

    *indent = *indent + 1;
    if(variable != NULL) variable->print();
    *indent = *indent - 1;
}

// void ReadNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
