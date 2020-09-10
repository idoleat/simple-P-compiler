#ifndef __AST_ASSIGNMENT_NODE_H
#define __AST_ASSIGNMENT_NODE_H

#include "AST/ast.hpp"
#include "AST/VariableReference.hpp"
#include "expression.hpp"

class AssignmentNode : public AstNode {
  public:
    AssignmentNode(const uint32_t line, const uint32_t col);
    AssignmentNode(const uint32_t line, const uint32_t col, VariableReferenceNode *var, ExpressionNode *exp, uint32_t *indent);
    ~AssignmentNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    VariableReferenceNode *variable = NULL;
    ExpressionNode * expression = NULL;
};

#endif
