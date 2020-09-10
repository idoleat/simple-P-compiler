#include "AST/CompoundStatement.hpp"

// TODO
CompoundStatementNode::CompoundStatementNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
CompoundStatementNode::CompoundStatementNode(const uint32_t line, const uint32_t col, std::list<DeclNode> *declarations, std::list<AstNode *> *statements, uint32_t *indent)
    : AstNode{line, col} , declarations(declarations), statements(statements), indent(indent) {}


void CompoundStatementNode::print() {
    outputIndentationSpace(*indent);
    std::printf("compound statement <line: %u, col: %u>\n",location.line, location.col);


    if(declarations != NULL){
        *indent = *indent+1;
        std::list<DeclNode>::reverse_iterator it = declarations->rbegin();
        while(it != declarations->rend()){
            it->print();
            ++it;
        }
        *indent = *indent-1;
    }
    if(statements != NULL){
        *indent = *indent+1;
        std::list<AstNode *>::iterator it = statements->begin();
        while(it != statements->end()){
            if ((*it) != NULL) (*it)->print();
            ++it;
        }
        *indent = *indent-1;
    }
}

// void CompoundStatementNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
