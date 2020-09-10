#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "AST/expression.hpp"

class ConstantValueNode : public ExpressionNode {
  public:
    ConstantValueNode(const uint32_t line, const uint32_t col
                      /* TODO: constant value */);
    ~ConstantValueNode() = default;

    void print() override;

  private:
    // TODO: constant value
};

#endif
