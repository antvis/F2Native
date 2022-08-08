#include "method.h"
#include "type.h"

std::string Method::GetDescription(bool showOwnerType) const{
    std::stringstream ss;
    if (isStatic) ss << "static ";
    ss << returnType.ToString() << " ";
    if (showOwnerType && ownerType) ss << ownerType->GetName() << "::";
    ss << name << "(";
    for (size_t i = 0, n = paramTypes.size(); i<n; i++){
        if (i != 0) ss << ", ";
        ss << paramTypes[i].ToString();
    }
    ss << ")";
    return ss.str();
}
