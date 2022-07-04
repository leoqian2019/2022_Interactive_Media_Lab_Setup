cmake_minimum_required(VERSION 2.8)
project(cuttlebone)

option(CB_BUILD_EXAMPLES "Build cuttlebone examples" ON)
## XXX some stuff stopped working after i added this. i think that setting
## CMAKE_BUILD_TYPE to "Release" compiles away asserts, which contained some
## critical code. stupid, i know.
##
#set(CMAKE_BUILD_TYPE Release)

#set(CMAKE_CXX_FLAGS                "-Wall -std=c++11 -Wno-deprecated-declarations")
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wno-deprecated-declarations")
elseif(APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-declarations")

elseif(WIN32)
    message("Windows Cuttlebone")
endif()

#set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
#set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
#set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
#set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")

set(CMAKE_MACOSX_RPATH 0)

#find_package(Crypto REQUIRED)
#set(libraries ${libraries} ${CRYPTO_LIBRARIES})

#find_package(ZLIB REQUIRED)
#set(libraries ${libraries} ${ZLIB_LIBRARIES})
#include_directories(${ZLIB_INCLUDE_DIRS} )
#include_directories(${CRYPTO_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../ms_demos)

#find_package(Threads REQUIRED)
#set(libraries ${CMAKE_THREAD_LIBS_INIT} ${libraries})

#if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#  set(libraries ${libraries} rt)
#endif()


IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    aux_source_directory(source/macos library_source)
    find_library(core_services CoreServices)
    set(library_library ${library_library} ${core_services})

    add_library(${PROJECT_NAME} SHARED ${library_source})
    target_link_libraries(${PROJECT_NAME} rt ${library_library})

    set_target_properties(${PROJECT_NAME} PROPERTIES
      DEBUG_POSTFIX d
      CXX_STANDARD 11
      CXX_STANDARD_REQUIRED ON
    )
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    aux_source_directory(source/linux library_source)
    #find_library(core_services CoreServices)
    #set(library_library ${library_library} ${core_services})

    add_library(${PROJECT_NAME} SHARED ${library_source})
    target_link_libraries(${PROJECT_NAME} rt ${library_library})

    set_target_properties(${PROJECT_NAME} PROPERTIES
      DEBUG_POSTFIX d
      CXX_STANDARD 11
      CXX_STANDARD_REQUIRED ON
    )

    set(libraries ${PROJECT_NAME} ${libraries})

ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")


IF(WIN32)
    aux_source_directory(source/windows library_source)
    #find_library(core_services CoreServices)
    #set(library_library ${library_library} ${core_services})

    add_library(${PROJECT_NAME} ${library_source})
    target_link_libraries(${PROJECT_NAME} ${library_library} ws2_32)

    set_target_properties(${PROJECT_NAME} PROPERTIES
      DEBUG_POSTFIX d
      CXX_STANDARD 11
      CXX_STANDARD_REQUIRED ON
    )

    set(libraries ${PROJECT_NAME} ${libraries})

       message(${libraries})
ENDIF(WIN32)


if (CB_BUILD_EXAMPLES)
    aux_source_directory(test source_list)
    aux_source_directory(example source_list)
    aux_source_directory(scratch source_list)
    foreach(source_file ${source_list})
      string(REGEX REPLACE "/" "_" target ${source_file})
      string(REGEX REPLACE ".cpp" "" target ${target})
      add_executable(${target} ${source_file})

      set_target_properties(${target} PROPERTIES
        DEBUG_POSTFIX d
        CXX_STANDARD 11
        CXX_STANDARD_REQUIRED ON
      )

      target_link_libraries(${target} ${PROJECT_NAME})
    endforeach(source_file)
endif()
