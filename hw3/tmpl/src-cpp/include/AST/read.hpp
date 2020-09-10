#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/ast.hpp"

class ReadNode : public AstNode {
  public:
    ReadNode(const uint32_t line, const uint32_t col
             /* TODO: variable reference */);
    ~ReadNode() = default;

    void print() override;

  private:
    // TODO: variable reference
};

#endif
