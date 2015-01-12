include(ExternalProject)

macro(set_project_var name)
  set(${name} ${ARGN})
  set(${name} ${ARGN} PARENT_SCOPE)
endmacro()

macro(append_project_var name)
  set_project_var(${name} ${${name}} ${ARGN})
endmacro()

add_custom_target(vendor)
add_custom_target(vendor_product)
add_custom_target(vendor_develop)

add_dependencies(vendor vendor_develop)
add_dependencies(vendor_develop vendor_product)

if(BUILD_TEST OR BUILD_DEPENDENCY)
  ## Boost C++ Libraries
  find_package(Boost 1.56 REQUIRED system thread)
  append_project_var(vendor_product_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
  append_project_var(vendor_product_LIBRARIES ${Boost_LIBRARIES})
  
  if(BUILD_TEST OR BUILD_OPTIONAL_DEPENDENCY)
    ## OpenSSL
    find_package(OpenSSL REQUIRED)
    append_project_var(vendor_product_INCLUDE_DIRS ${OPENSSL_INCLUDE_DIR})
    append_project_var(vendor_product_LIBRARIES ${OPENSSL_LIBRARIES})
    
    ## cpp-netlib
    ExternalProject_Add(cpp-netlib
      SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/cpp-netlib
      CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>
    )
    ExternalProject_Get_Property(cpp-netlib INSTALL_DIR)
    add_dependencies(vendor_product cpp-netlib)
    
    set_property(DIRECTORY ${PROJECT_SOURCE_DIR}
      APPEND PROPERTY LINK_DIRECTORIES
        ${INSTALL_DIR}/lib
    )
    set_property(DIRECTORY ${PROJECT_SOURCE_DIR}
      APPEND_STRING PROPERTY COMPILE_DEFINITIONS
        BOOST_NETWORK_ENABLE_HTTPS
    )
    append_project_var(vendor_product_INCLUDE_DIRS
      $<BUILD_INTERFACE:${INSTALL_DIR}/include>
      $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
    )
    append_project_var(vendor_product_LIBRARIES
      cppnetlib-uri
      cppnetlib-client-connections
    )
    
    install(DIRECTORY ${INSTALL_DIR}/include DESTINATION .)
    install(DIRECTORY ${INSTALL_DIR}/lib DESTINATION .)
  endif()
endif()

if(BUILD_TEST)
  ## bandit_with_gmock
  # dependency projects:
  # - bandit
  # - google mock
  # - google test
  ExternalProject_Add(google_mock
    SVN_REPOSITORY http://googlemock.googlecode.com/svn/tags/release-1.7.0
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
  )
  ExternalProject_Get_Property(google_mock BINARY_DIR)
  ExternalProject_Get_Property(google_mock SOURCE_DIR)
  add_dependencies(vendor_develop google_mock)
  
  set_property(DIRECTORY ${PROJECT_SOURCE_DIR}
    APPEND PROPERTY LINK_DIRECTORIES
      ${BINARY_DIR}
      ${BINARY_DIR}/gtest
  )
  append_project_var(vendor_develop_INCLUDE_DIRS
    ${SOURCE_DIR}/include
    ${SOURCE_DIR}/gtest/include
    ${PROJECT_SOURCE_DIR}/vendor/bandit
    ${PROJECT_SOURCE_DIR}/vendor/bandit_with_gmock
  )
  append_project_var(vendor_develop_LIBRARIES
    gmock
    gtest
  )
endif()
