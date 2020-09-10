#ifndef __AST_VARIABLE_REFERENCE_NODE_H
#define __AST_VARIABLE_REFERENCE_NODE_H

#include "AST/expression.hpp"
#include <list>
#include <string>

class VariableReferenceNode : public ExpressionNode {
  public:
    VariableReferenceNode(const uint32_t line, const uint32_t col);
    // normal reference
    VariableReferenceNode(const uint32_t line, const uint32_t col, const char *name, uint32_t *indent);
    // array reference
    VariableReferenceNode(const uint32_t line, const uint32_t col, const char *name, std::list<ExpressionNode *> *expList, uint32_t *indent);
    ~VariableReferenceNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    std::string name;
    std::list<ExpressionNode *> *expressions = NULL;
};

#endif
