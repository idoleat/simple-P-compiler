#ifndef __AST_IF_NODE_H
#define __AST_IF_NODE_H

#include "AST/ast.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/expression.hpp"

class IfNode : public AstNode {
  public:
    IfNode(const uint32_t line, const uint32_t col);
    IfNode(const uint32_t line, const uint32_t col, ExpressionNode *exp, CompoundStatementNode *body, uint32_t *indent);
    IfNode(const uint32_t line, const uint32_t col, ExpressionNode *exp, CompoundStatementNode *body, CompoundStatementNode *elsee, uint32_t *indent);
    ~IfNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    ExpressionNode *expression = NULL;
    CompoundStatementNode *body = NULL;
    CompoundStatementNode *elsee = NULL;
};

#endif
