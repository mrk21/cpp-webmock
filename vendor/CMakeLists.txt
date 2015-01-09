include(ExternalProject)

function(set_project_property)
  set_property(DIRECTORY ${PROJECT_SOURCE_DIR} ${ARGV})
endfunction()

macro(set_project_var name)
  set(${name} ${ARGN})
  set(${name} ${ARGN} PARENT_SCOPE)
endmacro()

macro(external_project name)
  ExternalProject_Add(${name}
    ${ARGN}
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
  )
  ExternalProject_Get_Property(${name} BINARY_DIR)
  set_project_var(${name}_BINARY_DIR ${BINARY_DIR})
  
  ExternalProject_Get_Property(${name} SOURCE_DIR)
  set_project_var(${name}_SOURCE_DIR ${SOURCE_DIR})
endmacro()


# update git submodule
execute_process(
  COMMAND git submodule update
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(vendor)
add_custom_target(vendor_product)
add_custom_target(vendor_develop)

add_dependencies(vendor_develop vendor_product)
add_dependencies(vendor vendor_develop)


# exteral project: bandit_with_gmock
external_project(gmock
  SVN_REPOSITORY http://googlemock.googlecode.com/svn/tags/release-1.7.0
)
set_project_var(bandit_with_gmock_INCLUDE_DIRS
  ${gmock_SOURCE_DIR}/include
  ${gmock_SOURCE_DIR}/gtest/include
  ${PROJECT_SOURCE_DIR}/vendor/bandit
  ${PROJECT_SOURCE_DIR}/vendor/bandit_with_gmock
)
set_project_var(bandit_with_gmock_LINK_DIRS
  ${gmock_BINARY_DIR}
  ${gmock_BINARY_DIR}/gtest
)
set_project_var(bandit_with_gmock_LIBRARIES
  gmock
  gtest
)
add_dependencies(vendor_develop gmock)


# exteral project: cpp-netlib
external_project(cpp-netlib
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/cpp-netlib
)
set_project_var(cpp-netlib_INCLUDE_DIRS
  ${cpp-netlib_SOURCE_DIR}
)
set_project_var(cpp-netlib_LINK_DIRS
  ${cpp-netlib_BINARY_DIR}/libs/network/src
)
set_project_var(cpp-netlib_LIBRARIES
  cppnetlib-uri
  cppnetlib-client-connections
)
add_dependencies(vendor_product cpp-netlib)
