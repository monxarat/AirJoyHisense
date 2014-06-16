ADD_DEFINITIONS(-DAIRJOY_STATIC)

ADD_EXECUTABLE(testUmp testUmp.cpp)
TARGET_LINK_LIBRARIES(testUmp AirUmp_static AirCommon_static AirUtil_static ${OS_LIB})
