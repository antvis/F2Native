#pragma once

#include "utils.h"
#include "qualified_type.h"
#include "param.h"

class Type;

class Constructor{
    friend class Type;
protected:
    const Type* type;
    std::vector<QualifiedType> paramTypes;
    
    Constructor(const Type* type, const std::vector<QualifiedType>& paramTypes){
        this->type = type;
        this->paramTypes = paramTypes;
    }

public:
    int GetParamCount() const{
        return paramTypes.size();
    }

    const std::vector<QualifiedType> GetParamTypes() const{
        return paramTypes;
    }

    const Type* GetType() const{
        return type;
    }

    std::string GetDescription() const;

    virtual Any Invoke() const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with no arguments");
    }

    virtual Any Invoke(Any p1) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any p1, Any p2) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any p1, Any p2, Any p3) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any p1, Any p2, Any p3, Any p4) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any p1, Any p2, Any p3, Any p4, Any p5) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ", " + p5.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any p1, Any p2, Any p3, Any p4, Any p5, Any p6) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ", " + p5.GetType().ToString() + ", " + p6.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ", " + p5.GetType().ToString() + ", " + p6.GetType().ToString() + ", " + p7.GetType().ToString() + ")");
    }

    virtual Any Invoke(Any p1, Any p2, Any p3, Any p4, Any p5, Any p6, Any p7, Any p8) const{
        THROW_EXCEPTION(InvalidArguments, "tried to invoke constructor '" + GetDescription() + "' with arguments(" + p1.GetType().ToString() + ", " + p2.GetType().ToString() + ", " + p3.GetType().ToString() + ", " + p4.GetType().ToString() + ", " + p5.GetType().ToString() + ", " + p6.GetType().ToString() + ", " + p7.GetType().ToString() + ", " + p8.GetType().ToString() + ")");
    }
};

template<class T, class... Args>
class ConstructorImpl : public Constructor{
private:
    const Type* type;

public:
    ConstructorImpl()
        : Constructor(typeof(T), { GetQualifiedType<Args>::Value() ... }){
        static_assert(std::is_constructible<T, Args...>::value, "tried to register an undeclared constructor");
    }

    virtual Any Invoke(typename AsType<Args, Any>::Value... params) const override{
        return (Any)new T(std::forward<Args>((Args)params)...);
    }
};

// doing partial specialization here to avoid a bug on Visual Studio which is fixed on Visual Studio 2015
#if defined(_MSC_VER) && _MSC_VER <= 1800

template<class T>
class ConstructorImpl<T> : public Constructor{
private:
    const Type* type;

public:
    ConstructorImpl()
        : Constructor(typeof(T), { }){
        static_assert(std::is_constructible<T>::value, "tried to register an undeclared constructor");
    }

    virtual Any Invoke() const override{
        return (Any)new T();
    }
};

#endif
