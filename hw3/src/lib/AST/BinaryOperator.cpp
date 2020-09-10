#include "AST/BinaryOperator.hpp"

BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col)
    : ExpressionNode{line, col} {}
BinaryOperatorNode::BinaryOperatorNode(const uint32_t line, const uint32_t col, const char *opra, ExpressionNode *LeftOp, ExpressionNode *RightOp, uint32_t *indent)
    : ExpressionNode{line, col}, ooperator(opra), indent(indent), LeftOperand(LeftOp), RightOprand(RightOp) {}

void BinaryOperatorNode::print() {
    outputIndentationSpace(*indent);

    std::printf("binary operator <line: %u, col: %u> %s\n",location.line, location.col, ooperator.c_str());

    if(LeftOperand != NULL){
        *indent = *indent + 1;
        LeftOperand->print();
        *indent = *indent - 1;
    }

    if(RightOprand != NULL){
        *indent = *indent + 1;
        RightOprand->print();
        *indent = *indent - 1;
    }
}

// void BinaryOperatorNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
