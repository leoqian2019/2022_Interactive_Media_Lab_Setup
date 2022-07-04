
set(OGRE_SDK "C:\\Users\\Andres\\source\\repos\\ogre-sdk-v2.2.5-vc19-x64\\sdk"
    CACHE STRING "Path to the OGRE SDK for Windows")

list(APPEND CMAKE_MODULE_PATH
${OGRE_SDK}/cmake
)

# Needed for iOS
#set( CMAKE_TOOLCHAIN_FILE CMake/iOS.cmake )
find_package(OGRE 2.2)

include("../BuildExamples.cmake")

if(OGRE_FOUND)
  set(THIS_EXTENSION_LIBRARY_NAME al_ogre3d)

  set(THIS_EXTENSION_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/al_Ogre3D.hpp
  )
  set(THIS_EXTENSION_SRC
    "${CMAKE_CURRENT_LIST_DIR}/src/al_Ogre3D.cpp"
  )

  if( APPLE )
      # Test if this is needed and it works
          file( GLOB_RECURSE RESOURCES ./src/*.storyboard )
          set( RESOURCES ${RESOURCES} ./Data/Resources.cfg ./bin/Data )
  endif()

  add_library(${THIS_EXTENSION_LIBRARY_NAME} ${THIS_EXTENSION_SRC} ${THIS_EXTENSION_HEADERS})

  # c++14 It's very important to set this for the extension as not
  # setting it will cause related targets to not use c++14
  set_target_properties(${THIS_EXTENSION_LIBRARY_NAME} PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    )


  target_link_libraries(${THIS_EXTENSION_LIBRARY_NAME} al ${THIS_EXTENSION_LIBRARIES} ${OGRE_LIBRARIES} ${OGRE_HlmsPbs_LIBRARY} ${OGRE_HlmsUnlit_LIBRARY})

  if (AL_WINDOWS)
#    if (${CMAKE_BUILD_TYPE} MATCHES Debug)
#        target_link_libraries(${THIS_EXTENSION_LIBRARY_NAME} al ${THIS_EXTENSION_LIBRARIES} ${OGRE_LIBRARIES} ${OGRE_BINARY_DBG})
#    else()
#        target_link_libraries(${THIS_EXTENSION_LIBRARY_NAME} al ${THIS_EXTENSION_LIBRARIES} ${OGRE_LIBRARIES} ${OGRE_BINARY_REL})
#    endif()
  endif (AL_WINDOWS)
  target_include_directories(${THIS_EXTENSION_LIBRARY_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})
  target_include_directories(${THIS_EXTENSION_LIBRARY_NAME} PUBLIC ${OGRE_INCLUDE_DIRS}
      "${OGRE_INCLUDE_DIRS}/OGRE/Hlms/Unlit"
      "${OGRE_INCLUDE_DIRS}/OGRE/Hlms/Common"
      "${OGRE_INCLUDE_DIRS}/OGRE/Hlms/Pbs"
      )

#  # Connections to outside
  set(CURRENT_EXTENSION_LIBRARIES ${THIS_EXTENSION_LIBRARY_NAME} ${THIS_EXTENSION_LIBRARIES})
  set(CURRENT_EXTENSION_INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})



  if( APPLE )
      # Test if this is needed and it works
          set_target_properties( ${THIS_EXTENSION_LIBRARY_NAME} PROPERTIES XCODE_ATTRIBUTE_ENABLE_BITCODE "NO" )
#          set_target_properties( ${THIS_EXTENSION_LIBRARY_NAME} PROPERTIES RESOURCE "${RESOURCES}" )
#          set_target_properties( ${THIS_EXTENSION_LIBRARY_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist )
          #set_target_properties( ${PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_ICON_FILE SampleBrowser_OSX.icns)
          set( CMAKE_EXE_LINKER_FLAGS "-framework Foundation -framework CoreGraphics -framework QuartzCore -framework UIKit -framework Metal -framework MetalKit -framework ModelIO" )
  endif()



  if(BUILD_EXAMPLES)
    BuildExamples("${CMAKE_CURRENT_LIST_DIR}/examples" ${THIS_EXTENSION_LIBRARY_NAME} "")
  endif(BUILD_EXAMPLES)
endif(OGRE_FOUND)
#  # unit tests

##  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/catch")
##      add_executable(convolverTests ${CMAKE_CURRENT_LIST_DIR}/unitTests/utConvolver.cpp)
##      target_link_libraries(convolverTests al ${SPATIALAUDIO_LINK_LIBRARIES} )
##      target_include_directories(convolverTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/catch")
##      add_test(NAME convolverTests
##          COMMAND $<TARGET_FILE:convolverTests> ${TEST_ARGS})
##      set_target_properties(convolverTests PROPERTIES
##          CXX_STANDARD 14
##          CXX_STANDARD_REQUIRED ON
##          )

##      #  add_memcheck_test(convolverTests)

##      add_executable(decorrelationTests ${CMAKE_CURRENT_LIST_DIR}/unitTests/utDecorrelation.cpp)
##      target_link_libraries(decorrelationTests al ${SPATIALAUDIO_LINK_LIBRARIES})
##      target_include_directories(decorrelationTests PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/external/catch")
##      add_test(NAME decorrelationTests
##          COMMAND $<TARGET_FILE:decorrelationTests> ${TEST_ARGS})
##      set_target_properties(decorrelationTests PROPERTIES
##          CXX_STANDARD 14
##          CXX_STANDARD_REQUIRED ON
##          )
##      #  add_memcheck_test(decorrelationTests)

##  endif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/external/catch")


