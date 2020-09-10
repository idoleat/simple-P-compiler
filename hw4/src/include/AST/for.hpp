#ifndef __AST_FOR_NODE_H
#define __AST_FOR_NODE_H

#include "AST/CompoundStatement.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/assignment.hpp"
#include "AST/ast.hpp"
#include "AST/decl.hpp"

#include <memory>

class ForNode : public AstNode {
  public:
    ForNode(const uint32_t line, const uint32_t col, DeclNode *p_var_decl,
            AssignmentNode *p_initial_statement, ConstantValueNode *p_condition,
            CompoundStatementNode *p_body);
    ~ForNode() = default;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    std::unique_ptr<DeclNode> var_decl;
    std::unique_ptr<AssignmentNode> initial_statement;
    std::unique_ptr<ConstantValueNode> condition;
    std::unique_ptr<CompoundStatementNode> body;
};

#endif
