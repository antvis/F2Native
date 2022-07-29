#include "enum.h"
#include "type.h"
#include "string_helper.h"
#include "exception.h"

const Type* Enum::GetEnum(const std::string& enumTypeName, bool throwIfNotFound){
    const Type* enumType = Type::GetType(enumTypeName);

    if (throwIfNotFound){
        if (enumType == nullptr){
            THROW_EXCEPTION(EnumNotFound, "enum '" + enumTypeName + "' not found");
        }
        else if (!enumType->IsEnum()){
            THROW_EXCEPTION(NotAnEnum, "'" + enumTypeName + "' is not an enum type");
        }
    }
    
    if (enumType != nullptr && !enumType->IsEnum()){
        return nullptr;
    }

    return enumType;
}

int64_t Enum::GetValue(const Type* enumType, const std::string& enumName){
    if (!enumType->IsEnum())
        THROW_EXCEPTION(NotAnEnum, "'" + enumType->GetName() + "' is not an enum type");

    auto& map = Enums()[enumType];
    if (map.find(enumName) == map.end())
        THROW_EXCEPTION(EnumNameNotFound, "enum '" + enumType->GetName() + "' has no member named '" + enumName + "'");

    return map[enumName];
}

std::string Enum::GetName(const Type* enumType, int64_t value){
    if (!enumType->IsEnum())
        THROW_EXCEPTION(NotAnEnum, "'" + enumType->GetName() + "' is not an enum type");

    auto& map = Enums2()[enumType];
    if (map.find(value) == map.end())
        THROW_EXCEPTION(EnumValueNotFound, "enum '" + enumType->GetName() + "' has no member valued '" + std::to_string(value) + "'");

    return map[value];
}

std::vector<int64_t> Enum::GetValues(const Type* enumType){
    if (!enumType->IsEnum())
        THROW_EXCEPTION(NotAnEnum, "'" + enumType->GetName() + "' is not an enum type");

    std::vector<int64_t> values;
    for (auto& i : Enums()[enumType]){
        values.push_back(i.second);
    }

    return values;
}

std::vector<std::string> Enum::GetNames(const Type* enumType){
    if (!enumType->IsEnum())
        THROW_EXCEPTION(NotAnEnum, "'" + enumType->GetName() + "' is not an enum type");

    std::vector<std::string> names;
    for (auto& i : Enums()[enumType]){
        names.push_back(i.first);
    }
    return names;
}

const Type* Enum::Register(const std::string& name, const std::string& values, const Type* underlyingType){
    auto items = StringHelper::Split(StringHelper::RemoveSpaces(values), ',');
    std::unordered_map<std::string, int64_t> map;
    const Type* type = Type::RegisterEnum(new Type, name, underlyingType);
    int num = 0;
    for (auto& i : items){
        if ((int)i.find('=') >= 0){
            auto kv = StringHelper::Split(i, '=');
            num = atoi(kv[1].c_str());
            map.insert(std::make_pair(kv[0], num));
        }
        else{
            map.insert(std::make_pair(i, num));
        }
        num++;
    }

    std::unordered_map<int64_t, std::string> map2;
    for (auto& i : map){
        map2.insert(std::make_pair(i.second, i.first));
    }
    Enums().insert(std::make_pair(type, map));
    Enums2().insert(std::make_pair(type, map2));

    return type;
}
