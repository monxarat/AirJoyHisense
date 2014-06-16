#-------------------------------------------------------------------------------------
#
# module: PlistCpp 
#
#-------------------------------------------------------------------------------------

SET(PLIST_INC ${CMAKE_CURRENT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PLIST_INC})

SET(PLIST_SRC include/pugixml.hpp
              include/pugiconfig.hpp
              src/pugixml.cpp)

ADD_LIBRARY(Plist_static STATIC ${PLIST_SRC})

ADD_SUBDIRECTORY(test)
