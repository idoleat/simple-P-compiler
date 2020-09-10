#include "sema/InfoHolder.hpp"
#include "sema/SemanticAnalyzer.hpp"
#include <cstring>

iNode::iNode(){
    Type = 0;
    HasError = false;
    parent = NULL;
    strValue = strdup("default");
    intValue = 0;
    RichType = "Default";
}

void iNode::SetType(const char* type){
    if(strcmp(type, "boolean") == 0) Type = TypeEnum::boolean;
    else if(strcmp(type, "integer") == 0) Type = TypeEnum::integer;
    else if(strcmp(type, "real") == 0) Type = TypeEnum::real;
    else if(strcmp(type, "string") == 0) Type = TypeEnum::string;
    else Type = TypeEnum::array;
}
/*
void iNode::SetRichType(const char * type){
    RichType = type;
}*/

// assign *here* to p_Node, prapagate error, point to *here*
void iNodeManager::initializeNode(iNode *here){
    here->parent = p_Node;
    if(p_Node->HasError) here->ParentHasError = true;
    here->parent->children.push_back(here);
    p_Node = here;
}

void iNodeManager::PropagateError(iNode *here){
    here->HasError = true;
    here->parent->ChildrenHasError = true;
    for(auto &child: here->children){
        child->ParentHasError = true;
    }
}

void iNode::PrintChildCount(){
    printf("children count: %d\n", children.size());
}

const char *iNode::GetTypeCString(){
    switch (Type)
    {
    case TypeEnum::integer :
        return "integer";
        break;
    case TypeEnum::real :
        return "real";
        break;
    case TypeEnum::string :
        return "string";
        break;
    case TypeEnum::boolean :
        return "boolean";
        break;
    default:
        return "array";
        break;
    }
}