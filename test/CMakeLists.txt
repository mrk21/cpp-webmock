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

add_executable(spec_bin
  spec_runner.cpp
  ${sources}
)
target_link_libraries(spec_bin
  webmock
  gmock
  gtest
)
cotire(spec_bin)

add_custom_target(spec
  COMMAND spec_bin --reporter=spec
  WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  DEPENDS spec_bin
)

foreach(path IN LISTS sources)
  string(REPLACE ${PROJECT_SOURCE_DIR}/test/spec webmock name ${path})
  string(REPLACE _spec.cpp "" name ${name})
  string(REPLACE / :: name ${name})
  
  add_test(
    NAME ${path}
    COMMAND spec_bin --only=${name}
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
endforeach()
