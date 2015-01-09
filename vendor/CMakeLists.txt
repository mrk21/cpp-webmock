include(ExternalProject)

macro(set_project_var name)
  set(${name} ${ARGN})
  set(${name} ${ARGN} PARENT_SCOPE)
endmacro()

macro(append_project_var name)
  set_project_var(${name} ${${name}} ${ARGN})
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


add_custom_target(vendor)
add_custom_target(vendor_product)
add_custom_target(vendor_develop)

add_dependencies(vendor vendor_develop)
add_dependencies(vendor_develop vendor_product)


## update git submodule
execute_process(
  COMMAND git submodule update
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)


## bandit_with_gmock
# dependency projects:
# - bandit
# - google mock
# - google test
external_project(google_mock
  SVN_REPOSITORY http://googlemock.googlecode.com/svn/tags/release-1.7.0
)
append_project_var(vendor_develop_INCLUDE_DIRS
  ${google_mock_SOURCE_DIR}/include
  ${google_mock_SOURCE_DIR}/gtest/include
  ${PROJECT_SOURCE_DIR}/vendor/bandit
  ${PROJECT_SOURCE_DIR}/vendor/bandit_with_gmock
)
append_project_var(vendor_develop_LINK_DIRS
  ${google_mock_BINARY_DIR}
  ${google_mock_BINARY_DIR}/gtest
)
append_project_var(vendor_develop_LIBRARIES
  gmock
  gtest
)
add_dependencies(vendor_develop google_mock)


## cpp-netlib
external_project(cpp-netlib
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/cpp-netlib
)
append_project_var(vendor_product_INCLUDE_DIRS
  ${cpp-netlib_SOURCE_DIR}
)
append_project_var(vendor_product_LINK_DIRS
  ${cpp-netlib_BINARY_DIR}/libs/network/src
)
append_project_var(vendor_product_LIBRARIES
  cppnetlib-uri
  cppnetlib-client-connections
)
add_dependencies(vendor_product cpp-netlib)
