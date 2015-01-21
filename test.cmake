#!/usr/bin/env cmake -P
cmake_minimum_required(VERSION 3.0.2)

set(build_dir ${CMAKE_CURRENT_LIST_DIR}/gen)
cmake_host_system_information(RESULT core_size QUERY NUMBER_OF_LOGICAL_CORES)

if(NOT EXISTS ${build_dir})
  file(MAKE_DIRECTORY ${build_dir})
  execute_process(
    COMMAND ${CMAKE_COMMAND}
      -DCOTIRE_MINIMUM_NUMBER_OF_TARGET_SOURCES=1
      -DBUILD_DEPENDENCY=ON
      -DBUILD_OPTIONAL_DEPENDENCY=ON
      -DBUILD_LIBRARY=ON
      -DBUILD_TEST=ON
      -DBUILD_EXAMPLE=ON
      ${CMAKE_CURRENT_LIST_DIR}
    WORKING_DIRECTORY ${build_dir}
    RESULT_VARIABLE result
  )
  if(NOT result EQUAL 0)
    message(FATAL_ERROR "")
  endif()
endif()

execute_process(
  COMMAND make -j ${core_size} spec_runner
  WORKING_DIRECTORY ${build_dir}
  RESULT_VARIABLE result
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "")
endif()

execute_process(
  COMMAND ctest -j ${core_size} --output-on-failure --no-label-summary -L ${CMAKE_ARGV3}
  WORKING_DIRECTORY ${build_dir}
  RESULT_VARIABLE result
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "")
endif()
