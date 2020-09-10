#include "sema/SemanticAnalyzer.hpp"
#include "sema/error.hpp"
#include "visitor/AstNodeInclude.hpp"

#include "visitor/AstNodeInclude.hpp"

#include <cassert>

SemanticAnalyzer::SemanticAnalyzer(const bool opt_dmp)
    : symbol_manager(opt_dmp) {}

SemanticAnalyzer::SemanticAnalyzer(const bool opt_dmp, CodeGenerator *cg)
    : symbol_manager(opt_dmp), CG(cg) {}

const char *kRedeclaraedSymbolErrMsg = "symbol %s is redeclared";

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    //// Code Generation
    CG->dumpInstrs("%s%s%s%s", 
        "\t.file \"", CG->getInFileName(), "\"\n",
        "\t.option nopic\n"
    );

    // global symbol table
    symbol_manager.pushGlobalScope();
    context_stack.emplace_back(SemanticContext::kGlobal);
    return_type_stack.emplace_back(p_program.getTypePtr());

    auto success = symbol_manager.addSymbol(
        p_program.getName(), SymbolEntry::KindEnum::kProgramKind,
        p_program.getTypePtr(), static_cast<const Constant *>(nullptr));
    if (!success) {
        logSemanticError(p_program.getLocation(), kRedeclaraedSymbolErrMsg,
                         p_program.getNameCString());
    }

    // Generate RISC-V instructions for program header
    
    p_program.visitChildNodes(*this);

    // TODO: semantic checks

    return_type_stack.pop_back();
    context_stack.pop_back();
    symbol_manager.popScope();
    
    
}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    p_decl.visitChildNodes(*this);
}

bool isInForLoop(SemanticAnalyzer &analyzer) {
    return analyzer.context_stack.back() ==
           SemanticAnalyzer::SemanticContext::kForLoop;
}

bool isInLocal(SemanticAnalyzer &analyzer) {
    return analyzer.context_stack.back() ==
           SemanticAnalyzer::SemanticContext::kLocal;
}

bool isInFunction(SemanticAnalyzer &analyzer) {
    return analyzer.context_stack.back() ==
           SemanticAnalyzer::SemanticContext::kFunction;
}

SymbolEntry *addSymbolFromVariableNode(SemanticAnalyzer &analyzer,
                                       VariableNode &p_variable) {
    SymbolEntry::KindEnum kind;
    const Constant *constant_ptr = p_variable.getConstantPtr();

    if (isInForLoop(analyzer)) {
        kind = SymbolEntry::KindEnum::kLoopVarKind;
    } else if (isInLocal(analyzer)) {
        kind = constant_ptr ? SymbolEntry::KindEnum::kConstantKind
                            : SymbolEntry::KindEnum::kVariableKind;
    } else if (isInFunction(analyzer)) {
        kind = SymbolEntry::KindEnum::kParameterKind;
    } else { // global scope
        kind = constant_ptr ? SymbolEntry::KindEnum::kConstantKind
                            : SymbolEntry::KindEnum::kVariableKind;
    }

    auto *entry = analyzer.symbol_manager.addSymbol(
        p_variable.getName(), kind, p_variable.getTypePtr(), constant_ptr);
    if (!entry) {
        logSemanticError(p_variable.getLocation(), kRedeclaraedSymbolErrMsg,
                         p_variable.getNameCString());
    }

    return entry;
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
    auto *entry = addSymbolFromVariableNode(*this, p_variable);

    p_variable.visitChildNodes(*this);

    if (!entry) {
        return;
    }

    for (auto dimension : p_variable.getTypePtr()->getDimensions()) {
        if (dimension == 0) {
            entry->setError();

            logSemanticError(p_variable.getLocation(),
                             "'%s' declared as an array with an index that is "
                             "not greater than 0",
                             p_variable.getNameCString());
        }
    }

    //// Code Generation
    if(entry->getLevel() == 0){
        if(p_variable.getConstantPtr() != nullptr){
            if(CG->FirstGlobalConstant){
                CG->dumpInstrs("%s\n", ".section    .rodata       # emit rodata section");
                CG->dumpInstrs("%s\n", "\t.align 2");
                CG->FirstGlobalConstant = false;
            }

            CG->dumpInstrs("%s%s%s\n", "    .globl ", p_variable.getNameCString(), "              # emit symbol 'd' to the global symbol table");
            CG->dumpInstrs("%s%s%s\n", "    .type ", p_variable.getNameCString(), ", @object");
            CG->dumpInstrs("%s%s\n", p_variable.getNameCString(), ":");
            CG->dumpInstrs("%s\n", "    .word 4");
        }
        else
        {
            CG->dumpInstrs("%s%s%s", ".comm ", p_variable.getNameCString(), ", 4, 4\n");
        }
    }
    else{
        if(p_variable.getConstantPtr() != nullptr){
            /*
addi sp, sp, -4
sw t0, 0(sp)     # push the address to the stack
li t1, [const value]
addi sp, sp, -4
sw t1, 0(sp)     # push the value to the stack
lw t1, 0(sp)     # pop the value from the stack
addi sp, sp, 4
lw t0, 0(sp)     # pop the address from the stack
addi sp, sp, 4
sw t1, 0(t0)     # b = 4
            */
            if(entry->getPrevEntry() != nullptr)    // fatal error here! Won't fix. Just like Chrome.
                entry->getAttribute().setDepth(entry->getPrevEntry()->getAttribute().getDepth() + 4);
            else
                entry->getAttribute().setDepth(4);
                
            CG->dumpInstrs("%s\n", "\taddi sp, sp, -4       # new variable");  
            CG->dumpInstrs("%s\n", "\taddi sp, sp, -4"); // new place for later addression storage
            CG->dumpInstrs("%s\n", "\taddi t0, sp, 4"); 
            CG->dumpInstrs("%s\n", "\tsw t0, 0(sp)     # push the address to the stack");
            CG->dumpInstrs("%s%s\n", "\tli t1, ", p_variable.getConstantPtr()->getConstantValueCString());
            CG->dumpInstrs("%s\n", "\taddi sp, sp, -4"); 
            CG->dumpInstrs("%s\n", "\tsw t1, 0(sp)     # push the value to the stack");
            CG->dumpInstrs("%s\n", "\tlw t1, 0(sp)     # pop the value from the stack");
            CG->dumpInstrs("%s\n", "\taddi sp, sp, 4"); 
            CG->dumpInstrs("%s\n", "\tlw t0, 0(sp)     # pop the address from the stack"); 
            CG->dumpInstrs("%s\n", "\taddi sp, sp, 4"); 
            CG->dumpInstrs("%s\n", "\tsw t1, 0(sp)     # assign constant value");
            
            //CG->dumpInstrs("%s%d%s\n", "\tsw t1, -", entry->getAttribute().getDepth(),"(s0)");
        }
        else
        {
            if(entry->getPrevEntry() != nullptr)    // fatal error here! Won't fix. Just like Chrome.
                entry->getAttribute().setDepth(entry->getPrevEntry()->getAttribute().getDepth() + 4);
            else
                entry->getAttribute().setDepth(4);
            CG->dumpInstrs("%s%s", "\taddi sp, sp, -4", "\n");
        }
    }


}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
    p_constant_value.setInferredType(
        p_constant_value.getTypePtr()->getStructElementType(0));
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
    auto success = symbol_manager.addSymbol(
        p_function.getName(), SymbolEntry::KindEnum::kFunctionKind,
        p_function.getTypePtr(), &p_function.getParameters());
    if (!success) {
        logSemanticError(p_function.getLocation(), kRedeclaraedSymbolErrMsg,
                         p_function.getNameCString());
    }

    context_stack.emplace_back(SemanticContext::kFunction);
    return_type_stack.emplace_back(p_function.getTypePtr());
    symbol_manager.pushScope();

    //// Code Generation
    CG->dumpInstrs("%s\n", ".section     .text");
       CG->dumpInstrs("%s\n", "\t.align 2");
       CG->dumpInstrs("%s\n", "\t.globl main          # emit symbol 'main' to the global symbol table");
       CG->dumpInstrs("%s%s%s\n", "\t.type main, @function");
       CG->dumpInstrs("%s\n", "main:");

    p_function.visitChildNodes(*this);

    // TODO: semantic checks

    symbol_manager.popScope();
    return_type_stack.pop_back();
    context_stack.pop_back();
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
    bool first_scope_of_function = isInFunction(*this);

    context_stack.emplace_back(SemanticContext::kLocal);
    if (!first_scope_of_function) {
        symbol_manager.pushScope();
    }

    //// Code Generation - fatal error here!
    if(symbol_manager.getCurrentLevel() == 1 && !isInFunction(*this) ){
        /*
.section     .text
    .global main          # emit symbol 'main' to the global symbol table
    .type main, @function
main:
        */
       CG->dumpInstrs("%s\n", ".section     .text");
       CG->dumpInstrs("%s\n", "\t.align 2");
       CG->dumpInstrs("%s\n", "\t.globl main          # emit symbol 'main' to the global symbol table");
       CG->dumpInstrs("%s\n", "\t.type main, @function");
       CG->dumpInstrs("%s\n", "main:");
    }

    p_compound_statement.visitChildNodes(*this);

    if (!first_scope_of_function) {
        symbol_manager.popScope();
    }
    context_stack.pop_back();
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
    

    p_print.visitChildNodes(*this);

    auto *target_type = p_print.getTarget()->getInferredType();
    if (!target_type) {
        return;
    }

    if (!target_type->isScalar()) {
        logSemanticError(p_print.getTarget()->getLocation(),
                         "expression of print statement must be scalar type");
    }

    //// Code Generaton
    /*
lw t1, 0(t0)     # load the value of 'a'
addi sp, sp, -4
sw t1, 0(sp)     # push the value to the stack
lw a0, 0(sp)     # pop the value from the stack to the first argument register 'a0' 
addi sp, sp, 4
jal ra, printInt # call function 'printInt'
    */

    CG->dumpInstrs("%s\n", "\tlw t1, 0(sp)     # load the value of child node");
    CG->dumpInstrs("%s\n", "\taddi sp, sp, -4");
    CG->dumpInstrs("%s\n", "\tsw t1, 0(sp)     # push the value to the stack");
    CG->dumpInstrs("%s\n", "\tlw a0, 0(sp)     # pop the value from the stack to the first argument register 'a0' ");
    CG->dumpInstrs("%s\n", "\taddi sp, sp, 4");
    CG->dumpInstrs("%s\n", "\taddi sp, sp, 4    # pop the value of chile node");
    CG->dumpInstrs("%s\n", "\tjal ra, printInt # call function 'printInt'");
}

static bool checkOperandsInArithmeticOp(Operator op, const PType *left_type,
                                        const PType *right_type) {
    if (op == Operator::kPlusOp && left_type->isString() &&
        right_type->isString()) {
        return true;
    }

    if ((left_type->isInteger() || left_type->isReal()) &&
        (right_type->isInteger() || right_type->isReal())) {
        return true;
    }

    return false;
}

static bool checkOperandsInRelationOp(const PType *left_type,
                                      const PType *right_type) {
    if ((left_type->isInteger() || left_type->isReal()) &&
        (right_type->isInteger() || right_type->isReal())) {
        return true;
    }
    return false;
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
    p_bin_op.visitChildNodes(*this);

    auto *left_type = p_bin_op.getLeftOperand()->getInferredType();
    auto *right_type = p_bin_op.getRightOperand()->getInferredType();
    if (!left_type || !right_type) {
        return;
    }

    switch (p_bin_op.getOp()) {
    case Operator::kPlusOp:
    case Operator::kMinusOp:
    case Operator::kMultiplyOp:
    case Operator::kDivideOp:
        if (!checkOperandsInArithmeticOp(p_bin_op.getOp(), left_type,
                                         right_type)) {
            break;
        }

        if (left_type->isString()) {
            p_bin_op.setInferredType(
                new PType(PType::PrimitiveTypeEnum::kStringType));
        } else if (left_type->isReal() || right_type->isReal()) {
            p_bin_op.setInferredType(
                new PType(PType::PrimitiveTypeEnum::kRealType));
        } else {
            p_bin_op.setInferredType(
                new PType(PType::PrimitiveTypeEnum::kIntegerType));

            //// Code Generation - take top 2 values in stack and add.
            // lw t1, 0(sp)
            // lw t2, 4(sp)
            // add t1, t1, t2
            // addi sp, sp, 4
            // sw t1, 0(sp)
            CG->dumpInstrs("%s\n", "\tlw t1, 0(sp)        # load oprands");
            CG->dumpInstrs("%s\n", "\tlw t2, 4(sp)");
            switch (p_bin_op.getOp()) {
            case Operator::kPlusOp:
                CG->dumpInstrs("%s\n", "\tadd t1, t1, t2      # add oprands");
                break;
            case Operator::kMinusOp:
                CG->dumpInstrs("%s\n", "\tsub t1, t1, t2      # add oprands");
                break;
            case Operator::kMultiplyOp:
                CG->dumpInstrs("%s\n", "\tadd t1, t1, t2      # MUL(add haha) oprands");
                break;
            case Operator::kDivideOp:
                CG->dumpInstrs("%s\n", "\tadd t1, t1, t2      # DIV(add haha) oprands");
                break;
            }
            CG->dumpInstrs("%s\n", "\taddi sp, sp, 4");
            CG->dumpInstrs("%s\n", "\tsw t1, 0(sp)        # save result");
        }

        return;
    case Operator::kModOp:
        if (!(left_type->isInteger() && right_type->isInteger())) {
            break;
        }
        p_bin_op.setInferredType(
            new PType(PType::PrimitiveTypeEnum::kIntegerType));
        //// Code Generation
        CG->dumpInstrs("%s\n", "\tlw t1, 0(sp)        # load oprands");
        CG->dumpInstrs("%s\n", "\tlw t2, 4(sp)");
        CG->dumpInstrs("%s\n", "MOD: sub t3, t2, t1   # mod op");
        CG->dumpInstrs("%s\n", "\tbge MOD, t3, 0");
        CG->dumpInstrs("%s\n", "\tadd t3, t3, t1");
        CG->dumpInstrs("%s\n", "\taddi sp, sp, 4");
        CG->dumpInstrs("%s\n", "\tsw t3, 0(sp)        # save result");
        return;
    case Operator::kAndOp:
    case Operator::kOrOp:
        if (!(left_type->isBool() && right_type->isBool())) {
            break;
        }
        p_bin_op.setInferredType(
            new PType(PType::PrimitiveTypeEnum::kBoolType));
        return;
    case Operator::kLessOp:
    case Operator::kLessOrEqualOp:
    case Operator::kEqualOp:
    case Operator::kGreaterOp:
    case Operator::kGreaterOrEqualOp:
    case Operator::kNotEqualOp:
        if (!checkOperandsInRelationOp(left_type, right_type)) {
            break;
        }
        p_bin_op.setInferredType(
            new PType(PType::PrimitiveTypeEnum::kBoolType));
        //// Code Generation
        CG->dumpInstrs("%s\n", "\tlw t1, 0(sp)        # load oprands");
        CG->dumpInstrs("%s\n", "\tlw t2, 4(sp)");
        CG->dumpInstrs("%s\n", "\tadd t1, t1, t2      # compare(add haha) oprands");
        CG->dumpInstrs("%s\n", "\taddi sp, sp, 4");
        CG->dumpInstrs("%s\n", "\tsw t1, 0(sp)        # save result");
        return;
    default:
        assert(false &&
               "Shouldn't reach here (there is an unknown op or unary op)");
    }

    logSemanticError(p_bin_op.getLocation(),
                     "invalid operands to binary operator '%s' ('%s' and '%s')",
                     p_bin_op.getOpCString(), left_type->getPTypeCString(),
                     right_type->getPTypeCString());
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    p_un_op.visitChildNodes(*this);

    auto *operand_type = p_un_op.getOperand()->getInferredType();
    if (!operand_type) {
        return;
    }

    switch (p_un_op.getOp()) {
    case Operator::kNegOp:
        if (operand_type->isInteger()) {
            p_un_op.setInferredType(
                new PType(PType::PrimitiveTypeEnum::kIntegerType));
        } else if (operand_type->isReal()) {
            p_un_op.setInferredType(
                new PType(PType::PrimitiveTypeEnum::kRealType));
        } else {
            break;
        }
        return;
    case Operator::kNotOp:
        if (!operand_type->isBool()) {
            break;
        }
        p_un_op.setInferredType(new PType(PType::PrimitiveTypeEnum::kBoolType));
        return;
    default:
        assert(false &&
               "Shouldn't reach here (there is an unknown op or unary op)");
    }

    logSemanticError(p_un_op.getLocation(),
                     "invalid operand to unary operator '%s' ('%s')",
                     p_un_op.getOpCString(), operand_type->getPTypeCString());
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    p_func_invocation.visitChildNodes(*this);

    auto *entry = symbol_manager.lookup(p_func_invocation.getName());
    if (!entry) {
        logSemanticError(p_func_invocation.getLocation(),
                         "use of undeclared symbol '%s'",
                         p_func_invocation.getNameCString());
        return;
    }

    if (entry->getKind() != SymbolEntry::KindEnum::kFunctionKind) {
        logSemanticError(p_func_invocation.getLocation(),
                         "call of non-function symbol '%s'",
                         p_func_invocation.getNameCString());
        return;
    }

    auto &arguments = p_func_invocation.getArguments();
    auto &parameters = *entry->getAttribute().parameters();
    if (arguments.size() != FunctionNode::getParametersNum(parameters)) {
        logSemanticError(p_func_invocation.getLocation(),
                         "too few/much arguments provided for function '%s'",
                         p_func_invocation.getNameCString());
        return;
    }

    FunctionInvocationNode::Exprs::const_iterator argument = arguments.begin();
    for (std::size_t i = 0; i < parameters.size(); ++i) {
        auto &variables = parameters[i]->getVariables();
        for (auto &variable : variables) {
            auto *expr_type = (*argument)->getInferredType();
            if (!expr_type) {
                return;
            }

            if (!expr_type->compare(variable->getTypePtr())) {
                logSemanticError(
                    (*argument)->getLocation(),
                    "incompatible type passing '%s' to parameter of type '%s'",
                    expr_type->getPTypeCString(),
                    variable->getTypePtr()->getPTypeCString());
                return;
            }
            argument++;
        }
    }

    p_func_invocation.setInferredType(
        new PType(entry->getTypePtr()->getPrimitiveType()));
}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
    p_variable_ref.visitChildNodes(*this);

    auto *entry = symbol_manager.lookup(p_variable_ref.getName());

    if (entry == nullptr) {
        logSemanticError(p_variable_ref.getLocation(),
                         "use of undeclared symbol '%s'",
                         p_variable_ref.getNameCString());
        return;
    }

    if (!(entry->getKind() == SymbolEntry::KindEnum::kParameterKind ||
          entry->getKind() == SymbolEntry::KindEnum::kVariableKind ||
          entry->getKind() == SymbolEntry::KindEnum::kLoopVarKind ||
          entry->getKind() == SymbolEntry::KindEnum::kConstantKind)) {
        logSemanticError(p_variable_ref.getLocation(),
                         "use of non-variable symbol '%s'",
                         p_variable_ref.getNameCString());
        return;
    }

    if (entry->hasError()) {
        return;
    }

    for (auto &index : p_variable_ref.getIndices()) {
        if (index->getInferredType() == nullptr) {
            return;
        }

        if (!index->getInferredType()->isInteger()) {
            logSemanticError(index->getLocation(),
                             "index of array reference must be an integer");
            return;
        }
    }

    auto *inferred_type = entry->getTypePtr()->getStructElementType(
        p_variable_ref.getIndices().size());
    if (!inferred_type) {
        logSemanticError(p_variable_ref.getLocation(),
                         "there is an over array subscript on '%s'",
                         p_variable_ref.getNameCString());
        return;
    }

    p_variable_ref.setInferredType(inferred_type);

    //// Code Generation
    if(entry->getLevel() == 0){
        CG->dumpInstrs("%s%s\n", "\tla t0, ", entry->getNameCString());
        CG->dumpInstrs("%s\n", "lw t1, 0(t0)");
        CG->dumpInstrs("%s\n", "\taddi sp, sp, -4     #push the var value into the stack"); 
        CG->dumpInstrs("%s\n", "\tsw t1, 0(sp)");
    }
    else{
        CG->dumpInstrs("%s%d\n", "\taddi t0, s0, -", entry->getAttribute().getDepth());
        CG->dumpInstrs("%s\n", "lw t1, 0(t0)");
        CG->dumpInstrs("%s\n", "\taddi sp, sp, -4     #push the var value into the stack"); 
        CG->dumpInstrs("%s\n", "\tsw t1, 0(sp)");
    }
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    p_assignment.visitChildNodes(*this);
    // TODO: semantic checks
    //
    auto *lvalue = p_assignment.getLvalue();

    auto *lvalue_type = lvalue->getInferredType();
    if (!lvalue_type) {
        return;
    }

    if (!lvalue_type->isScalar()) {
        logSemanticError(lvalue->getLocation(),
                         "array assignment is not allowed");
        return;
    }

    auto *entry = symbol_manager.lookup(lvalue->getName());
    if (entry->getKind() == SymbolEntry::KindEnum::kConstantKind) {
        logSemanticError(lvalue->getLocation(),
                         "cannot assign to variable '%s' which is a constant",
                         lvalue->getNameCString());
        return;
    }

    if (entry->getKind() == SymbolEntry::KindEnum::kLoopVarKind &&
        context_stack.back() != SemanticContext::kForLoop) {
        logSemanticError(lvalue->getLocation(),
                         "the value of loop variable cannot be modified inside "
                         "the loop body");
        return;
    }

    auto *expr = p_assignment.getExpr();
    auto *expr_type = expr->getInferredType();
    if (!expr_type) {
        return;
    }

    if (!expr_type->isScalar()) {
        logSemanticError(expr->getLocation(),
                         "array assignment is not allowed");
        return;
    }

    if (!lvalue_type->compare(expr_type)) {
        logSemanticError(p_assignment.getLocation(),
                         "assigning to '%s' from incompatible type '%s'",
                         lvalue_type->getPTypeCString(),
                         expr_type->getPTypeCString());
        return;
    }
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    p_read.visitChildNodes(*this);

    auto *target_type = p_read.getTarget()->getInferredType();
    if (!target_type) {
        return;
    }

    if (!target_type->isScalar()) {
        logSemanticError(
            p_read.getTarget()->getLocation(),
            "variable reference of read statement must be scalar type");
    }

    auto *entry = symbol_manager.lookup(p_read.getTarget()->getName());
    assert(entry && "Shouldn't read here. This should be catched during the "
                    "visits of child nodes");

    if (entry->getKind() == SymbolEntry::KindEnum::kConstantKind ||
        entry->getKind() == SymbolEntry::KindEnum::kLoopVarKind) {
        logSemanticError(p_read.getTarget()->getLocation(),
                         "variable reference of read statement cannot be a "
                         "constant or loop variable");
    }

    //// Code Generation
    /*
addi sp, sp, -4 
sw t0, 0(sp)     # push the address to the stack
jal ra, readInt  # call function 'readInt'
lw t0, 0(sp)     # pop the address from the stack
addi sp, sp, 4
sw a0, 0(t0)     # save the return value to 'a'
    */
    CG->dumpInstrs("%s\n", "\taddi sp, sp, -4     # load address from child node");
    CG->dumpInstrs("%s\n", "\tsw t0, 0(sp)        # push the address to the stack");
    CG->dumpInstrs("%s\n", "\tjal ra, readInt  # call function 'readInt'");
    CG->dumpInstrs("%s\n", "\tsw t0, 0(sp)        # push the address to the stack");
    CG->dumpInstrs("%s\n", "\taddi sp, sp, 4");
    CG->dumpInstrs("%s\n", "\tsw a0, 0(t0)     # save the return value to chile node")
}

static void checkConditionValidation(const ExpressionNode *condition) {
    auto *condition_type = condition->getInferredType();
    if (!condition_type) {
        return;
    }

    if (!condition_type->isBool()) {
        logSemanticError(condition->getLocation(),
                         "the expression of condition must be boolean type");
    }
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    p_if.visitChildNodes(*this);

    checkConditionValidation(p_if.getCondition());
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    p_while.visitChildNodes(*this);

    checkConditionValidation(p_while.getCondition());
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    context_stack.emplace_back(SemanticContext::kForLoop);
    symbol_manager.pushScope();

    p_for.visitChildNodes(*this);

    auto *initial_value_node = p_for.getLowerBoundNode();
    auto *condition_value_node = p_for.getUpperBoundNode();

    if (initial_value_node->getConstantPtr()->integer() >
        condition_value_node->getConstantPtr()->integer()) {
        logSemanticError(p_for.getLocation(),
                         "the lower bound and upper bound of iteration count "
                         "must be in the incremental order");
    }

    symbol_manager.popScope();
    context_stack.pop_back();
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    p_return.visitChildNodes(*this);

    auto *expected_return_type = return_type_stack.back();
    if (expected_return_type->isVoid()) {
        logSemanticError(p_return.getLocation(),
                         "program/procedure should not return a value");
        return;
    }

    auto *retval = p_return.getRetval();
    auto *real_return_type = retval->getInferredType();
    if (!real_return_type) {
        return;
    }

    if (!expected_return_type->compare(real_return_type)) {
        logSemanticError(retval->getLocation(),
                         "return '%s' from a function with return type '%s'",
                         real_return_type->getPTypeCString(),
                         expected_return_type->getPTypeCString());
        return;
    }
}
