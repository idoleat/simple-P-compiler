#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/expression.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/assignment.hpp"

class ForNode : public AstNode {
  public:
    ForNode(const uint32_t line, const uint32_t col);
    ForNode(const uint32_t line, const uint32_t col, DeclNode *decl, AssignmentNode *ass, ExpressionNode *exp, CompoundStatementNode *body, uint32_t *indent);
            /* TODO: declaration, assignment, expression,
             *       compound statement */
    ~ForNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    DeclNode *declration = NULL;
    AssignmentNode *assignment = NULL;
    ExpressionNode *expression = NULL;
    CompoundStatementNode *body = NULL;
};

#endif
