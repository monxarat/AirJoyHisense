
#ifndef __AJCLIENTENTRY_H__
#define __AJCLIENTENTRY_H__


#include <stdio.h>
#include <stdlib.h>

#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "libJNIAirJoyClient"


#include "AirJoyNsd.hpp"
#include "AirJoyNsdDiscoveryListener.hpp"

#include "AirJoyClient.hpp"
#include "AirMediaClient.hpp"
#include "AirMediaClientQueryListener.hpp"
#include "AirMediaClientAudio.hpp"
#include "AirMediaClientPhoto.hpp"
#include "AirMediaClientVideo.hpp"
#include "AirMediaClientVolume.hpp"
#include "AirFileServer.hpp"



#include "com_fqx_airjoy_client_AJClient.h"

#include "AJDiscoveryListener.h"
#include "AJMediaClientListener.h"
//#include "AJClientEventListener.h"


#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


using namespace airjoy;

class AJClientEntry
{
	public:
		AJClientEntry(JNIEnv *env, jobject obj);
		virtual ~AJClientEntry();

		JNIEnv *getEnv();
		jobject getObj();
		JavaVM *getVM();

		void setDiscoveryListenerObj(jobject listener);
		jobject getDiscoveryListenerObj();

		void setMediaResultListenerObj(jobject listener);
		jobject getMediaResultListenerObj();

		void setConnectServerListenerObj(jobject listener);
		jobject getConnectServerListenerObj();

		void setEventListenerObj(jobject listener);
		jobject getEventListenerObj();

		AirMediaClient *getAirMediaClient();
		AirJoyNsd *getAirJoyNsd();
		AirFileServer *getAirFileServer();
		AJDiscoveryListener *getAJDiscoveryListener();
		AJMediaClientListener *getAJMediaClientListener();

//		AJClientEventListener *getAJClientEventListener();

#if 0
		jobject	newPlaybackObj(JNIEnv *env, AirJoyPlayBackInfo &info);
		jobject newServerinfoObj(JNIEnv *env, AirJoyServiceInfo &ServerInfo);
#endif

		jclass getServerInfoClass();
		jclass getPlayBackClass();

		int getPort();
		std::string getIP();
		void setIPort(const std::string &ip, int port);

	private:
		JavaVM 					*m_jvm;
		JNIEnv 					*m_env;
		jobject 				m_obj;

		AirMediaClient 			*m_AirMediaClient;
		AirJoyNsd 				*m_AirJoyNsd;
		AirFileServer 			*m_AirFileServer;

		AJDiscoveryListener 	*m_AJDiscoveryListener;
		AJMediaClientListener 	*m_AJMediaClientListener;
//		AJClientEventListener   *m_AJClientEventListener;

		jobject 				m_discovery_listener_obj;
		jobject 				m_media_result_listener_obj;
		jobject 				m_connect_server_listener_obj;
		jobject 				m_event_listener_obj;

		
        std::string 			m_ip; 
		int 					m_port;
};


extern AJClientEntry *g_AJClientEntry;

#endif
