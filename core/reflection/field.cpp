#include "field.h"
#include "type.h"

std::string Field::GetDescription(bool showOwnerType) const{
    std::stringstream ss;
    if (isStatic) ss << "static ";
    ss << fieldType.ToString() << " ";
    if (showOwnerType && ownerType) ss << ownerType->GetName() << "::";
    ss << name;
    return ss.str();
}
