//
// Created by weiqing.twq on 8/29/22.
//

#include "JNIReflection.h"

using namespace xg;

Any JNIReflection::CreateArray(json *array, const Type *type) {

}

std::vector<std::string> JNIReflection::CreateStringArray(json *array, const Type *type) {
    std::vector<std::string> rst;
    return rst;
}

std::vector<std::vector<long long>> JNIReflection::CreateTimeRangeArray(json *array, const Type *type) {
    std::vector<std::vector<long long>> rst;
    return rst;
}
std::vector<float> JNIReflection::CreateNumberArray(json *array, const Type *type) {
    std::vector<float> rst;
    return rst;
}

Any JNIReflection::CreateStruct(const std::string &config, const Type *type) {
    Any rst;
    return rst;
}

util::XSourceItem JNIReflection::CreateaSourceItem(json *data) {
    util::XSourceItem item
    return item;
}

std::unordered_map<std::string, Any> JNIReflection::CreateMap(json *config) {
    std::unordered_map<std::string, Any> rst;
    return rst;
}

event::Event JNIReflection::CreateTouchEvent(const std::string &config) {
    event::Event event;
    return event;
}
