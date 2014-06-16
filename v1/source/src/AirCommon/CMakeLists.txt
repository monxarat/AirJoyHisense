#-------------------------------------------------------------------------------------
#
# module: AirCommon
#
#-------------------------------------------------------------------------------------
SET(COMMON_SRC include/AirJoyTypeDef.hpp
               include/AirJoyApi.hpp
               include/AirJoyTimer.hpp
               include/UmpMessage.hpp
               include/HttpMessage.hpp
               include/HttpMessageType.hpp
               include/HttpTextParam.hpp
               include/XmlParser.hpp
               src/BoostSystemPrebuild.cpp
               src/BoostFileSystemPrebuild.cpp
               src/BoostDateTimePrebuild.cpp
               src/BoostThreadPrebuild.cpp
               src/UmpMessage.cpp
               src/HttpMessage.cpp
               src/HttpMessageType.cpp
               src/HttpTextParam.cpp)

ADD_LIBRARY(AirCommon_static STATIC ${COMMON_SRC})
