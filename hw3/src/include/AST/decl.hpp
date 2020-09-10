#ifndef __AST_DECL_NODE_H
#define __AST_DECL_NODE_H

#include "AST/ast.hpp"
#include "AST/variable.hpp"
#include "AST/ConstantValue.hpp"
#include <string>
#include <list>
#include <cstdio>

class DeclNode : public AstNode {
  public:
    // variable declaration
    DeclNode(const uint32_t line, const uint32_t col, std::list<VariableNode> *idList, uint32_t *indent);

    // constant variable declaration ???

    ~DeclNode() = default;

    void print() override;
    std::list<VariableNode> *GetVariables();

  private:
    std::list<VariableNode> *variables;
    uint32_t *_indent;
};

#endif
