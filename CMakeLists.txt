CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
project(pp)

#need to include FindOPENCL.cmake to
SET( CMAKE_MODULE_PATH ${pp_SOURCE_DIR})

message("CMake module path: ${CMAKE_MODULE_PATH}\n")

include($ENV{OpenCV_DIR}/OpenCVConfig.cmake)

#Need some OpenGL packages
FIND_PACKAGE(OpenGL)
FIND_PACKAGE(GLUT)
FIND_PACKAGE(GLEW)

INCLUDE_DIRECTORIES( 
    ${GLUT_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIR}
    ${GLEW_INCLUDE_PATH}
	${OPENCV_INCLUDE_DIR}
)

message("include directories: \n")
get_directory_property(includes INCLUDE_DIRECTORIES)
message("${includes}\n")

#build in debug mode
ADD_DEFINITIONS(-g)

#create the executable
SET(EXEC pp.x)
ADD_EXECUTABLE(${EXEC} main.cpp util.cpp texMapping.cpp)

TARGET_LINK_LIBRARIES (${EXEC}
   ${GLUT_LIBRARIES}
   ${OPENGL_LIBRARIES}
   ${GLEW_LIBRARY}
   ${OpenCV_LIBS}
)
