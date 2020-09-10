#ifndef __SEMA_ERROR_H
#define __SEMA_ERROR_H

struct Location;

void logSemanticError(const Location &location, const char *format, ...);

#endif
