include(LibFindMacros)

set( CINDER_FIND_PATHS ${CINDER_ROOT}
                       $ENV{CINDER_ROOT}/CMake
                       ${CINDER_ROOT}
                       /usr/local
                       /usr/opt
   )


find_path( CINDER_INCLUDE_DIR 
           NAMES Cinder.h 
           PATH_SUFFIXES cinder
           PATHS $ENV{CINDER_ROOT}/include
)

find_library( CINDER_LIBRARY
              NAMES cinderRelease 
              PATH_SUFFIXES libs 
              PATHS $ENV{CINDER_ROOT}/CMake
)

find_library( CINDER_LIBRARY_DEBUG 
              NAMES cinderDebug
              PATH_SUFFIXES libs
              PATHS $ENV{CINDER_ROOT}/CMake
)

    set( CINDER_PROCESS_INCLUDES CINDER_INCLUDE_DIR )
    set( CINDER_PROCESS_LIBS CINDER_LIBRARY )
    set( CINDER_PROCESS_LIBS_DEBUG CINDER_LIBRARY_DEBUG )
    libfind_process(CINDER)
    libfind_process(CINDER_DEBUG)
