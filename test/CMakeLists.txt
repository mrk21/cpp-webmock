## build
file(GLOB_RECURSE sources *_spec.cpp)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/webmock/*.hpp)

add_executable(spec_runner
  spec_runner.cpp
  ${sources}
  ${headers}
)
target_include_directories(spec_runner PRIVATE
  ${PROJECT_SOURCE_DIR}/test
  ${vendor_develop_INCLUDE_DIRS}
)
target_link_libraries(spec_runner PRIVATE
  webmock
  ${vendor_develop_LIBRARIES}
)
add_dependencies(spec_runner vendor_develop)
cotire(spec_runner)


## test target
add_custom_target(spec
  COMMAND $<TARGET_FILE:spec_runner> --reporter=spec
  DEPENDS spec_runner
)

foreach(path IN LISTS sources)
  string(REPLACE ${PROJECT_SOURCE_DIR}/test/spec/ "" basepath ${path})
  string(REPLACE _spec.cpp "" basepath ${basepath})
  string(REPLACE / :: name webmock/${basepath})
  
  add_test(
    NAME ${name}
    COMMAND $<TARGET_FILE:spec_runner> --only=${name}
  )
  set_property(TEST ${name} PROPERTY LABELS
    ${path}
    ${PROJECT_SOURCE_DIR}/lib/${basepath}.cpp
    ${PROJECT_SOURCE_DIR}/webmock/${basepath}.hpp
    ${PROJECT_SOURCE_DIR}/webmock/${basepath}.ipp
  )
endforeach()
