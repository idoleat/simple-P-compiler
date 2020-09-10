#include "AST/constant.hpp"
#include "AST/PType.hpp"

#include <cstdlib>

#include <cassert>

static const char *kTFString[] = {"false", "true"};

Constant::Constant(const PTypeSharedPtr &p_type, ConstantValue &p_value)
    : type(p_type), value(p_value) {}

Constant::~Constant() {
    if (type->getPrimitiveType() == PType::PrimitiveTypeEnum::kStringType) {
        free(value.string);
    }
}

const PTypeSharedPtr &Constant::getTypePtr() const { return type; }

// logical constness
const char *Constant::getConstantValueCString() const {
    if (!constant_value_string_is_valid) {
        switch (type->getPrimitiveType()) {
        case PType::PrimitiveTypeEnum::kIntegerType:
            constant_value_string = std::to_string(value.integer);
            break;
        case PType::PrimitiveTypeEnum::kRealType:
            constant_value_string = std::to_string(value.real);
            break;
        case PType::PrimitiveTypeEnum::kBoolType:
            constant_value_string = kTFString[value.boolean];
            break;
        case PType::PrimitiveTypeEnum::kStringType:
            constant_value_string = value.string;
            break;
        case PType::PrimitiveTypeEnum::kVoidType:
        default:
            break;
        }
        constant_value_string_is_valid = true;
    }
    return constant_value_string.c_str();
}

const int32_t Constant::integer() const {
    if (type->getPrimitiveType() != PType::PrimitiveTypeEnum::kIntegerType) {
        assert(false && "Shouldn't reach here");
        return 0;
    }
    return value.integer;
}

const double Constant::real() const {
    if (type->getPrimitiveType() != PType::PrimitiveTypeEnum::kRealType) {
        assert(false && "Shouldn't reach here");
        return 0.0;
    }
    return value.real;
}

const bool Constant::boolean() const {
    if (type->getPrimitiveType() != PType::PrimitiveTypeEnum::kBoolType) {
        assert(false && "Shouldn't reach here");
        return false;
    }
    return value.boolean;
}

const char *Constant::string() const {
    if (type->getPrimitiveType() != PType::PrimitiveTypeEnum::kStringType) {
        assert(false && "Shouldn't reach here");
        return NULL;
    }
    return value.string;
}
