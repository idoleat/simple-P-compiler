#ifndef __SEMA_INFO_HOLDER_H
#define __SEMA_INFO_HOLDER_H

#include "visitor/AstNodeInclude.hpp"
#include <list>
#include <vector>

enum TypeEnum {real = 1, integer, boolean, string, array};

class iNode{
    
    public:
        iNode();
        int Type;
        std::string RichType; // storing details only for array type.
        std::list<const char*> RichTypeList;
        iNode *parent;
        std::vector<iNode *> children;
        bool HasError = false;
        bool ChildrenHasError = false;
        bool ParentHasError = false;
        int intValue;
        char *strValue;
        std::pair<long, long> location;

        void SetType(const char* type);
        const char *GetTypeCString();
        void PrintChildCount();
        //void SetRichType(const char * type);
};

class iNodeManager{
    public:
        iNode root;
        iNode *p_Node = &root;
        void initializeNode(iNode *here);
        void PropagateError(iNode *here);
};

#endif