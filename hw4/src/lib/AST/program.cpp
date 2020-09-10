#include "AST/program.hpp"
#include "visitor/AstNodeVisitor.hpp"

ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                         const PType *p_return_type, const char *p_name,
                         Decls *p_var_decls, Funcs *p_funcs,
                         CompoundStatementNode *p_body)
    : AstNode{line, col}, return_type(p_return_type), name(p_name),
      var_decls(std::move(*p_var_decls)), funcs(std::move(*p_funcs)),
      body(p_body) {}

const char *ProgramNode::getNameCString() const { return name.c_str(); }

const char *ProgramNode::getTypeCString() const {
    return return_type->getPTypeCString();
}

void ProgramNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &decl : var_decls) {
        decl->accept(p_visitor);
    }
    for (auto &func : funcs) {
        func->accept(p_visitor);
    }
    body->accept(p_visitor);
}
