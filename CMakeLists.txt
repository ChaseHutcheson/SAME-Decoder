cmake_minimum_required(VERSION 4.3)

project(
    SAME-Decoder 
    VERSION 0.1.0 
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(
    SAME-Decoder
    src/main.c 
    src/wav.c
)

target_include_directories(
    SAME-Decoder 
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(
    SAME-Decoder
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

target_link_libraries(
    SAME-Decoder 
    PRIVATE
        m
)