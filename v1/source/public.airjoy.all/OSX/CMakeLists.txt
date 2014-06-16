CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(DemoForAirJoy CXX)

#---------------------------------------------------------------------------------------
#
# cxx flags
#
#---------------------------------------------------------------------------------------
IF(WIN32)
	ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
ELSE(WIN32)
	ADD_DEFINITIONS(-fPIC)
ENDIF(WIN32)

IF(APPLE)
    MESSAGE(STATUS "build demo for airjoy for osx")
	ADD_DEFINITIONS(-D__MAC_OSX__)
ENDIF(APPLE)

#---------------------------------------------------------------------------------------
#
# include dir
#
#---------------------------------------------------------------------------------------
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

#--------------------------------------------------------------------------------------
#
# library dir
#
#--------------------------------------------------------------------------------------
SET(LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
LINK_DIRECTORIES(${LIB_DIR})

#--------------------------------------------------------------------------------------
#
# lib
#
#--------------------------------------------------------------------------------------
IF (WIN32)
ELSE (WIN32)
ENDIF (WIN32)

#-------------------------------------------------------------------------------------
# set output dir
#-------------------------------------------------------------------------------------
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)

#-------------------------------------------------------------------------------------
#
# project
#
#-------------------------------------------------------------------------------------
ADD_SUBDIRECTORY(testAirPlayServer)
ADD_SUBDIRECTORY(testAirTunesServer)
ADD_SUBDIRECTORY(testAirJoyServer)
ADD_SUBDIRECTORY(testAirJoyClient)
