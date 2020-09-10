#ifndef __AST_UNARY_OPERATOR_NODE_H
#define __AST_UNARY_OPERATOR_NODE_H

#include "AST/expression.hpp"
#include <string>

class UnaryOperatorNode : public ExpressionNode {
  public:
    UnaryOperatorNode(const uint32_t line, const uint32_t col);
    UnaryOperatorNode(const uint32_t line, const uint32_t col, const char *opra, ExpressionNode *Op, uint32_t *indent);
    ~UnaryOperatorNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    std::string ooperator;
    ExpressionNode *Oprand = NULL;
};

#endif
