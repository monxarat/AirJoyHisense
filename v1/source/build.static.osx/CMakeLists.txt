CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
PROJECT(AirJoy CXX)

#---------------------------------------------------------------------------------------
# for OSX
#---------------------------------------------------------------------------------------
#SET(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/usr/bin/llvm-g++)
SET(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/usr/bin/llvm-g++)
#ADD_DEFINITIONS(-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.dk)

#---------------------------------------------------------------------------------------
# for ios/armv7 (xcode 4.3.2 - SDK: IOS5.1)
#---------------------------------------------------------------------------------------
#SET(CMAKE_CXX_COMPILER /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/usr/bin/gcc)
#ADD_DEFINITIONS(-arch armv7 -mthumb)
#ADD_DEFINITIONS(-isysroot /Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS5.1.sdk)

#---------------------------------------------------------------------------------------
#
# cxx flags
#
#---------------------------------------------------------------------------------------
ADD_DEFINITIONS(-D__MAC_OSX__)
ADD_DEFINITIONS(-fPIC)
SET(ROOT_DIR "${PROJECT_SOURCE_DIR}/..")

#---------------------------------------------------------------------------------------
#
# include dir
#
#---------------------------------------------------------------------------------------
SET(BOOST_DIR ${ROOT_DIR}/boost_1_49_0)
INCLUDE_DIRECTORIES(${BOOST_DIR})
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/3rdpartInc)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/3rdpartSrc/PlistCpp/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirCommon/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirUtil/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirSdp/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirUmp/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirPlayX/AirPlay/server/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/server/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoy/client/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirJoyX/AirJoyNsd/include)
INCLUDE_DIRECTORIES(${ROOT_DIR}/src/AirJoyAll/AirStreamX/AirFileServer/include)

#-------------------------------------------------------------------------------------
# set output dir
#-------------------------------------------------------------------------------------
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/output)
MESSAGE(STATUS "executable output:  ${EXECUTABLE_OUTPUT_PATH}")
MESSAGE(STATUS "library output: ${LIBRARY_OUTPUT_PATH}")

#-------------------------------------------------------------------------------------
#
# libairplay.a
#
#-------------------------------------------------------------------------------------

SET(SRC_AIRCOMMON   ${ROOT_DIR}/src/AirCommon/src/BoostSystemPrebuild.cpp
                    ${ROOT_DIR}/src/AirCommon/src/BoostFileSystemPrebuild.cpp
                    ${ROOT_DIR}/src/AirCommon/src/BoostDateTimePrebuild.cpp
                    ${ROOT_DIR}/src/AirCommon/src/BoostThreadPrebuild.cpp
                    ${ROOT_DIR}/src/AirCommon/src/UmpMessage.cpp
                    ${ROOT_DIR}/src/AirCommon/src/HttpMessage.cpp
                    ${ROOT_DIR}/src/AirCommon/src/HttpTextParam.cpp)

SET(SRC_AIRUTIL     ${ROOT_DIR}/src/AirUtil/src/UtilBase64.cpp
                    ${ROOT_DIR}/src/AirUtil/src/UtilUtf8.cpp
                    ${ROOT_DIR}/src/AirUtil/src/UtilItoa.cpp
                    ${ROOT_DIR}/src/AirUtil/src/UtilMd5.cpp
                    ${ROOT_DIR}/src/AirUtil/src/UtilType.cpp)

SET(SRC_AIRUMP      ${ROOT_DIR}/src/AirUmp/src/TcpClient.cpp
                    ${ROOT_DIR}/src/AirUmp/src/TcpSession.cpp
                    ${ROOT_DIR}/src/AirUmp/src/UmpClient.cpp
                    ${ROOT_DIR}/src/AirUmp/src/UmpServer.cpp)

SET(SRC_PLIST       ${ROOT_DIR}/src/3rdpartSrc/PlistCpp/src/pugixml.cpp)

SET(SRC_AIRPLAY     ${ROOT_DIR}/src/AirJoyAll/AirPlayX/AirPlay/server/src/AirPlayServer.cpp
                    ${ROOT_DIR}/src/AirJoyAll/AirPlayX/AirPlay/server/src/AirPlayServerCore.cpp
                    ${ROOT_DIR}/src/AirJoyAll/AirPlayX/AirPlay/server/src/AirPlayServerHandler.cpp)

ADD_LIBRARY(AirPlay_static STATIC ${SRC_AIRCOMMON}
                                  ${SRC_AIRUTIL}
                                  ${SRC_AIRUMP}
                                  ${SRC_PLIST}
                                  ${SRC_AIRPLAY})

SET_TARGET_PROPERTIES(AirPlay_static PROPERTIES OUTOUT_NAME "airplay")

#-------------------------------------------------------------------------------------
#
# testAirPlayServer
#
#-------------------------------------------------------------------------------------
SET(SRC_TEST_AIRPLAY_SERVER ${ROOT_DIR}/src/AirJoyAll/AirPlayX/testAirPlayServer/testAirPlayServer.cpp)
ADD_EXECUTABLE(testAirPlayServer ${SRC_TEST_AIRPLAY_SERVER})
TARGET_LINK_LIBRARIES(testAirPlayServer AirPlay_static)
