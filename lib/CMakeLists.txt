file(GLOB_RECURSE sources RELATIVE ${PROJECT_SOURCE_DIR}/lib *.cpp)

add_library(${cpp-webmock_LIBNAME}
  ${sources}
)
target_link_libraries(${cpp-webmock_LIBNAME}
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  cppnetlib-uri
  cppnetlib-client-connections
)
