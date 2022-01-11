# Copyright 2022 LoskutnikovGames Inc. All rights reserved.

cmake_minimum_required(VERSION 3.4.1)

project(readStruct)


# Output library files into `obj`.
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/obj)


set(readS
    fio/read_struct.h
    fio/read_struct.cpp)

# this was add_library

message("STATUS >    ok read_struct ")



  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" PARENT_SCOPE)
  set(CMAKE_C_FLAGS_DEFAULT "${CMAKE_C_FLAGS}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}" PARENT_SCOPE)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" PARENT_SCOPE)
  set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}" PARENT_SCOPE)
  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}" PARENT_SCOPE)

add_library(readStruct ${readS})

target_include_directories(readStruct PRIVATE ${cur_dir}/read_s/fio)
target_link_libraries(readStruct ${readStruct})





