cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(cpp-webmock)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/vendor/cotire/CMake)
include(cotire)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  if(CMAKE_CXX_COMPILER_VERSION LESS 3.5)
    message(FATAL_ERROR "A supported Clang version is equal to or greater than 3.5!")
  endif()
  add_compile_options(-std=gnu++14 -stdlib=libc++)
else()
  message(FATAL_ERROR "Supported compiler is Clang!")
endif()

find_package(Boost 1.56 REQUIRED system thread)
find_package(OpenSSL REQUIRED)

add_compile_options(
  -Wall
  -Wextra
  -Wno-unused-parameter
  -Wno-missing-field-initializers
)
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
set_directory_properties(PROPERTIES
  COTIRE_PREFIX_HEADER_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/vendor
)
set(cpp-webmock_LIBRARIES
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  cppnetlib-uri
  cppnetlib-client-connections
)
option(USE_LIBRARY "Use library" ON)

enable_testing()

if(USE_LIBRARY)
  list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS ipp)
  include_regular_expression("^[^.]+$|[.]h$|[.]icc$|[.]hxx$|[.]hpp$")
  
  add_definitions(-DWEBMOCK_USE_LIBRARY)
  link_directories(${PROJECT_SOURCE_DIR}/webmock)
  list(APPEND cpp-webmock_LIBRARIES webmock)
  
  add_subdirectory(webmock)
endif()

add_subdirectory(examples)
add_subdirectory(test)
