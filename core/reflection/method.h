#pragma once

#include "utils.h"
#include "param.h"
#include "type.h"
#include "exception.h"
#include "qualified_type.h"

class Method{
    friend class Type;
protected:
    std::string name;
    const QualifiedType returnType = nullptr;
    const Type* ownerType = nullptr;
    std::vector<QualifiedType> paramTypes;
    bool isStatic;

    Method(const std::string& name, const Type* ownerType, const QualifiedType returnType, const std::vector<QualifiedType>& paramTypes, bool isStatic = false)
        : returnType(returnType){
        this->name = name;
        this->ownerType = ownerType;
        this->paramTypes = paramTypes;
        this->isStatic = isStatic;
    }

public:
    int GetParamCount() const{
        return paramTypes.size();
    }

    bool IsStatic() const{
        return isStatic;
    }

    const std::vector<QualifiedType> GetParamTypes() const{
        return paramTypes;
    }

    const std::string& GetName() const{
        return name;
    }

    const QualifiedType GetReturnType() const{
        return returnType;
    }

    const Type* GetOwnerType() const{
        return ownerType;
    }

    std::string GetDescription(bool showOwnerType = true) const;

    virtual Any Invoke(Any obj) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with no arguments");
    }

    virtual Any Invoke(Any obj, Any p1) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any obj, Any p1, Any p2) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any obj, Any p1, Any p2, Any p3) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any obj, Any p1, Any p2, Any p3, Any p4) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any obj, Any p1, Any p2, Any p3, Any p4, Any p5) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ", " + p5.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any obj, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ", " + p5.GetType().ToString() + ", " + p6.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any obj, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ", " + p5.GetType().ToString() + ", " + p6.GetType().ToString() + ", " + p7.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any obj, Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ", " + p5.GetType().ToString() + ", " + p6.GetType().ToString() + ", " + p7.GetType().ToString() + ", " + p8.GetType().ToString() + ")");
    }

};

template<class OwnerType, class ReturnType, class... Args>
class MemberMethod : public Method{
private:
    typedef ReturnType(OwnerType::*FUN) (Args...);
    FUN fun;

public:
    MemberMethod(const std::string& name, FUN fun)
        : Method(name, typeof(OwnerType), GetQualifiedType<ReturnType>::Value(), std::vector<QualifiedType>{ GetQualifiedType<Args>::Value()... }){
        this->fun = fun;
    }

    virtual Any Invoke(Any obj, typename AsType<Args, Any>::Value... params) const override{
        if (obj.IsNull()) THROW_EXCEPTION(NullPointerException, "null pointer of '" + ownerType->GetName() + "'");
        return (Any)(((OwnerType*)obj)->*fun)((Args)params...);//ParamCast<Args>(params)...);
    }

};

template<class OwnerType, class... Args>
class MemberMethod<OwnerType, void, Args...> : public Method{
private:
    typedef void (OwnerType::*FUN) (Args...);
    FUN fun;

public:
    MemberMethod(const std::string& name, FUN fun)
        : Method(name, typeof(OwnerType), typeof(void), std::vector<QualifiedType>{ GetQualifiedType<Args>::Value()... }){///* TODO */typename BaseType<Args>::Value)... }){
        this->fun = fun;
    }

    virtual Any Invoke(Any obj, typename AsType<Args, Any>::Value... params) const override{
        if (obj.IsNull()) THROW_EXCEPTION(NullPointerException, "null pointer of '" + ownerType->GetName() + "'");
        (((OwnerType*)obj)->*fun)((Args)params...);//ParamCast<Args>(params)...);
        return nullptr;
    }
};

template<class ReturnType, class... Args>
class StaticMethod : public Method{
private:
    typedef ReturnType(*FUN) (Args...);
    FUN fun;

public:
    StaticMethod(const std::string& name, FUN fun)
        : Method(name, nullptr, GetQualifiedType<ReturnType>::Value(), std::vector<QualifiedType>{ GetQualifiedType<Args>::Value()... }, true){
        this->fun = fun;
    }

    virtual Any Invoke(Any obj, typename AsType<Args, Any>::Value... params) const override{
        return (Any)fun((Args)params...);//ParamCast<Args>(params)...);
    }

};

template<class... Args>
class StaticMethod<void, Args...> : public Method{
private:
    typedef void(*FUN) (Args...);
    FUN fun;

public:
    StaticMethod(const std::string& name, FUN fun)
        : Method(name, nullptr, typeof(void), std::vector< QualifiedType> { GetQualifiedType<Args>::Value()... }, true){
        this->fun = fun;
    }

    virtual Any Invoke(Any obj, typename AsType<Args, Any>::Value... params) const override{
        fun((Args)params...);//ParamCast<Args>(params)...);
        return nullptr;
    }

};

// doing partial specialization here to avoid a bug on Visual Studio which is fixed on Visual Studio 2015
#if defined(_MSC_VER) && _MSC_VER <= 1800

template<class OwnerType, class ReturnType>
class MemberMethod<OwnerType, ReturnType> : public Method{
private:
    typedef ReturnType(OwnerType::*FUN) ();
    FUN fun;

public:
    MemberMethod(const std::string& name, FUN fun)
        : Method(name, typeof(OwnerType), GetQualifiedType<ReturnType>::Value(), {}){
        this->fun = fun;
    }

    virtual Any Invoke(Any obj) const override{
        if (obj.IsNull()) THROW_EXCEPTION(NullPointerException, "null pointer of '" + ownerType->GetName() + "'");
        return Any((((OwnerType*)obj)->*fun)());
    }

};

template<class OwnerType>
class MemberMethod<OwnerType, void> : public Method{
private:
    typedef void (OwnerType::*FUN) ();
    FUN fun;

public:
    MemberMethod(const std::string& name, FUN fun)
        : Method(name, typeof(OwnerType), typeof(void), {}){
        this->fun = fun;
    }

    virtual Any Invoke(Any obj) const override{
        if (obj.IsNull()) THROW_EXCEPTION(NullPointerException, "null pointer of '" + ownerType->GetName() + "'");
        (((OwnerType*)obj)->*fun)();
        return nullptr;
    }
};

template<class ReturnType>
class StaticMethod<ReturnType> : public Method{
private:
    typedef ReturnType(*FUN) ();
    FUN fun;

public:
    StaticMethod(const std::string& name, FUN fun)
        : Method(name, nullptr, GetQualifiedType<ReturnType>::Value(), {}, true){
        this->fun = fun;
    }

    virtual Any Invoke(Any obj) const override{
        return (Any)fun();
    }

};

template<>
class StaticMethod<void> : public Method{
private:
    typedef void(*FUN) ();
    FUN fun;

public:
    StaticMethod(const std::string& name, FUN fun)
        : Method(name, nullptr, typeof(void), {}, true){
        this->fun = fun;
    }

    virtual Any Invoke(Any obj) const override{
        fun();
        return nullptr;
    }

};

#endif // defined(_MSC_VER) && _MSC_VER <= 1800
