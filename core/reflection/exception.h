#pragma once

#include <string>

#define THROW_EXCEPTION(name, msg) { name ex; ex.What() = msg; ex.Where() = std::string(__FILE__ " (" + std::to_string(__LINE__) + ")"); throw ex; }

class Exception {
protected:
    std::string what;
    std::string where;

public:
    const std::string& What() const{
        return what;
    }

    const std::string& Where() const{
        return where;
    }

    std::string& What(){
        return what;
    }

    std::string& Where(){
        return where;
    }
};

class NotAnEnum : public Exception{};           // throw while treating a normal type as an enum type
class EnumNotFound : public Exception{};
class EnumNameNotFound : public Exception{};
class EnumValueNotFound : public Exception{};
class TypeNotFound : public Exception{};
class ConstructorNotFound : public Exception{};
class FieldNotFound : public Exception{};
class MethodNotFound : public Exception{};
class InvalidArguments : public Exception{};
class InvalidAccess : public Exception{};
class InternalError : public Exception{};
class InvalidCast : public Exception{};
class InvalidOperation : public Exception{};
class NullPointerException : public Exception{};
class AmbiguousMatched : public Exception{};
class TypeAlreadyExists : public Exception{};
class NoDefualtConstructor : public Exception{};
