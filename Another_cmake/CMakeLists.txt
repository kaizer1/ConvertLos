# Copyright 2022 Loskutnikov Games Inc.


cmake_minimum_required(VERSION 3.10.1)

project(fbxLos_common)
include("cmakeLos_fbx.txt")
fbx_compile_options()

set(fbxLos_common_SRCS
    include/fbx_common.h
    files/fbx_common.cpp)

add_library(fbxLos_common ${fbxLos_common_SRCS})
message("STATUS >    ${CMAKE_CXX_COMPILER_ID}")
fbx_configure_target(fbxLos_common)

