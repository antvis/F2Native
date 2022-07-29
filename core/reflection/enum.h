#pragma once

#include "utils.h"
#include "type.h"
#include "constructor.h"

class Enum{
private:
    static std::unordered_map<const Type*, std::unordered_map<std::string, int64_t>>& Enums(){
        static std::unordered_map<const Type*, std::unordered_map<std::string, int64_t>> enums;
        return enums;
    }

    static std::unordered_map<const Type*, std::unordered_map<int64_t, std::string>>& Enums2(){
        static std::unordered_map<const Type*, std::unordered_map<int64_t, std::string>> enums;
        return enums;
    }

public:
    static int64_t GetValue(const Type* enumType, const std::string& enumName);

    static std::string GetName(const Type* enumType, int64_t value);

    static const Type* GetEnum(const std::string& enumTypeName, bool throwIfNotFound = false);

    static std::vector<int64_t> GetValues(const Type* enumType);

    static std::vector<std::string> GetNames(const Type* enumType);

    template<class EnumClass>
    static EnumClass GetValue(const std::string& enumName){
        return (EnumClass)GetValue(typeof(EnumClass), enumName);
    }

    template<class EnumClass>
    static std::string GetName(EnumClass enumValue){
        return GetName(typeof(EnumClass), (int64_t)enumValue);
    }
    
    template<class EnumClass>
    static std::vector<EnumClass> GetValues(){
        std::vector<EnumClass> ret;
        for (auto i : GetValues(typeof(EnumClass)))
            ret.push_back((EnumClass)i);
        return ret;
    }

    template<class EnumClass>
    static std::vector<std::string> GetNames(){
        return GetNames(typeof(EnumClass));
    }

    static const Type* Register(const std::string& name, const std::string& values, const Type* underlyingType);
};

// macros to define a reflectable enum
// define an enum like this:
//
//      REFLECT_ENUM(Color,
//               Red,
//               Green,
//               Blue
//      )
//
// and then you can use it like this:
//
//      std::string name = Enum::GetName(Color::Red);
//      Color color = Enum::GetValue<Color>(name);
//
// or you can list all the items in the enum:
//
//      for (auto i : Enum::GetValues<Color>()){
//          std::cout << Enum::GetName(i) << " = " << (int)i << std::endl;
//      }
//

#define REFLECT_ENUM(typeName, ...) enum class typeName{ __VA_ARGS__ }; \
struct Enum_##typeName{ \
    static const Type* GetType(){\
        static const Type* type = Enum::Register(#typeName, #__VA_ARGS__, typeof(std::underlying_type<typeName>::type)); \
        return type; \
    } \
}; \
template<> \
struct ReflectType<typeName>{ \
    static const Type* Value(){ \
        return Enum_##typeName::GetType(); \
    } \
}; \
static const Type* ____##typeName##_dummy = typeof(typeName);
