#pragma once

#include "exception.h"
#include "type.h"
#include "enum.h"
#include "constructor.h"
#include "field.h"
#include "method.h"


template<class OwnerType, class FieldType>
const Field* make_field(const std::string& name, FieldType OwnerType::* field){
    return new MemberField<OwnerType, FieldType>(name.substr(name.find_last_of(':') + 1), field);
}

template<class FieldType>
const Field* make_field(const std::string& name, FieldType* field){
    return new StaticField<FieldType>(name.substr(name.find_last_of(':') + 1), field);
}

template<class OwnerType, class FieldType>
const Field* make_field(const std::string& name, const FieldType OwnerType::* field){
    return new ReadonlyField<OwnerType, const FieldType>(name.substr(name.find_last_of(':') + 1), field);
}

template<class FieldType>
const Field* make_field(const std::string& name, const FieldType* field){
    return new ConstField<const FieldType>(name.substr(name.find_last_of(':') + 1), field);
}

template<class OwnerType, class ReturnType, class... Args>
const Method* make_method(const std::string& name, ReturnType(OwnerType::*fun)(Args...)){
    return new MemberMethod<OwnerType, ReturnType, Args...>(name.substr(name.find_last_of(':') + 1), fun);
}

template<class OwnerType, class ReturnType, class... Args>
const Method* make_method(const std::string& name, ReturnType(OwnerType::*fun)(Args...) const){
    return new MemberMethod<OwnerType, ReturnType, Args...>(name.substr(name.find_last_of(':') + 1), (ReturnType(OwnerType::*)(Args...))fun);
}

template<class ReturnType, class... Args>
const Method* make_method(const std::string& name, ReturnType(*fun)(Args...)){
    return new StaticMethod<ReturnType, Args...>(name.substr(name.find_last_of(':') + 1), fun);
}

#define BEGIN_DERIVED_TYPE(TYPE, BASE)  \
public: \
    static const Type* StaticType(){ \
        static const Type* type = nullptr; \
        if (type == nullptr){ \
            type = new Type(); \
            type = Type::RegisterType(const_cast<Type*>(typeof(TYPE)), #TYPE, typeof(BASE))
#define END_TYPE ; \
        } \
        return type; \
    } \
    virtual const Type* GetType() const{ \
            return StaticType(); \
    } \
private:
#define BEGIN_TYPE(TYPE)                BEGIN_DERIVED_TYPE(TYPE, std::nullptr_t)

#define CTORS(...)                              ->AddConstructors(std::vector<const Constructor*>{__VA_ARGS__})
#define FIELDS(...)                             ->AddFields(std::vector<const Field*>{__VA_ARGS__})
#define METHODS(...)                            ->AddMethods(std::vector<const Method*>{__VA_ARGS__})
#define CTOR(TYPE, ...)                         new ConstructorImpl<TYPE, __VA_ARGS__>()
#define EMPTY_CTOR(TYPE)                        new ConstructorImpl<TYPE>()
#define COPY_CTOR(TYPE)                         new ConstructorImpl<TYPE, const TYPE&>()
#define DEFAULT_CTOR(TYPE)                      EMPTY_CTOR(TYPE), COPY_CTOR(TYPE)
#define FIELD(field)                            make_field(#field, field)
#define METHOD(fun)                             make_method(#fun, fun)

#undef THROW_EXCEPTION
