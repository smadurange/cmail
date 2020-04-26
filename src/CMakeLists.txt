cmake_minimum_required(VERSION 3.16)

project(Cmail LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(cmail 
    main.cpp
)

include_directories(cmail "${CMAKE_SOURCE_DIR}/lib/include")

find_package(Curses REQUIRED)
include_directories(cmail ${CURSES_INCLUDE_DIRS})
target_link_libraries(cmail ${CURSES_LIBRARIES})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(cmail ${OPENSSL_LIBRARIES})

find_package(spdlog REQUIRED)
target_link_libraries(cmail spdlog::spdlog)

find_package(RapidJSON CONFIG REQUIRED)
include_directories(cmail ${RAPIDJSON_INCLUDE_DIRS})

include_directories(cmail "${CMAKE_SOURCE_DIR}/lib/mailio/0.18.0/arch_x64")
target_link_libraries(cmail "${CMAKE_CURRENT_SOURCE_DIR}/lib/mailio/0.18.0/arch_x64/libmailio.so")
