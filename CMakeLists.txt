cmake_minimum_required(VERSION 4.1.2)
project(english-server)

set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp" "src/*.h")
add_executable(english-server ${SOURCES})

target_link_libraries(english-server PRIVATE
    /usr/lib/libmariadbcpp.so
    /usr/include/nlohmann/json.hpp
    /usr/include/httplib.h)
