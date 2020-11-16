cmake_minimum_required(VERSION 3.4.1)

set(PROJECT_NAME "utils")
project(${PROJECT_NAME})

#增加覆盖率宏
if(${BUILD_TARGET} STREQUAL "TESTS")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} --coverage")
endif()

file(GLOB_RECURSE DIR_SRC_INCS ./*.h)

aux_source_directory(./ DIR_SRC_UTILS)

#把.h也加进来 xcode里面才会展示
add_library(${PROJECT_NAME} 
STATIC 
${DIR_SRC_INCS}
${DIR_SRC_UTILS}
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES")
