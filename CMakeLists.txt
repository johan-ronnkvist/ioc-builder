cmake_minimum_required(VERSION 3.11)
project("project_name" C CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "predefined")
set_property(GLOBAL PROPERTY CMAKE_EXPORT_COMPILE_COMMANDS ON)

include("cmake/project_settings.cmake")
include("cmake/conan_settings.cmake")
include("cmake/compiler_warnings.cmake")
include("cmake/compiler_features.cmake")
include("cmake/sanitizers.cmake")

add_conan_package("gtest/1.8.1@bincrafters/stable")
run_conan()

add_subdirectory("project_name")