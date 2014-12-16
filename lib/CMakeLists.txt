file(GLOB_RECURSE sources RELATIVE ${PROJECT_SOURCE_DIR}/lib *.cpp)
file(GLOB_RECURSE headers RELATIVE ${PROJECT_SOURCE_DIR}/webmock *.hpp)

add_library(webmock
  ${sources}
  ${headers}
)
target_link_libraries(webmock
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  cppnetlib-uri
  cppnetlib-client-connections
)
cotire(webmock)
