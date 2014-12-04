cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
project(cpp-webmock)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  if(CMAKE_CXX_COMPILER_VERSION LESS "3.5")
    message(FATAL_ERROR "A supported Clang version is equal to or greater than 3.5!")
  endif()
else()
  message(FATAL_ERROR "Supported compiler is Clang!")
endif()

find_package(Boost 1.56 REQUIRED system thread)
find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_FLAGS "-std=gnu++14 -stdlib=libc++ -Wall -Wextra -Wno-unused-parameter")

include_directories(
  ${PROJECT_SOURCE_DIR}
  ${PROJECT_SOURCE_DIR}/vendor/cpp-netlib
  ${Boost_INCLUDE_DIRS}
  ${OPENSSL_INCLUDE_DIR}
)
link_directories(
  ${PROJECT_SOURCE_DIR}/vendor/gen/cpp-netlib/libs/network/src
)
add_definitions(
  -DBOOST_NETWORK_ENABLE_HTTPS
)
set(cpp-webmock_LIBNAME webmock CACHE STRING [[A library name of cpp-webmock.]])

enable_testing()
add_subdirectory(lib)
add_subdirectory(examples)
add_subdirectory(test)
