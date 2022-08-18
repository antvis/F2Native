cmake_minimum_required(VERSION 3.4.1)

set(PROJECT_NAME "f2wasm")

project(${PROJECT_NAME})


add_definitions(-DWASI)

SET(CMAKE_C_COMPILER  /Users/weiqing.twq/Documents/wasi-sdk-16.0/bin/clang)
SET(CMAKE_CXX_COMPILER  /Users/weiqing.twq/Documents/wasi-sdk-16.0/bin/clang++)
set(CMAKE_SYSROOT /Users/weiqing.twq/Documents/wasi-sdk-16.0/share/wasi-sysroot)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -fno-exceptions -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --target=wasm32-wasi -Wall -Wextra -fno-exceptions")

include_directories(../core)

# 添加 core/utils 下所有源文件
aux_source_directory(../core/utils DIR_SRC_UTILS)

# 添加core/reflection 下所有源文件
aux_source_directory(../core/reflection/ DIR_SRC_REFLECTION)

# 添加core/webassembly 下所有源文件
aux_source_directory(../core/wasi/ DIR_SRC_WEBASSEMBLY)

# 添加core/graphics 下所有源文件
aux_source_directory(../core/graphics DIR_SRC_GRAPHICS)
aux_source_directory(../core/graphics/axis DIR_SRC_GRAPHICS_AXIS)
aux_source_directory(../core/graphics/canvas DIR_SRC_GRAPHICS_CANVAS)
aux_source_directory(../core/graphics/geom DIR_SRC_GRAPHICS_GEOM)
aux_source_directory(../core/graphics/geom/attr DIR_SRC_GRAPHICS_GEOM_ATTR)
aux_source_directory(../core/graphics/geom/shape DIR_SRC_GRAPHICS_GEOM_SHAPE)
aux_source_directory(../core/graphics/guide DIR_SRC_GRAPHICS_GUIDE)
aux_source_directory(../core/graphics/scale DIR_SRC_GRAPHICS_SCALE)
aux_source_directory(../core/graphics/scale/continuous DIR_SRC_GRAPHICS_SCALE_CONTINUOUS)
aux_source_directory(../core/graphics/shape DIR_SRC_GRAPHICS_SHAPE)
aux_source_directory(../core/graphics/util DIR_SRC_GRAPHICS_UTIL)
aux_source_directory(../core/graphics/adjust DIR_SRC_GRAPHICS_ADJUST)
aux_source_directory(../core/graphics/event DIR_SRC_GRAPHICS_EVENT)
aux_source_directory(../core/graphics/tooltip DIR_SRC_GRAPHICS_TOOLTIP)
aux_source_directory(../core/graphics/interaction DIR_SRC_GRAPHICS_INTERACTION)
aux_source_directory(../core/graphics/legend DIR_SRC_GRAPHICS_LEGEND)
aux_source_directory(../core/graphics/func DIR_SRC_GRAPHICS_FUNC)
aux_source_directory(../core/graphics/animate DIR_SRC_GRAPHICS_ANIMATE)

list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_AXIS})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_CANVAS})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_GEOM})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_GEOM_ATTR})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_GEOM_SHAPE})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_GUIDE})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_SCALE})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_SCALE_CONTINUOUS})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_SHAPE})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_UTIL})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_ADJUST})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_EVENT})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_TOOLTIP})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_INTERACTION})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_LEGEND})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_FUNC})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_ANIMATE})

list(APPEND DIR_SRC ${DIR_SRC_UTILS})
list(APPEND DIR_SRC ${DIR_SRC_REFLECTION})
list(APPEND DIR_SRC ${DIR_SRC_WEBASSEMBLY})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS})

add_executable(${PROJECT_NAME} ${DIR_SRC})