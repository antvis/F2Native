cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DGL_GLEXT_PROTOTYPES)

# 头文件搜索目录
include_directories(../../core)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -flto=thin")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto=thin")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--gc-sections -flto=thin")

# g++
add_definitions(-ffunction-sections)
add_definitions(-fdata-sections)
add_definitions(-Wno-deprecated)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -fno-exceptions -funwind-tables")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Oz")

# 添加 core 下所有源文件
aux_source_directory(../../core/Core.h DIR_SRC_CORE)
aux_source_directory(../../core/graphics DIR_SRC_GRAPHICS)
aux_source_directory(../../core/bridge DIR_SRC_BRIDGE)
aux_source_directory(../../core/utils DIR_SRC_UTILS)
aux_source_directory(../../core/nlohmann DIR_SRC_NLOHMANN)
aux_source_directory(../../core/android DIR_SRC_ANDROID)
aux_source_directory(../../core/token DIR_SRC_TOKEN)


aux_source_directory(../../core/graphics/axis DIR_SRC_GRAPHICS_AXIS)
aux_source_directory(../../core/graphics/animate DIR_SRC_GRAPHICS_ANIMATE)
aux_source_directory(../../core/graphics/canvas DIR_SRC_GRAPHICS_CANVAS)
aux_source_directory(../../core/graphics/geom DIR_SRC_GRAPHICS_GEOM)
aux_source_directory(../../core/graphics/geom/attr DIR_SRC_GRAPHICS_GEOM_ATTR)
aux_source_directory(../../core/graphics/geom/shape DIR_SRC_GRAPHICS_GEOM_SHAPE)
aux_source_directory(../../core/graphics/guide DIR_SRC_GRAPHICS_GUIDE)
aux_source_directory(../../core/graphics/scale DIR_SRC_GRAPHICS_SCALE)
aux_source_directory(../../core/graphics/scale/continuous DIR_SRC_GRAPHICS_SCALE_CONTINUOUS)
aux_source_directory(../../core/graphics/shape DIR_SRC_GRAPHICS_SHAPE)
aux_source_directory(../../core/graphics/util DIR_SRC_GRAPHICS_UTIL)
aux_source_directory(../../core/graphics/adjust DIR_SRC_GRAPHICS_ADJUST)
aux_source_directory(../../core/graphics/event DIR_SRC_GRAPHICS_EVENT)
aux_source_directory(../../core/graphics/tooltip DIR_SRC_GRAPHICS_TOOLTIP)
aux_source_directory(../../core/graphics/interaction DIR_SRC_GRAPHICS_INTERACTION)
aux_source_directory(../../core/graphics/legend DIR_SRC_GRAPHICS_LEGEND)
aux_source_directory(../../core/graphics/func DIR_SRC_GRAPHICS_FUNC)


list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_AXIS})
list(APPEND DIR_SRC_GRAPHICS ${DIR_SRC_GRAPHICS_ANIMATE})
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

list(APPEND DIR_SRC ${DIR_SRC_CORE})
list(APPEND DIR_SRC ${DIR_SRC_UTILS})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS})
list(APPEND DIR_SRC ${DIR_SRC_NLOHMANN})
list(APPEND DIR_SRC ${DIR_SRC_BRIDGE})
list(APPEND DIR_SRC ${DIR_SRC_ANDROID})
list(APPEND DIR_SRC ${DIR_SRC_TOKEN})


if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-D DEBUG)
endif ()


add_library(f2
        SHARED
        ${DIR_SRC})

find_library(log-lib log)

target_link_libraries(f2 ${log-lib})
target_link_libraries(f2 android -Wl,--gc-sections)
