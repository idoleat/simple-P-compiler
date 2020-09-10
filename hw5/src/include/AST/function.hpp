#ifndef __AST_FUNCTION_NODE_H
#define __AST_FUNCTION_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/PType.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"

#include <memory>
#include <vector>

class FunctionNode : public AstNode {
  public:
    typedef std::vector<std::unique_ptr<DeclNode>> Decls;

    FunctionNode(const uint32_t line, const uint32_t col, const char *p_name,
                 Decls *p_parameters, const PType *p_type,
                 CompoundStatementNode *p_body);
    ~FunctionNode() = default;

    const std::string &getName() const;
    const char *getNameCString() const;

    const PType *getTypePtr() const;
    const char *getTypeCString() const;

    static std::string getParametersTypeString(const Decls &parameters);
    const Decls &getParameters() const;
    const char *getPrototypeCString() const;

    bool isDefined() const;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  public:
    static Decls::size_type getParametersNum(const Decls &parameters);

  private:
    const std::string name;
    Decls parameters;
    PTypeSharedPtr return_type;
    mutable std::string prototype_string;
    mutable bool prototype_string_is_valid = false;
    std::unique_ptr<CompoundStatementNode> body;
};

#endif
