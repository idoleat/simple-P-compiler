#ifndef __AST_COMPOUND_STATEMENT_NODE_H
#define __AST_COMPOUND_STATEMENT_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"

class CompoundStatementNode : public AstNode {
  public:
    CompoundStatementNode(const uint32_t line, const uint32_t col);
    CompoundStatementNode(const uint32_t line, const uint32_t col, std::list<DeclNode> *declarations, std::list<AstNode *> *statements, uint32_t *indent);
    ~CompoundStatementNode() = default;

    void print() override;

  private:
    uint32_t *indent;
    std::list<DeclNode> *declarations = NULL;
    std::list<AstNode *> *statements = NULL;
};

#endif
