##
#   main CmakeList.txt  fot convertFBX_toLosMesh
##

cmake_minimum_required(VERSION 3.4.1)

project(convertFBX_toLosMesh)



set(Los_Convert_FILES
  src_losCon/losMain.cpp
)


  #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -pedantic -Wall")  # -std=gnu99
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Werror -Wextra")



  # Certain platforms such as ARM do not use signed chars by default
  # which causes issues with certain bounds checks.
  

  if(CMAKE_COMPILER_IS_GNUCC OR APPLE)
     message("STATUS is working ")
  endif()

if(${CMAKE_CXX_COMPILER_ID} MATCHES "AppleClang")
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -pedantic -Werror \
                          -Wextra")
  if(NOT "${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
  if(NOT ("${CMAKE_SYSTEM_NAME}" MATCHES "FreeBSD" OR
          "${CMAKE_SYSTEM_NAME}" MATCHES "Linux"))
    set(CMAKE_EXE_LINKER_FLAGS
        "${CMAKE_EXE_LINKER_FLAGS} -lc++abi")
  endif()
endif()

  # Certain platforms such as ARM do not use signed chars by default
  # which causes issues with certain bounds checks.

  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -fsigned-char")

set(cur_dir "/Users/sergeiloskutnikov/Desktop/Goodness/LosConvector")

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

