#include "param.h"
#include "enum.h"

std::string Any::ToString(){
    if (GetType() == qualified_typeof(const char*)) return static_cast<_Holder<const char*>*>(value)->data;
    auto t = GetType().RemoveCV();
#define TOSTR(T) if(t == qualified_typeof(T)) return std::to_string(static_cast<_Holder<T>*>(value)->data); \
                 if(t == qualified_typeof(T&)) return std::to_string(static_cast<_Holder<T&>*>(value)->data);
    TOSTR(int8_t);
    TOSTR(int16_t);
    TOSTR(int32_t);
    TOSTR(int64_t);
    TOSTR(uint8_t);
    TOSTR(uint16_t);
    TOSTR(uint32_t);
    TOSTR(uint64_t);
    TOSTR(bool);
    TOSTR(float);
    TOSTR(double);
#undef TOSTR
    if (t == qualified_typeof(char*)) return static_cast<_Holder<const char*>*>(value)->data;
    if (t == qualified_typeof(std::string)) return static_cast<_Holder<std::string>*>(value)->data;
    if (t == qualified_typeof(std::string&)) return static_cast<_Holder<std::string&>*>(value)->data;
    if (t.GetType()->IsEnum()) return Enum::GetName(t.GetType(), static_cast<_Holder<int64_t>*>(value)->data);

    return "(unknown)";
}
