cmake_minimum_required(VERSION 3.10)
project(mini_utf8 LANGUAGES C VERSION 1.0.0)
set(CMAKE_C_STANDARD 99)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/mini_utf8-${PROJECT_VERSION}")

set(MINI_UTF8_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(MINI_UTF8_HEADER_FILES
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/internal/bitmasks.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/internal/gen_array.h>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include/mini_utf8.h>)

add_library(mini_utf8 STATIC src/mini_utf8.c)
target_include_directories(mini_utf8 PUBLIC "$<BUILD_INTERFACE:${MINI_UTF8_HEADERS}>")

add_subdirectory(cmake)

install(DIRECTORY ${MINI_UTF8_HEADERS}
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        FILES_MATCHING
        PATTERN "*.h")

target_sources(mini_utf8 INTERFACE
        FILE_SET HEADERS
        TYPE HEADERS
        BASE_DIRS ${CMAKE_INSTALL_PREFIX}/include
        FILES ${MINI_UTF8_HEADER_FILES})

add_executable(mini_utf8_test src/main.c)
target_link_libraries(mini_utf8_test PRIVATE mini_utf8)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        mini_utf8ConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

install(TARGETS mini_utf8
        EXPORT mini_utf8-export
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
        FILE_SET HEADERS)

install(EXPORT mini_utf8-export
        FILE mini_utf8Targets.cmake
        CONFIGURATIONS Debug Release
        DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake)
