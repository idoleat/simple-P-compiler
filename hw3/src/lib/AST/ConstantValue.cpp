#include "AST/ConstantValue.hpp"

ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col)
    : ExpressionNode{line, col} {}
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, float value, uint32_t *indent)
    : ExpressionNode{line, col, "real"} , floatValue(value), indent(indent) {}
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, int value, uint32_t *indent)
    : ExpressionNode{line, col, "integer"} , intValue(value), indent(indent) {}
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, char *value, uint32_t *indent)
    : ExpressionNode{line, col, "string"} , strValue(std::string(value)), indent(indent) {}
ConstantValueNode::ConstantValueNode(const uint32_t line, const uint32_t col, bool value, uint32_t *indent)
    : ExpressionNode{line, col, "boolean"} , boolValue(value), indent(indent) {} 

void ConstantValueNode::print() {
    outputIndentationSpace(*indent);
/*
    switch (v.tag)
    {
        case value::INT : printf("constant <line: %u, col: %u> %d\n", location.line, location.col, v.intValue); break;
        case value::FLOAT : printf("constant <line: %u, col: %u> %d\n", location.line, location.col, v.floatValue); break;
        case value::STRING : printf("constant <line: %u, col: %u> %d\n", location.line, location.col, v.strValue); break;
        case value::BOOL : printf("constant <line: %u, col: %u> %d\n", location.line, location.col, v.boolValue); break;
    }
*/
    if(type == "integer")
        printf("constant <line: %u, col: %u> %d\n", location.line, location.col, intValue);
    if(type == "real")
        printf("constant <line: %u, col: %u> %f\n", location.line, location.col, floatValue);
    if(type == "string")
        printf("constant <line: %u, col: %u> %s\n", location.line, location.col, strValue.c_str());
    if(type == "boolean")
        printf("constant <line: %u, col: %u> %s\n", location.line, location.col, boolValue?"true":"false");
}

// excuse me WTF?
/*
float ConstantValueNode::getValue(float a){
    return floatValue;
}

int ConstantValueNode::getValue(int a){
    return intValue;
}

std::string ConstantValueNode::getValue(std::string a){
    return strValue;
}

bool ConstantValueNode::getValue(bool a){
    return boolValue;
}
*/