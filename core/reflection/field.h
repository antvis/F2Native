#pragma once

#include "utils.h"
#include "param.h"
#include "exception.h"
#include "qualified_type.h"

class Field{
    friend class Type;
protected:
    std::string name;
    const Type* ownerType;
    const QualifiedType fieldType;
    bool isStatic;

    Field(const std::string& name, const Type* ownerType, const QualifiedType fieldType, bool isStatic = false)
        : fieldType(fieldType){
        this->name = name;
        this->ownerType = ownerType;
        this->isStatic = isStatic;
    }

public:
    // is it a static field;
    bool IsStatic() const{
        return isStatic;
    }

    // whether if the field is qualified to const.
    bool IsReadonly() const{
        return fieldType.IsConst();
    }

    // whether if the field is a constant, which means it's qualified to 'static const'.
    bool IsConstant() const{
        return IsStatic() && IsReadonly();
    }

    std::string GetDescription(bool showOwnerType = true) const;

    // @throw InvalidAccess
    virtual void Set(Any obj, Any value) const{
        THROW_EXCEPTION(InternalError, "unimplemented setter");
    }

    virtual Any Get(Any obj) const{
        THROW_EXCEPTION(InternalError, "unimplemented getter");
    }

    const std::string& GetName() const{
        return name;
    }

    const QualifiedType GetType() const{
        return fieldType;
    }

    const Type* GetOwnerType() const{
        return ownerType;
    }

};

template<class OwnerType, class FieldType>
class MemberField : public Field{
private:
    FieldType OwnerType::* field;

public:
    MemberField(const std::string& name, FieldType OwnerType::* field)
        : Field(name, typeof(OwnerType), GetQualifiedType<FieldType>::Value()) {
        this->field = field;
    }

    virtual void Set(Any obj, Any value) const{
        if (obj.IsNull()) THROW_EXCEPTION(NullPointerException, "null pointer of '" + ownerType->GetName() + "'");
        ((OwnerType*)obj)->*field = value.Cast<FieldType>();//ParamCast<FieldType>(value);
    }

    virtual Any Get(Any obj) const{
        if (obj.IsNull()) THROW_EXCEPTION(NullPointerException, "null pointer of '" + ownerType->GetName() + "'");
        return (Any)((OwnerType*)obj->*field);
    }
};

template<class FieldType>
class StaticField : public Field{
private:
    FieldType* field;

public:
    StaticField(const std::string& name, FieldType* field)
        : Field(name, nullptr, GetQualifiedType<FieldType>::Value(), true) {
        this->field = field;
    }

    virtual void Set(Any obj, Any value) const{
        *field = (FieldType)value;//ParamCast<FieldType>(value);
    }

    virtual Any Get(Any obj) const{
        return (Any)*field;
    }
};

template<class OwnerType, class FieldType>
class ReadonlyField : public Field{
private:
    FieldType OwnerType::* field;

public:
    ReadonlyField(const std::string& name, FieldType OwnerType::* field)
        : Field(name, typeof(OwnerType), GetQualifiedType<FieldType>::Value(), false) {
        this->field = field;
    }

    virtual void Set(Any obj, Any value) const{
        THROW_EXCEPTION(InvalidAccess, "tried to set value on a readonly field '" + GetDescription() + "'");
    }

    virtual Any Get(Any obj) const{
        if (obj == nullptr) THROW_EXCEPTION(NullPointerException, "null pointer of '" + ownerType->GetName() + "'");
        return (Any)((OwnerType*)obj->*field);
    }
};

template<class FieldType>
class ConstField : public Field{
private:
    FieldType* field;

public:
    ConstField(const std::string& name, FieldType* field)
        : Field(name, nullptr, GetQualifiedType<FieldType>::Value(), true) {
        this->field = field;
    }

    virtual void Set(Any obj, Any value) const{
        THROW_EXCEPTION(InvalidAccess, "tried to set value on a const field '" + GetDescription() + "'");
    }

    virtual Any Get(Any obj) const{
        return (Any)*field;
    }
};

template<class FieldType>
class TemplateField : public Field{
    
};
