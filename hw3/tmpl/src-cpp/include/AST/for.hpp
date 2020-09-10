#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/ast.hpp"

class ForNode : public AstNode {
  public:
    ForNode(const uint32_t line, const uint32_t col
            /* TODO: declaration, assignment, expression,
             *       compound statement */);
    ~ForNode() = default;

    void print() override;

  private:
    // TODO: declaration, assignment, expression, compound statement
};

#endif
