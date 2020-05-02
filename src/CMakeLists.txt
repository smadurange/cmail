cmake_minimum_required(VERSION 3.16)

project(Cmail LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_subdirectory(imap)

add_executable(cmail 
    main.cpp
)

target_include_directories(cmail PUBLIC "${CMAKE_SOURCE_DIR}/lib/include")

target_link_libraries(cmail imaplib)

find_package(spdlog REQUIRED)
target_link_libraries(cmail spdlog::spdlog)

find_package(RapidJSON CONFIG REQUIRED)
include_directories(cmail ${RAPIDJSON_INCLUDE_DIRS})
