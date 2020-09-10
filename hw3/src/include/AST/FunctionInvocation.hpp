#ifndef __AST_FUNCTION_INVOCATION_NODE_H
#define __AST_FUNCTION_INVOCATION_NODE_H

#include "AST/expression.hpp"
#include <string>
#include <list>

class FunctionInvocationNode : public ExpressionNode {
  public:
    FunctionInvocationNode(const uint32_t line, const uint32_t col);
    FunctionInvocationNode(const uint32_t line, const uint32_t col, const char *name, std::list<ExpressionNode *> *Args, uint32_t *indent);
    ~FunctionInvocationNode() = default;

    const char *getNameCString() const;

    void print() override;

  private:
    uint32_t *indent;
    std::string name;
    std::list<ExpressionNode *> *args = NULL;
};

#endif
