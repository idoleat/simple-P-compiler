#ifndef __AST_PROGRAM_NODE_H
#define __AST_PROGRAM_NODE_H

#include "AST/ast.hpp"

class ProgramNode : public AstNode {
  public:
    ProgramNode(const uint32_t line, const uint32_t col,
                const char *p_name
                /* TODO: return type, declarations, functions,
                 *       compound statement */);
    ~ProgramNode() = default;

    // visitor pattern version: const char *getNameCString() const; 

    void print() override;

  private:
    const std::string name;
    // TODO: return type, declarations, functions, compound statement
};

#endif
