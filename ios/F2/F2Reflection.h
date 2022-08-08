//
//  F2Reflection.hpp
//  F2
//
//  Created by weiqing.twq on 8/8/22.
//

#pragma once
#import <Foundation/Foundation.h>
#import "../../core/reflection/type.h"
#import "../../core/reflection/param.h"

class F2Reflection {
public:
    static Any CreateArray(NSArray *array, const Type *type);
    static std::vector<std::string> CreateStringArray(NSArray *array, const Type *type);
    static std::vector<float> CreateNumberArray(NSArray *array, const Type *type);
    static Any CreateStruct(NSDictionary *config, const Type *type);
};

