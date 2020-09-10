#include "AST/decl.hpp"


DeclNode::DeclNode(const uint32_t line, const uint32_t col, std::list<VariableNode> *idList, uint32_t *indent)
    : AstNode{line, col}, variables(idList), _indent(indent) {}


void DeclNode::print() {
    outputIndentationSpace(*_indent);

    std::printf("declaration <line: %u, col: %u> \n",
                location.line, location.col);

    *_indent = *_indent+1;

    std::list<VariableNode>::iterator it = variables->begin();
    while(it != variables->end()){
        //printf("Variable");
        it->print();
        ++it;
    }

    *_indent = *_indent-1;
}

std::list<VariableNode> *DeclNode::GetVariables(){
    return variables;
}

// void DeclNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
