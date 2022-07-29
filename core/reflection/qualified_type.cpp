#include "qualified_type.h"
#include "type.h"

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
    return type == int8Type || type == int16Type || type == int32Type || type == int64Type
        || type == uint8Type || type == uint16Type || type == uint32Type || type == uint64Type
        || type == floatType || type == doubleType;
}

bool QualifiedType::IsFloatingNumber() const{
    return type == floatType || type == doubleType;
}

bool QualifiedType::IsIntegerNumber() const{
    return type == int8Type || type == int16Type || type == int32Type || type == int64Type
        || type == uint8Type || type == uint16Type || type == uint32Type || type == uint64Type;
}

bool QualifiedType::IsBool() const{
    return type == boolType;
}

bool QualifiedType::IsString() const{
    return type == stringType || (type == charType && pointerCount == 1);
}

bool QualifiedType::IsEnum() const{
    return type->IsEnum();
}

bool QualifiedType::IsArray() const {
    return type == numberArrayType ;
}
