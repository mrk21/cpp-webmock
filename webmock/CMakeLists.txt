if(USE_LIBRARY)
  list(APPEND CMAKE_CXX_SOURCE_FILE_EXTENSIONS ipp)
  include_regular_expression("^[^.]+$|[.]h$|[.]icc$|[.]hxx$|[.]hpp$")
  add_definitions(-DWEBMOCK_USE_LIBRARY)
  
  file(GLOB_RECURSE headers *.hpp)
  file(GLOB_RECURSE sources *.ipp)
  
  foreach(path IN LISTS sources)
    set_source_files_properties(${path} PROPERTIES LANGUAGE CXX)
  endforeach()
  
  set(type PUBLIC)
  add_library(webmock ${sources} ${headers})
  set_target_properties(webmock PROPERTIES COMPILE_FLAGS "-x c++")
  cotire(webmock)
  add_dependencies(webmock vendor_product)
else()
  set(type INTERFACE)
  add_library(webmock INTERFACE)
endif()

target_include_directories(webmock ${type}
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
  $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/include>
  ${vendor_product_INCLUDE_DIRS}
)
target_compile_definitions(webmock ${type}
  BOOST_NETWORK_ENABLE_HTTPS
)
target_link_libraries(webmock ${type}
  ${vendor_product_LIBRARIES}
)

install(TARGETS webmock DESTINATION lib EXPORT webmock)
install(EXPORT webmock DESTINATION lib NAMESPACE webmock::)
install(DIRECTORY . DESTINATION include/webmock)
