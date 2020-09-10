#ifndef __AST_AST_DUMPER_H
#define __AST_AST_DUMPER_H

#include "visitor/AstNodeVisitor.h"

#include <stdint.h>

typedef struct __AstDumper AstDumper;

/******************************************************************************
 * > Interface related
 *   - check AstNodeVisitor.h for more info
 ******************************************************************************/
typedef struct __IAstDumper {
    void (*construct)(AstDumper *self);
    void (*destruct)(AstDumper *self);

    AstDumper *(*operatorNew)(void);
    void (*operatorDelete)(AstDumper *ast_dumper);

    AST_NODE_VISITOR_VIRTUAL_FUNCTIONS;
} IAstDumper;

extern const IAstDumper gAstDumper;

#endif
