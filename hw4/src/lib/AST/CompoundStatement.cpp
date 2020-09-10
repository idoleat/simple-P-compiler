#include "AST/CompoundStatement.hpp"
#include "visitor/AstNodeVisitor.hpp"

CompoundStatementNode::CompoundStatementNode(const uint32_t line,
                                             const uint32_t col,
                                             Decls *p_var_decls,
                                             Nodes *p_statements)
    : AstNode{line, col}, var_decls(std::move(*p_var_decls)),
      statements(std::move(*p_statements)) {}

void CompoundStatementNode::accept(AstNodeVisitor &p_visitor) {
    p_visitor.visit(*this);
}

void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &p_decl : var_decls) {
        p_decl->accept(p_visitor);
    }
    for (auto &p_statement : statements) {
        p_statement->accept(p_visitor);
    }
}
