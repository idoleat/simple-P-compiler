#include <AST/ast.hpp>

AstNode::AstNode(const uint32_t line, const uint32_t col)
    : location(line, col) {}

// prevent the linker from complaining
AstNode::~AstNode() {}

const Location &AstNode::getLocation() const { return location; }
