#ifndef __AST_PRINT_NODE_H
#define __AST_PRINT_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"

class PrintNode : public AstNode {
  public:
    PrintNode(const uint32_t line, const uint32_t col);
    PrintNode(const uint32_t line, const uint32_t col, ExpressionNode *target, uint32_t *indent);
    ~PrintNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    ExpressionNode *target = NULL;
};

#endif
