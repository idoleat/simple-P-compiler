#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/CompoundStatement.hpp"
#include <string>
#include <list>

class FunctionNode : public AstNode {
  public:
    FunctionNode(const uint32_t line, const uint32_t col);
    FunctionNode(const uint32_t line, const uint32_t col, const char *name, std::list<DeclNode> *decls, const char *rt_type, CompoundStatementNode *cp_stats, uint32_t *indent);
    FunctionNode(const uint32_t line, const uint32_t col, const char *name, std::list<DeclNode> *decls, const char *rt_type, uint32_t *indent);
    ~FunctionNode() = default;

    void print() override;

  private:
      uint32_t *indent;
      std::string name;
      std::list<DeclNode> *declarations = NULL;
      std::string return_type;
      CompoundStatementNode *cp_statement;
      std::list<std::string> *ParseArgs(std::list<DeclNode> *decls);
};

#endif
