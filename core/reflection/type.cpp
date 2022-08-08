#include "type.h"
#include "constructor.h"
#include "field.h"
#include "method.h"

std::string Type::GetDescription() const{
    std::stringstream ss;
    ss << "class " << name;
    if (baseType) ss << " : " << baseType->GetName();
    ss << " {" << std::endl;
    for (auto f : GetFields()){
        ss << "    " << f->GetDescription(f->GetOwnerType() != this) << ";" << std::endl;
    }
    ss << std::endl;
    for (auto c : GetConstructors()){
        ss << "    " << c->GetDescription() << ";" << std::endl;
    }
    ss << std::endl;
    for (auto m : GetMethods()){
        ss << "    " << m->GetDescription(m->GetOwnerType() != this) << ";" << std::endl;
    }
    ss << "};" << std::endl;
    return ss.str();
}

Type* Type::AddFields(const std::vector<const Field*>& fields){
    for (auto f : fields){
        if (f->GetOwnerType() == nullptr)
            const_cast<Field*>(f)->ownerType = this;
        (f->IsStatic() ? this->static_fields : this->fields).push_back(f);
        fieldsMap.insert(std::make_pair(f->GetName(), f));
    }
    return this;
}

Type* Type::AddMethods(const std::vector<const Method*>& methods){
    for (auto m : methods){
        if (m->GetOwnerType() == nullptr)
            const_cast<Method*>(m)->ownerType = this;
        (m->IsStatic() ? this->static_methods : this->methods).push_back(m);
        methodsMap[m->GetName()].push_back(m);
    }
    return this;
}

Type* Type::AddConstructors(const std::vector<const Constructor*>& constructors){
    for (auto c : constructors){
        this->constructors.push_back(c);
    }
    return this;
}

const Constructor* Type::GetConstructor(const std::vector<QualifiedType>& paramTypes) const{
    std::vector<const Constructor*> matches;

    for (auto c : constructors){
        if (MatchParams(c->GetParamTypes(), paramTypes))//c->GetParamTypes() == paramTypes)
            return c;//matches.push_back(c);
    }

    if (matches.empty()){
        return nullptr;
    }
    if (matches.size() > 1){
        std::stringstream err;
        err << "constructor '" << name << "' ambiguous matched between: " << std::endl;
        for (auto m : matches){
            err << "\t" << m->GetDescription() << std::endl;
        }
        THROW_EXCEPTION(AmbiguousMatched, err.str());
    }
    else{
        return matches[0];
    }
}

const Field* Type::GetField(const std::string& fieldName) const{
    auto f = fieldsMap[fieldName];

    if (f == nullptr && baseType != nullptr){
        return baseType->GetField(fieldName);
    }

    return f;
}

const Method* Type::GetMethod(const std::string& methodName) const{
    auto ms = GetMethods();
    std::vector<const Method*> matches;

    for (auto m : ms){
        if (m->GetName() == methodName)
            matches.push_back(m);
    }

    if (matches.empty()) {
        return nullptr;
    }
    else if (matches.size() > 1) {
        std::stringstream err;
        err << "method '" << methodName << "' ambiguous matched between: " << std::endl;
        for (auto m : matches){
            err << "\t" << m->GetDescription() << std::endl;
        }
        THROW_EXCEPTION(AmbiguousMatched, err.str());
    }
    else {
        return matches[0];
    }
    /*auto ms = methodsMap[methodName];

    if (ms.size()>1) {
    std::stringstream err;
    err << "method '" << methodName << "' ambiguous matched between: " << std::endl;
    for(auto m : ms){
    err << "\t" << m->GetDescription() << endl;
    }
    throw err.str();
    }

    if (ms.empty()) {
    if(baseType == nullptr)
    return nullptr;
    else
    return baseType->GetMethod(methodName);
    } else {
    if(baseType != nullptr && baseType->GetMethod(methodName) != nullptr){
    std::stringstream err;
    err << "method '" << methodName << "' ambiguous matched between: " << std::endl;
    err << "\t" << ms[0]->GetDescription() << endl;
    err << "\t" << baseType->GetMethod(methodName)->GetDescription() << endl;
    throw err.str();
    }else{
    return ms[0];
    }
    }*/
}

const Method* Type::GetMethod(const std::string& methodName, const std::vector<QualifiedType>& paramTypes) const{
    auto ms = methodsMap[methodName];
    std::vector<const Method*> matches;

    if (!ms.empty()) {
        for (auto m : ms) {
            if (MatchParams(m->GetParamTypes(), paramTypes)){//paramTypes == m->GetParamTypes()){
                matches.push_back(m);
            }
        }
    }

    if (baseType != nullptr){
        //return baseType->GetMethod(methodName, paramTypes);
        auto r = baseType->GetMethod(methodName, paramTypes);
        if (r) matches.push_back(r);
    }

    if (matches.empty()){
        return nullptr;
    }
    else if (matches.size() > 1){
        std::stringstream err;
        err << "method '" << name << "(";
        for (size_t i = 0, n = paramTypes.size(); i < n; i++){
            if (i != 0) err << ", ";
            err << paramTypes[i].ToString();
        }
        err << ")' ambiguous matched between: " << std::endl;
        for (auto m : matches){
            err << "\t" << m->GetDescription() << std::endl;
        }
        THROW_EXCEPTION(AmbiguousMatched, err.str());
    }
    else{
        return matches[0];
    }
}

bool TemplatedType::Is(const Type* type) const{
    if (templateType == template_typeof(std::shared_ptr) && type->IsTemplate() && type->IsComplete() && ((TemplatedType*)type)->templateType == template_typeof(std::shared_ptr)){
        auto t1 = templates[0];
        auto t2 = ((TemplatedType*)type)->templates[0];
        return t1.ReferType() == t2.ReferType() && !t1.IsPointer() && !t2.IsPointer() && (!t1.IsConst() || t2.IsConst()) && (!t1.IsVolatile() || t2.IsVolatile()) && t1.GetType()->Is(t2.GetType());
    }
    return Type::Is(type);
}

const Type* Null::GetType(){
    static const Type type;
    return &type;
}

#define IMPL_REFLECT_PRIMITIVE_TYPE(typeName) \
const Type* ReflectType<typeName>::type = typeof(typeName);
#define IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(TYPE, REAL_TYPE) \
const Type* ReflectType<TYPE>::type = typeof(TYPE);

IMPL_REFLECT_PRIMITIVE_TYPE(void)
IMPL_REFLECT_PRIMITIVE_TYPE(int8_t)
IMPL_REFLECT_PRIMITIVE_TYPE(int16_t)
IMPL_REFLECT_PRIMITIVE_TYPE(int32_t)
IMPL_REFLECT_PRIMITIVE_TYPE(int64_t)
IMPL_REFLECT_PRIMITIVE_TYPE(uint8_t)
IMPL_REFLECT_PRIMITIVE_TYPE(uint16_t)
IMPL_REFLECT_PRIMITIVE_TYPE(uint32_t)
IMPL_REFLECT_PRIMITIVE_TYPE(uint64_t)
IMPL_REFLECT_PRIMITIVE_TYPE(float)
IMPL_REFLECT_PRIMITIVE_TYPE(double)
IMPL_REFLECT_PRIMITIVE_TYPE(bool)
IMPL_REFLECT_PRIMITIVE_TYPE(std::string)
IMPL_REFLECT_PRIMITIVE_TYPE(std::wstring)
//IMPL_REFLECT_PRIMITIVE_TYPE(std::vector<std::string>)

#if defined(CHAR_MIN) && (CHAR_MIN == 0)
IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(char, uint8_t)
#else
IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(char, int8_t)
#endif

#if defined(WCHAR_MAX) && (WCHAR_MAX == 0xffffffffL)
IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(wchar_t, uint32_t)
#else
IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(wchar_t, uint16_t)
#endif

#if defined(LONG_MAX) && (LONG_MAX > 0xffffffffL)
IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(signed long, int64_t)
IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(unsigned long, uint64_t)
#else
IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(signed long, int32_t)
IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE(unsigned long, uint32_t)
#endif

#undef IMPL_REFLECT_PRIMITIVE_TYPE
#undef IMPL_REFLECT_ALIASED_PRIMITIVE_TYPE

____DUMMY_ADD_ALIASED::____DUMMY_ADD_ALIASED(){
#define ADD_ALIAS(type) Type::AddAlias(#type, typeof(type))

    ADD_ALIAS(int);
    ADD_ALIAS(signed int);
    ADD_ALIAS(unsigned int);
    ADD_ALIAS(short);
    ADD_ALIAS(signed short);
    ADD_ALIAS(unsigned short);
    ADD_ALIAS(long);
    ADD_ALIAS(long long);
    ADD_ALIAS(signed long long);
    ADD_ALIAS(unsigned long long);
    ADD_ALIAS(signed char);
    ADD_ALIAS(unsigned char);
#undef ADD_ALIAS

#define ADD_ALIAS2(type) Type::AddAlias(#type, typeof(type##_t))
    ADD_ALIAS2(int8);
    ADD_ALIAS2(int16);
    ADD_ALIAS2(int32);
    ADD_ALIAS2(int64);
    ADD_ALIAS2(uint8);
    ADD_ALIAS2(uint16);
    ADD_ALIAS2(uint32);
    ADD_ALIAS2(uint64);
#undef ADD_ALIAS2

    Type::AddAlias("Int8", typeof(int8_t));
    Type::AddAlias("Int16", typeof(int16_t));
    Type::AddAlias("Int32", typeof(int32_t));
    Type::AddAlias("Int64", typeof(int64_t));
    Type::AddAlias("UInt8", typeof(uint8_t));
    Type::AddAlias("UInt16", typeof(uint16_t));
    Type::AddAlias("UInt32", typeof(uint32_t));
    Type::AddAlias("UInt64", typeof(uint64_t));

    Type::AddAlias("Int", typeof(int));
    Type::AddAlias("Integer", typeof(int));
    Type::AddAlias("Float", typeof(float));
    Type::AddAlias("Double", typeof(double));
    Type::AddAlias("Long", typeof(long));
    Type::AddAlias("Bool", typeof(bool));
    Type::AddAlias("Boolean", typeof(bool));
    Type::AddAlias("string", typeof(std::string));
    Type::AddAlias("String", typeof(std::string));
    Type::AddAlias("wstring", typeof(std::wstring));
    Type::AddAlias("WString", typeof(std::wstring));

    const_cast<Type*>(typeof(std::string))->AddConstructors({ new ConstructorImpl<std::string>(), new ConstructorImpl<std::string, const char*>(), new ConstructorImpl<std::string, const std::string&>() });
}

const ____DUMMY_ADD_ALIASED instance;
