#include "AST/variable.hpp"

VariableNode::VariableNode(const uint32_t line, const uint32_t col)
    : AstNode{line, col} {}
VariableNode::VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, float value, uint32_t *indent)
    : AstNode{line, col}, name(std::string(name)), type(std::string(type)), indent(indent){
        constantValue = new ConstantValueNode(line, col, value, indent);
    }
VariableNode::VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, int value, uint32_t *indent)
    : AstNode{line, col}, name(std::string(name)), type(std::string(type)), indent(indent){
        constantValue = new ConstantValueNode(line, col, value, indent);
    }
VariableNode::VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, char *value, uint32_t *indent)
    : AstNode{line, col}, name(std::string(name)), type(std::string(type)), indent(indent){
        constantValue = new ConstantValueNode(line, col, value, indent);
    }
VariableNode::VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, bool value, uint32_t *indent)
    : AstNode{line, col}, name(std::string(name)), type(std::string(type)), indent(indent){
        constantValue = new ConstantValueNode(line, col, value, indent);
    }
VariableNode::VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, uint32_t *indent)
    : AstNode{line, col}, name(std::string(name)), type(std::string(type)), indent(indent){}
VariableNode::VariableNode(const uint32_t line, const uint32_t col, const char *name, ConstantValueNode *constNode, uint32_t *indent)
    : AstNode{line, col}, name(std::string(name)), constantValue(constNode), indent(indent){
        type = constantValue->getType();
    }


void VariableNode::print() {
    outputIndentationSpace(*indent);

    std::printf("variable <line: %u, col: %u> %s %s\n",
                location.line, location.col, name.c_str() , type.c_str());

    

    if(constantValue != NULL) {
        *indent = *indent+1;

        constantValue->print();
        
        *indent = *indent-1;
    }

    
}

std::string VariableNode::GetType(){
    return type;
}

// void VariableNode::visitChildNodes(AstNodeVisitor &p_visitor) {
//     // TODO
// }
