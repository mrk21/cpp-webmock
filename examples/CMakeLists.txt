if(NOT BUILD_EXAMPLE)
  return()
endif()

add_custom_target(example)
add_dependencies(example vendor_product)

file(GLOB sources *.cpp)

foreach(path ${sources})
  get_filename_component(target ${path} NAME_WE)
  add_executable(${target} ${path})
  target_link_libraries(${target} webmock)
  cotire(${target})
  add_dependencies(example ${target})
endforeach()
