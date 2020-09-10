#include "sema/SemanticAnalyzer.hpp"
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cstring>

void SemanticAnalyzer::SymbolManager::pushScope(SemanticAnalyzer::SymbolTable *new_scope){
    if(isStackLocked) return;
    tables.push_front(new_scope);
    topTablelevel++;
}

void SemanticAnalyzer::SymbolManager::popScope(){
    if(isStackLocked) return;
    SymbolTable *topTable = tables.front();

    dumpDemarcation('=');
    printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type", "Attribute");
    dumpDemarcation('-');

    std::list<const char *>::iterator it = topTable->GetDumpListBegin();
    while(it != topTable->GetDumpListEnd()){
        dumpSymbol( topTable->lookup(*it) );
        it++;
    }

    dumpDemarcation('-');
    tables.pop_front();
    topTablelevel--;
}

void SemanticAnalyzer::SymbolManager::popScopeLastSymbol(){
  if(isStackLocked) return;
    SymbolTable *topTable = tables.front();

    dumpDemarcation('=');
    if(ToDump) {
      printf("%-33s%-11s%-11s%-17s%-11s\n", "Name", "Kind", "Level", "Type", "Attribute");
    }
    dumpDemarcation('-');

    std::list<const char *>::iterator it = topTable->GetDumpListEnd();
    --it;
    dumpSymbol( topTable->lookup(*it) );

    dumpDemarcation('-');
    tables.pop_front();
    topTablelevel--;

}

SemanticAnalyzer::SymbolTable *SemanticAnalyzer::SymbolManager::GetTopTable(){
  return tables.front();
}

uint32_t SemanticAnalyzer::SymbolManager::GetLevel(){
  return tables.size()-1;
}

void SemanticAnalyzer::SymbolManager::LockStack(){
  isStackLocked = true;
}

void SemanticAnalyzer::SymbolManager::UnlockStack(){
  isStackLocked = false;
}

bool SemanticAnalyzer::SymbolManager::IsStackLocked(){
  return isStackLocked;
}

void SemanticAnalyzer::SymbolManager::dumpDemarcation(const char chr) {
  if(!ToDump) return;
  for (size_t i = 0; i < 110; ++i) {
    printf("%c", chr);
  }
  puts("");
}

void SemanticAnalyzer::SymbolManager::dumpSymbol(SemanticAnalyzer::SymbolEntry *symbol) {
  if(!ToDump) return;
  printf("%-33s", symbol->GetName());
  printf("%-11s", symbol->GetKind());
  printf("%-11s", symbol->GetLevel());
  printf("%-17s", symbol->GetType()); 
  if(symbol->GetAttribute() != NULL)  printf("%-11s", symbol->GetAttribute());
  //else printf("%-11s", /*symbol->GetAttribute()*/"456");
  puts("");
}

SemanticAnalyzer::SymbolEntry *SemanticAnalyzer::SymbolManager::lookup(const char *name) {
  SymbolEntry *entry = NULL;
  std::list<SymbolTable *>::iterator it = tables.begin();
  while(it != tables.end()){
    entry = (*it)->lookup(name);
    if(entry != NULL) return entry;
    it++;
  }
  return NULL;
}

SemanticAnalyzer::SymbolEntry *SemanticAnalyzer::SymbolManager::lookup_outer(const char *name) {
  SymbolEntry *entry = NULL;
  std::list<SymbolTable *>::iterator it = tables.begin();
  it++;
  while(it != tables.end()){
    entry = (*it)->lookup(name);
    if(entry != NULL) return entry;
    it++;
  }
  return NULL;
}