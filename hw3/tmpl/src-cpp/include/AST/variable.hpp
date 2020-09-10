#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ast.hpp"

class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col
                 /* TODO: variable name, type, constant value */);
    ~VariableNode() = default;

    void print() override;

  private:
    // TODO: variable name, type, constant value
};

#endif
