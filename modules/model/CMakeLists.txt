add_library(model INTERFACE)
target_include_directories(model INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(model INTERFACE ${CMAKE_SOURCE_DIR}/libs/glm)
target_include_directories(model INTERFACE ${CMAKE_SOURCE_DIR}/libs/stb)
target_include_directories(model INTERFACE ${CMAKE_SOURCE_DIR}/modules/mesh)
target_include_directories(model INTERFACE ${CMAKE_SOURCE_DIR}/libs/assimp/include)
target_link_libraries(model INTERFACE ${CMAKE_SOURCE_DIR}/libs/assimp/build/bin/libassimp.so)

target_include_directories(model INTERFACE ${CMAKE_SOURCE_DIR}/modules/shaders)
