#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/ast.hpp"

class IfNode : public AstNode {
  public:
    IfNode(const uint32_t line, const uint32_t col
           /* TODO: expression, compound statement, compound statement */);
    ~IfNode() = default;

    void print() override;

  private:
    // TODO: expression, compound statement, compound statement
};

#endif
