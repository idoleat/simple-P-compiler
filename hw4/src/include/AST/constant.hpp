#ifndef __AST_CONSTANT_H
#define __AST_CONSTANT_H

#include "AST/PType.hpp"

#include <cstdint>
#include <memory>

class Constant {
  public:
    union ConstantValue {
        int32_t integer;
        double real;
        bool boolean;
        char *string;
    };
    Constant(const PTypeSharedPtr &p_type, ConstantValue &p_value);
    ~Constant();

    const PTypeSharedPtr &getTypePtr() const;
    const char *getConstantValueCString() const;

  private:
    PTypeSharedPtr type;
    ConstantValue value;
    mutable std::string constant_value_string;
    mutable bool constant_value_string_is_valid = false;
};

#endif
