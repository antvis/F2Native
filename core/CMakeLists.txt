cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DGL_GLEXT_PROTOTYPES)

# 头文件搜索目录
include_directories(./)

# g++
add_definitions(-ffunction-sections)
add_definitions(-fdata-sections)
add_definitions(-Wno-deprecated)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -fno-exceptions -funwind-tables")

# 添加 core 下所有源文件
aux_source_directory(Core.h DIR_SRC_CORE)
aux_source_directory(graphics DIR_SRC_GRAPHICS)
aux_source_directory(bridge DIR_SRC_BRIDGE)
aux_source_directory(utils DIR_SRC_UTILS)
aux_source_directory(nlohmann DIR_SRC_NLOHMANN)
aux_source_directory(android DIR_SRC_ANDROID)
aux_source_directory(reflection DIR_SRC_REFLECTION)

aux_source_directory(graphics/axis DIR_SRC_GRAPHICS_AXIS)
aux_source_directory(graphics/animate DIR_SRC_GRAPHICS_ANIMATE)
aux_source_directory(graphics/canvas DIR_SRC_GRAPHICS_CANVAS)
aux_source_directory(graphics/geom DIR_SRC_GRAPHICS_GEOM)
aux_source_directory(graphics/geom/attr DIR_SRC_GRAPHICS_GEOM_ATTR)
aux_source_directory(graphics/geom/shape DIR_SRC_GRAPHICS_GEOM_SHAPE)
aux_source_directory(graphics/guide DIR_SRC_GRAPHICS_GUIDE)
aux_source_directory(graphics/scale DIR_SRC_GRAPHICS_SCALE)
aux_source_directory(graphics/scale/continuous DIR_SRC_GRAPHICS_SCALE_CONTINUOUS)
aux_source_directory(graphics/shape DIR_SRC_GRAPHICS_SHAPE)
aux_source_directory(graphics/util DIR_SRC_GRAPHICS_UTIL)
aux_source_directory(graphics/adjust DIR_SRC_GRAPHICS_ADJUST)
aux_source_directory(graphics/event DIR_SRC_GRAPHICS_EVENT)
aux_source_directory(graphics/tooltip DIR_SRC_GRAPHICS_TOOLTIP)
aux_source_directory(graphics/interaction DIR_SRC_GRAPHICS_INTERACTION)
aux_source_directory(graphics/legend DIR_SRC_GRAPHICS_LEGEND)
aux_source_directory(graphics/func DIR_SRC_GRAPHICS_FUNC)


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
list(APPEND DIR_SRC ${DIR_SRC_REFLECTION})


# 添加unit下所有文件
aux_source_directory(tests/unit/bridge DIR_TESTS_BRIDGE)
aux_source_directory(tests/unit/canvas DIR_TESTS_CANVAS)
aux_source_directory(tests/unit/scale DIR_TESTS_SCALE)
aux_source_directory(tests/unit/utils DIR_TESTS_UTILS)

list(APPEND DIR_TESTS ${DIR_TESTS_BRIDGE})
list(APPEND DIR_TESTS ${DIR_TESTS_CANVAS})
list(APPEND DIR_TESTS ${DIR_TESTS_SCALE})
list(APPEND DIR_TESTS ${DIR_TESTS_UTILS})


if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-D DEBUG)
endif ()


add_library(f2
        SHARED
        ${DIR_SRC})

find_library(log-lib log)
target_link_libraries(f2 ${log-lib})
target_link_libraries(f2 android -Wl,--gc-sections)

