add_library(mesh INTERFACE)
target_include_directories(model INTERFACE ${CMAKE_SOURCE_DIR}/libs/glm)
target_include_directories(mesh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/modules/shaders)
