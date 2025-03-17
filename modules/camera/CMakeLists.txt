add_library(camera camera.cpp)
target_include_directories(camera PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(camera PUBLIC ../../libs/glm)
