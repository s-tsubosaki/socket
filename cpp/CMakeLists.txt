cmake_minimum_required(VERSION 2.8)
set(BOOST_ROOT "/usr/")
find_package(Boost COMPONENTS system filesystem random REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
set(CMAKE_CXX_FLAGS "-O4")
add_definitions(-Iwebsocketpp/ -lboost_system -lboost_random -DBOOST_ASIO_HAS_STD_CHRONO -std=c++11)
add_executable(server server.cpp)
target_link_libraries(server ${Boost_LIBRARIES})