#-------------------------------------------------------------------------------------
#
# module: AirUtil
#
#-------------------------------------------------------------------------------------
SET(UTIL_SRC include/UtilBase64.hpp
             include/UtilUtf8.hpp
             include/UtilItoa.hpp
             include/UtilMd5.hpp
             include/UtilType.hpp
			 include/UtilString.hpp
			 src/UtilBase64.cpp
			 src/UtilUtf8.cpp
             src/UtilItoa.cpp
             src/UtilMd5.cpp
             src/UtilType.cpp
			 src/UtilString.cpp)

ADD_LIBRARY(AirUtil_static STATIC ${UTIL_SRC})
