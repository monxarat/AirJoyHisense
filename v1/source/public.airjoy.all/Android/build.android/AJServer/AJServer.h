
/*
 * Copyright (C), 2011-2212, AirJoy
 *
 * File name:  	AJServer.cpp
 * Author:     	SangWencheng
 * Version:    	V1.0
 * Date: 		2012-11-01
 *
 * Description:    
 * 		AirJoy实现了核心库Java到C++的JNI调用
 *
 * History:        
 * 	1. 	Date:
 * 		Author:
 * 		Modification:
 * 	2. ...
 */

#ifndef __AJSERVER_H__
#define __AJSERVER_H__


#include <stdio.h>
#include <stdlib.h>

#include "AirJoyNsd.hpp"
#include "AirJoyServer.hpp"

#include "AJDiscoveryListener.h"
#include "AJMediaListener.h"
#include "AJState.h"
//#include "AJPublishListener.h"


#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "libJNIAirPlay"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


using namespace airjoy;

class AJServer
{
	public:
		AJServer(JNIEnv *env, jobject obj);
		virtual ~AJServer();

		JNIEnv *getEnv();
		jobject getObj();
		JavaVM *getVM();
		
		void setPhotoListenerObj(jobject listener);
		void setVideoListenerObj(jobject listener);
		void setAudioListenerObj(jobject listener);
		void setMediaListenerObj(jobject listener);
		void setNsdDiscoveryListenerObj(jobject listener);
		void setNsdPublishListenerObj(jobject listener);

		jobject getPhotoListenerObj();
		jobject getVideoListenerObj();
		jobject getAudioListenerObj();
		jobject getMediaListenerObj();
		jobject getNsdDiscoveryListenerObj();
		jobject getNsdPublishListenerObj();

		jobject newServerinfoObj(JNIEnv *env, AirJoyServiceInfo &serviceInfo);

		AirJoyServer *getAirJoyServer();
		AirJoyServiceInfo *getAirJoyServerInfo();
		AirJoyNsd *getAirJoyNsd();
		AJDiscoveryListener *getAJDiscoveryListener();
//		AJPublishListener *getAJPublishListener();
		AJMediaListener *getAJMediaListener();
		AJState *getAJState();

		jclass getServerinfoClass();

 	private:
		JavaVM 					*m_jvm;
		JNIEnv 					*m_env;
		jobject 				m_obj;

		AirJoyServer 			*m_AirJoyServer;
	    AirJoyServiceInfo	    *m_AirJoyServerInfo;
		AirJoyNsd 				*m_AirJoyNsd;

		AJDiscoveryListener 	*m_AJDiscoveryListener;
//		AJPublishListener 		*m_AJPublishListener;
		AJMediaListener 		*m_AJMediaListener;
		AJState 				*m_AJState;

		jobject 				m_photo_listener_obj;
		jobject 				m_video_listener_obj;
		jobject 				m_audio_listener_obj;
		jobject 				m_media_listener_obj;
		jobject 				m_discovery_listener_obj;
		jobject 				m_publish_listener_obj;

		jclass 					m_serverinfo_cls;
};

extern AJServer * g_AJServer;

#endif
