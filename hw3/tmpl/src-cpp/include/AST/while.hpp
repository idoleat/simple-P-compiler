#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/ast.hpp"

class WhileNode : public AstNode {
  public:
    WhileNode(const uint32_t line, const uint32_t col
              /* TODO: expression, compound statement */);
    ~WhileNode() = default;

    void print() override;

  private:
    // TODO: expression, compound statement
};

#endif
