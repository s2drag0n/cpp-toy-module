cmake_minimum_required(VERSION 3.20)
project(MemoryPoolTest)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")

add_executable(MemoryPoolTest "main.cpp" "../MemoryPool.cpp" "String.cpp" "STDString.cpp")
