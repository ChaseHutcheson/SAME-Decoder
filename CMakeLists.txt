cmake_minimum_required(VERSION 4.3)

project(
    SAME-Decoder 
    VERSION 0.1.0 
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SNDFILE REQUIRED sndfile)

add_executable(
    SAME-Decoder
    src/main.c 
    src/goertzel.c
)

target_include_directories(
    SAME-Decoder 
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${SNDFILE_INCLUDE_DIRS}
)


target_link_libraries(
    SAME-Decoder 
    PRIVATE
        m
        ${SNDFILE_LIBRARIES}
)

target_compile_options(
    SAME-Decoder
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)