#include "sema/SemanticAnalyzer.hpp"
#include <cstring>
#include <string>
/*
SemanticAnalyzer::SymbolEntry::SymbolEntry(const char *nname, int kind, int level, const char *type, int att)
    : kind(kind), level(level), type(type){
        strcpy(name, nname);
        SetAttribute(att);
}

SemanticAnalyzer::SymbolEntry::SymbolEntry(const char *nname, int kind, int level, const char *type, float att)
    : kind(kind), level(level), type(type){
        strcpy(name, nname);
        SetAttribute(att);
}

SemanticAnalyzer::SymbolEntry::SymbolEntry(const char *nname, int kind, int level, const char *type, bool att)
    : kind(kind), level(level), type(type){
        strcpy(name, nname);
        SetAttribute(att);
}*/

SemanticAnalyzer::SymbolEntry::SymbolEntry(const char *nname, int kind, int level, const char *type, const char *att)
    : kind(kind), level(level), type(type){
        strcpy(name, nname);
        SetAttribute(att);
}

const char *SemanticAnalyzer::SymbolEntry::GetName(){
    return name;
}

const char *SemanticAnalyzer::SymbolEntry::GetKind(){
    switch (kind)
    {
    case SymbolEntryProperty::program :
        return "program";
    case SymbolEntryProperty::function :
        return "function";
    case SymbolEntryProperty::parameter :
        return "parameter";
    case SymbolEntryProperty::variable :
        return "variable";
    case SymbolEntryProperty::loop_var:
        return "loop_var";
    case SymbolEntryProperty::constant :
        return "constant";
    default:
        return "error";
    }
}

const char *SemanticAnalyzer::SymbolEntry::GetLevel(){
    if(level == 0){
        return "0(global)";
    }
    else{
        std::string lv = std::to_string(level) + "(local)";
        return lv.c_str();
    }
}

const char *SemanticAnalyzer::SymbolEntry::GetType(){
    return type;
}

const char *SemanticAnalyzer::SymbolEntry::GetAttribute(){
    /*if( strcmp(type,"integer") == 0) return std::to_string(attribute.intValue).c_str();
    else if( strcmp(type,"real") == 0) return std::to_string(attribute.floatValue).c_str();
    else if( strcmp(type,"boolean") == 0) return attribute.boolValue? "true": "false";
    else return attribute.strValue;*/
    return attribute;
}
/*
void SemanticAnalyzer::SymbolEntry::SetAttribute(int att){
    attribute.intValue = att;
}

void SemanticAnalyzer::SymbolEntry::SetAttribute(float att){
    attribute.floatValue = att;
}

void SemanticAnalyzer::SymbolEntry::SetAttribute(bool att){
    attribute.boolValue = att;
}*/

void SemanticAnalyzer::SymbolEntry::SetAttribute(const char *att){
    attribute = att;   // why we can change const value?
    // if you write attribute.strValue = att , it will cause segment fault when printing.
}

void SemanticAnalyzer::SymbolEntry::SetKind(int kkind){
    kind = kkind;
}

void SemanticAnalyzer::SymbolEntry::SetName(const char *nam){
    strcpy(name, nam);
}

void SemanticAnalyzer::SymbolEntry::IncreaseLevel(){
    level++;
}