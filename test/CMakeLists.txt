include_directories(
  ${PROJECT_SOURCE_DIR}/test
  ${PROJECT_SOURCE_DIR}/vendor/bandit
  ${PROJECT_SOURCE_DIR}/vendor/bandit_with_gmock
  ${PROJECT_SOURCE_DIR}/vendor/gmock/include
  ${PROJECT_SOURCE_DIR}/vendor/gmock/gtest/include
)
link_directories(
  ${PROJECT_BINARY_DIR}/test
  ${PROJECT_SOURCE_DIR}/vendor/gen/gmock
  ${PROJECT_SOURCE_DIR}/vendor/gen/gmock/gtest
)

file(GLOB_RECURSE sources *_spec.cpp)
file(GLOB_RECURSE headers RELATIVE ${PROJECT_SOURCE_DIR}/webmock *.hpp)

add_executable(spec_runner
  spec_runner.cpp
  ${sources}
  ${headers}
)
target_link_libraries(spec_runner
  ${cpp-webmock_LIBRARIES}
  gmock
  gtest
)
cotire(spec_runner)

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
  )
endforeach()
