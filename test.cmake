#!/usr/bin/env cmake -P
cmake_minimum_required(VERSION 3.0.2)

set(build_dir ${CMAKE_CURRENT_LIST_DIR}/gen)

if(NOT EXISTS ${build_dir})
  file(MAKE_DIRECTORY ${build_dir})
  execute_process(
    COMMAND ${CMAKE_COMMAND} ..
    WORKING_DIRECTORY ${build_dir}
  )
endif()

cmake_host_system_information(RESULT core_size QUERY NUMBER_OF_LOGICAL_CORES)

execute_process(
  COMMAND make -j ${core_size} spec_runner
  WORKING_DIRECTORY ${build_dir}
)

execute_process(
  COMMAND ctest -j ${core_size} --output-on-failure --no-label-summary -L ${CMAKE_ARGV3}
  WORKING_DIRECTORY ${build_dir}
)
