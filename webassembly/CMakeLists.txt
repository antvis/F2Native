cmake_minimum_required(VERSION 3.4.1)

set(PROJECT_NAME "f2wasm")

project(${PROJECT_NAME})

# JSON_NOEXCEPTION定义
add_definitions(-DJSON_NOEXCEPTION=1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "10.9")

# -flto
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -fno-exceptions -DEMSCRIPTEN_HAS_UNBOUND_TYPE_NAMES=0")
add_definitions(-ffunction-sections)
add_definitions(-fdata-sections)

include_directories(../core)

# 添加 core/utils 下所有源文件
aux_source_directory(../core/utils DIR_SRC_UTILS)

# 添加core/webassembly 下所有源文件
aux_source_directory(../core/webassembly/ DIR_SRC_WEBASSEMBLY)

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

list(APPEND DIR_SRC ${DIR_SRC_WEBASSEMBLY})
list(APPEND DIR_SRC ${DIR_SRC_UTILS})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS})

# ###############
add_executable(${PROJECT_NAME} ${DIR_SRC})

# https://github.com/timhutton/sdl-canvas-wasm
# emcc core.cpp -s WASM=1 -s USE_SDL=2 -O3 -o index.js
# 执行上面的命令 预下载SDL2
# -Oz > Os > O3 > O2 > O1 > O0 (O0-default)
# -s STANDALONE_WASM  独立wasm
# --no-entry 没有main函数
# -nostdlib 声明不要使用系统库
# -s ENVIRONMENT='node'
# -s FILESYSTEM=0 关闭文件系统 不能减小包大小
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "--bind  -Wall -Werror -s  ALLOW_MEMORY_GROWTH=1 -s ALLOW_TABLE_GROWTH -s -Oz -fno-rtti -fno-exceptions -s ENVIRONMENT='web' -s FILESYSTEM=0")