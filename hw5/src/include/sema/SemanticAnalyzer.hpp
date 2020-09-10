#ifndef __SEMA_SEMANTIC_ANALYZER_H
#define __SEMA_SEMANTIC_ANALYZER_H

#include "sema/SymbolTable.hpp"
#include "visitor/AstNodeVisitor.hpp"
#include "codegen/CodeGenerator.hpp"

class SemanticAnalyzer : public AstNodeVisitor {
  public:
    SemanticAnalyzer(const bool opt_dmp);
    SemanticAnalyzer(const bool opt_dmp, CodeGenerator *cg);
    ~SemanticAnalyzer() = default;

    void visit(ProgramNode &p_program) override;
    void visit(DeclNode &p_decl) override;
    void visit(VariableNode &p_variable) override;
    void visit(ConstantValueNode &p_constant_value) override;
    void visit(FunctionNode &p_function) override;
    void visit(CompoundStatementNode &p_compound_statement) override;
    void visit(PrintNode &p_print) override;
    void visit(BinaryOperatorNode &p_bin_op) override;
    void visit(UnaryOperatorNode &p_un_op) override;
    void visit(FunctionInvocationNode &p_func_invocation) override;
    void visit(VariableReferenceNode &p_variable_ref) override;
    void visit(AssignmentNode &p_assignment) override;
    void visit(ReadNode &p_read) override;
    void visit(IfNode &p_if) override;
    void visit(WhileNode &p_while) override;
    void visit(ForNode &p_for) override;
    void visit(ReturnNode &p_return) override;

  private:
    friend SymbolEntry *addSymbolFromVariableNode(SemanticAnalyzer &analyzer,
                                                  VariableNode &p_variable);
    friend bool isInForLoop(SemanticAnalyzer &analyzer);
    friend bool isInLocal(SemanticAnalyzer &analyzer);
    friend bool isInFunction(SemanticAnalyzer &analyzer);

  private:
    enum class SemanticContext : uint8_t {
        kGlobal,
        kFunction,
        kForLoop,
        kLocal
    };

    SymbolManager symbol_manager;
    CodeGenerator *CG;

    // record the context of current scope for performing special behavior
    std::vector<SemanticContext> context_stack;
    std::vector<const PType *> return_type_stack;
};

#endif
