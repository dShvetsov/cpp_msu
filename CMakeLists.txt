cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Werror")


add_executable(size Size.cpp)

add_executable(complex ComplexNumber.cpp Complex.hpp)

add_executable(titems TItems.cpp)

add_executable(composer Compose.cpp)

add_executable(alloactor Allocator.cpp)

add_executable(intrusove TIntrusivePtr.cpp)

add_subdirectory(Task2)

add_subdirectory(Task3)

add_subdirectory(googletest)
