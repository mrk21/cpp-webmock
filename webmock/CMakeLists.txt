if(BUILD_LIBRARY)
  list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS ipp)
  include_regular_expression("^[^.]+$|[.]h$|[.]icc$|[.]hxx$|[.]hpp$")
  
  file(GLOB_RECURSE headers *.hpp)
  file(GLOB_RECURSE sources *.ipp)
  
  foreach(path IN LISTS sources)
    set_source_files_properties(${path} PROPERTIES LANGUAGE CXX)
  endforeach()
  
  set(type PUBLIC)
  add_library(webmock ${sources} ${headers})
  target_compile_definitions(webmock
    PUBLIC
      WEBMOCK_BUILD_LIBRARY
  )
  set_target_properties(webmock PROPERTIES COMPILE_FLAGS "-x c++")
  cotire(webmock)
  add_dependencies(webmock vendor_product)
else()
  file(GLOB_RECURSE headers *.[hi]pp)
  set(type INTERFACE)
  add_library(webmock INTERFACE)
endif()

set(webmock_headers ${headers} PARENT_SCOPE)

target_include_directories(webmock ${type}
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
  ${vendor_product_INCLUDE_DIRS}
)
target_link_libraries(webmock ${type}
  ${vendor_product_LIBRARIES}
)

install(TARGETS webmock DESTINATION lib EXPORT webmock)
install(EXPORT webmock DESTINATION lib NAMESPACE webmock::)
install(DIRECTORY . DESTINATION include/webmock
  PATTERN CMakeLists.txt EXCLUDE
)
