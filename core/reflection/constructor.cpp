#include "constructor.h"
#include "type.h"

std::string Constructor::GetDescription() const{
    std::stringstream ss;
    ss << type->GetName() << "(";
    for (int i = 0, n = paramTypes.size(); i<n; i++){
        if (i != 0) ss << ", ";
        ss << paramTypes[i].ToString();
    }
    ss << ")";
    return ss.str();
}
