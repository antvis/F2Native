#pragma once

#include <cstdint>
#include <cstdlib>

#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include <type_traits>

// to support std::to_string on mingw32
#ifdef __MINGW32__

#include <sstream>

namespace std{
    template<typename T>
    string to_string(T t){
        stringstream ss;
        ss << t;
        return ss.str();
    }
}

#endif

template<class A, class B>
struct AsType{
    typedef B Value;
};

class Type;

struct Null{
    static const Type* GetType();
};

template<class T>
struct ReflectType{
    static const Type* Value(){
        //return Null::GetType();
        return T::StaticType();
    }
};

template<>
struct ReflectType<std::nullptr_t>{
    static const Type* Value(){
        return nullptr;
    }
};

#define typeof(TYPE) ReflectType<TYPE>::Value()

template<template<class...>class C>
struct TemplateType;

#define template_typeof(TYPE) TemplateType<TYPE>::Value()
