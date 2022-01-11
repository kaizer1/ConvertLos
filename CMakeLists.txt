# 2022 Loskutnikov Games Inc.

cmake_minimum_required(VERSION 3.10.1)
project(convertFBX_toLosMesh)



set(Los_Convert_FILES
  src_losCon/losMain.cpp
)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -stdlib=libc++")
  #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Werror  -Wextra")

if (MSVC)
    add_compile_options(/W4 /MDd) # was /WX
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

 
  if(CMAKE_COMPILER_IS_GNUCC OR APPLE)
     message(STATUS "Apple or CNUC ")
  endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "AppleClang")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic -Werror -Wextra")
  if(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
  if(NOT ("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD" OR "${CMAKE_SYSTEM_NAME}" MATCHES "Linux"))
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
  endif()
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsigned-char")
set(cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
message (STATUS " my directory = " ${cur_dir})

add_subdirectory(Another_cmake)
add_subdirectory(read_s)
#add_subdirectory(examp_cxx)
#add_subdirectory(gltfLoaderFile)

fbx_compile_options()


add_executable(convertFBX_toLosMesh ${Los_Convert_FILES})
 
target_include_directories(convertFBX_toLosMesh PUBLIC 
                        read_s/fio
                        examp_cxx/incl
                        gltfLoaderFile
                        )

target_link_libraries(convertFBX_toLosMesh 
                #exampleCpp
                readStruct
                )
 

fbx_configure_target(convertFBX_toLosMesh)

