#include "AST/program.hpp"

// TODO
ProgramNode::ProgramNode(const uint32_t line, const uint32_t col,
            const char *p_name, const char *re_type,
            std::list<DeclNode> *declarations, std::list<FunctionNode> *functions,
            CompoundStatementNode *CompoundStatement, uint32_t *indent)
    : AstNode{line, col}, name(p_name), re_type(re_type), declarations(declarations), functions(functions), CompoundStatement(CompoundStatement), _indent(indent) {}

// visitor pattern version: const char *ProgramNode::getNameCString() const { return name.c_str(); }

void ProgramNode::print() {
    outputIndentationSpace(*_indent);

    std::printf("program <line: %u, col: %u> %s %s\n",
                location.line, location.col,
                name.c_str(), "void");

    if(declarations != NULL) {
        *_indent = *_indent+1;
        std::list<DeclNode>::reverse_iterator it = declarations->rbegin();
        while(it != declarations->rend()){
            it->print();
            ++it;
        }
        *_indent = *_indent-1;
    }

    if(functions != NULL) {
        *_indent = *_indent+1;
        std::list<FunctionNode>::reverse_iterator it = functions->rbegin();
        while(it != functions->rend()){
            it->print();
            ++it;
        }
        *_indent = *_indent-1;
    }

    if(CompoundStatement != NULL){
        *_indent = *_indent+1;
        CompoundStatement->print();
        *_indent = *_indent-1;
    }
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
