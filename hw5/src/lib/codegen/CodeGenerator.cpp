#include "codegen/CodeGenerator.hpp"
#include "visitor/AstNodeInclude.hpp"

#include <cassert>
#include <cstdarg>
#include <cstdio>

CodeGenerator::CodeGenerator(const char *in_file_name, const char *out_file_name) 
    : in_file_name(in_file_name) {
    this->out_fp = fopen(out_file_name, "w");
    assert(this->out_fp != NULL && "fopen() fails.\n");
}

CodeGenerator::~CodeGenerator() {
    fclose(this->out_fp);
}

void CodeGenerator::dumpInstrs(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(this->out_fp, format, args);
    va_end(args);
}

const char *CodeGenerator::getInFileName(){
    return in_file_name;
}

void CodeGenerator::visit(ProgramNode &p_program) {
    p_program.visitChildNodes(*this);
}

void CodeGenerator::visit(DeclNode &p_decl) {
    
}

void CodeGenerator::visit(VariableNode &p_variable) {
    
    p_variable.visitChildNodes(*this);
}

void CodeGenerator::visit(ConstantValueNode &p_constant_value) {
    
}

void CodeGenerator::visit(FunctionNode &p_function) {
    
}

void CodeGenerator::visit(CompoundStatementNode &p_compound_statement) {
    
}

void CodeGenerator::visit(PrintNode &p_print) {
    
}

void CodeGenerator::visit(BinaryOperatorNode &p_bin_op) {
    
}

void CodeGenerator::visit(UnaryOperatorNode &p_un_op) {
    
}

void CodeGenerator::visit(FunctionInvocationNode &p_func_invocation) {
    
}

void CodeGenerator::visit(VariableReferenceNode &p_variable_ref) {
    
}

void CodeGenerator::visit(AssignmentNode &p_assignment) {
    p_assignment.visitChildNodes(*this);
}

void CodeGenerator::visit(ReadNode &p_read) {
    
}

void CodeGenerator::visit(IfNode &p_if) {
    
}

void CodeGenerator::visit(WhileNode &p_while) {
    
}

void CodeGenerator::visit(ForNode &p_for) {
    
}

void CodeGenerator::visit(ReturnNode &p_return) {
    
}