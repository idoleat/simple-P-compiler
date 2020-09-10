#include "AST/VariableReference.hpp"

VariableReferenceNode::VariableReferenceNode(const uint32_t line, const uint32_t col)
    : ExpressionNode{line, col} {}
VariableReferenceNode::VariableReferenceNode(const uint32_t line, const uint32_t col, const char *name, uint32_t *indent)
    : ExpressionNode{line, col}, name(name), indent(indent) {}
VariableReferenceNode::VariableReferenceNode(const uint32_t line, const uint32_t col, const char *name, std::list<ExpressionNode *> *expList, uint32_t *indent)
    : ExpressionNode{line, col}, name(name), expressions(expList), indent(indent) {}

void VariableReferenceNode::print() {
    outputIndentationSpace(*indent);
    std::printf("variable reference <line: %u, col: %u> %s\n",location.line, location.col, name.c_str());

    if(expressions != NULL){
        *indent = *indent+1;
        std::list<ExpressionNode *>::iterator it = expressions->begin();
        while(it != expressions->end()){
            (*it)->print();
            ++it;
        }
        *indent = *indent-1;
    }
}

// void VariableReferenceNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
