#include "AST/FunctionInvocation.hpp"

FunctionInvocationNode::FunctionInvocationNode(const uint32_t line, const uint32_t col)
    : ExpressionNode{line, col} {}
FunctionInvocationNode::FunctionInvocationNode(const uint32_t line, const uint32_t col, const char *name, std::list<ExpressionNode *> *Args, uint32_t *indent)
    : ExpressionNode{line, col}, name(std::string(name)), args(Args) , indent(indent){}

void FunctionInvocationNode::print() {
    outputIndentationSpace(*indent);
    std::printf("function invocation <line: %u, col: %u> %s\n",location.line, location.col, name.c_str());


    if(args != NULL){
        *indent = *indent+1;
        std::list<ExpressionNode *>::iterator it = args->begin();
        while(it != args->end()){
            (*it)->print();
            ++it;
        }
        *indent = *indent-1;
    }
}

// void FunctionInvocationNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
