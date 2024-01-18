cmake_minimum_required(VERSION 3.20)
project(SQLConnectionPoolTest)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")

add_executable(SQLConnectionPoolTest "test.cpp" "../SQLConnectionPool.cpp" "../SQLConnection.cpp")
target_link_libraries(SQLConnectionPoolTest /opt/homebrew/Cellar/mysql/8.2.0_1/lib/libmysqlclient.dylib)
