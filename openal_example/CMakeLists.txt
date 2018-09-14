project( play_wav )
cmake_minimum_required( VERSION 3.0 )

file( GLOB oal_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.[ch]pp )

#add_definitions( -DLIBAUDIO=1 )

find_package( OpenAL REQUIRED )
include_directories( ${OPENAL_INCLUDE_DIR} )

add_executable( ${PROJECT_NAME} ${oal_sources} )
target_link_libraries( ${PROJECT_NAME} ${OPENAL_LIBRARY} )
set_target_properties( ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES )

add_executable( example examples/example.cpp )
target_link_libraries( example ${OPENAL_LIBRARY} )
