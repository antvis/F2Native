cmake_minimum_required(VERSION 3.4.1)
set(PROJECT_NAME "scale")

project(${PROJECT_NAME})

aux_source_directory(./ SCALE)
aux_source_directory(./continuous SCALE_CONTINUS)

list(APPEND DIR_SRC ${SCALE})
list(APPEND DIR_SRC ${SCALE_CONTINUS})

add_library(${PROJECT_NAME} 
            STATIC 
            ${DIR_SRC})