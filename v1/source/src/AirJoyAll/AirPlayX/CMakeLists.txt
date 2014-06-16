#-------------------------------------------------------------------------------------
#
# module:AirPlayX
#
#-------------------------------------------------------------------------------------

SET(PLIST_INC ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdpartSrc/PlistCpp/include)
INCLUDE_DIRECTORIES(${PLIST_INC})

#---------------------------------------------------------------------------------------
# Common
#---------------------------------------------------------------------------------------
SET(Common_SRC      Common/include/AirPlayServiceInfo.hpp
                    Common/include/AirTunesServiceInfo.hpp
                    Common/src/AirPlayServiceInfo.cpp
                    Common/src/AirTunesServiceInfo.cpp)

#---------------------------------------------------------------------------------------
# Nsd
#---------------------------------------------------------------------------------------
SET(AirPlayNsd_SRC    AirPlayNsd/include/AirPlayNsd.hpp
                      AirPlayNsd/include/AirPlayNsdCore.hpp
                      AirPlayNsd/include/AirPlayNsdDiscoveryHandler.hpp
                      AirPlayNsd/include/AirPlayNsdDiscoveryListener.hpp
                      AirPlayNsd/include/AirPlayNsdPublishHandler.hpp
                      AirPlayNsd/include/AirPlayNsdPublishListener.hpp
                      AirPlayNsd/src/AirPlayNsd.cpp
                      AirPlayNsd/src/AirPlayNsdCore.cpp
                      AirPlayNsd/src/AirPlayNsdDiscoveryHandler.cpp
                      AirPlayNsd/src/AirPlayNsdPublishHandler.cpp)

#---------------------------------------------------------------------------------------
#
# AirPlay
#
#---------------------------------------------------------------------------------------
SET(AirPlayServer_SRC    AirPlay/server/include/AirPlayServerDefault.hpp
                         AirPlay/server/include/AirPlayServer.hpp
                         AirPlay/server/include/AirPlayServerCore.hpp
                         AirPlay/server/include/AirPlayServerHandler.hpp
                         AirPlay/server/include/AirPlayServerListener.hpp
                         AirPlay/server/include/AirPlayEvent.hpp
                         AirPlay/server/src/AirPlayServer.cpp
                         AirPlay/server/src/AirPlayServerCore.cpp
                         AirPlay/server/src/AirPlayServerHandler.cpp
                         AirPlay/server/src/AirPlayEvent.cpp)

SET(AirMirroringServer_SRC   AirPlay/server/include/AirMirroringServerDefault.hpp
                             AirPlay/server/include/AirMirroringServer.hpp
                             AirPlay/server/include/AirMirroringServerCore.hpp
                             AirPlay/server/include/AirMirroringServerHandler.hpp
                             AirPlay/server/include/AirMirroringServerListener.hpp
                             AirPlay/server/src/AirMirroringServer.cpp
                             AirPlay/server/src/AirMirroringServerCore.cpp
                             AirPlay/server/src/AirMirroringServerHandler.cpp)

IF(APPLE)
    ADD_LIBRARY(AirPlay_shared SHARED ${Common_SRC} ${AirPlayServer_SRC} ${AirMirroringServer_SRC})
    SET_TARGET_PROPERTIES(AirPlay_shared PROPERTIES OUTPUT_NAME "airplay")
    TARGET_LINK_LIBRARIES(AirPlay_shared AirUmp_static 
                                         AirCommon_static 
                                         AirUtil_static
                                         Plist_static
                                         ${OS_LIB})
ELSE(APPLE)
    ADD_LIBRARY(AirPlay_shared SHARED ${Common_SRC} ${AirPlayServer_SRC} ${AirMirroringServer_SRC} ${AirPlayNsd_SRC})
    SET_TARGET_PROPERTIES(AirPlay_shared PROPERTIES OUTPUT_NAME "airplay")
    TARGET_LINK_LIBRARIES(AirPlay_shared AirUmp_static 
                                         AirCommon_static 
                                         AirUtil_static
                                         AirNsd_static
                                         Plist_static
                                         ${OS_LIB}
                                         ${DNSSD_LIB})
ENDIF(APPLE)

#---------------------------------------------------------------------------------------
#
# AirTunes
#
#---------------------------------------------------------------------------------------
SET(ALAC_SRC             AirTunes/server/include/stdint_win.h
                         AirTunes/server/include/alac.h
                         AirTunes/server/src/alac.cpp)
   
ADD_LIBRARY(libalac_static STATIC ${ALAC_SRC})
SET_TARGET_PROPERTIES(libalac_static PROPERTIES OUTPUT_NAME "libalac")

IF(WIN32)
INCLUDE_DIRECTORIES(AirTunes/libao_win32)

SET(AirTunesServer_SRC   AirTunes/server/include/AirTunesServerDefault.hpp
                         AirTunes/server/include/AirTunesServer.hpp
                         AirTunes/server/include/AirTunesServerCore.hpp
                         AirTunes/server/include/AirTunesServerHandler.hpp
                         AirTunes/server/include/AirTunesServerListener.hpp
                         AirTunes/server/include/AirCrypto.hpp
                         AirTunes/server/include/AirBase64.hpp
						 AirTunes/server/include/RtspMessage.hpp
                         AirTunes/server/include/AirPort.hpp
                         AirTunes/server/include/AirPortControlHandler.hpp
                         AirTunes/server/include/AirPortDataHandler.hpp
                         AirTunes/server/include/AirPortTimingHandler.hpp
                         AirTunes/server/include/alac.h
						 AirTunes/server/include/dmap_parser.h
						 AirTunes/server/include/Dmap.hpp
                         AirTunes/server/include/AudioBuffer.hpp
                         AirTunes/server/include/AudioDriver.hpp
                         AirTunes/server/include/AudioPlayer.hpp
                         AirTunes/server/include/DacpClient.hpp
                         AirTunes/server/include/DacpClientCore.hpp
                         AirTunes/server/include/DacpClientHandler.hpp
                         AirTunes/server/include/DacpClientListener.hpp
                         AirTunes/server/src/AirTunesServer.cpp
                         AirTunes/server/src/AirTunesServerCore.cpp
                         AirTunes/server/src/AirTunesServerHandler.cpp
                         AirTunes/server/src/AirCrypto.cpp
                         AirTunes/server/src/AirBase64.cpp
						 AirTunes/server/src/RtspMessage.cpp
                         AirTunes/server/src/AirPort.cpp
                         AirTunes/server/src/AirPortControlHandler.cpp
                         AirTunes/server/src/AirPortDataHandler.cpp
                         AirTunes/server/src/AirPortTimingHandler.cpp
                         AirTunes/server/src/dmap_parser.cpp
                         AirTunes/server/src/Dmap.cpp
						 AirTunes/server/src/AudioBuffer.cpp
                         AirTunes/server/src/AudioDriver.cpp
                         AirTunes/server/src/AudioPlayer.cpp
                         AirTunes/server/src/DacpClient.cpp
                         AirTunes/server/src/DacpClientCore.cpp
                         AirTunes/server/src/DacpClientHandler.cpp)

#                         AirTunes/libao_win32/ao/ao.h
#                         AirTunes/libao_win32/ao/ao_private.h
#                         AirTunes/libao_win32/ao/os_types.h

#SET(AO_SRC               AirTunes/libao_win32/ao/ao.h
#                         AirTunes/libao_win32/ao_wmm.cpp)

#ADD_LIBRARY(libao_static STATIC ${AO_SRC})
#SET_TARGET_PROPERTIES(libao_static PROPERTIES OUTPUT_NAME "libao")

ADD_LIBRARY(AirTunes_shared SHARED ${Common_SRC} ${AirTunesServer_SRC} ${AirPlayNsd_SRC})
SET_TARGET_PROPERTIES(AirTunes_shared PROPERTIES OUTPUT_NAME "airtunes")
TARGET_LINK_LIBRARIES(AirTunes_shared AirUmp_static 
                                      AirCommon_static 
                                      AirUtil_static
                                      AirNsd_static
                                      libalac_static
                                      ${OS_LIB}
                                      ${DNSSD_LIB}
                                      ${OPENSSL_LIB})
 
ELSE(WIN32)

SET(AirTunesServer_SRC   AirTunes/server/include/AirTunesServerDefault.hpp
                         AirTunes/server/include/AirTunesServer.hpp
                         AirTunes/server/include/AirTunesServerCore.hpp
                         AirTunes/server/include/AirTunesServerHandler.hpp
                         AirTunes/server/include/AirTunesServerListener.hpp
                         AirTunes/server/include/AirCrypto.hpp
                         AirTunes/server/include/AirBase64.hpp
						 AirTunes/server/include/RtspMessage.hpp
                         AirTunes/server/include/AirPort.hpp
                         AirTunes/server/include/AirPortControlHandler.hpp
                         AirTunes/server/include/AirPortDataHandler.hpp
                         AirTunes/server/include/AirPortTimingHandler.hpp
                         AirTunes/server/include/alac.h
						 AirTunes/server/include/dmap_parser.h
						 AirTunes/server/include/Dmap.hpp
                         AirTunes/server/include/AudioBuffer.hpp
                         AirTunes/server/include/AudioDriver.hpp
                         AirTunes/server/include/AudioPlayer.hpp
                         AirTunes/server/include/DacpClient.hpp
                         AirTunes/server/include/DacpClientCore.hpp
                         AirTunes/server/include/DacpClientHandler.hpp
                         AirTunes/server/include/DacpClientListener.hpp
                         AirTunes/server/src/AirTunesServer.cpp
                         AirTunes/server/src/AirTunesServerCore.cpp
                         AirTunes/server/src/AirTunesServerHandler.cpp
                         AirTunes/server/src/AirCrypto.cpp
                         AirTunes/server/src/AirBase64.cpp
						 AirTunes/server/src/RtspMessage.cpp
                         AirTunes/server/src/AirPort.cpp
                         AirTunes/server/src/AirPortControlHandler.cpp
                         AirTunes/server/src/AirPortDataHandler.cpp
                         AirTunes/server/src/AirPortTimingHandler.cpp
                         AirTunes/server/src/dmap_parser.cpp
                         AirTunes/server/src/Dmap.cpp
						 AirTunes/server/src/AudioBuffer.cpp
                         AirTunes/server/src/AudioDriver.cpp
                         AirTunes/server/src/AudioPlayer.cpp
                         AirTunes/server/src/DacpClient.cpp
                         AirTunes/server/src/DacpClientCore.cpp
                         AirTunes/server/src/DacpClientHandler.cpp)

  #-------------------------------------------------
  # for MacOSX
  #-------------------------------------------------
  IF(APPLE)
    ADD_LIBRARY(AirTunes_shared SHARED ${Common_SRC} ${AirTunesServer_SRC})
    SET_TARGET_PROPERTIES(AirTunes_shared PROPERTIES OUTPUT_NAME "airtunes")
    TARGET_LINK_LIBRARIES(AirTunes_shared AirUmp_static 
                                          AirCommon_static 
                                          AirUtil_static
                                          libalac_static
                                          ${OS_LIB}
                                          ${OPENSSL_LIB})
  #-------------------------------------------------
  # for Linux 
  #-------------------------------------------------
  ELSE(APPLE)
    INCLUDE_DIRECTORIES("/opt/local/include")
    ADD_LIBRARY(AirTunes_shared SHARED ${Common_SRC} ${AirTunesServer_SRC} ${AirPlayNsd_SRC})
    SET_TARGET_PROPERTIES(AirTunes_shared PROPERTIES OUTPUT_NAME "airtunes")
    TARGET_LINK_LIBRARIES(AirTunes_shared AirUmp_static 
                                          AirCommon_static 
                                          AirUtil_static
                                          AirNsd_static
                                          libalac_static
                                          ${OS_LIB}
                                          ${DNSSD_LIB}
                                          ${OPENSSL_LIB}
                                          ${AO_LIB})
  ENDIF(APPLE)

ENDIF(WIN32)

#-------------------------------------------------------------------------------------
#
# test
#
#-------------------------------------------------------------------------------------
IF(WIN32)
  ADD_SUBDIRECTORY(testAirPlayServer)
  ADD_SUBDIRECTORY(testAirTunesServer)
  ADD_SUBDIRECTORY(testAirMirroringServer)
ELSE(WIN32)
  IF(APPLE)
    ADD_SUBDIRECTORY(testAirPlayServer)
    ADD_SUBDIRECTORY(testAirTunesServer)
    ADD_SUBDIRECTORY(testAirMirroringServer)
  ELSE(APPLE)
  ENDIF(APPLE)
ENDIF(WIN32)

#ADD_SUBDIRECTORY(testLibao)
