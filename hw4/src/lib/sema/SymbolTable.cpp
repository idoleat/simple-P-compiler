#include "sema/SemanticAnalyzer.hpp"
/*
std::unordered_map<std::string, SemanticAnalyzer::SymbolEntry *>::iterator SemanticAnalyzer::SymbolTable::addSymbol(const char *name, int kind, uint32_t level, const char *type, int attribute){
    dumpList.push_back(name);
    return insert(name, new SymbolEntry(name, kind, level, type, attribute));
}

std::unordered_map<std::string, SemanticAnalyzer::SymbolEntry *>::iterator SemanticAnalyzer::SymbolTable::addSymbol(const char *name, int kind, uint32_t level, const char *type, float attribute){
    dumpList.push_back(name);
    return insert(name, new SymbolEntry(name, kind, level, type, attribute));
}

std::unordered_map<std::string, SemanticAnalyzer::SymbolEntry *>::iterator SemanticAnalyzer::SymbolTable::addSymbol(const char *name, int kind, uint32_t level, const char *type, bool attribute){
    dumpList.push_back(name);
    return insert(name, new SymbolEntry(name, kind, level, type, attribute));
}*/

std::unordered_map<std::string, SemanticAnalyzer::SymbolEntry *>::iterator SemanticAnalyzer::SymbolTable::addSymbol(const char *name, int kind, uint32_t level, const char *type, const char *attribute){
    dumpList.push_back(name);
    return insert(name, new SymbolEntry(name, kind, level, type, attribute));
}

std::unordered_map<std::string, SemanticAnalyzer::SymbolEntry *>::iterator SemanticAnalyzer::SymbolTable::insert(const char *name, SymbolEntry *symbol){
    return entries.insert(std::pair<std::string, SymbolEntry *>(std::string(name), symbol)).first;
}

void  SemanticAnalyzer::SymbolTable::SetLevel(uint32_t lv){
    level = lv;
}

uint32_t SemanticAnalyzer::SymbolTable::GetLevel(){
    return level;
}

std::list<const char *>::iterator SemanticAnalyzer::SymbolTable::GetDumpListBegin(){
    return dumpList.begin();
}

std::list<const char *>::iterator SemanticAnalyzer::SymbolTable::GetDumpListEnd(){
    return dumpList.end();
}

int SemanticAnalyzer::SymbolTable::GetDumpListSize(){
    return (int)dumpList.size();
}

SemanticAnalyzer::SymbolEntry *SemanticAnalyzer::SymbolTable::lookup(const char *name){
    std::unordered_map<std::string, SymbolEntry *>::iterator got =  entries.find(std::string(name));
    if(got != entries.end()) return (got->second);
    else return NULL;
}

bool SemanticAnalyzer::SymbolTable::EraseSymbol(const char *name){
    int result = entries.erase(std::string(name));
    dumpList.remove(std::string(name).c_str());
    if(result > 0) return true;
    else return false;
}