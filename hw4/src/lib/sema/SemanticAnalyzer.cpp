#include "sema/SemanticAnalyzer.hpp"
#include "sema/InfoHolder.hpp"
#include "visitor/AstNodeInclude.hpp"
#include <string>
#include <cstring>
#include <iostream>

    /*
     * TODO:
     * 1. Push a new symbol table if this node forms a scope.
     * 2. Insert the symbol into current symbol table if this node is related to
     *    declaration (ProgramNode, VariableNode, FunctionNode).
     * 3. Travere child nodes of this node.
     * 4. Perform semantic analyses of this node. // only redecl pre-order
     * 5. Pop the symbol table pushed at the 1st step.
     */

    // all the list<const chat*> msg, msg.push_back is becuase of [non-unicode character issue] .
    // A cleaner msg construction would be:
    // sprintf(msg, "there is an over array subscript on %s", p_variable_ref.getNameCString());
    // For C String all the way just use strcat. That would be similar to list.

void SemanticAnalyzer::visit(ProgramNode &p_program) {
    symbol_manager.pushScope(new SymbolTable());
    symbol_manager.GetTopTable()->addSymbol(p_program.getNameCString(),
                        SymbolEntryProperty::program,
                        0,
                        "void",
                        "");
    // check whether functions and global variables are redeclatred or not
    
    iNode_manager.root.strValue = strdup("program");
    p_program.visitChildNodes(*this);
    //printf("program node ");
    //iNode_manager.root.PrintChildCount();
    symbol_manager.popScope();

    error_handler.DeumpErrorMessages();
}

void SemanticAnalyzer::visit(DeclNode &p_decl) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);

    p_decl.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("decl node");
    //here->PrintChildCount();

    // index not integer
    //DeclNode::VarNodes vars = p_decl.variables(); // why this cause compiler error?
    bool error = false;
    for (auto &var : p_decl.variables()) {
        int _dimCount = 0;
        const char *ch = var->getTypeCString();
        while(*ch != '\0'){
            if(*ch == '[' || *ch == ']') _dimCount += 1;
            ch += 1;
        }

        long int index;
        std::string str = "";
        for(auto &ch: std::string(var->getTypeCString())){
            if(ch>'9' || ch<'0') ch = ' ';
            str += ch;
        }
        char *pEdn = strdup(str.c_str());
        for(int i=0; i<_dimCount/2; i++){
            index = strtol(pEdn, &pEdn, 10);
            if(index <= 0) {
                error = true;
                break;
            }
        }

        if(!error) continue; // next variable
        std::list<const char *> msg;
        msg.push_back("'");
        msg.push_back(var->getNameCString());
        msg.push_back("' declared as an array with an index that is not greater than 0");
        context_manager.InvalidVariable.insert(var->getNameCString());
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(var->getLocation().line, var->getLocation().col),
            msg,
            CB->codebase[p_decl.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        context_manager.InvalidVariable.insert( std::string(var->getNameCString()) );
    }
    
    
}

void SemanticAnalyzer::visit(VariableNode &p_variable) {
    iNode *here = new iNode();
    here->SetType(p_variable.getTypeCString());
    here->location = std::pair<long, long>(p_variable.getLocation().line, p_variable.getLocation().col);
    iNode_manager.initializeNode(here);

    //redeclration error
    auto entry_inner = symbol_manager.GetTopTable()->lookup(p_variable.getNameCString());
    //auto potentialLoopVar = context_manager.LoopVar.find(p_variable.getNameCString());
    auto potentialLoopVar = context_manager.LoopVar.lookup(p_variable.getNameCString());
    if( entry_inner != NULL /*&& strcmp(entry_inner->GetKind(), "parameter") != 0*/){ 
        std::list<const char *> msg;
        msg.push_back("symbol ");
        msg.push_back(p_variable.getNameCString());
        msg.push_back(" is redeclared");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_variable.getLocation().line, p_variable.getLocation().col),
            msg,
            CB->codebase[p_variable.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        iNode_manager.p_Node = here->parent;
        return;
    }

    if(potentialLoopVar != NULL){ // found a loop var outside
        std::list<const char *> msg;
        msg.push_back("symbol ");
        msg.push_back(p_variable.getNameCString());
        msg.push_back(" is redeclared");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_variable.getLocation().line, p_variable.getLocation().col),
            msg,
            CB->codebase[p_variable.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        iNode_manager.p_Node = here->parent;
        return;
    }

    if(symbol_manager.InForDecl == true){
        context_manager.LoopVar.addSymbol(p_variable.getNameCString(),
            SymbolEntryProperty::loop_var,
            symbol_manager.GetLevel()+1,
            p_variable.getTypeCString(),
            ""
        );
    }
    else {
        auto it = symbol_manager.GetTopTable()->addSymbol(p_variable.getNameCString(),
                        symbol_manager.InParameterDecl? SymbolEntryProperty::parameter : SymbolEntryProperty::variable,
                        symbol_manager.GetLevel() + context_manager.LoopVar.GetDumpListSize(),
                        p_variable.getTypeCString(),
                        "");
        context_manager.lastEntry = it->second;
    }

    p_variable.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("variable node ");
    //here->PrintChildCount();
    context_manager.lastEntry = NULL;
}

void SemanticAnalyzer::visit(ConstantValueNode &p_constant_value) {
    // add the constant value to the variable entry and change kind to constant
    if(context_manager.lastEntry != NULL) {
        context_manager.lastEntry->SetAttribute(p_constant_value.getConstantValueCString());
        context_manager.lastEntry->SetKind(SymbolEntryProperty::constant);
        //context_manager.lastEntry->IncreaseLevel();
        context_manager.lastEntry = NULL;
    }
    /*p_constant_value.visitChildNodes(*this); <-why this got no compile error?*/
    iNode *here = new iNode();
    here->location = std::pair<long, long>(p_constant_value.getLocation().line, p_constant_value.getLocation().col);
    iNode_manager.initializeNode(here);
    iNode_manager.p_Node = here->parent;
    here->SetType(p_constant_value.getTypePtr()->getPTypeCString()); // TODO: temp
    here->strValue = strdup(p_constant_value.getConstantValueCString());
    //printf("constant node ");
    //here->PrintChildCount();
}

void SemanticAnalyzer::visit(FunctionNode &p_function) {
    //redeclration error
    auto entry_inner = symbol_manager.GetTopTable()->lookup(p_function.getNameCString());
    auto entry_outer = symbol_manager.lookup_outer(p_function.getNameCString());
    if( entry_inner != NULL && strcmp(entry_inner->GetKind(), "parameter") != 0){ 
        std::list<const char *> msg;
        msg.push_back("symbol ");
        msg.push_back(p_function.getNameCString());
        msg.push_back(" is redeclared");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_function.getLocation().line, p_function.getLocation().col),
            msg,
            CB->codebase[p_function.getLocation().line-1]
            )
        );
        symbol_manager.pushScope(new SymbolTable());
        symbol_manager.popScope();
        return;
    }

    if( entry_outer != NULL && strcmp(entry_outer->GetKind(), "loop_var") == 0){
        std::list<const char *> msg;
        msg.push_back("symbol ");
        msg.push_back(p_function.getNameCString());
        msg.push_back(" is redeclared");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_function.getLocation().line, p_function.getLocation().col),
            msg,
            CB->codebase[p_function.getLocation().line-1]
            )
        );
        symbol_manager.pushScope(new SymbolTable());
        symbol_manager.popScope();
        return;
    }


        /* [non-unicode character issue] */
    int start = (int)std::string(p_function.getPrototypeCString()).find("("); // temp solution for non-unicode issue
    symbol_manager.GetTopTable()->addSymbol(p_function.getNameCString(),
                        SymbolEntryProperty::function,
                        symbol_manager.GetLevel(),
                        p_function.getTypeCString(),
                        &(p_function.getPrototypeCString()[start+1]));
    symbol_manager.pushScope(new SymbolTable());
    symbol_manager.LockStack();
    symbol_manager.InParameterDecl = true;

    iNode *here = new iNode();
    here->SetType(p_function.getTypeCString());
    here->strValue = strdup(p_function.getTypeCString());
    iNode_manager.initializeNode(here);
    p_function.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("function node ");
    //here->PrintChildCount();

    //symbol_manager.UnlockStack();
    //symbol_manager.popScope();
}

void SemanticAnalyzer::visit(CompoundStatementNode &p_compound_statement) {
    if(symbol_manager.InForDecl == true){
        symbol_manager.InForDecl = false;
        //symbol_manager.popScope();
    }
    symbol_manager.InParameterDecl = false;
    symbol_manager.pushScope(new SymbolTable());
    symbol_manager.UnlockStack();

    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_compound_statement.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("compound statement node");
    //here->PrintChildCount();
    
    symbol_manager.popScope();
}

void SemanticAnalyzer::visit(PrintNode &p_print) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_print.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("print node ");
    //here->PrintChildCount();

    // error in expression node // just return. no need to pring error
    if(here->ChildrenHasError == true) return;
    
    // expression not scalar type 
    if(here->children.front()->Type == 5) {
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_print.getLocation().line, here->children[0]->location.second),
            "expression of print statement must be scalar type",
            CB->codebase[p_print.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }
}

void SemanticAnalyzer::visit(BinaryOperatorNode &p_bin_op) {
    // please do type coercion
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    here->location = std::pair<long, long>(p_bin_op.getLocation().line, p_bin_op.getLocation().col);
    p_bin_op.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("binary node ");
    //here->PrintChildCount();

    std::list<const char *> msg;
            msg.push_back("invalid operands to binary operator '");
            msg.push_back(p_bin_op.getOpCString());
            msg.push_back("' ('");
            if(here->children[0]->RichType != "Default") msg.push_back(here->children[0]->RichType.c_str());
            else msg.push_back(here->children[0]->GetTypeCString());
            msg.push_back("' and '");
            if(here->children[1]->RichType != "Default") msg.push_back(here->children[1]->RichType.c_str());
            else msg.push_back(here->children[1]->GetTypeCString());
            msg.push_back("')");

    /* [ + - * / ] */
    if(strstr("+-*/", p_bin_op.getOpCString()) != NULL){
            // error in oprands 
            if(here->ChildrenHasError == true) return;

            // [string concatinate] oprand not both string
            if(strcmp("+", p_bin_op.getOpCString()) == 0){
                // condition: not (*both string* or *both not string*), like XOR
                if((here->children[0]->Type == TypeEnum::string) != (here->children[1]->Type == TypeEnum::string)) {
                    error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                        std::pair<long, long>(p_bin_op.getLocation().line, p_bin_op.getLocation().col),
                        msg,
                        CB->codebase[p_bin_op.getLocation().line-1]
                        )
                    );
                    iNode_manager.PropagateError(here);
                    return;
                }
            }
            if(here->children[0]->Type == TypeEnum::string && here->children[1]->Type == TypeEnum::string){
                if(strstr("-*/", p_bin_op.getOpCString()) != NULL){
                    error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                        std::pair<long, long>(p_bin_op.getLocation().line, p_bin_op.getLocation().col),
                        msg,
                        CB->codebase[p_bin_op.getLocation().line-1]
                        )
                    );
                    iNode_manager.PropagateError(here);
                    return;
                }
                here->Type = TypeEnum::string;
                return;
            }
            // oprand not integer or real
            for(auto &child: here->children){
                if(child->Type != TypeEnum::integer && child->Type != TypeEnum::real) {
                    error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                        std::pair<long, long>(p_bin_op.getLocation().line, p_bin_op.getLocation().col),
                        msg,
                        CB->codebase[p_bin_op.getLocation().line-1]
                        )
                    );
                    iNode_manager.PropagateError(here);
                    return;
                }
            }
            here->Type = (here->children[0]->Type + here->children[1]->Type)/2;
    }
    //The operation produces an integer or a real value.
    

    

    /* [ mod ] */
    else if(strcmp("mod", p_bin_op.getOpCString()) == 0){
        //for(auto i: {1}){
            // error in oprands 
            if(here->ChildrenHasError == true) return;

            // oprand not integer
            for(auto &child: here->children){
                if(child->Type != TypeEnum::integer) {
                    error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                        std::pair<long, long>(p_bin_op.getLocation().line, p_bin_op.getLocation().col),
                        msg,
                        CB->codebase[p_bin_op.getLocation().line-1]
                        )
                    );
                    iNode_manager.PropagateError(here);
                    return;
                }
            }
            here->Type = TypeEnum::integer;
    }
    //The operation produces an integer value.

    /* [ and or not ] */
    else if(strstr("andornot", p_bin_op.getOpCString()) != NULL){
        //for(auto i: {1}){
            // error in oprands 
            if(here->ChildrenHasError == true) return;

            // oprand not boolean
            for(auto &child: here->children){
                if(child->Type != TypeEnum::boolean) {
                    error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                        std::pair<long, long>(p_bin_op.getLocation().line, p_bin_op.getLocation().col),
                        msg,
                        CB->codebase[p_bin_op.getLocation().line-1]
                        )
                    );
                    iNode_manager.PropagateError(here);
                    return;
                }
            }
            here->Type = TypeEnum::boolean;
    }
    //The operation produces a boolean value.

    /* [ relational ] */
    else if(strstr("<,<=,=,>=,>,<>", p_bin_op.getOpCString()) != NULL){
        //for(auto i: {1}){
            // error in oprands 
            if(here->ChildrenHasError == true) return;

            // oprand not integer or real // TODO: need type coercion
            for(auto &child: here->children){
                if(child->Type != TypeEnum::integer && child->Type != TypeEnum::real) {
                    error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                        std::pair<long, long>(p_bin_op.getLocation().line, p_bin_op.getLocation().col),
                        msg,
                        CB->codebase[p_bin_op.getLocation().line-1]
                        )
                    );
                    iNode_manager.PropagateError(here);
                    return;
                }
            }
            here->Type = TypeEnum::boolean;
    }
    //The operation produces a boolean value.
}

void SemanticAnalyzer::visit(UnaryOperatorNode &p_un_op) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_un_op.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("unary opration node ");
    //here->PrintChildCount();

    std::list<const char *> msg;
    msg.push_back("invalid operand to unary operator '");
    msg.push_back(p_un_op.getOpCString());
    msg.push_back("' (\'");
    if(here->children[0]->RichType != "Default") msg.push_back(here->children[0]->RichType.c_str());
    else msg.push_back(here->children[0]->GetTypeCString());
    msg.push_back("\')");

    // error in oprands // just return. no need to pring error
    if(here->ChildrenHasError == true) return;

    // oprand not integer or real
    if(strcmp("-", p_un_op.getOpCString()) == 0){
        if(here->children[0]->Type != TypeEnum::integer && here->children[0]->Type != TypeEnum::real) {
                
            error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                        std::pair<long, long>(p_un_op.getLocation().line, p_un_op.getLocation().col),
                        msg,
                        CB->codebase[p_un_op.getLocation().line-1]
                        )
                    );
            iNode_manager.PropagateError(here);
            return;
        }
        //The operation produces an integer or a real value.
    }

    else if(strcmp("not", p_un_op.getOpCString()) == 0){
        //printf("it's NOTTTTTT!\n");
        if(here->children[0]->Type != TypeEnum::boolean) {
            error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                        std::pair<long, long>(p_un_op.getLocation().line, p_un_op.getLocation().col),
                        msg,
                        CB->codebase[p_un_op.getLocation().line-1]
                        )
                    );
            iNode_manager.PropagateError(here);
            return;
        }
    }
    here->Type = here->children[0]->Type;
    here->strValue = strdup(here->children[0]->GetTypeCString());
}

void SemanticAnalyzer::visit(FunctionInvocationNode &p_func_invocation) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_func_invocation.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("function invo node ");
    //here->PrintChildCount();
    auto entry = symbol_manager.lookup(p_func_invocation.getNameCString());

    // identifier not in table
    std::list<const char*> msg;
    msg.push_back("use of undeclared symbol '");
    msg.push_back(p_func_invocation.getNameCString());
    msg.push_back("'");
    if(entry == NULL){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col),
            msg,
            CB->codebase[p_func_invocation.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }
    here->SetType(entry->GetType());

    // kind isn't function
    msg.clear();
    msg.push_back("call of non-function symbol '");
    msg.push_back(p_func_invocation.getNameCString());
    msg.push_back("'");
    if(strcmp(symbol_manager.lookup(p_func_invocation.getNameCString())->GetKind(), "function") != 0){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col),
            msg,
            CB->codebase[p_func_invocation.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // amount of argument(filled in ones) and parameter(declarations) are not matched
    int paraCount = 0;
    const char *ch = entry->GetAttribute();
    while(*ch != '\0'){
        if(*ch == ',') paraCount += 1;
        ch += 1;
    }
    if((int)strlen(entry->GetAttribute()) == 0) paraCount = -1;
    msg.clear();
    msg.push_back("too few/much arguments provided for function '");
    msg.push_back(p_func_invocation.getNameCString());
    msg.push_back("'");
    if((int)here->children.size() != paraCount+1){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_func_invocation.getLocation().line, p_func_invocation.getLocation().col),
            msg,
            CB->codebase[p_func_invocation.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // expression errors in arguments // just return. no need to pring error
    if(here->ChildrenHasError) return;

    // arguments don't have same type corresponding to parameters, even with type coercio
    
    std::vector<std::string> info;
    if(type_co.FunctionFillInCoersion(entry->GetAttribute(), here->children, info)){
        msg.clear();
        msg.push_back("incompatible type passing '");
        msg.push_back(strdup(info[0].c_str()));
        msg.push_back("' to parameter of type '");
        msg.push_back(strdup(info[1].c_str()));
        msg.push_back("'");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_func_invocation.getLocation().line, atol(strdup(info[2].c_str())) ),
            msg,
            CB->codebase[p_func_invocation.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }
}

void SemanticAnalyzer::visit(VariableReferenceNode &p_variable_ref) {
    iNode *here = new iNode();
    here->location = std::pair<long, long>(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col);
    here->strValue = strcat(strdup(p_variable_ref.getNameCString()), "-name");
    iNode_manager.initializeNode(here);
    p_variable_ref.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("var ref node ");
    //here->PrintChildCount();

    // can not find symbol in the table
    const char *kind;
    auto entry = symbol_manager.lookup(p_variable_ref.getNameCString());
    auto loopVar = context_manager.LoopVar.lookup(p_variable_ref.getNameCString());
    if(entry == NULL && loopVar == NULL) {
        std::list<const char *> msg;
        msg.push_back("use of undeclared symbol '");
        msg.push_back(p_variable_ref.getNameCString());
        msg.push_back("'");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col),
            msg,
            CB->codebase[p_variable_ref.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }
    if(entry == NULL) entry = loopVar;
    here->SetType(entry->GetType());
    //here->RichType = std::string(entry->GetType());
    if(strcmp(entry->GetKind(), "constant") == 0) here->intValue = 9487;

    // wrong kind
    kind = entry->GetKind();
    if(strcmp(kind, "program") == 0 || strcmp(kind, "function") == 0) {
        std::list<const char *> msg;
        msg.push_back("use of non-variable symbol '");
        msg.push_back(p_variable_ref.getNameCString());
        msg.push_back("'");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col),
            msg,
            CB->codebase[p_variable_ref.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // error in declration. // just return. no need to print error
    if(context_manager.InvalidVariable.find(std::string(p_variable_ref.getNameCString()) ) != context_manager.InvalidVariable.end()){
        iNode_manager.PropagateError(here);
        return;
    }

    // index not integer
    int count = 0;
    for(auto &child: (here->children)){
        if(child->Type != TypeEnum::integer){
            error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                std::pair<long, long>(p_variable_ref.getLocation().line, here->children[count]->location.second),
                "index of array reference must be an integer",
                CB->codebase[p_variable_ref.getLocation().line-1]
                )
            );
            iNode_manager.PropagateError(here);
            return;
        }
        count++;
    }

    // error occored in index(expression)
    if(here->ChildrenHasError == true) return;

    // dimension exceeded
    float _dimCount = 0;
    const char *ch = entry->GetType();
    while(*ch != '\0'){
        if(*ch == '[' || *ch == ']') _dimCount += 1;
        ch += 1;
    }
        /* [non-unicode character issue] - solved */ 
    if(_dimCount/2 == (float)here->children.size() && here->Type == TypeEnum::array) {
        if(strstr(entry->GetType(),"boolean") != nullptr) here->Type = TypeEnum::boolean;
        else if(strstr(entry->GetType(),"integer") != nullptr) here->Type = TypeEnum::integer;
        else if(strstr(entry->GetType(),"real") != nullptr) here->Type = TypeEnum::real;
        else if(strstr(entry->GetType(),"string") != nullptr) here->Type = TypeEnum::string;
    }
    else if(_dimCount/2 > (int)here->children.size()) {
        here->RichType = context_manager.TrimDimension(entry->GetType(), (int)here->children.size(), false);
        here->Type = TypeEnum::array;
    }
    else if(_dimCount/2 < (float)here->children.size()){
        std::list<const char *> msg;
        msg.push_back("there is an over array subscript on '");
        msg.push_back(p_variable_ref.getNameCString());
        msg.push_back("'");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                std::pair<long, long>(p_variable_ref.getLocation().line, p_variable_ref.getLocation().col),
                msg,
                CB->codebase[p_variable_ref.getLocation().line-1]
                )
            );
            iNode_manager.PropagateError(here);
            return;
    }
}

void SemanticAnalyzer::visit(AssignmentNode &p_assignment) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_assignment.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("assignment node ");
    //here->PrintChildCount();

    std::string name = std::string(here->children[0]->strValue);
    std::size_t left = name.find("-");
    name.erase(name.begin()+left, name.end());

    // error in lvalue // just return. no need to proint error
    if(here->children[0]->HasError) return;

    // lvalue is an array
    if(here->children[0]->Type == TypeEnum::array) {
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                std::pair<long, long>(p_assignment.getLocation().line, here->children[0]->location.second),
                "array assignment is not allowed",
                CB->codebase[p_assignment.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // lvalue is constant
    if( here->children[0]->intValue == 9487) {
        std::list<const char *> msg;
        msg.push_back("cannot assign to variable '");
        msg.push_back(strdup(name.c_str()));
        msg.push_back("' which is a constant");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                std::pair<long, long>(p_assignment.getLocation().line, here->children[0]->location.second),
                msg,
                CB->codebase[p_assignment.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // lvalue is a loop_var in use
    if(context_manager.LoopVar.lookup(name.c_str()) != NULL && strcmp(here->parent->strValue, "FOR_NODE") != 0){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                std::pair<long, long>(p_assignment.getLocation().line, here->children[0]->location.second),
                "the value of loop variable cannot be modified inside the loop body",
                CB->codebase[p_assignment.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // error in rvalue
    if(here->children[1]->HasError) return;

    // rvalue is an array
    if(here->children[1]->Type == TypeEnum::array) {
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                std::pair<long, long>(p_assignment.getLocation().line, here->children[1]->location.second),
                "array assignment is not allowed",
                CB->codebase[p_assignment.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // lvalue doesn't have the same type with rvalue, even with type coercion
    if(here->children[0]->Type != here->children[1]->Type){
        if(!(here->children[0]->Type == TypeEnum::real && here->children[1]->Type == TypeEnum::integer)){
            std::list<const char*> msg; //assigning to '{type_of_variable_reference}' from incompatible type '{type_of_expression}'
            msg.push_back("assigning to '");
            msg.push_back(here->children[0]->RichType == "Default"? here->children[0]->GetTypeCString(): here->children[0]->RichType.c_str());
            msg.push_back("' from incompatible type '");
            msg.push_back(here->children[1]->RichType == "Default"? here->children[1]->GetTypeCString(): here->children[1]->RichType.c_str());
            msg.push_back("'");
            error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                std::pair<long, long>(p_assignment.getLocation().line, p_assignment.getLocation().col),
                msg,
                CB->codebase[p_assignment.getLocation().line-1]
                )
            );
            iNode_manager.PropagateError(here);
            return;
        }
    }
}

void SemanticAnalyzer::visit(ReadNode &p_read) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_read.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("read node ");
    //here->PrintChildCount();

    // no errror in var ref
    if(here->ChildrenHasError) return;

    // var ref not scalar type
    if(here->children[0]->Type == 5){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_read.getLocation().line, here->children[0]->location.second),
            "variable reference of read statement must be scalar type",
            CB->codebase[p_read.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // var ref is const or loop_var
    if(strstr(here->children[0]->strValue, "-") != NULL) {
        std::string name = std::string(here->children[0]->strValue);
        std::size_t left = name.find("-");
        name.erase(name.begin()+left, name.end());
        if(context_manager.LoopVar.lookup(name.c_str()) != NULL || strcmp(symbol_manager.lookup(name.c_str())->GetAttribute(), "") != 0){
            error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
                std::pair<long, long>(p_read.getLocation().line, here->children[0]->location.second),
                "variable reference of read statement cannot be a constant or loop variable",
                CB->codebase[p_read.getLocation().line-1]
                )
            );
            iNode_manager.PropagateError(here);
            return;
        }
    }
    else {
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_read.getLocation().line, here->children[0]->location.second),
            "variable reference of read statement cannot be a constant or loop variable",
            CB->codebase[p_read.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }   
    
}

void SemanticAnalyzer::visit(IfNode &p_if) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_if.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("if node");
    //here->PrintChildCount();

    // error in condition(expression)
    if(here->children[0]->HasError) return;

    // type of condition(expression) isn't bool
    if(here->children[0]->Type != TypeEnum::boolean){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_if.getLocation().line, here->children[0]->location.second),
            "the expression of condition must be boolean type",
            CB->codebase[p_if.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
    }
}

void SemanticAnalyzer::visit(WhileNode &p_while) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_while.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("while node ");
    //here->PrintChildCount();

    // error in condition(expression)
    if(here->children[0]->HasError) return;

    // condition(expression) not bool
    if(here->children[0]->Type != TypeEnum::boolean){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_while.getLocation().line, here->children[0]->location.second),
            "the expression of condition must be boolean type",
            CB->codebase[p_while.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
    }
}

void SemanticAnalyzer::visit(ForNode &p_for) {
    symbol_manager.InForDecl = true;

    iNode *here = new iNode();
    here->strValue = strdup("FOR_NODE");
    iNode_manager.initializeNode(here);
    p_for.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("for node ");
    //here->PrintChildCount();

    // loop var not incremented
    if(atoi(here->children[1]->children[1]->strValue) >= atoi(here->children[2]->strValue)){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_for.getLocation().line, p_for.getLocation().col),
            "the lower bound and upper bound of iteration count must be in the incremental order",
            CB->codebase[p_for.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
    }
    
    //symbol_manager.pushScope(&(context_manager.LoopVar));
    //symbol_manager.popScopeLastSymbol();
    SymbolTable *table = new SymbolTable();
    if(!here->children[0]->children[0]->HasError){
        table->addSymbol(*(--context_manager.LoopVar.GetDumpListEnd()), 
            SymbolEntryProperty::loop_var,
            symbol_manager.GetLevel()+1,
            "integer",
            ""
        );
    }
    symbol_manager.pushScope(table);
    symbol_manager.popScope();
    context_manager.LoopVar.EraseSymbol(*(--context_manager.LoopVar.GetDumpListEnd()));
    //context_manager.LoopVarList.pop_back();
    // How about loop var not integer?
    //symbol_manager.popScope();
}

void SemanticAnalyzer::visit(ReturnNode &p_return) {
    iNode *here = new iNode();
    iNode_manager.initializeNode(here);
    p_return.visitChildNodes(*this);
    iNode_manager.p_Node = here->parent;
    //printf("return node ");
    //here->PrintChildCount();

    // this node appear in program or void function
    if( strcmp(here->parent->strValue, "void") == 0){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_return.getLocation().line, p_return.getLocation().col),
            "program/procedure should not return a value",
            CB->codebase[p_return.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }
    if( strcmp(here->parent->strValue, "default") == 0 && strcmp(here->parent->parent->strValue, "program") == 0){
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_return.getLocation().line, p_return.getLocation().col),
            "program/procedure should not return a value",
            CB->codebase[p_return.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }

    // error in return value(expression)
    if(here->ChildrenHasError) return;

    // return value(expression) doesn't have the same type with function return type, even with type coercion. 
    //strcmp(here->children[0]->GetTypeCString(), here->parent->parent->strValue) !=0
    if(here->children[0]->Type != here->parent->parent->Type && 
    !(here->children[0]->Type == TypeEnum::integer && here->parent->parent->Type == TypeEnum::real)){
        std::list<const char *> msg;
        msg.push_back("return \'");
        msg.push_back(here->children[0]->RichType == "Default"? here->children[0]->GetTypeCString() : here->children[0]->RichType.c_str());
        msg.push_back("' from a function with return type '");
        msg.push_back(strcmp(here->parent->parent->strValue, "default") == 0 ? "void":here->parent->parent->strValue);
        msg.push_back("\'");
        error_handler.ErrorMessageQueue.push_back(new ErrorMessage(
            std::pair<long, long>(p_return.getLocation().line, here->children[0]->location.second),
            msg,
            CB->codebase[p_return.getLocation().line-1]
            )
        );
        iNode_manager.PropagateError(here);
        return;
    }
}
