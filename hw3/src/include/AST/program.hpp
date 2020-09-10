#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/function.hpp"
#include "AST/CompoundStatement.hpp"

class ProgramNode : public AstNode {
  public:
    ProgramNode(const uint32_t line, const uint32_t col,
                const char *p_name, const char *re_type,
                std::list<DeclNode> *declarations, std::list<FunctionNode> *functions,
                CompoundStatementNode *CompoundStatement, uint32_t *indent);

    ~ProgramNode() = default;

    // visitor pattern version: const char *getNameCString() const;

    void print() override;

  private:
    uint32_t *_indent;
    const std::string name;
    const char *re_type;
    std::list<DeclNode> *declarations;
    std::list<FunctionNode> *functions;
    CompoundStatementNode *CompoundStatement;
};

#endif
