#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/PType.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"
#include "AST/function.hpp"

#include <memory>
#include <vector>

class ProgramNode : public AstNode {
  public:
    typedef std::vector<std::unique_ptr<DeclNode>> Decls;
    typedef std::vector<std::unique_ptr<FunctionNode>> Funcs;

    ProgramNode(const uint32_t line, const uint32_t col,
                const PType *p_return_type, const char *p_name,
                Decls *p_var_decls, Funcs *p_funcs,
                CompoundStatementNode *p_body);
    ~ProgramNode() = default;

    const char *getNameCString() const;
    const char *getTypeCString() const;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::unique_ptr<const PType> return_type;
    const std::string name;
    Decls var_decls;
    Funcs funcs;
    std::unique_ptr<CompoundStatementNode> body;
};

#endif
