#ifndef __AST_READ_NODE_H
#define __AST_READ_NODE_H

#include "AST/ast.hpp"
#include "AST/VariableReference.hpp"

class ReadNode : public AstNode {
  public:
    ReadNode(const uint32_t line, const uint32_t col);
    ReadNode(const uint32_t line, const uint32_t col, VariableReferenceNode *var, uint32_t *indent);
    ~ReadNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    VariableReferenceNode *variable = NULL;
};

#endif
