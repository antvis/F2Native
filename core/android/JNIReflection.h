//
// Created by weiqing.twq on 8/29/22.
//

#ifndef F2NATIVE_JNIREFLECTION_H
#define F2NATIVE_JNIREFLECTION_H

#include "reflection/type.h"
#include "reflection/param.h"
#include "graphics/util/json_data.h"
#include "nlohmann/json.hpp"

using namespace nlohmann;

class JNIReflection {
public:
    static Any CreateArray(json *array, const Type *type);
    static std::vector<std::string> CreateStringArray(json *array, const Type *type);
    static std::vector<std::vector<long long>> CreateTimeRangeArray(json *array, const Type *type);
    static std::vector<float> CreateNumberArray(json *array, const Type *type);
    static Any CreateStruct(const std::string &config, const Type *type);
    static xg::util::XSourceItem CreateaSourceItem(json *data);
    static std::unordered_map<std::string, Any> CreateMap(json *config);
    static xg::event::Event CreateTouchEvent(const std::string &config);
};


#endif //F2NATIVE_JNIREFLECTION_H
