#pragma once

#include "utils.h"

class Type;

enum class ReferType{ None, Refer, RightRefer };

class QualifiedType{
protected:
    const Type* type = nullptr;
    bool isConst = false;
    bool isVolatile = false;
    int pointerCount = 0;
    ReferType referType = ReferType::None;

public:

    QualifiedType(){

    }

    QualifiedType(const Type* type){
        this->type = type;
    }

    QualifiedType(QualifiedType type, bool isPointer, bool isConst, bool isVolatile, ReferType referType){
        this->type = type.type;
        this->isConst = type.isConst;
        this->isVolatile = type.isVolatile;
        this->referType = type.referType;
        this->pointerCount = type.pointerCount;

        if (isPointer) pointerCount++;
        if (isConst) this->isConst = true;
        if (isVolatile) this->isVolatile = true;
        if (referType != ReferType::None) this->referType = referType;
    }

    const Type* GetType() const{
        return type;
    }

    bool IsNumber() const;

    bool IsFloatingNumber() const;

    bool IsIntegerNumber() const;

    bool IsBool() const;

    bool IsString() const;

    bool IsEnum() const;

    QualifiedType RemoveCV() const{
        QualifiedType t = *this;
        t.isConst = false;
        t.isVolatile = false;
        return t;
    }

    QualifiedType RemoveReference() const{
        QualifiedType t = *this;
        t.referType = ReferType::None;
        return t;
    }

    QualifiedType RemovePointer() const{
        if (!IsPointer())
            return *this;
        QualifiedType t = *this;
        t.pointerCount--;
        return t;
    }

    bool CanCast(const QualifiedType& t) const;

    bool IsConst() const{
        return isConst;
    }

    bool IsVolatile() const{
        return isVolatile;
    }

    bool IsPointer() const{
        return pointerCount > 0;
    }

    int PointerCount() const{
        return pointerCount;
    }

    ReferType ReferType() const{
        return referType;
    }

    bool IsReference() const{
        return referType != ReferType::None;
    }

    bool IsLValueReference() const{
        return referType == ReferType::Refer;
    }

    bool IsRValueReference() const{
        return referType == ReferType::RightRefer;
    }

    std::string ToString() const;

    /*operator const Type*() const{
        return type;
    }*/

    bool operator==(const QualifiedType& t) const{
        return type == t.type && isConst == t.isConst && isVolatile == t.isVolatile && pointerCount == t.pointerCount && referType == t.referType;
    }

    bool operator!=(const QualifiedType& t) const{
        return !operator==(t);
    }
};

template<class T>
struct GetQualifiedType{
    static const QualifiedType Value(){
        return typeof(T);
    }
};

#define qualified_typeof(type) GetQualifiedType<type>::Value()

#define GET_QUALIFIED_TYPE(TYPE, ...) \
template<class T> \
struct GetQualifiedType<TYPE>{ \
    static const QualifiedType Value(){ \
        return QualifiedType(GetQualifiedType<T>::Value(), __VA_ARGS__); \
    } \
};

GET_QUALIFIED_TYPE(T&, false, false, false, ReferType::Refer);
GET_QUALIFIED_TYPE(T&&, false, false, false, ReferType::RightRefer);
GET_QUALIFIED_TYPE(T*, true, false, false, ReferType::None);
GET_QUALIFIED_TYPE(const T, false, true, false, ReferType::None);
GET_QUALIFIED_TYPE(const T&, false, true, false, ReferType::Refer);
GET_QUALIFIED_TYPE(const T&&, false, true, false, ReferType::RightRefer);
GET_QUALIFIED_TYPE(const T*, true, true, false, ReferType::None);
GET_QUALIFIED_TYPE(volatile T, false, false, true, ReferType::None);
GET_QUALIFIED_TYPE(volatile T&, false, false, true, ReferType::Refer);
GET_QUALIFIED_TYPE(volatile T&&, false, false, true, ReferType::RightRefer);
GET_QUALIFIED_TYPE(volatile T*, true, false, true, ReferType::None);
GET_QUALIFIED_TYPE(const volatile T, false, true, true, ReferType::None);
GET_QUALIFIED_TYPE(const volatile T&, false, true, true, ReferType::Refer);
GET_QUALIFIED_TYPE(const volatile T&&, false, true, true, ReferType::RightRefer);
GET_QUALIFIED_TYPE(const volatile T*, true, true, true, ReferType::None);

#undef GET_QUALIFIED_TYPE
