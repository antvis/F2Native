#pragma once

#include "utils.h"
#include "exception.h"
#include "qualified_type.h"
#include "type.h"

enum class TypeKind{
    Fundamental,
    Enum,
    Class,
    Pointer,
    AnyPointer,
    Reference,
    RightValueReference
};

template<class T, bool isEnum = false>
struct GetTypeKind{
    static const TypeKind Value = TypeKind::Class;
};

template<>
struct GetTypeKind<void*, false>{
    static const TypeKind Value = TypeKind::AnyPointer;
};

template<class T>
struct GetTypeKind<T, true>{
    static const TypeKind Value = TypeKind::Enum;
};

template<class T>
struct GetTypeKind<T&, false>{
    static const TypeKind Value = TypeKind::Reference;
};

template<class T>
struct GetTypeKind<T*, false>{
    static const TypeKind Value = TypeKind::Pointer;
};

template<class T>
struct GetTypeKind<T&&, false>{
    static const TypeKind Value = TypeKind::RightValueReference;
};

#define GET_TYPE_KIND(T) \
template<> \
struct GetTypeKind<T, false>{ \
    static const TypeKind Value = TypeKind::Fundamental; \
};

GET_TYPE_KIND(int8_t);
GET_TYPE_KIND(int16_t);
GET_TYPE_KIND(int32_t);
GET_TYPE_KIND(int64_t);
GET_TYPE_KIND(uint8_t);
GET_TYPE_KIND(uint16_t);
GET_TYPE_KIND(uint32_t);
GET_TYPE_KIND(uint64_t);
GET_TYPE_KIND(bool);
GET_TYPE_KIND(float);
GET_TYPE_KIND(double);
GET_TYPE_KIND(signed long);
GET_TYPE_KIND(unsigned long);
GET_TYPE_KIND(wchar_t);

#undef GET_TYPE_KIND

#define kindof(T) GetTypeKind<typename std::decay<T>::type, std::is_enum<T>::value>::Value

class Value{
protected:
    QualifiedType type;
public:
    virtual ~Value(){}
    virtual Value* Clone() const = 0;
    virtual Value* operator*() const{
        THROW_EXCEPTION(InvalidOperation, "can not use * operator on a non-pointer object '" + type.ToString() + "'");
    }
    virtual void* AddressOf() const{
        THROW_EXCEPTION(InvalidOperation, "umimplemented addressof operation on class '" + type.ToString() + "'");
    }
    const QualifiedType& GetType() const{
        return type;
    }
};

template<class T, TypeKind kind>
class Holder : public Value{
private:
    friend class Any;
    T data;
    
public:
    Holder(const T& data) : data(data) { type = qualified_typeof(T); }
    
    virtual Value* Clone() const override{
        return new Holder<T, kind>(data);
    }
    
    virtual void* AddressOf() const override{
        return (void*)&data;//new _Holder<T*>((T*)&data);
    }
};

template<class T>
using _Holder = Holder < T, kindof(T) > ;

template<>
class Holder<void*, TypeKind::AnyPointer> : public Value{
private:
    friend class Any;
    void* data;
    
public:
    Holder(void* data) : data(data) { type = qualified_typeof(void*); }
    
    virtual Value* Clone() const override{
        return new Holder<void*, TypeKind::AnyPointer>(data);
    }
    
    virtual Value* operator*() const override{
        THROW_EXCEPTION(InvalidOperation, "can not use * operator on 'void*'");
    }
    
    /*virtual Value* AddressOf() const override{
     return new _Holder<void*>((void*)&data);
     }*/
};

template<class T>
class Holder<T*, TypeKind::Pointer> : public Value{
private:
    friend class Any;
    T* data;
    
public:
    Holder(T* data) : data(data) { type = qualified_typeof(T*); }
    
    virtual Value* Clone() const override{
        return new Holder<T*, TypeKind::Pointer>(data);
    }
    
    virtual Value* operator*() const override{
        return new Holder<T, kindof(T)>(*data);
    }
    
    /*virtual Value* AddressOf() const override{
     return new _Holder<T*>((T*)&data);
     }*/
};

template<class T>
class Holder<T*&, TypeKind::Reference> : public Value{
private:
    friend class Any;
    T*& data;
    
public:
    Holder(const T*& data) : data(data) { type = qualified_typeof(T*&); }
    
    virtual Value* Clone() const override{
        return new Holder<T*&, TypeKind::Reference>((T*&)data);
    }
    
    virtual Value* operator*() const override{
        return new Holder<T&, TypeKind::Reference>((T&)*data);
    }
    
    /*virtual Value* AddressOf() const override{
     return new _Holder<T*&>((T*&)&data);
     }*/
};

//template<class T>
//class Holder<T, TypeKind::Enum> : Value{
//private:
//    friend class Any;
//    int64_t data;
//public:
//    Holder(const T& data) : data((int64_t)data) { type = qualified_typeof(T); }

//    virtual Value* Clone() const override{
//        return new Holder<T, TypeKind::Enum>((T)data);
//    }

//    /*virtual Value* AddressOf() const override{
//        return new _Holder<T*>(&data);
//    }*/
//};

class Any{
private:
    Value* value = nullptr;

    Any(Value* value) : value(value){ }

public:
    static const Type* StaticType(){
        static const Type* type = nullptr;
        if (type == nullptr){
            type = new Type();
            type = Type::RegisterType(const_cast<Type*>(typeof(Any)), "Any", nullptr);
        }
        return type;
    }
    
    /*Any(const Any& any) : value(any.value ? any.value->Clone() : nullptr){

    }*/

    /*Any(Any&& any){
        std::swap(value, any.value);
    }*/
    Any():value(nullptr) {};
    Any(const Any& any) : value(any.value ? any.value->Clone() : nullptr){
        
    }

    Any(Any& any) : value(any.value ? any.value->Clone() : nullptr){

    }

    //template<class T>
    //struct UseRef{
    //    static const bool Value = !std::is_fundamental<std::decay<T>::type>::value && !std::is_pointer<std::decay<T>::type>::value;
    //};

    template<class T>
    Any(T* value){
        this->value = new _Holder<typename std::decay<T*>::type>(value);
    }

    /*template<class T>
    Any(std::shared_ptr<T> value){
        this->value = new _Holder<typename std::decay<T*>::type>(value.get());
    }*/

    template<class T, class = typename std::enable_if<!std::is_pointer<T>::value>::type>
    Any(const T& value){
        this->value = new _Holder<typename std::decay<T>::type>(value);
    }

    template<class T>
    Any(T& value){
        //this->value = new _Holder<typename std::conditional<std::is_fundamental<T>::value || std::is_enum<T>::value || std::is_pointer<typename std::decay<T>::type>::value, typename std::decay<T>::type, typename std::decay<T>::type&>::type>(value);
        this->value = new _Holder<typename std::decay<T>::type&>(value);
    }

    ~Any(){
        delete value;
        value = nullptr;
    }

    bool IsEmpty() const{
        return !value;
    }

    bool IsNull() const{
        return GetType().IsPointer() && static_cast<_Holder<void*>*>(value)->data == nullptr;
    }

    inline const QualifiedType& GetType() const{
        return value->GetType();
    }
    
    inline bool IsArray() const {
        return GetType().IsArray();
    }
    
    inline bool IsFloatingNumber() const {
        return GetType().IsFloatingNumber();
    }
    
    inline bool IsNumber() const {
        return GetType().IsNumber();
    }
    
    inline bool IsString() const {
        return GetType().IsString();
    }

    Any& Swap(Any& any){
        std::swap(value, any.value);
        return *this;
    }

    template<class T>
    Any& operator = (const T& value){
        Any any(value);
        return Swap(any);
    }

    Any& operator = (Any any){
        return Swap(any);
    }

    Any operator*() const{
        return Any(**value);
    }

    /*Any& operator = (Any&& any){
        return Swap(any);
    }*/

    void* Cast() const{
        if (IsEmpty())
            THROW_EXCEPTION(InvalidCast, "invalid cast from an empty object to 'void*'");
        
        if (!GetType().IsPointer())
            return value->AddressOf();
            //THROW_EXCEPTION(InvalidCast, "invalid cast from '" + GetType().ToString() + "' to 'void*'");

        return static_cast<_Holder<void*>*>(value)->data;
    }

    template<class T>
    T Cast() const{
        if (IsEmpty())
            THROW_EXCEPTION(InvalidCast, "invalid cast from an empty object to '" + qualified_typeof(T).ToString() + "'");

        auto type = GetType();

        if (std::is_pointer<T>::value && type == qualified_typeof(void*)) return (T) static_cast<_Holder<T>*>(value)->data;
        if (std::is_pointer<T>::value && type == qualified_typeof(void*&)) return (T) static_cast<_Holder<T&>*>(value)->data;

        QualifiedType t = qualified_typeof(T);
        if (t == GetType()) return (T) static_cast<_Holder<T>*>(value)->data;

        if (std::is_enum<T>::value && type == typeof(int64_t))
        {
            if (t.IsReference())
                return (T) static_cast<_Holder<typename std::conditional<std::is_enum<T>::value, int64_t, T>::type&>*>(value)->data;
            else
                return (T) static_cast<_Holder<typename std::conditional<std::is_enum<T>::value, int64_t, T>::type>*>(value)->data;
        }

        if (t.GetType() == type.GetType()){
            switch (t.PointerCount())
            {
            case 0:
                switch (type.PointerCount())
                {
                case 1:
                    return *static_cast<_Holder<typename std::remove_reference<T>::type*>*>(value)->data;
                }
            //case 1:
            //    switch (GetType().PointerCount())
            //    {
            //    case 0:
            //        return &static_cast<Holder<typename std::remove_pointer<T>::type>*>(value)->data;
            //    }
            }
        }

        /*if (t.IsPointer() && type.GetType()->IsTemplate()){
            TemplatedType* tt = (TemplatedType*)type.GetType();
            if (tt->GetTemplateType() == template_typeof(std::shared_ptr)){
                if (tt->MatchTemplates({ t.RemovePointer().GetType() })){
                    return static_cast<_Holder<std::shared_ptr<std::remove_pointer<T>::type>>*>(value)->data.get();
                }
            }
        }*/

        // T& -> T          ok
        // const T& -> T    ok
        // const T -> T     ok
        // T -> const T&    ok
        // T -> const T     ok
        // T -> T&          error
        if (t.GetType() == type.GetType() && t.PointerCount() == type.PointerCount()){
            if (type.IsLValueReference())
                return (T)static_cast<_Holder<typename std::remove_reference<T>::type&>*>(value)->data;
            else if (!std::is_reference<T>::value || std::is_const<T>::value)
                return (T)static_cast<_Holder<typename std::remove_reference<T>::type>*>(value)->data;
        }

        THROW_EXCEPTION(InvalidCast, "invalid cast from '" + type.ToString() + "' to '" + qualified_typeof(T).ToString() + "'");
    }

    operator void*() const{
        return Cast();
    }

    template<class T>
    operator T() const{
        return Cast<T>();
    }
    
    std::size_t hash() const {
        if(GetType().IsNumber()) {
            return std::hash<double>{}(static_cast<_Holder<double>*>(value)->data);
        } else if(GetType().IsString()) {
            return std::hash<std::string>{}(static_cast<_Holder<std::string>*>(value)->data);
        }
        return 0;
    }
    
    bool IsEqual(const Any &other) const {
        if (GetType() != other.GetType()) {
            return false;
        }
        
        if (GetType().IsNumber()) {
            return static_cast<_Holder<double>*>(value)->data == static_cast<_Holder<double>*>(other.value)->data;
        } else if (GetType().IsString()) {
            return static_cast<_Holder<std::string>*>(value)->data == static_cast<_Holder<std::string>*>(other.value)->data;
        } else {
            return false;
        }
    }

    std::string ToString();
};

#define CAST_(from, to) if (t == typeof(from)) return GetType().IsReference() ? (to)static_cast<_Holder<from&>*>(value)->data : (to)static_cast<_Holder<from>*>(value)->data;
#define CAST_ENUM(from, to) if (t->GetUnderlyingType() == typeof(from)) return GetType().IsReference() ? (to)static_cast<_Holder<from&>*>(value)->data : (to)static_cast<_Holder<from>*>(value)->data;
#define DEF_CAST(to) \
template<> \
inline to Any::Cast<to>() const{ \
    if(IsEmpty()) \
        THROW_EXCEPTION(InvalidCast, "invalid cast from an empty object to '" + qualified_typeof(to).ToString() + "'"); \
    if (!GetType().IsPointer()){ \
        auto t = GetType().GetType(); \
        CAST_(int8_t, to); \
        CAST_(int16_t, to); \
        CAST_(int32_t, to); \
        CAST_(int64_t, to); \
        CAST_(uint8_t, to); \
        CAST_(uint16_t, to); \
        CAST_(uint32_t, to); \
        CAST_(uint64_t, to); \
        CAST_(float, to); \
        CAST_(double, to); \
        \
        if (t->IsEnum()){ \
            CAST_ENUM(int8_t, to); \
            CAST_ENUM(int16_t, to); \
            CAST_ENUM(int32_t, to); \
            CAST_ENUM(int64_t, to); \
            CAST_ENUM(uint8_t, to); \
            CAST_ENUM(uint16_t, to); \
            CAST_ENUM(uint32_t, to); \
            CAST_ENUM(uint64_t, to); \
        } \
    } \
    THROW_EXCEPTION(InvalidCast, "invalid cast from '" + GetType().ToString() + "' to '" + qualified_typeof(to).ToString() + "'"); \
}

/*template<>
 int64_t Cast<int64_t>() const{
 if (IsEmpty())
 THROW_EXCEPTION(InvalidCast, "invalid cast from an empty object to '" + qualified_typeof(int64_t).ToString() + "'");
 if (!GetType().IsPointer()){
 
 auto t = GetType().GetType();
 CAST_(int8_t, int64_t);
 CAST_(int16_t, int64_t);
 CAST_(int32_t, int64_t);
 CAST_(int64_t, int64_t);
 CAST_(uint8_t, int64_t);
 CAST_(uint16_t, int64_t);
 CAST_(uint32_t, int64_t);
 CAST_(uint64_t, int64_t);
 CAST_(float, int64_t);
 CAST_(double, int64_t);
 
 if (GetType().IsEnum())
 return GetType().IsReference() ? (int64_t) static_cast<_Holder<int64_t&>*>(value)->data : (int64_t) static_cast<_Holder<int64_t>*>(value)->data;
 
 }
 THROW_EXCEPTION(InvalidCast, "invalid cast from '" + GetType().ToString() + "' to '" + qualified_typeof(int64_t).ToString() + "'");
 }*/

DEF_CAST(int8_t);
DEF_CAST(int16_t);
DEF_CAST(int32_t);
DEF_CAST(int64_t);
DEF_CAST(uint8_t);
DEF_CAST(uint16_t);
DEF_CAST(uint32_t);
DEF_CAST(uint64_t);
DEF_CAST(float);
DEF_CAST(double);
#undef CAST_
#undef DEF_CAST
