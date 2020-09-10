#include "AST/function.hpp"
#include "visitor/AstNodeVisitor.hpp"

#include <cassert>

FunctionNode::FunctionNode(const uint32_t line, const uint32_t col,
                           const char *p_name, Decls *p_parameters,
                           const PType *p_type, CompoundStatementNode *p_body)
    : AstNode{line, col}, name(p_name), parameters(std::move(*p_parameters)),
      return_type(PTypeSharedPtr(p_type)), body(p_body) {}

const std::string &FunctionNode::getName() const { return name; }

const char *FunctionNode::getNameCString() const { return name.c_str(); }

const PType *FunctionNode::getTypePtr() const { return return_type.get(); }

const char *FunctionNode::getTypeCString() const {
    return return_type->getPTypeCString();
}

const FunctionNode::Decls &FunctionNode::getParameters() const {
    return parameters;
}

std::string FunctionNode::getParametersTypeString(const Decls &parameters) {
    std::string type_string;

    for (const auto &parameter : parameters) {
        for (const auto &var : parameter->getVariables()) {
            type_string.append(var->getTypeCString()).append(", ");
        }
    }

    if (!parameters.empty()) {
        // remove the trailing ", "
        type_string.erase(type_string.end() - 2, type_string.end());
    }

    return type_string;
}

const char *FunctionNode::getPrototypeCString() const {
    if (!prototype_string_is_valid) {
        prototype_string += return_type->getPTypeCString();

        prototype_string += " (";
        prototype_string += getParametersTypeString(parameters);
        prototype_string += ")";
        prototype_string_is_valid = true;
    }

    return prototype_string.c_str();
}

bool FunctionNode::isDefined() const { return (body) ? true : false; }

FunctionNode::Decls::size_type
FunctionNode::getParametersNum(const Decls &parameter_decls) {
    FunctionNode::Decls::size_type num = 0;

    for (const auto &decl : parameter_decls) {
        num += decl->getVariables().size();
    }

    return num;
}

void FunctionNode::accept(AstNodeVisitor &p_visitor) { p_visitor.visit(*this); }

void FunctionNode::visitChildNodes(AstNodeVisitor &p_visitor) {
    for (auto &parameter : parameters) {
        parameter->accept(p_visitor);
    }
    if (body) {
        body->accept(p_visitor);
    }
}
