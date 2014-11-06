file(GLOB CppNetLibMock_MAIN_SOURCES "*.cpp")

foreach(_path ${CppNetLibMock_MAIN_SOURCES})
  get_filename_component(_name ${_path} NAME_WE)
  add_executable(${_name} ${_path})
  
  foreach(_lib ${CppNetLibMock_LIBLARIES})
    target_link_libraries(${_name} ${_lib})
  endforeach()
endforeach()
