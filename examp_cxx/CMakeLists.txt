##
#   main CmakeList.txt  fot convertFBX_toLosMesh
##

cmake_minimum_required(VERSION 3.4.1)

project(exampleCpp)


set(myExamplFile
     incl/example_c.h
     c_fil/example_c.cpp
 )



  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS_DEFAULT "${CMAKE_C_FLAGS}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" PARENT_SCOPE)
  set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}" PARENT_SCOPE)
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" PARENT_SCOPE)



  add_library(exampleCpp ${myExamplFile})

target_include_directories(exampleCpp PUBLIC ${cur_dir}/examp_cxx/incl)
target_link_libraries(exampleCpp ${exampleCpp})

#fbx_configure_target(exampleCpp)


