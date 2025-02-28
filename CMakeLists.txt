cmake_minimum_required(VERSION 3.15)
project(Snake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
cmake_policy(SET CMP0135 NEW)

include(FetchContent)

find_package(SDL2)
if (NOT SDL2_FOUND)
    FetchContent_Declare(
        SDL2
        URL https://github.com/libsdl-org/SDL/releases/download/release-2.32.0/SDL2-2.32.0.zip
    )
    FetchContent_MakeAvailable(SDL2)
endif()

find_package(SDL2_image)
if (NOT SDL2_image_FOUND)
    FetchContent_Declare(
        SDL2_image
        URL https://github.com/libsdl-org/SDL_image/releases/download/release-2.8.5/SDL2_image-2.8.5.zip
    )
    FetchContent_MakeAvailable(SDL2_image)
endif()

find_package(SDL2_ttf)
if (NOT SDL2_ttf_FOUND)
    FetchContent_Declare(
        SDL2_ttf
        URL https://github.com/libsdl-org/SDL_ttf/releases/download/release-2.24.0/SDL2_ttf-2.24.0.zip
    )
    FetchContent_MakeAvailable(SDL2_ttf)
endif()

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
