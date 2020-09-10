#ifndef __AST_CONSTANT_VALUE_NODE_H
#define __AST_CONSTANT_VALUE_NODE_H

#include "AST/expression.hpp"
#include <variant>
#include <string>

class ConstantValueNode : public ExpressionNode {
  public:
    ConstantValueNode(const uint32_t line, const uint32_t col);
    ConstantValueNode(const uint32_t line, const uint32_t col, float value, uint32_t *indent);
    ConstantValueNode(const uint32_t line, const uint32_t col, int value, uint32_t *indent);
    ConstantValueNode(const uint32_t line, const uint32_t col, char *value, uint32_t *indent);
    ConstantValueNode(const uint32_t line, const uint32_t col, bool value, uint32_t *indent);
    ~ConstantValueNode() = default;

    void print() override;

    // excuse me WTF?
    /*
    float getValue(float a) override;
    int getValue(int a) override;
    std::string getValue(std::string a) override;
    bool getValue(bool a) override;
    */

  private:
      uint32_t *indent;
      /*
      struct value
      {
        value() = default;
        ~value() = default;
        enum{FLOAT, INT, STRING, BOOL} tag;
        union {
          float floatValue;
          int intValue;
          std::string strValue;
          bool boolValue;
        };
      }v;
      */
      float floatValue;
      int intValue;
      std::string strValue;
      bool boolValue;
};

#endif
