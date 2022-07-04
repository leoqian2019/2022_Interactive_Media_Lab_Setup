
list(APPEND CMAKE_MODULE_PATH
${CMAKE_CURRENT_LIST_DIR}
)

include("../BuildExamples.cmake")

set(OPENCV_SDK "${CMAKE_CURRENT_LIST_DIR}/opencv" CACHE PATH "")

if (EXISTS "${OPENCV_SDK}")
    set(OpenCV_DIR "${OPENCV_SDK}/build")
else()
endif()

find_package (OpenCV)


if (NOT OPENCV_CORE_FOUND)
    message("OpenCV not found. Not building al_opencv extension.")
else()

    message("Using OpenCV ${OpenCV_LIBS} SDK from: ${OPENCV_SDK}")
    message("${OPENCV_INCLUDE_DIRS}")
    set(THIS_EXTENSION_SRC
      "${CMAKE_CURRENT_LIST_DIR}/src/al_OpenCV.cpp"
    )

    set(THIS_EXTENSION_HEADERS
      "${CMAKE_CURRENT_LIST_DIR}/al_OpenCV.hpp"
    )

    set(THIS_EXTENSION_LIBRARY_NAME al_opencv)

    add_library(${THIS_EXTENSION_LIBRARY_NAME} ${THIS_EXTENSION_SRC} ${THIS_EXTENSION_HEADERS})


    target_include_directories(${THIS_EXTENSION_LIBRARY_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}/opencv/sources/include/" )

    target_link_libraries(${THIS_EXTENSION_LIBRARY_NAME} PUBLIC al)
    set_target_properties(${THIS_EXTENSION_LIBRARY_NAME} PROPERTIES
      CXX_STANDARD 14
    )
    foreach(CV_LIB "${OpenCV_LIBS}")
        target_link_libraries(${THIS_EXTENSION_LIBRARY_NAME} PUBLIC  "${CV_LIB}")
    endforeach()
    target_compile_definitions(${THIS_EXTENSION_LIBRARY_NAME} PUBLIC AL_EXT_OPENVR)

    include_directories(${OpenCV_INCLUDE_DIRS})
    target_include_directories(${THIS_EXTENSION_LIBRARY_NAME} PUBLIC "${OPENCV_INCLUDE_DIRS}")


    if(BUILD_EXAMPLES)
        message("Building OpenCV examples")
        BuildExamples("${CMAKE_CURRENT_LIST_DIR}/examples" ${THIS_EXTENSION_LIBRARY_NAME} al_opencv)
    endif(BUILD_EXAMPLES)

#  # unit tests
#  add_executable(soundfileBufferedRecordTests ${CMAKE_CURRENT_LIST_DIR}/unitTests/utSoundfileBufferedRecord.cpp)
#  target_link_libraries(soundfileBufferedRecordTests al ${SPATIALAUDIO_LINK_LIBRARIES} )
#  target_include_directories(soundfileBufferedRecordTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/catch")
#  set_target_properties(soundfileBufferedRecordTests PROPERTIES
#    CXX_STANDARD 14
#    CXX_STANDARD_REQUIRED ON
#    )
#  add_test(NAME soundfileBufferedRecordTests
#    COMMAND $<TARGET_FILE:soundfileBufferedRecordTests> ${TEST_ARGS})

#  # Connections to outside
#  set(CURRENT_EXTENSION_LIBRARIES ${THIS_EXTENSION_LIBRARY_NAME} ${THIS_EXTENSION_LIBRARIES})
#  set(CURRENT_EXTENSION_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR} ${ASSIMP_INCLUDE_DIR})

#  set(CURRENT_EXTENSION_DEFINITIONS -DAL_EXT_ASSETS3D)
#  get_filename_component(ASSIMP_ROOT_DIR "${ASSIMP_LIBRARY}" DIRECTORY)
#  set(CURRENT_EXTENSION_DLLS "${ASSIMP_ROOT_DIR}/../../bin/x64/assimp-vc140-mt.dll")

endif()
