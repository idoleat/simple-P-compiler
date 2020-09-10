#ifndef __AST_UTILS_H
#define __AST_UTILS_H

#include "AST/ast.hpp"

#include <cstdint>

// For carrying identifier through the IdList
struct IdInfo {
    IdInfo(const uint32_t line, const uint32_t col, const char *p_id)
        : location(line, col), id(p_id) {}

    Location location;
    std::string id;
};

#endif
