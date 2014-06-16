#-------------------------------------------------------------------------------------
#
# module:AirStreamX
#
#-------------------------------------------------------------------------------------

SET(AirFileServer_SRC   AirFileServer/include/AirFileServer.hpp
                        AirFileServer/include/AirHttpServer.hpp
                        AirFileServer/include/mongoose.hpp
                        AirFileServer/src/AirFileServer.cpp
                        AirFileServer/src/AirHttpServer.cpp
                        AirFileServer/src/mongoose.cpp)

ADD_LIBRARY(AirStream_shared SHARED ${AirFileServer_SRC})
SET_TARGET_PROPERTIES(AirStream_shared PROPERTIES OUTPUT_NAME "airstream")
TARGET_LINK_LIBRARIES(AirStream_shared AirCommon_static
                                       AirUtil_static
                                       ${OS_LIB})
