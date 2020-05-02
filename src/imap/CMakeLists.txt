add_library(imaplib
    connection.hpp
    connection.cpp
)

target_include_directories(imaplib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIRS})
target_link_libraries(imaplib ${OPENSSL_LIBRARIES})

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(imaplib ${BOOST_LIBRARIES})

find_package(spdlog REQUIRED)
target_link_libraries(imaplib spdlog::spdlog)
