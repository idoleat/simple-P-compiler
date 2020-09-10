#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ast.hpp"
#include "AST/ConstantValue.hpp"
#include <string>

class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col);
    VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, float value, uint32_t *indent);
    VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, int value, uint32_t *indent);
    VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, char *value, uint32_t *indent);
    VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, bool value, uint32_t *indent);
    VariableNode(const uint32_t line, const uint32_t col, const char *name, const char *type, uint32_t *indent);
    VariableNode(const uint32_t line, const uint32_t col, const char *name, ConstantValueNode *constNode, uint32_t *indent);
    ~VariableNode() = default;

    void print() override;
    std::string GetType();

  private:
      uint32_t *indent;
      std::string name;
      std::string type = "default";
      ConstantValueNode *constantValue = NULL;
};

#endif
