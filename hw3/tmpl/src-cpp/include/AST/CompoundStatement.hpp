#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include "AST/ast.hpp"

class CompoundStatementNode : public AstNode {
  public:
    CompoundStatementNode(const uint32_t line, const uint32_t col
                          /* TODO: declarations, statements */);
    ~CompoundStatementNode() = default;

    void print() override;

  private:
    // TODO: declarations, statements
};

#endif
