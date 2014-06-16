ADD_EXECUTABLE(testPlist testPlist.cpp)
TARGET_LINK_LIBRARIES(testPlist Plist_static AirCommon_static ${OS_LIB})

