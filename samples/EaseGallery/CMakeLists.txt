cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

### Check to see if the environment is set. 
### The path to the openFrameworks root dir is a prerequisite
set (CINDER_ROOT $ENV{CINDER_ROOT})
if ( "${CINDER_ROOT}" STREQUAL "" )
	message(FATAL_ERROR "
	**** 
	You must export the env variable CINDER_ROOT pointing to your Cinder root directory in order to proceed.. 
	Example: export CINDER_ROOT=/some/dir 
	Or you can add it permantly by modifying your .bashrc file 	
	NOTE: Always use absolute path to point to your Cinder installation
	****" )
endif()

include (/home/petroska/Software/CinderLinux/CMakeModules/CinderInclude.cmake)
include (/home/petroska/Software/CinderLinux/CMakeModules/CinderSrcs.cmake)
include (/home/petroska/Software/CinderLinux/CMakeModules/CinderDepends.cmake)

### Project name.
PROJECT( CinderLinuxEaseGalleryApp ) 

set ( CMAKE_COLOR_MAKEFILE ON )
set ( CMAKE_VERBOSE_MAKEFILE ON )

set ( SOURCE_CODE_DIRECTORY ${CMAKE_SOURCE_DIR} )
set ( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

### Set various compiler flags depending on the target
if( CMAKE_COMPILER_IS_GNUCXX )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++11 -Wall -Wextra -Winvalid-pch -Wnon-virtual-dtor -Wsign-promo -Wshadow -Winit-self -Wno-unknown-pragmas -Wno-unused-parameter -Wno-write-strings" )
    set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -Wuninitialized" )
endif( CMAKE_COMPILER_IS_GNUCXX )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include ${CINDER_INCLUDE})


list (APPEND source_files src/EaseGalleryApp.cpp)


find_library(   CINDER_DEBUG 
                NAMES cinderDebug
                PATH_SUFFIXES CMake/libs libs
                PATHS $ENV{CINDER_ROOT}
            )

find_library(   CINDER_RELEASE 
                NAMES cinderRelease
                PATH_SUFFIXES CMake/libs libs
                PATHS $ENV{CINDER_ROOT}
            )

find_library(   GLFW3 
                NAMES glfw3
                PATH_SUFFIXES CMake/libs libs
                PATHS $ENV{CINDER_ROOT}
            )

if( CINDER_DEBUG AND NOT CINDER_RELEASE )
    set( CINDER_RELEASE ${CINDER_DEBUG} )
elseif( CINDER_RELEASE AND NOT CINDER_DEBUG)
    set( CINDER_DEBUG ${CINDER_RELEASE} )
elseif( NOT CINDER_DEBUG AND NOT CINDER_RELEASE )
    message(FATAL_ERROR " Neither Debug or Release build of Cinder could be found !! ")
endif( CINDER_DEBUG AND NOT CINDER_RELEASE )


if(CMAKE_BUILD_TYPE MATCHES Debug)
    set( bin ${PROJECT_NAME}_D )
else()
    set( bin ${PROJECT_NAME} )
endif()

add_executable( ${bin} ${source_files} )

set( CINDER debug ${CINDER_DEBUG} optimized ${CINDER_RELEASE} )

target_link_libraries( ${bin} ${CINDER} ${GLFW3}  Xxf86vm pthread Xrandr Xi Xcursor ${CINDER_DEPENDS}) 
