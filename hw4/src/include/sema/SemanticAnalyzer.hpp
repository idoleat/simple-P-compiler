#ifndef __SEMA_SEMANTIC_ANALYZER_H
#define __SEMA_SEMANTIC_ANALYZER_H

#include "visitor/AstNodeVisitor.hpp"
#include "InfoHolder.hpp"
#include "CodeBase.hpp"
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <string>

class SemanticAnalyzer : public AstNodeVisitor {
  public:
    SemanticAnalyzer() = default;
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

    struct SymbolEntryProperty
    {
      enum kind {program, function, parameter, variable, loop_var, constant};
      /*union Attribute{
          int intValue;
          float floatValue;
          bool boolValue;
          const char *strValue;
      };*/
    };

    class SymbolEntry {
      public:/*
        SymbolEntry(const char *name, int kind, int level, const char *type, int attribute);
        SymbolEntry(const char *name, int kind, int level, const char *type, float attribute);
        SymbolEntry(const char *name, int kind, int level, const char *type, bool attribute);*/
        SymbolEntry(const char *name, int kind, int level, const char *type, const char *attribute);
        const char *GetName();
        const char *GetKind();
        const char *GetLevel();
        const char *GetType();
        const char *GetAttribute();/*
        void SetAttribute(int att);
        void SetAttribute(float att);
        void SetAttribute(bool att);*/
        void SetAttribute(const char *att);
        void SetKind(int kkind);
        void SetName(const char *nam);
        void IncreaseLevel();

      private:
        char name[32];
        int kind;
        int level;
        const char *type;
        const char *attribute;
    };

    class SymbolTable {
      public:
        SymbolTable() = default;
        //TODO:
        //SymbolTable destructor needed.
/*
        std::unordered_map<std::string, SymbolEntry *>::iterator  addSymbol(const char *name, int kind, uint32_t level, const char *type, int attribute); // kind should be enum
        std::unordered_map<std::string, SymbolEntry *>::iterator addSymbol(const char *name, int kind, uint32_t level, const char *type, float attribute);
        std::unordered_map<std::string, SymbolEntry *>::iterator addSymbol(const char *name, int kind, uint32_t level, const char *type, bool attribute);*/
        std::unordered_map<std::string, SymbolEntry *>::iterator addSymbol(const char *name, int kind, uint32_t level, const char *type, const char *attribute);
        void SetLevel(uint32_t level);
        uint32_t GetLevel();
        std::list<const char *>::iterator GetDumpListBegin();
        std::list<const char *>::iterator GetDumpListEnd();
        int GetDumpListSize();
        SymbolEntry *lookup(const char *name);
        bool EraseSymbol(const char *name);
        
      private:
        std::unordered_map<std::string, SymbolEntry *>::iterator insert(const char *name, SymbolEntry *symbol);
        std::unordered_map<std::string, SymbolEntry *> entries;
        std::list<const char *> dumpList;
        uint32_t level;
    };

    class SymbolManager {
      public:
        void pushScope(SymbolTable *new_scope);
        void popScope();
        void popScopeLastSymbol();
        void LockStack();
        void UnlockStack();
        bool IsStackLocked();
        SymbolTable *GetTopTable();
        uint32_t GetLevel();
        bool InParameterDecl = false; // Fast and Nasty
        bool InForDecl = false; 
        bool ToDump;
        SymbolEntry *lookup(const char *name);
        SymbolEntry *lookup_outer(const char *name);

      private:
        std::list<SymbolTable *> tables;
        void dumpDemarcation(const char chr);
        void dumpSymbol(SymbolEntry *symbol);
        uint32_t topTablelevel = 0;
        bool isStackLocked = false;
    }symbol_manager; // should use Singleton here

    class ContextManager {
      public:
        SymbolTable LoopVar;
        //std::unordered_map<const char*, SymbolEntry *> LoopVar;  <-stupid
        std::list<const char*> LoopVarList;
        std::unordered_set<std::string> InvalidVariable;
        SymbolEntry *lastEntry = NULL;
        const char *name = NULL;

        std::string TrimDimension(const char* typeString, int amount, bool TrimWhiteSpace);
    }context_manager; // should use Singleton here

    struct ErrorMessage {
        ErrorMessage(std::pair<long, long> loc, const char *msg, const char *src);
        ErrorMessage(std::pair<long, long> loc, std::list<const char *> msg, const char *src);
        std::pair<long, long> location;
        std::list<const char *> Message;
        const char *SourceCode;
    };

    class ErrorHandler {
      public:
        std::list<ErrorMessage *> ErrorMessageQueue;
        void DeumpErrorMessages();
        void PrintError(ErrorMessage *error);
        void PrintError(const char *ch);
    }error_handler; // should use Singleton here

    class Typee {
      public:
        bool FunctionFillInCoersion(const char *FillIn, std::vector<iNode *> parameters, std::vector<std::string> &info);
    }type_co;

    CodeBase *CB;

  private:
    iNodeManager iNode_manager;
};

#endif
