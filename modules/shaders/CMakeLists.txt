add_library(shaders shaders.cpp)
target_include_directories(shaders PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(shaders PUBLIC ../../libs/glm)
