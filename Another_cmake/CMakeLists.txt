# Copyright 2017 LoskutnikovGames Inc. All rights reserved.
#
#
#  fbx_commonLos  
#

cmake_minimum_required(VERSION 3.4.1)

project(fbxLos_common)

# Include common setup for FBX projects.
include("cmakeLos_fbx.txt")

# Output library files into `obj`.
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/obj)

# Set compile options for FBX programs.
fbx_compile_options()

# Source files for the library.
set(fbxLos_common_SRCS
    include/fbx_common.h
    files/fbx_common.cpp)

# Create the executable for mesh_pipeline.
add_library(fbxLos_common ${fbxLos_common_SRCS})

message("STATUS >    ${CMAKE_CXX_COMPILER_ID}")

# Add common link libraries and include directories.
fbx_configure_target(fbxLos_common)

