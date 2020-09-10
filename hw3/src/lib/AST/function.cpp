#include "AST/function.hpp"


FunctionNode::FunctionNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col, const char *name, std::list<DeclNode> *decls, const char *rt_type, CompoundStatementNode *cp_stats, uint32_t *indent)
    : AstNode{line, col} , name(std::string(name)), declarations(decls), return_type(std::string(rt_type))
    , cp_statement(cp_stats), indent(indent) {}
FunctionNode::FunctionNode(const uint32_t line, const uint32_t col, const char *name, std::list<DeclNode> *decls, const char *rt_type, uint32_t *indent)
    : AstNode{line, col}, name(std::string(name)), declarations(decls), return_type(std::string(rt_type))
    , indent(indent) {}


void FunctionNode::print() {
    outputIndentationSpace(*indent);

    if(declarations == NULL) {
        std::printf("function declaration <line: %u, col: %u> %s %s ()\n",
                    location.line, location.col, name.c_str(), return_type.c_str());
    }
    else {
        std::list<std::string> *typeList = ParseArgs(declarations);
        std::list<std::string>::reverse_iterator it = typeList->rbegin();
        std::printf("function declaration <line: %u, col: %u> %s %s (",
                    location.line, location.col, name.c_str(), return_type.c_str());
        while(it != typeList->rend()){
            if(it == --typeList->rend()){
                std::printf("%s)\n", (*it).c_str());
            }
            else{
                std::printf("%s, ", (*it).c_str());
            }
            it++;
        }


        *indent = *indent+1;

        std::list<DeclNode>::iterator itt = declarations->begin();
        while(itt != declarations->end()){
            itt->print();
            ++itt;
        }

        *indent = *indent-1;

    }

    if(cp_statement != NULL) {
        *indent = *indent+1;
        cp_statement->print();
        *indent = *indent-1;
    }
}

std::list<std::string> *FunctionNode::ParseArgs(std::list<DeclNode> *decls){
    std::list<DeclNode>::iterator it = decls->begin();
    std::list<VariableNode>::iterator itt;
    std::list<std::string> *typeList = new std::list<std::string>;
    while(it != decls->end()){
        itt = (*it).GetVariables()->begin();
        while(itt != (*it).GetVariables()->end()){
            typeList->push_front(itt->GetType());
            itt++;
        }
        it++;
    }

    return typeList;
}

// void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
