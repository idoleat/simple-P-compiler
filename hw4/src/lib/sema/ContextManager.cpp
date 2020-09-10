#include "sema/SemanticAnalyzer.hpp"

std::string SemanticAnalyzer::ContextManager::TrimDimension(const char* typeString, int amount, bool TrimWhiteSpace){
    std::string type = std::string(typeString);
    std::size_t left, right;
    while(amount != 0){
        left = type.find_first_of("[");
        right = type.find_first_of("]");
        type.erase(type.begin() + left, type.begin() + right+1);
        amount -= 1;
    }
    if(TrimWhiteSpace){
        while(type.find(" ") != std::string::npos){
            type.erase(type.find(" "));
        }
    }

    return type;
}
