#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include "AST/ConstantValue.hpp"
#include <string>
#include <memory>

class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col);
    BinaryOperatorNode(const uint32_t line, const uint32_t col, const char *opra, ExpressionNode *LeftOp, ExpressionNode *RightOp, uint32_t *indent);
    ~BinaryOperatorNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    std::string ooperator;
    ExpressionNode *LeftOperand = NULL;
    ExpressionNode *RightOprand = NULL;
    
};

#endif
