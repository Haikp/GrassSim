cmake_minimum_required(VERSION 3.20)
project (grass VERSION 0.1.0)

add_definitions(-DSHADER_PATH=${CMAKE_SOURCE_DIR}/shaders/)
add_definitions(-DMODEL_PATH=${CMAKE_SOURCE_DIR}/assets/models/)

add_executable(app src/main.cpp
                   ${CMAKE_SOURCE_DIR}/libs/stb/stb.cpp)

set(OSBitness 32)
if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(OSBitness 64)
endif()

set (FullOutputDir "${CMAKE_SOURCE_DIR}/build/bin/${CMAKE_SYSTEM_NAME}${OSBitness}/${CMAKE_BUILD_TYPE}")
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FullOutputDir}/static libs")
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FullOutputDir}")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FullOutputDir}")

set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${FullOutputDir}
)

set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake/glfw3/glfw3Config.cmake" ${CMAKE_PREFIX_PATH})
set(CMAKE_PREFIX_PATH "/usr/local/lib/cmake/glew/glew-config.cmake" ${CMAKE_PREFIX_PATH})
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(app PRIVATE glfw3)
target_link_libraries(app PRIVATE glfw3 dl)
target_link_libraries(app PRIVATE glfw3 ${OPENGL_LIBRARIES} pthread)
target_link_libraries(app PRIVATE GLEW::GLEW)

#### MODULES ####

#include directories for core and modules
target_include_directories(app PRIVATE include)

#add subdirectories
add_subdirectory(modules/camera)
add_subdirectory(modules/shaders)
add_subdirectory(modules/model)

#link against modules
target_link_libraries(app PRIVATE camera)
target_link_libraries(app PRIVATE shaders)
target_link_libraries(app PRIVATE model)

#### LIBRARIES/DEPENDENCIES ####

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glm)
include_directories(${CMAKE_SOURCE_DIR}/libs/glm)

target_include_directories(app PRIVATE ${CMAKE_SOURCE_DIR}/libs/assimp/include)
target_link_libraries(app PRIVATE ${CMAKE_SOURCE_DIR}/libs/assimp/build/bin/libassimp.so)
