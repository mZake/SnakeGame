cmake_minimum_required(VERSION 3.15)
project(Snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB SOURCE_FILES "src/*.cpp")
add_executable(Snake "${SOURCE_FILES}")

target_include_directories(Snake PRIVATE "${CMAKE_SOURCE_DIR}/include")

if (MINGW AND TARGET mingw32)
    target_link_libraries(Snake PRIVATE mingw32)
endif()

if (TARGET SDL2::SDL2main)
    target_link_libraries(Snake PRIVATE SDL2::SDL2main)
endif()

target_link_libraries(Snake PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)

set_target_properties(Snake PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}")
