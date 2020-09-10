#ifndef __AST_EXPRESSION_NODE_H
#define __AST_EXPRESSION_NODE_H

#include "AST/ast.hpp"
#include <string>

class ExpressionNode : public AstNode {
  public:
    ExpressionNode(const uint32_t line, const uint32_t col);
    ExpressionNode(const uint32_t line, const uint32_t col, const char *type);
    ~ExpressionNode() = default;
    std::string getType();
    
    // excuse me WTF?
    /*
    virtual float getValue(float a) = 0;
    virtual int getValue(int a) = 0;
    virtual std::string getValue(std::string a) = 0;
    virtual bool getValue(bool a) = 0;
    */

  protected:
    std::string type;
    // TODO: for next assignment
};

#endif
