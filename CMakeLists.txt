cmake_minimum_required(VERSION 3.16)
project(xm65)

set(C_MAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin
)

add_executable(xm65)

target_compile_options(xm65 PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wconversion
    -Wstrict-prototypes
)

find_package(SDL3 REQUIRED)

target_sources(xm65 PRIVATE
    src/main.c
    src/cli.c
    src/vm_6502.c
    src/vm.c
    src/utils.c
)

target_include_directories(xm65 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(xm65 PRIVATE SDL3::SDL3)
target_link_libraries(xm65 PRIVATE m)
