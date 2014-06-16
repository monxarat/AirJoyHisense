ADD_EXECUTABLE(testLibao testLibao.cpp)

IF(WIN32)
TARGET_LINK_LIBRARIES(testLibao libao_static ${OS_LIB})
ELSE(WIN32)
TARGET_LINK_LIBRARIES(testLibao ${AO_LIB} ${OS_LIB})
ENDIF(WIN32)

