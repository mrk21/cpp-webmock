file(GLOB_RECURSE headers *.hpp)
file(GLOB_RECURSE sources *.ipp)

foreach(path IN LISTS sources)
  set_source_files_properties(${path} PROPERTIES LANGUAGE CXX)
endforeach()

add_library(webmock ${sources} ${headers})
add_dependencies(webmock vendor_product)
set_target_properties(webmock PROPERTIES COMPILE_FLAGS "-x c++")
cotire(webmock)

