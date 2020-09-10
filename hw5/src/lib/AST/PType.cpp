#include "AST/PType.hpp"

#include <cassert>

const char *kTypeString[] = {"void", "integer", "real", "boolean", "string"};

PType::PType(PrimitiveTypeEnum type) : type(type) {}

void PType::setDimensions(std::vector<uint64_t> &dims) {
    dimensions = std::move(dims);
}

const PType::PrimitiveTypeEnum PType::getPrimitiveType() const { return type; }

// logical constness
const char *PType::getPTypeCString() const {
    if (!type_string_is_valid) {
        type_string += kTypeString[static_cast<int>(type)];

        if (dimensions.size() != 0) {
            type_string += " ";

            for (const auto &dim : dimensions) {
                type_string += "[" + std::to_string(dim) + "]";
            }
        }
        type_string_is_valid = true;
    }

    return type_string.c_str();
}

const std::vector<uint64_t> &PType::getDimensions() const { return dimensions; }

bool PType::isVoid() const {
    return type == PrimitiveTypeEnum::kVoidType && dimensions.empty();
}

bool PType::isPrimitiveInteger() const {
    return type == PrimitiveTypeEnum::kIntegerType;
}

bool PType::isPrimitiveReal() const {
    return type == PrimitiveTypeEnum::kRealType;
}

bool PType::isPrimitiveBool() const {
    return type == PrimitiveTypeEnum::kBoolType;
}

bool PType::isPrimitiveString() const {
    return type == PrimitiveTypeEnum::kStringType;
}

bool PType::isInteger() const {
    return isPrimitiveInteger() && dimensions.empty();
}

bool PType::isReal() const { return isPrimitiveReal() && dimensions.empty(); }

bool PType::isBool() const { return isPrimitiveBool() && dimensions.empty(); }

bool PType::isString() const {
    return isPrimitiveString() && dimensions.empty();
}

bool PType::isScalar() const {
    return dimensions.empty() && type != PrimitiveTypeEnum::kVoidType;
}

bool PType::compare(const PType *p_type) const {
    switch (type) {
    case PrimitiveTypeEnum::kIntegerType:
    case PrimitiveTypeEnum::kRealType:
        if (!(p_type->isPrimitiveInteger() || p_type->isPrimitiveReal())) {
            return false;
        }
        break;
    case PrimitiveTypeEnum::kBoolType:
        if (!p_type->isPrimitiveBool()) {
            return false;
        }
        break;
    case PrimitiveTypeEnum::kStringType:
        if (!p_type->isPrimitiveString()) {
            return false;
        }
        break;
    default:
        assert(false && "comparing unknown primitive type or void type");
        return false;
    }

    auto &p_type_dims = p_type->getDimensions();
    if (dimensions.size() != p_type_dims.size()) {
        return false;
    }

    for (std::size_t i = 0; i < dimensions.size(); ++i) {
        if (dimensions[i] != p_type_dims[i]) {
            return false;
        }
    }
    return true;
}

PType *PType::getStructElementType(const std::size_t nth) const {
    if (nth > dimensions.size()) {
        return nullptr;
    }

    auto *p_type = new PType(type);

    std::vector<uint64_t> dims;
    for (std::size_t i = nth; i < dimensions.size(); ++i) {
        dims.emplace_back(dimensions[i]);
    }
    p_type->setDimensions(dims);

    return p_type;
}
