/*
* AirJoyLog.hpp
*
* coding.tom@gmail.com
*
* 2012-6-8
*
*/

#ifndef __AIRJOY_LOG_H__
#define __AIRJOY_LOG_H__

#ifdef __ANDROID__
#include <android/log.h>
#define LOG_TAG "libairjoy"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
#define LOGI(...)
#define LOGD(...)
#endif // __ANDROID__




#endif // __AIRJOY_LOG_H__
