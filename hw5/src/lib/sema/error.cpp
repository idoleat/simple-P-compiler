#include "sema/error.hpp"
#include "AST/ast.hpp"

#include <cstdarg>
#include <cstdio>

extern long line_positions[];
extern FILE *yyin;
extern bool has_error;

static const uint32_t kIndentionWidth = 4u;

void logSemanticError(const Location &location, const char *format, ...) {
    std::fprintf(stderr, "<Error> Found in line %u, column %u: ", location.line,
                 location.col);

    va_list args;
    va_start(args, format);
    std::vfprintf(stderr, format, args);
    va_end(args);

    // print notation
    if (std::fseek(yyin, line_positions[location.line], SEEK_SET) == 0) {
        char buffer[512];
        std::fgets(buffer, sizeof(buffer), yyin);
        std::fprintf(stderr, "\n%*s%s", kIndentionWidth, "", buffer);
        std::fprintf(stderr, "%*s\n", kIndentionWidth + location.col, "^");
    } else {
        std::fprintf(stderr, "Fail to reposition the yyin file stream.\n");
    }

    has_error = true;
}
