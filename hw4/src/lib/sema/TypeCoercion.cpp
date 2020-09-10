#include "sema/SemanticAnalyzer.hpp"
#include <cstring>

// return true if types are not matched
bool SemanticAnalyzer::Typee::FunctionFillInCoersion(const char *prototype, std::vector<iNode *> args, std::vector<std::string> &info) {
    if(args.size() == 0) return false;

    const char s[3] = ",";
    char *token;
    std::vector<std::string> proto;
   
    token = strtok(strdup(prototype), s);
    while( token != NULL ) {
        proto.push_back(std::string(token));
        token = strtok(NULL, s);
    }

    int idx = 0;
    // The concept here is "Double elimination".
    for(auto &arg: args){
        //Trim white spaces
        std::string argRich = arg->RichType;
        while(proto[idx].find(" ") != std::string::npos){ proto[idx].erase(proto[idx].find(" "),1); }
        while(argRich.find(" ") != std::string::npos){ argRich.erase(argRich.find(" "),1); }

        if(proto[idx] == std::string(arg->GetTypeCString()) || (proto[idx] == "real" && std::string(arg->GetTypeCString()) == "integer")){
            idx += 1;
            continue;
        }
        else{
            if(std::string(arg->GetTypeCString()) == "array"){
                if(proto[idx] == argRich) {
                    idx += 1;
                    continue;
                }
                proto[idx].insert(argRich.find_first_of("["), " "); //normalize output
                info.push_back(arg->RichType);
                info.push_back(proto[idx]);
                info.push_back(std::to_string(arg->location.second));
                idx += 1;
                return true;
            }
            info.push_back(arg->GetTypeCString());
            info.push_back(proto[idx]);
            info.push_back(std::to_string(arg->location.second));
            idx += 1;
            return true;
        }
    }

    return false;
}