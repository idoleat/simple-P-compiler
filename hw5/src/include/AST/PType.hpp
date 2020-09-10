#ifndef __AST_PTYPE_H
#define __AST_PTYPE_H

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

class PType;

typedef std::shared_ptr<const PType> PTypeSharedPtr;

class PType {
  public:
    enum class PrimitiveTypeEnum : uint8_t {
        kVoidType,
        kIntegerType,
        kRealType,
        kBoolType,
        kStringType
    };

    PType(PrimitiveTypeEnum type);
    ~PType() = default;

    void setDimensions(std::vector<uint64_t> &dims);

    const PrimitiveTypeEnum getPrimitiveType() const;
    const char *getPTypeCString() const;
    const std::vector<uint64_t> &getDimensions() const;

    bool isPrimitiveInteger() const;
    bool isPrimitiveReal() const;
    bool isPrimitiveBool() const;
    bool isPrimitiveString() const;

    bool isVoid() const;
    bool isInteger() const;
    bool isReal() const;
    bool isBool() const;
    bool isString() const;

    bool isScalar() const;

    bool compare(const PType *p_type) const;

    PType *getStructElementType(std::size_t nth) const;

  private:
    PrimitiveTypeEnum type;
    std::vector<uint64_t> dimensions;
    mutable std::string type_string;
    mutable bool type_string_is_valid = false;
};

#endif
