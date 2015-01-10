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
else()
  set(type INTERFACE)
  add_library(webmock INTERFACE)
endif()

target_include_directories(webmock ${type}
  ${PROJECT_SOURCE_DIR}
  ${vendor_product_INCLUDE_DIRS}
)
target_compile_definitions(webmock ${type}
  BOOST_NETWORK_ENABLE_HTTPS
)
target_link_libraries(webmock ${type}
  ${vendor_product_LIBRARIES}
)
add_dependencies(webmock vendor_product)
