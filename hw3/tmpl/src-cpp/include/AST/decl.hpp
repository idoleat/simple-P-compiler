#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.hpp"

class DeclNode : public AstNode {
  public:
    // variable declaration
    DeclNode(const uint32_t line, const uint32_t col
             /* TODO: identifiers, type */);

    // constant variable declaration
    //DeclNode(const uint32_t, const uint32_t col
    //         /* TODO: identifiers, constant */);

    ~DeclNode() = default;

    void print() override;

  private:
    // TODO: variables
};

#endif
