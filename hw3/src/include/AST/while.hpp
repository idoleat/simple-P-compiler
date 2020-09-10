#ifndef __AST_WHILE_NODE_H
#define __AST_WHILE_NODE_H

#include "AST/ast.hpp"
#include "AST/expression.hpp"
#include "AST/CompoundStatement.hpp"

class WhileNode : public AstNode {
  public:
    WhileNode(const uint32_t line, const uint32_t col);
    WhileNode(const uint32_t line, const uint32_t col, ExpressionNode *exp, CompoundStatementNode *body, uint32_t *indent);
    ~WhileNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    ExpressionNode *expression;
    CompoundStatementNode *body;
};

#endif
