cmake_minimum_required(VERSION 3.10)
project(mini_utf8 LANGUAGES C)
set(CMAKE_C_STANDARD 99)

add_library(mini_utf8 src/mini_utf8.c)
target_include_directories(mini_utf8 PUBLIC include/)

add_executable(mini_utf8_test src/main.c)
target_link_libraries(mini_utf8_test PRIVATE mini_utf8)

