cmake_minimum_required(VERSION 3.16)

project(Cindel LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(cindel 
    main.cpp
    ImapClient.hpp
    ImapClient.cpp
    ImapStatusCode.hpp
)

find_package(Curses REQUIRED)
include_directories(cindel ${CURSES_INCLUDE_DIRS})
target_link_libraries(cindel ${CURSES_LIBRARIES})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(cindel ${OPENSSL_LIBRARIES})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(cindel ${BOOST_LIBRARIES})

find_package(spdlog REQUIRED)
target_link_libraries(cindel spdlog::spdlog)
