#pragma once

#include "utils.h"
#include "exception.h"
#include "qualified_type.h"

class Field;
class Method;
class Constructor;

class Type{
protected:
    friend class TemplatedType;

    std::string name;
    const Type* baseType;
    const Type* underlyingType = nullptr;

    std::vector<const Constructor*> constructors;
    std::vector<const Field*> fields;
    std::vector<const Method*> methods;

    std::vector<const Field*> static_fields;
    std::vector<const Method*> static_methods;

    mutable std::unordered_map<std::string, const Field*> fieldsMap;
    mutable std::unordered_map<std::string, std::vector<const Method*>> methodsMap;

    bool isEnum = false;
    int templateCount = 0;
    bool templated = false;

    static std::unordered_map<std::string, const Type*>& GetTypes(){
        static std::unordered_map<std::string, const Type*> types;
        return types;
    }

    static bool MatchParams(const std::vector<QualifiedType>& src, const std::vector<QualifiedType>& match){
        if (src.size() != match.size())
            return false;

        for (size_t i = 0, n = src.size(); i < n; i++){
            if (match[i] == src[i]);
            else if (match[i].CanCast(src[i]));
            else return false;
        }

        return true;
    }

public:
    Type(int templateCount = 0){
        this->templateCount = templateCount;
        this->name = "Unregistered_Type";
        this->baseType = nullptr;
    }

    bool IsTemplate() const{
        return templateCount != 0;
    }

    bool IsComplete() const{
        return !IsTemplate() || templated;
    }

    std::string GetDescription() const;

    static Type* RegisterEnum(Type* type, const std::string& name, const Type* underlyingType){
        if (GetTypes().find(name) != GetTypes().end()) {
            return const_cast<Type *>(GetTypes().find(name)->second);
        }
            

        //std::cout << "register enum " << name << std::endl;

        const_cast<Type*>(type)->name = name;
        const_cast<Type*>(type)->isEnum = true;
        GetTypes().insert(std::make_pair(name, type));
        type->underlyingType = underlyingType;

        return type;
    }
    
    static Type* RegisterTemplate(Type* type, const std::string& name, const Type* underlyingType){
        if (GetTypes().find(name) != GetTypes().end()){
            return const_cast<Type *>(GetTypes().find(name)->second);
        }

        //std::cout << "register enum " << name << std::endl;

        const_cast<Type*>(type)->name = name;
        const_cast<Type*>(type)->isEnum = false;
        GetTypes().insert(std::make_pair(name, type));
        type->underlyingType = underlyingType;

        return type;
    }

    static Type* RegisterType(Type* type, const std::string& name, const Type* baseType){
        if (GetTypes().find(name) != GetTypes().end()) {
            return const_cast<Type *>(GetTypes().find(name)->second);
        }

        //std::cout << "register class " << name << std::endl;

        const_cast<Type*>(type)->name = name;
        const_cast<Type*>(type)->baseType = baseType;
        GetTypes().insert(std::make_pair(name, type));

        return type;
    }

    Type* AddConstructors(const std::vector<const Constructor*>& constructors);
    Type* AddFields(const std::vector<const Field*>& fields);
    Type* AddMethods(const std::vector<const Method*>& methods);

    virtual bool Is(const Type* type) const{
        if (type == this)
            return true;

        if (baseType != nullptr)
            return baseType->Is(type);

        return false;
    }

    static const Type* AddAlias(const std::string& name, const Type* type){
        if (GetTypes().find(name) != GetTypes().end())
            THROW_EXCEPTION(TypeAlreadyExists, "there is already a type named '" + name + "'");

        //std::cout << "add alias " << name << " -> " << type->GetName() << std::endl;

        GetTypes().insert(std::make_pair(name, type));

        return type;
    }

    static const Type* GetType(const std::string& typeName, bool throwIfNotFound = false){
        auto type = GetTypes()[typeName];

        if (throwIfNotFound && type == nullptr){
            THROW_EXCEPTION(TypeNotFound, "type '" + typeName + "' not found");
        }

        return type;
    }

    bool IsEnum() const{
        return isEnum;
    }

    virtual std::string GetName() const{
        return name;
    }

    const Type* GetBaseType() const{
        return baseType;
    }

    const Type* GetUnderlyingType() const{
        return underlyingType;
    }

    const std::vector<const Constructor*>& GetConstructors() const{
        return constructors;
    }

    const Constructor* GetConstructor() const{
        auto ctr = GetConstructor({});
        if (ctr == nullptr)
            THROW_EXCEPTION(NoDefualtConstructor, "class '" + name + "' has no default constructor");
        return ctr;
    }

    const Constructor* GetConstructor(const std::vector<QualifiedType>& paramTypes) const;

    std::vector<const Field*> GetMemberFields() const{
        auto fs = fields;

        if (baseType != nullptr){
            auto baseFields = baseType->GetMemberFields();
            fs.insert(fs.begin(), baseFields.begin(), baseFields.end());
        }

        return fs;
    }

    std::vector<const Field*> GetStaticFields() const{
        auto fs = static_fields;

        if (baseType != nullptr){
            auto baseFields = baseType->GetStaticFields();
            fs.insert(fs.begin(), baseFields.begin(), baseFields.end());
        }

        return fs;
    }

    std::vector<const Method*> GetMemberMethods() const{
        auto ms = methods;

        if (baseType != nullptr){
            auto baseMethods = baseType->GetMemberMethods();
            ms.insert(ms.begin(), baseMethods.begin(), baseMethods.end());
        }

        return ms;
    }

    std::vector<const Method*> GetStaticMethods() const{
        auto ms = static_methods;

        if (baseType != nullptr){
            auto baseMethods = baseType->GetStaticMethods();
            ms.insert(ms.begin(), baseMethods.begin(), baseMethods.end());
        }

        return ms;
    }

    std::vector<const Field*> GetFields() const{
        auto fs = GetMemberFields();
        auto fs2 = GetStaticFields();
        fs.insert(fs.end(), fs2.begin(), fs2.end());
        return fs;
    }

    std::vector<const Method*> GetMethods() const{
        auto ms = GetMemberMethods();
        auto ms2 = GetStaticMethods();
        ms.insert(ms.end(), ms2.begin(), ms2.end());
        return ms;
    }

    // find a field by name
    const Field* GetField(const std::string& fieldName) const;

    // find a method by name, just uses while the method has no overloads
    const Method* GetMethod(const std::string& methodName) const;

    // find a method by its signature
    const Method* GetMethod(const std::string& methodName, const std::vector<QualifiedType>& paramTypes) const;

};

class TemplatedType : public Type{
protected:
    std::vector<QualifiedType> templates;
    const Type* templateType;

public:
    TemplatedType(const Type* type, const std::vector<QualifiedType>& templates){
        templateType = type;
        if (templateCount == 0)
            THROW_EXCEPTION(InvalidArguments, "tried to register an non-template class as an templated type");
        this->templates = templates;
        templated = true;
    }

    virtual std::string GetName() const override{
        std::stringstream ss;
        ss << templateType->name << "<";
        ss << templates[0].ToString();
        for (size_t i = 1, n = templates.size(); i < n; i++){
        ss << ", " << templates[i].ToString();
        }
        ss << ">";
        return ss.str();
    }

    const std::vector<QualifiedType>& GetTemplates() const{
        return templates;
    }

    const Type* GetTemplateType() const{
        return templateType;
    }

    bool MatchTemplates(const std::vector<QualifiedType>& templates){
        return templates == this->templates;
    }

    virtual bool Is(const Type* type) const override;

};

#define REFLECT_PRIMITIVE_TYPE(typeName) \
template<> \
struct ReflectType<typeName>{ \
    static const Type* type; \
    static const Type* Value(){ \
        static const Type* type = Type::RegisterType(new Type, #typeName, nullptr); \
        return type; \
    } \
};

#define REFLECT_ALIASED_PRIMITIVE_TYPE(TYPE, REAL_TYPE) \
template<> \
struct ReflectType<TYPE>{ \
    static const Type* type; \
    static const Type* Value(){ \
        static const Type* type = Type::AddAlias(#TYPE, typeof(REAL_TYPE)); \
        return type; \
    } \
};

// make primitive types reflectable
// doesn't consider long double, it's not recommanded to use

REFLECT_PRIMITIVE_TYPE(void)
REFLECT_PRIMITIVE_TYPE(int8_t)
REFLECT_PRIMITIVE_TYPE(int16_t)
REFLECT_PRIMITIVE_TYPE(int32_t)
REFLECT_PRIMITIVE_TYPE(int64_t)
REFLECT_PRIMITIVE_TYPE(uint8_t)
REFLECT_PRIMITIVE_TYPE(uint16_t)
REFLECT_PRIMITIVE_TYPE(uint32_t)
REFLECT_PRIMITIVE_TYPE(uint64_t)
REFLECT_PRIMITIVE_TYPE(float)
REFLECT_PRIMITIVE_TYPE(double)
REFLECT_PRIMITIVE_TYPE(bool)
REFLECT_PRIMITIVE_TYPE(std::string)
REFLECT_PRIMITIVE_TYPE(std::wstring)
//REFLECT_PRIMITIVE_TYPE(std::vector<std::string>)

#undef REFLECT_PRIMITIVE_TYPE

#if defined(CHAR_MIN) && (CHAR_MIN == 0)
REFLECT_ALIASED_PRIMITIVE_TYPE(char, uint8_t)
#else
REFLECT_ALIASED_PRIMITIVE_TYPE(char, int8_t)
#endif

#if defined(WCHAR_MAX) && (WCHAR_MAX == 0xffffffffL)
REFLECT_ALIASED_PRIMITIVE_TYPE(wchar_t, uint32_t)
#else
REFLECT_ALIASED_PRIMITIVE_TYPE(wchar_t, uint16_t)
#endif

#if defined(LONG_MAX) && (LONG_MAX > 0xffffffffL)
REFLECT_ALIASED_PRIMITIVE_TYPE(signed long, int64_t)
REFLECT_ALIASED_PRIMITIVE_TYPE(unsigned long, uint64_t)
#else
REFLECT_ALIASED_PRIMITIVE_TYPE(signed long, int32_t)
REFLECT_ALIASED_PRIMITIVE_TYPE(unsigned long, uint32_t)
#endif

#undef REFLECT_ALIASED_PRIMITIVE_TYPE

class ____DUMMY_ADD_ALIASED{
    static const ____DUMMY_ADD_ALIASED instance;
public:
    ____DUMMY_ADD_ALIASED();
};

#define DEF_TMPL(T, n) \
template<> \
struct TemplateType<T>{ \
    static const Type* type; \
    static const Type* Value(){ \
        static const Type* type = Type::RegisterTemplate(new Type(n), #T, nullptr); \
        return type; \
    } \
};

DEF_TMPL(std::shared_ptr, 1)
DEF_TMPL(std::vector, 2)
#undef DEF_TMPL

#define DEF_CT1(C,n) \
template<class T> \
struct ReflectType<C<T>>{ \
    static const Type* type; \
    static const Type* Value(){ \
        static const Type* type = Type::RegisterTemplate(new Type(n), #C, nullptr); \
        return type; \
    } \
};


DEF_CT1(std::shared_ptr, 1);
DEF_CT1(std::vector, 2);
#undef DEF_CT1


#define DEF_CT1(C,n) \
template<class T1, class T2> \
struct ReflectType<C<T1, T2>>{ \
    static const Type* type; \
    static const Type* Value(){ \
        static const Type* type = Type::RegisterTemplate(new Type(n), #C, nullptr); \
        return type; \
    } \
};

DEF_CT1(std::unordered_map, 3);
#undef DEF_CT1

#define DEF_CT1(C) \
template<class... T> \
struct ReflectType<C<T...>>{ \
        static const Type* type; \
        static const Type* Value(){ \
            static const Type* type = new TemplatedType(TemplateType<C>::Value(), {qualified_typeof(T)...}); \
            return type; \
    } \
};

DEF_CT1(std::shared_ptr);
DEF_CT1(std::vector);
#undef DEF_CT1

//template<class T>
//struct ReflectType<std::vector<T>>{
//        static const Type* type;
//        static const Type* Value(){
//            static const Type* type = Type::RegisterType(new Type, "std::vector<" + qualified_typeof(T).ToString() + ">", nullptr);
//            return type;
//    }
//};

