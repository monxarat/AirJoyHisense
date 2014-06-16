/*
 * Copyright (C), 2011-2212, AJClientEntry
 *
 * File name:  	AJClientEntry.cpp
 * Author:     	SangWencheng
 * Version:    	V1.0
 * Date: 		2012-11-01
 *
 * Description:    
 * 		AJClientEntry.cpp负责管理下层JNI模块
 *
 * History:        
 * 	1. 	Date:
 * 		Author:
 * 		Modification:
 * 	2. ...
 */


#include "AJClientEntry.h"
using namespace airjoy;

AJClientEntry *g_AJClientEntry;

static jclass m_serverinfo_cls;
static jclass m_playback_cls;
AJClientEntry::AJClientEntry(JNIEnv *env, jobject obj)
{
	m_discovery_listener_obj = NULL;
	m_media_result_listener_obj = NULL;
	m_connect_server_listener_obj = NULL;

	m_env = env;

	env->GetJavaVM(&m_jvm);
	m_obj = env->NewGlobalRef(obj);

	m_AirMediaClient = new AirMediaClient();
	m_AirJoyNsd = new AirJoyNsd();
	m_AirFileServer = new AirFileServer();

	m_AJDiscoveryListener = new AJDiscoveryListener();
	m_AJMediaClientListener = new AJMediaClientListener();
//	m_AJClientEventListener = new AJClientEventListener();

#if 0
	m_serverinfo_cls = env->FindClass("com/fqx/airjoy/clientlistener/ServerInfo");
	if(m_serverinfo_cls == NULL)
		LOGE("AJClientEntry get m_serverinfo_cls = NULL");
	else
		LOGI("AJClientEntry: m_serverinfo_cls = %ld", m_serverinfo_cls);
#endif

	m_playback_cls = env->FindClass("com/fqx/airjoy/clientlistener/AJPlayBackInfo");
	if(m_playback_cls == NULL)
		LOGE("AJClientEntry get m_playback_cls = NULL");
	else
		LOGI("AJClientEntry: m_playback_cls = %ld", m_playback_cls);
}


AJClientEntry::~AJClientEntry()
{
	if(m_AirMediaClient != NULL) 
		delete m_AirMediaClient;

	if(m_AirJoyNsd != NULL) 
		delete m_AirJoyNsd;

	if(m_AirFileServer != NULL) 
		delete m_AirFileServer;

	if(m_AJDiscoveryListener != NULL) 
		delete m_AJDiscoveryListener;

	if(m_AJMediaClientListener != NULL) 
		delete m_AJMediaClientListener;

#if 0
	if(m_AJClientEventListener!= NULL) 
		delete m_AJClientEventListener;
#endif
}

int AJClientEntry::getPort()
{
	return m_port;
}

std::string AJClientEntry::getIP()
{
	return m_ip;
}

void AJClientEntry::setIPort(const std::string &ip, int port)
{
	m_ip = ip;
	m_port = port;
}



jclass AJClientEntry::getServerInfoClass()
{
	return m_serverinfo_cls;
}

jclass AJClientEntry::getPlayBackClass()
{
	return m_playback_cls;
}

JNIEnv *AJClientEntry::getEnv()
{
	return m_env;
}

jobject AJClientEntry::getObj()
{
	return m_obj;
}

JavaVM *AJClientEntry::getVM()
{
	return m_jvm;
}


void AJClientEntry::setDiscoveryListenerObj(jobject listener)
{
	m_discovery_listener_obj = listener;
}

jobject AJClientEntry::getDiscoveryListenerObj()
{
	return m_discovery_listener_obj;
}


void AJClientEntry::setMediaResultListenerObj(jobject listener)
{
	m_media_result_listener_obj = listener;
}

jobject AJClientEntry::getMediaResultListenerObj()
{
	return m_media_result_listener_obj;
}


void AJClientEntry::setConnectServerListenerObj(jobject listener)
{
	m_connect_server_listener_obj = listener;
}

jobject AJClientEntry::getConnectServerListenerObj()
{
	return m_connect_server_listener_obj;
}

void AJClientEntry::setEventListenerObj(jobject listener)
{
	m_event_listener_obj = listener;
}

jobject AJClientEntry::getEventListenerObj()
{
	return m_event_listener_obj;
}


AirMediaClient *AJClientEntry::getAirMediaClient()
{
	return m_AirMediaClient;
}

AirJoyNsd *AJClientEntry::getAirJoyNsd()
{
	return m_AirJoyNsd;
}

AirFileServer *AJClientEntry::getAirFileServer()
{
	return m_AirFileServer;
}

AJDiscoveryListener *AJClientEntry::getAJDiscoveryListener()
{
	return m_AJDiscoveryListener;
}

AJMediaClientListener *AJClientEntry::getAJMediaClientListener() 
{
	return m_AJMediaClientListener;
}

#if 0
AJClientEventListener *AJClientEntry::getAJClientEventListener()
{
	return m_AJClientEventListener;
}
#endif

#if 0
jobject AJClientEntry::newPlaybackObj(JNIEnv *env, AirJoyPlayBackInfo &info)
{
	jstring jstr;

	jclass cls = g_AJClientEntry->getPlayBackClass();
	jmethodID mid = env->GetMethodID(cls, "<init>","()V");
	jobject dev_obj = env->NewObject(cls, mid); 	 		  //创建java对象

	jfieldID jid = env->GetFieldID(cls, "mid", "Ljava/lang/String;");
	jstr = env->NewStringUTF(info.id.c_str());
	env->SetObjectField(dev_obj, jid, jstr);
	env->DeleteLocalRef(jstr);

	jfieldID jurl = env->GetFieldID(cls, "murl", "Ljava/lang/String;");
	jstr = env->NewStringUTF(info.url.c_str());
	env->SetObjectField(dev_obj, jurl, jstr);
	env->DeleteLocalRef(jstr);


	jfieldID name = env->GetFieldID(cls, "mname", "Ljava/lang/String;");
	jstr = env->NewStringUTF(info.name.c_str());
	env->SetObjectField(dev_obj, name, jstr);
	env->DeleteLocalRef(jstr);


	jfieldID jrate = env->GetFieldID(cls, "mrate", "F");
	env->SetFloatField(dev_obj, jrate, info.rate);

	jfieldID jposition = env->GetFieldID(cls, "mposition", "F");
	env->SetFloatField(dev_obj, jposition, info.position);

	jfieldID jduration = env->GetFieldID(cls, "mduration", "F");
	env->SetFloatField(dev_obj, jduration, info.duration);

	return dev_obj;
}


jobject AJClientEntry::newServerinfoObj(JNIEnv *env, AirJoyServiceInfo &ServerInfo)
{
	jstring jstr;

	jclass cls = g_AJClientEntry->getServerInfoClass();
	jmethodID mid = env->GetMethodID(cls, "<init>","()V");
	if(mid == 0)
	{
		LOGE("Err: ServerInfo mid=NULL");
		return NULL;
	}
	jobject dev_obj = env->NewObject(cls, mid); 	 		  //创建java对象

	jfieldID jid = env->GetFieldID(cls, "mID", "Ljava/lang/String;");
	jstr = env->NewStringUTF(ServerInfo.deviceId().c_str());
	env->SetObjectField(dev_obj, jid, jstr);
	env->DeleteLocalRef(jstr);

	jfieldID name = env->GetFieldID(cls, "mName", "Ljava/lang/String;");
	jstr = env->NewStringUTF(ServerInfo.deviceName().c_str());
	env->SetObjectField(dev_obj, name, jstr);
	env->DeleteLocalRef(jstr);

	jfieldID ip = env->GetFieldID(cls, "mIP", "Ljava/lang/String;");
	jstr = env->NewStringUTF(ServerInfo.ip().c_str());
	env->SetObjectField(dev_obj, ip, jstr);
	env->DeleteLocalRef(jstr);


	jfieldID jtype = env->GetFieldID(cls, "mType", "I");
	env->SetIntField(dev_obj, jtype, (int)ServerInfo.deviceType());

	jfieldID jport= env->GetFieldID(cls, "mPort", "I");
	env->SetIntField(dev_obj, jport, ServerInfo.port());

	return dev_obj;
}

#endif
