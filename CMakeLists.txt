cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DGL_GLEXT_PROTOTYPES)

# 头文件搜索目录
include_directories(core)

## third_party
include_directories(third_party)

#if (TARGET_STANDALONE)
#    include_directories(third_party/GCanvas/core/src)
#    include_directories(third_party/GCanvas/core/src/gcanvas)
#    include_directories(third_party/GCanvas/core/src/gcanvas/GL)
#    include_directories(third_party/GCanvas/core/src/platform)
#    include_directories(third_party/GCanvas/core/src/platform/Android)
#    include_directories(third_party/GCanvas/core/src/support)
#    include_directories(third_party/GCanvas/core/android)
#    include_directories(third_party/GCanvas/node/third_party/font/freetype2)
#elseif (TARGET_ALIPAY)
    include_directories(third_party/)
    include_directories(third_party/AntGraphic)
#endif ()

include_directories(third_party/json/single_include)

# g++
add_definitions(-ffunction-sections)
add_definitions(-fdata-sections)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fno-rtti -fno-exceptions -funwind-tables")

# 添加 core 下所有源文件
aux_source_directory(core/graphics DIR_SRC_GRAPHICS)
aux_source_directory(core/utils DIR_SRC_UTILS)

aux_source_directory(core/graphics/axis DIR_SRC_GRAPHICS_AXIS)
aux_source_directory(core/graphics/animate DIR_SRC_GRAPHICS_ANIMATE)
aux_source_directory(core/graphics/canvas DIR_SRC_GRAPHICS_CANVAS)
aux_source_directory(core/graphics/geom DIR_SRC_GRAPHICS_GEOM)
aux_source_directory(core/graphics/geom/attr DIR_SRC_GRAPHICS_GEOM_ATTR)
aux_source_directory(core/graphics/geom/shape DIR_SRC_GRAPHICS_GEOM_SHAPE)
aux_source_directory(core/graphics/guide DIR_SRC_GRAPHICS_GUIDE)
aux_source_directory(core/graphics/scale DIR_SRC_GRAPHICS_SCALE)
aux_source_directory(core/graphics/scale/continuous DIR_SRC_GRAPHICS_SCALE_CONTINUOUS)
aux_source_directory(core/graphics/shape DIR_SRC_GRAPHICS_SHAPE)
aux_source_directory(core/graphics/util DIR_SRC_GRAPHICS_UTIL)
aux_source_directory(core/graphics/adjust DIR_SRC_GRAPHICS_ADJUST)
aux_source_directory(core/graphics/event DIR_SRC_GRAPHICS_EVENT)
aux_source_directory(core/graphics/tooltip DIR_SRC_GRAPHICS_TOOLTIP)
aux_source_directory(core/graphics/interaction DIR_SRC_GRAPHICS_INTERACTION)
aux_source_directory(core/graphics/legend DIR_SRC_GRAPHICS_LEGEND)
aux_source_directory(core/graphics/func DIR_SRC_GRAPHICS_FUNC)


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


list(APPEND DIR_SRC ${DIR_SRC_UTILS})
list(APPEND DIR_SRC ${DIR_SRC_GRAPHICS})

#list(APPEND CMAKE_MODULE_PATH "../../cmake")

if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
    add_definitions(-D DEBUG)
endif ()

aux_source_directory(core/android DIR_SRC_ANDROID)

if (TARGET_ALIPAY)
    list(REMOVE_ITEM DIR_SRC_ANDROID "core/android/F2CanvasView.cpp")
endif ()


aux_source_directory(third_party/WilkinsonExtended WilkinsonExtended)

add_library(f2
        SHARED
        ${DIR_SRC} ${DIR_SRC_ANDROID} ${WilkinsonExtended})

find_library(log-lib log)

target_link_libraries(f2 ${log-lib})
target_link_libraries(f2 android -Wl,--gc-sections)
target_link_libraries(f2 GLESv2 -Wl,--gc-sections)
target_link_libraries(f2 EGL -Wl,--gc-sections)

#if (TARGET_STANDALONE)
#    target_link_libraries(f2 ${CMAKE_SOURCE_DIR}/third_party/GCanvas/core/android/freetype-prebuilt/libs/${ANDROID_ABI}/libfreetype.so)
#    target_link_libraries(f2 ${CMAKE_SOURCE_DIR}/third_party/GCanvas/android/gcanvas_library/build/intermediates/cmake/standalone/release/obj/${ANDROID_ABI}/libgcanvas.so)
#elseif (TARGET_ALIPAY)
    target_link_libraries(f2 ${CMAKE_SOURCE_DIR}/third_party/AntGraphic/jniLibs/${ANDROID_ABI}/libfreetype.so)
    target_link_libraries(f2 ${CMAKE_SOURCE_DIR}/third_party/AntGraphic/jniLibs/${ANDROID_ABI}/libantgraphic.so)
#endif ()
