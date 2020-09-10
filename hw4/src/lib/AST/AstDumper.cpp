#include "AST/AstDumper.hpp"
#include "AST/BinaryOperator.hpp"
#include "AST/CompoundStatement.hpp"
#include "AST/ConstantValue.hpp"
#include "AST/FunctionInvocation.hpp"
#include "AST/UnaryOperator.hpp"
#include "AST/VariableReference.hpp"
#include "AST/assignment.hpp"
#include "AST/decl.hpp"
#include "AST/for.hpp"
#include "AST/function.hpp"
#include "AST/if.hpp"
#include "AST/print.hpp"
#include "AST/program.hpp"
#include "AST/read.hpp"
#include "AST/return.hpp"
#include "AST/variable.hpp"
#include "AST/while.hpp"

#include <cstdio>

void AstDumper::incrementIndentation() {
    m_indentation += m_indentation_stride;
}

void AstDumper::decrementIndentation() {
    m_indentation -= m_indentation_stride;
}

static void outputIndentationSpace(const uint32_t indentation) {
    std::printf("%*s", indentation, "");
}

void AstDumper::visit(ProgramNode &p_program) {
    outputIndentationSpace(m_indentation);

    std::printf("program <line: %u, col: %u> %s %s\n",
                p_program.getLocation().line, p_program.getLocation().col,
                p_program.getNameCString(), p_program.getTypeCString());

    incrementIndentation();
    p_program.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(DeclNode &p_decl) {
    outputIndentationSpace(m_indentation);

    std::printf("declaration <line: %u, col: %u>\n", p_decl.getLocation().line,
                p_decl.getLocation().col);
    incrementIndentation();
    p_decl.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(VariableNode &p_variable) {
    outputIndentationSpace(m_indentation);

    std::printf("variable <line: %u, col: %u> %s %s\n",
                p_variable.getLocation().line, p_variable.getLocation().col,
                p_variable.getNameCString(), p_variable.getTypeCString());

    incrementIndentation();
    p_variable.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(ConstantValueNode &p_constant_value) {
    outputIndentationSpace(m_indentation);

    std::printf("constant <line: %u, col: %u> %s\n",
                p_constant_value.getLocation().line,
                p_constant_value.getLocation().col,
                p_constant_value.getConstantValueCString());
}

void AstDumper::visit(FunctionNode &p_function) {
    outputIndentationSpace(m_indentation);

    std::printf("function declaration <line: %u, col: %u> %s %s\n",
                p_function.getLocation().line, p_function.getLocation().col,
                p_function.getNameCString(), p_function.getPrototypeCString());

    incrementIndentation();
    p_function.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(CompoundStatementNode &p_compound_statement) {
    outputIndentationSpace(m_indentation);

    std::printf("compound statement <line: %u, col: %u>\n",
                p_compound_statement.getLocation().line,
                p_compound_statement.getLocation().col);

    incrementIndentation();
    p_compound_statement.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(PrintNode &p_print) {
    outputIndentationSpace(m_indentation);

    std::printf("print statement <line: %u, col: %u>\n",
                p_print.getLocation().line, p_print.getLocation().col);

    incrementIndentation();
    p_print.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(BinaryOperatorNode &p_bin_op) {
    outputIndentationSpace(m_indentation);

    std::printf("binary operator <line: %u, col: %u> %s\n",
                p_bin_op.getLocation().line, p_bin_op.getLocation().col,
                p_bin_op.getOpCString());

    incrementIndentation();
    p_bin_op.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(UnaryOperatorNode &p_un_op) {
    outputIndentationSpace(m_indentation);

    std::printf("unary operator <line: %u, col: %u> %s\n",
                p_un_op.getLocation().line, p_un_op.getLocation().col,
                p_un_op.getOpCString());

    incrementIndentation();
    p_un_op.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(FunctionInvocationNode &p_func_invocation) {
    outputIndentationSpace(m_indentation);

    std::printf("function invocation <line: %u, col: %u> %s\n",
                p_func_invocation.getLocation().line,
                p_func_invocation.getLocation().col,
                p_func_invocation.getNameCString());

    incrementIndentation();
    p_func_invocation.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(VariableReferenceNode &p_variable_ref) {
    outputIndentationSpace(m_indentation);

    std::printf("variable reference <line: %u, col: %u> %s\n",
                p_variable_ref.getLocation().line,
                p_variable_ref.getLocation().col,
                p_variable_ref.getNameCString());

    incrementIndentation();
    p_variable_ref.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(AssignmentNode &p_assignment) {
    outputIndentationSpace(m_indentation);

    std::printf("assignment statement <line: %u, col: %u>\n",
                p_assignment.getLocation().line,
                p_assignment.getLocation().col);

    incrementIndentation();
    p_assignment.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(ReadNode &p_read) {
    outputIndentationSpace(m_indentation);

    std::printf("read statement <line: %u, col: %u>\n",
                p_read.getLocation().line, p_read.getLocation().col);

    incrementIndentation();
    p_read.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(IfNode &p_if) {
    outputIndentationSpace(m_indentation);

    std::printf("if statement <line: %u, col: %u>\n", p_if.getLocation().line,
                p_if.getLocation().col);

    incrementIndentation();
    p_if.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(WhileNode &p_while) {
    outputIndentationSpace(m_indentation);

    std::printf("while statement <line: %u, col: %u>\n",
                p_while.getLocation().line, p_while.getLocation().col);

    incrementIndentation();
    p_while.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(ForNode &p_for) {
    outputIndentationSpace(m_indentation);

    std::printf("for statement <line: %u, col: %u>\n", p_for.getLocation().line,
                p_for.getLocation().col);

    incrementIndentation();
    p_for.visitChildNodes(*this);
    decrementIndentation();
}

void AstDumper::visit(ReturnNode &p_return) {
    outputIndentationSpace(m_indentation);

    std::printf("return statement <line: %u, col: %u>\n",
                p_return.getLocation().line, p_return.getLocation().col);

    incrementIndentation();
    p_return.visitChildNodes(*this);
    decrementIndentation();
}
