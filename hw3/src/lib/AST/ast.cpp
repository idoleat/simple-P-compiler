#include <AST/ast.hpp>

AstNode::AstNode(const uint32_t line, const uint32_t col)
    : location(line, col) {}

// prevent the linker from complaining
AstNode::~AstNode() {}

const Location &AstNode::getLocation() const { return location; }

void AstNode::outputIndentationSpace(const uint32_t &indent) {
    int i = 0;
    while( i < indent*2){
        std::cout << " ";
        i++;
    }
}
