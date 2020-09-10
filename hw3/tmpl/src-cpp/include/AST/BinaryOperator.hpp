#ifndef __AST_BINARY_OPERATOR_NODE_H
#define __AST_BINARY_OPERATOR_NODE_H

#include "AST/expression.hpp"

#include <memory>

class BinaryOperatorNode : public ExpressionNode {
  public:
    BinaryOperatorNode(const uint32_t line, const uint32_t col
                       /* TODO: operator, expressions */);
    ~BinaryOperatorNode() = default;

    void print() override;

  private:
    // TODO: operator, expressions
};

#endif
