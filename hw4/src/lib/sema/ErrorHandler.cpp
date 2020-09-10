#include "sema/SemanticAnalyzer.hpp"

SemanticAnalyzer::ErrorMessage::ErrorMessage(std::pair<long, long> loc, const char *msg, const char *src)
    : location(loc), SourceCode(src) { Message.push_front(msg); }

SemanticAnalyzer::ErrorMessage::ErrorMessage(std::pair<long, long> loc, std::list<const char *> msg, const char *src)
    : location(loc), Message(msg), SourceCode(src) {}

void SemanticAnalyzer::ErrorHandler::PrintError(SemanticAnalyzer::ErrorMessage *err){ 
    printf("<Error> Found in line %d, column %d: ", err->location.first, err->location.second);
    for(auto &msg: err->Message){
        printf("%s", msg);
    }
    // TODO: use iNode to hold indentation info
    printf("\n    %s\n", err->SourceCode);
    for(int i=0; i<err->location.second-1; i++){
        printf(" ");
    }
    printf("    ^\n");
}

void SemanticAnalyzer::ErrorHandler::PrintError(const char *ch){
    printf("<Error> %s", ch);
}

void SemanticAnalyzer::ErrorHandler::DeumpErrorMessages(){
    if(ErrorMessageQueue.size() == 0){
        printf("\n");
        printf("|---------------------------------------------------|\n");
        printf("|  There is no syntactic error and semantic error!  |\n");
        printf("|---------------------------------------------------|\n");
    }
    else{
        for(auto &msg: ErrorMessageQueue){
            PrintError(msg);
        }
    }
}