#-------------------------------------------------------------------------------------
#
# AirJoy
#
#-------------------------------------------------------------------------------------

ADD_SUBDIRECTORY(AirJoyAll)
ADD_SUBDIRECTORY(AirCommon)
ADD_SUBDIRECTORY(AirUtil)
ADD_SUBDIRECTORY(AirSdp)
ADD_SUBDIRECTORY(AirUmp)
ADD_SUBDIRECTORY(3rdpartSrc)

IF(APPLE)
  ADD_SUBDIRECTORY(AirNsd)
ELSE(APPLE)
  ADD_SUBDIRECTORY(AirNsd)
ENDIF(APPLE)
