##
#   main CmakeList.txt  fot convertFBX_toLosMesh
##

cmake_minimum_required(VERSION 3.4.1)

project(LosGltf)



  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Werror -Wextra")


# /Users/SergeyLoscutnikov/Desktop/GradleAndOtherBooks/OpenGL/gltF/tinygltf-devel

add_library(LosGltf LosLoadGltf.hpp)
target_include_directories(LosGltf PRIVATE /Users/SergeyLoscutnikov/Desktop/GradleAndOtherBooks/OpenGL/gltF/tinygltf-devel)  

 #${cur_dir}/examp_cxx/incl)


#fbx_configure_target(convertFBX_toLosMesh)

