### ROOT PROJECT CMakeLists.txt ###

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

### Project name.
PROJECT(CinderLinux)

set (CMAKE_COLOR_MAKEFILE ON)
set (CMAKE_VERBOSE_MAKEFILE OFF)

set(CINDERLINUX_MAJOR_VERSION 0)
set(CINDERLINUX_MINOR_VERSION 8)
set(CINDERLINUX_PATCH_VERSION 6)
set(CINDERLINUX_VERSION ${CINDERLINUX_MAJOR_VERSION}.${CINDERLINUX_MINOR_VERSION}.${CINDERLINUX_PATCH_VERSION})

### The variables that are being set here propagate along the source tree and are available to subdir CMakeLists.txt files. ###

### Convenient macro to add srcs from child directories directly
### Found : http://stackoverflow.com/questions/7046956/populating-srcs-from-cmakelists-txt-in-subdirectories
set ( SOURCE_CODE_DIRECTORY "${CMAKE_SOURCE_DIR}/src" )
macro (add_sources)
	file (RELATIVE_PATH _relPath "${SOURCE_CODE_DIRECTORY}" "${CMAKE_CURRENT_SOURCE_DIR}")
	message(STATUS "${CMAKE_SOURCE_DIR}")
	foreach (_src ${ARGN})
		if (_relPath)
		   list(APPEND source_files "${_relPath}/${_src}")
		else ()
		   list(APPEND source_files "${_src}")
		endif ()
	endforeach()
	if (_relPath)
		#propagate source_files to parent directory
		set (source_files ${source_files} PARENT_SCOPE)
	endif()
endmacro()

### Check to see if the environment is set. 
### The path to cinder's root dir is a prerequisite
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

### Where to find the various .cmake custom scripts like FindLib.cmake etc.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

### The directory where your executable will be installed when running : make install 
#set(CMAKE_INSTALL_PREFIX "/home/$ENV{USER}/Desktop/${PROJECT_NAME}")

### Where we want our executable to be placed during the build phase
### According to the doc cmake_archive_output_directory should be used for
### static libs.
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/CMake/libs/)

### Triggered by : make install . This will copy the binary
### folder and all subfolders in the directory specified by CMAKE_INSTALL_PREFIX
### If the dir doesnt exist it will be created automatically .
#install ( DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#	  DESTINATION "${CMAKE_INSTALL_PREFIX}"
#	  PATTERN ".svn" EXCLUDE
#	  PATTERN ".git" EXCLUDE
#	)

#set (dataDirectory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data")
#file(MAKE_DIRECTORY ${dataDirectory})

### This command here copies the contents of the root data folder to the build target data folder so it can be used from the application.
### This command is executed whenever you run make TARGET for the first time or when you run make releaseConfig / make debugConfig.
#execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/data ${dataDirectory})

if ( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Debug CACHE STRING 
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel. " FORCE )
endif()


### Set various compiler flags depending on the target
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Winvalid-pch -Wnon-virtual-dtor -Wsign-promo -Wshadow -Winit-self -Wno-unknown-pragmas -Wno-unused-parameter -Wno-write-strings")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++11 -Wuninitialized")
endif(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

### Add source subdirectory so that cmake propagates through the source tree and can find the various CMakeLists.txt files included in the src subdirs.
add_subdirectory ( lib/linux/glfw )
add_subdirectory ( src )
