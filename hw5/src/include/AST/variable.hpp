#ifndef __AST_VARIABLE_NODE_H
#define __AST_VARIABLE_NODE_H

#include "AST/ConstantValue.hpp"
#include "AST/PType.hpp"
#include "AST/ast.hpp"

#include <memory>

class VariableNode : public AstNode {
  public:
    VariableNode(const uint32_t line, const uint32_t col,
                 const std::string &name, const PTypeSharedPtr &p_type,
                 const std::shared_ptr<ConstantValueNode> &p_constant);
    ~VariableNode() = default;

    const std::string &getName() const;
    const char *getNameCString() const;

    const PType *getTypePtr() const;
    const char *getTypeCString() const;

    const Constant *getConstantPtr() const;

    void accept(AstNodeVisitor &p_visitor) override;
    void visitChildNodes(AstNodeVisitor &p_visitor) override;

  private:
    const std::string name;
    const PTypeSharedPtr type;
    std::shared_ptr<ConstantValueNode> constant;
};

#endif
