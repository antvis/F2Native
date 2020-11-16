cmake_minimum_required(VERSION 3.4.1)

set(PROJECT_NAME "graphics")

project(${PROJECT_NAME})

#增加覆盖率宏
if(${BUILD_TARGET} STREQUAL "TESTS")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} --coverage")
endif()


#找出所有的.h文件
file(GLOB_RECURSE DIR_SRC_INCS ./*.h)

aux_source_directory(./ DIR_SRC_GRAPHICS)
aux_source_directory(./scale DIR_SRC_GRAPHICS_SCALE)
aux_source_directory(./scale/continuous DIR_SRC_GRAPHICS_SCALE_CONTINUOUS)
aux_source_directory(./shape DIR_SRC_GRAPHICS_SHAPE)
aux_source_directory(./util DIR_SRC_GRAPHICS_UTIL)
aux_source_directory(./axis DIR_SRC_GRAPHICS_AXIS)
aux_source_directory(./canvas DIR_SRC_GRAPHICS_CANVAS)
aux_source_directory(./geom DIR_SRC_GRAPHICS_GEOM)
aux_source_directory(./geom/attr DIR_SRC_GRAPHICS_GEOM_ATTR)
aux_source_directory(./geom/shape DIR_SRC_GRAPHICS_GEOM_SHAPE)
aux_source_directory(./guide DIR_SRC_GRAPHICS_GUIDE)
aux_source_directory(./adjust DIR_SRC_GRAPHICS_ADJUST)
aux_source_directory(./event DIR_SRC_GRAPHICS_EVENT)
aux_source_directory(./tooltip DIR_SRC_GRAPHICS_TOOLTIP)

list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_SCALE})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_SCALE_CONTINUOUS})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_SHAPE})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_UTIL})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_AXIS})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_CANVAS})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_GEOM_ATTR})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_GEOM_SHAPE})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_GEOM})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_GUIDE})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_ADJUST})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_EVENT})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS_TOOLTIP})


add_library(${PROJECT_NAME} 
            STATIC 
            ${DIR_SRC}
            ${DIR_SRC_INCS})

include(${PROJECT_SOURCE_DIR}/../../cmake/ios.toolchain.cmake) #set_xcode_property
set_xcode_property(${PROJECT_NAME} FRAMEWORK_SEARCH_PATHS ${PROJECT_SOURCE_DIR}/../../lib/gcanvas "All")
include_directories(${PROJECT_SOURCE_DIR}/..)
include_directories(${PROJECT_SOURCE_DIR}/../../src)
include_directories(${PROJECT_SOURCE_DIR}/../../lib)
include_directories(${PROJECT_SOURCE_DIR}/../../lib/gcanvas)
include_directories(${PROJECT_SOURCE_DIR}/../../lib/gcanvas/GCanvas4Alipay/)
include_directories(${PROJECT_SOURCE_DIR}/../../lib/gcanvas/GCanvas4Alipay/gcanvas)
include_directories(${PROJECT_SOURCE_DIR}/../../lib/gcanvas/GCanvas4Alipay/gcanvas/GL)
include_directories(${PROJECT_SOURCE_DIR}/../../lib/gcanvas/GCanvas4Alipay/support)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC "YES")
