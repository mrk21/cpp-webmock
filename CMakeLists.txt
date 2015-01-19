cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project(cpp-webmock)

execute_process(COMMAND git submodule init   WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
execute_process(COMMAND git submodule update WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/vendor/cotire/CMake)
include(cotire)
set_directory_properties(PROPERTIES
  COTIRE_PREFIX_HEADER_INCLUDE_PATH ${PROJECT_SOURCE_DIR}/vendor
)

option(BUILD_LIBRARY "Whether to build the libraries" OFF)
option(BUILD_TEST "Whether to build the tests" OFF)
option(BUILD_EXAMPLE "Whether to build the examples" OFF)
option(BUILD_DEPENDENCY "Whether to build the dependency libraries" OFF)
option(BUILD_OPTIONAL_DEPENDENCY "Whether to build the optional dependency libraries" OFF)

if(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  if(CMAKE_CXX_COMPILER_VERSION LESS 3.5)
    message(FATAL_ERROR "A supported Clang version is equal to or greater than 3.5!")
  endif()
  add_compile_options(-std=gnu++14 -stdlib=libc++)
  set(CMAKE_EXE_LINKER_FLAGS -stdlib=libc++)
  set(VENDOR_CXX_FLAGS -stdlib=libc++)
else()
  message(FATAL_ERROR "Supported compiler is Clang!")
endif()

add_compile_options(
  -Wall
  -Wextra
  -Wno-unused-parameter
  -Wno-missing-field-initializers
)

enable_testing()
add_subdirectory(vendor)
add_subdirectory(webmock)
add_subdirectory(test)
add_subdirectory(examples)
