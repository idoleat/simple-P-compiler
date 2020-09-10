#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/ast.hpp"

class FunctionNode : public AstNode {
  public:
    FunctionNode(const uint32_t line, const uint32_t col
                 /* TODO: name, declarations, return type,
                  *       compound statement (optional) */);
    ~FunctionNode() = default;

    void print() override;

  private:
    // TODO: name, declarations, return type, compound statement
};

#endif
