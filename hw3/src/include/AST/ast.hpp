#ifndef __AST_H
#define __AST_H

#include <cstdint>
#include <string>
#include <iostream>

struct Location {
    Location(const uint32_t line, const uint32_t col) : line(line), col(col) {}

    uint32_t line;
    uint32_t col;
};

class AstNode {
  public:
    AstNode(const uint32_t line, const uint32_t col);
    virtual ~AstNode() = 0;

    const Location &getLocation() const;
    void outputIndentationSpace(const uint32_t &indent);
    virtual void print() = 0;

  protected:
    const Location location;
};

#endif
