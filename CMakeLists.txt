cmake_minimum_required(VERSION 4.1.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT DEFINED SKBUILD_PROJECT_NAME)
    set(SKBUILD_PROJECT_NAME "default")
endif()

project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

find_package(Python COMPONENTS Interpreter Development.Module NumPy REQUIRED)

add_subdirectory(external/eigen)

python_add_library(reynolds MODULE src/reynolds.cpp)

target_include_directories(reynolds PRIVATE ${Python_NumPy_INCLUDE_DIRS})

install(TARGETS reynolds LIBRARY DESTINATION .)
target_link_libraries(reynolds PRIVATE Eigen3::Eigen)