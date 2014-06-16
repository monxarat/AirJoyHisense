ADD_EXECUTABLE(testNsd testNsd.cpp)
TARGET_LINK_LIBRARIES(testNsd AirNsd_static AirCommon_static ${DNSSD_LIB} ${OS_LIB})
