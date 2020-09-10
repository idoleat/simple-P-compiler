#include "AST/UnaryOperator.hpp"

UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col)
    : ExpressionNode{line, col} {}
UnaryOperatorNode::UnaryOperatorNode(const uint32_t line, const uint32_t col, const char *opra, ExpressionNode *Op, uint32_t *indent)
    : ExpressionNode{line, col}, ooperator(std::string(opra)), Oprand(Op), indent(indent) {}

void UnaryOperatorNode::print() {
    outputIndentationSpace(*indent);
    if(ooperator == "-") ooperator = "neg";
    std::printf("unary operator <line: %u, col: %u> %s\n",location.line, location.col, ooperator.c_str());

    if(Oprand != NULL){
        *indent = *indent + 1;
        Oprand->print();
        *indent = *indent - 1;
    }
}

// void UnaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
