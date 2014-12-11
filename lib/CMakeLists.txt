file(GLOB_RECURSE sources RELATIVE ${PROJECT_SOURCE_DIR}/lib *.cpp)

add_library(webmock
  ${sources}
)
target_link_libraries(webmock
  ${Boost_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  cppnetlib-uri
  cppnetlib-client-connections
)
cotire(webmock)
