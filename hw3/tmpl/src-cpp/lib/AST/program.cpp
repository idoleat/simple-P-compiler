#include "AST/program.hpp"

// TODO
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
                         const char *p_name)
    : AstNode{line, col}, name(p_name) {}

// visitor pattern version: const char *ProgramNode::getNameCString() const { return name.c_str(); }

void ProgramNode::print() {
    // TODO
    // outputIndentationSpace();

    std::printf("program <line: %u, col: %u> %s %s\n",
                location.line, location.col,
                name.c_str(), "void");

    // TODO
    // incrementIndentation();
    // visitChildNodes();
    // decrementIndentation();
}


// void ProgramNode::visitChildNodes(AstNodeVisitor &p_visitor) { // visitor pattern version
//     /* TODO
//      *
//      * for (auto &decl : var_decls) {
//      *     decl->accept(p_visitor);
//      * }
//      *
//      * // functions
//      *
//      * body->accept(p_visitor);
//      */
// }
