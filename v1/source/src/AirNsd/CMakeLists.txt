#-------------------------------------------------------------------------------------
#
# module:AirNsd
#
#-------------------------------------------------------------------------------------

SET(NSD_SRC    include/Nsd.hpp
               include/NsdDiscoveryHandler.hpp
               include/NsdPublishHandler.hpp
               include/NsdServiceInfo.hpp
               src/Nsd.cpp)

ADD_LIBRARY(AirNsd_static STATIC ${NSD_SRC})

ADD_SUBDIRECTORY(test)

