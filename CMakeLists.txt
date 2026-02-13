cmake_minimum_required(VERSION 4.1.1)

if (NOT DEFINED SKBUILD_PROJECT_NAME)
    set(SKBUILD_PROJECT_NAME "default")
endif()

project(${SKBUILD_PROJECT_NAME} LANGUAGES CXX)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)

add_subdirectory(external/eigen)

set(pybind11_DIR ${Python3_SITEARCH}/pybind11/share/cmake/pybind11)

set(PYBIND11_FINDPYTHON ON)
find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(reynolds src/reynolds.cpp)

install(TARGETS reynolds LIBRARY DESTINATION .)
target_link_libraries(reynolds PRIVATE Eigen3::Eigen)