#include "qualified_type.h"
#include "type.h"

inline const Type *int8Type() {
    static const Type* int8Type = typeof(int8_t);
    return int8Type;
}

inline const Type *int16Type() {
    static const Type* int16Type = typeof(int16_t);
    return int16Type;
}

inline const Type *int32Type() {
    static const Type* int32Type = typeof(int32_t);
    return int32Type;
}

inline const Type *int64Type() {
    static const Type* int64Type = typeof(int64_t);
    return int64Type;
}

inline const Type *uint8Type() {
    static const Type* uint8Type = typeof(uint8_t);
    return uint8Type;
}


inline const Type *uint16Type() {
    static const Type* uint16Type = typeof(uint16_t);
    return uint16Type;
}

inline const Type *uint32Type() {
    static const Type* uint32Type = typeof(uint32_t);
    return uint32Type;
}

inline const Type *uint64Type() {
    static const Type* uint64Type = typeof(uint64_t);
    return uint64Type;
}

inline const Type *intType() {
    static const Type* intType = typeof(int);
    return intType;
}

inline const Type *longType() {
    static const Type* longType = typeof(long);
    return longType;
}

inline const Type *longlongType() {
    static const Type* longlongType = typeof(long long);
    return longlongType;
}

inline const Type *charType() {
    static const Type* charType = typeof(char);
    return charType;
}

inline const Type *ucharType() {
    static const Type* ucharType = typeof(unsigned char);
    return ucharType;
}

inline const Type *scharType() {
    static const Type* scharType = typeof(signed char);
    return scharType;
}

inline const Type *uintType() {
    static const Type* uintType = typeof(unsigned int);
    return uintType;
}

inline const Type *ulongType() {
    static const Type* ulongType = typeof(unsigned long);
    return ulongType;
}

inline const Type *ulonglongType() {
    static const Type* ulonglongType = typeof(unsigned long long);
    return ulonglongType;
}

inline const Type *floatType() {
    static const Type* floatType = typeof(float);
    return floatType;
}

inline const Type *doubleType() {
    static const Type* doubleType = typeof(double);
    return doubleType;
}

inline const Type *boolType() {
    static const Type* boolType = typeof(bool);
    return boolType;
}

inline const Type *wcharType() {
    static const Type* wcharType = typeof(wchar_t);
    return wcharType;
}

inline const Type *stringType() {
    static const Type* stringType = typeof(std::string);
    return stringType;
}

inline const Type *arrayType() {
    static const Type* arrayType = template_typeof(std::vector);
    return arrayType;
}


std::string QualifiedType::ToString() const{
    if (!type) return "(null)";

    std::stringstream ss;
    if (isConst) ss << "const ";
    if (isVolatile) ss << "volatile ";
    ss << type->GetName();
    ss << std::string(pointerCount, '*');
    if (referType == ReferType::Refer) ss << "&";
    else if (referType == ReferType::RightRefer) ss << "&&";
    return ss.str();
}

bool QualifiedType::CanCast(const QualifiedType& t) const{
    if (t.type->IsEnum()){  // const char* -> const string&
        if (t.type == type)
            return true;
        if (type == typeof(int64_t))
            return true;
    }
    if (type->Is(t.type)){
        if (pointerCount == 0 && t.pointerCount == 0 && !t.IsReference())
            return true;
        if (pointerCount == t.pointerCount && (!isConst || t.isConst) && (!isVolatile || t.isVolatile) && (referType == ReferType::None || (referType == ReferType::Refer && t.referType != ReferType::None) || t.referType == ReferType::RightRefer))
            return true;
    }
    return false;//t.type->GetConstructor({ *this }) != nullptr;
}

bool QualifiedType::IsNumber() const{
    return type == int8Type() || type == int16Type() || type == int32Type() || type == int64Type()
        || type == uint8Type() || type == uint16Type() || type == uint32Type() || type == uint64Type()
        || type == floatType() || type == doubleType();
}

bool QualifiedType::IsFloatingNumber() const{
    return type == floatType() || type == doubleType();
}

bool QualifiedType::IsIntegerNumber() const{
    return type == int8Type() || type == int16Type() || type == int32Type() || type == int64Type()
        || type == uint8Type() || type == uint16Type() || type == uint32Type() || type == uint64Type();
}

bool QualifiedType::IsBool() const{
    return type == boolType();
}

bool QualifiedType::IsString() const{
    return type == stringType() || (type == charType() && pointerCount == 1);
}

bool QualifiedType::IsEnum() const{
    return type->IsEnum();
}

bool QualifiedType::IsArray() const{
    return type == arrayType();
}
