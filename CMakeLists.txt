cmake_minimum_required(VERSION 3.20)

project(tachyon LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CompilerWarnings.cmake)

add_subdirectory(src)
add_subdirectory(tests)