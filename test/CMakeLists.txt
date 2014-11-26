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
function(spec_build_commands)
  file(GLOB sources *_spec.cpp)
  
  foreach(path IN LISTS sources)
    get_filename_component(name ${path} NAME_WE)
    set(target "${name}_bin")
    
    add_executable(${target} ${path})
    target_link_libraries(${target}
      spec_main
      gmock
      gtest
    )
    
    add_test(
      NAME ${name}
      COMMAND ${target} --reporter=spec
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    )
    add_dependencies(build_test ${target})
  endforeach()
endfunction()

add_custom_target(build_test)
add_library(spec_main spec_main.cpp)
add_subdirectory(spec)
