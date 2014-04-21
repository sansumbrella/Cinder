include (${CMAKE_MODULE_PATH}/CinderInclude.cmake)
include (${CMAKE_MODULE_PATH}/CinderSrcs.cmake)

include_directories ( ${CINDER_INCLUDE} )

if(CMAKE_BUILD_TYPE MATCHES Debug)
	add_library(cinderDebug ${srcsAntTweakBar} ${srcsJsonCpp} ${srcsLineBreak} ${srcsLibTess2} ${srcsCinder})
else()
	add_library(cinderRelease ${srcsAntTweakBar} ${srcsJsonCpp} ${srcsLineBreak} ${srcsLibTess2} ${srcsCinder})
endif()
