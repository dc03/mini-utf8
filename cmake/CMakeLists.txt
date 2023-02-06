include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_subdirectory(modules)

configure_package_config_file(
        in/mini_utf8Config.cmake.in
        ${CMAKE_INSTALL_PREFIX}/cmake/${PROJECT_NAME}Config.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_PREFIX}/cmake
)
