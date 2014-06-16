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

#include "AJServer.h"

AJServer::AJServer(JNIEnv *env, jobject obj)
{
	m_photo_listener_obj = NULL;
	m_video_listener_obj = NULL;
	m_audio_listener_obj = NULL;
	m_discovery_listener_obj = NULL;
	m_publish_listener_obj = NULL;

	m_env = env;
	m_obj = obj;

	env->GetJavaVM(&m_jvm);

	m_AirJoyServer = new AirJoyServer();
	m_AirJoyServerInfo = new AirJoyServiceInfo();
	m_AirJoyNsd = new AirJoyNsd();

	m_AJDiscoveryListener = new AJDiscoveryListener();
//	m_AJPublishListener = new AJPublishListener();
	m_AJMediaListener = new AJMediaListener();
	m_AJState = new AJState();

	m_serverinfo_cls = env->FindClass("com/fqx/airjoy/server/ServerInfo");
	LOGI("AJServer V1.2");
}

AJServer::~AJServer()
{
	if(m_AirJoyServer != NULL)
		delete m_AirJoyServer;

	if(m_AirJoyServerInfo != NULL)
		delete m_AirJoyServerInfo;

	if(m_AirJoyNsd != NULL)
		delete m_AirJoyNsd;

	if(m_AJDiscoveryListener != NULL)
		delete m_AJDiscoveryListener;

//	if(m_AJPublishListener != NULL)
//		delete m_AJPublishListener;

	if(m_AJMediaListener != NULL)
		delete m_AJMediaListener;
}

JNIEnv * AJServer::getEnv()
{
	return m_env;
}

jobject AJServer::getObj()
{
	return m_obj;
}

JavaVM * AJServer::getVM()
{
	return m_jvm;
}

jobject AJServer::newServerinfoObj(JNIEnv *env, AirJoyServiceInfo &ServerInfo)
{
	jstring jstr;

	jclass cls = m_serverinfo_cls;
	jmethodID mid = env->GetMethodID(cls, "<init>","()V");
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


void AJServer::setPhotoListenerObj(jobject listener)
{
	m_photo_listener_obj = listener;
}

void AJServer::setVideoListenerObj(jobject listener)
{
	m_video_listener_obj = listener;
}

void AJServer::setAudioListenerObj(jobject listener)
{
	m_audio_listener_obj = listener;
}

void AJServer::setMediaListenerObj(jobject listener)
{
	m_media_listener_obj = listener;
}


void AJServer::setNsdDiscoveryListenerObj(jobject listener)
{
	m_discovery_listener_obj = listener;
}

void AJServer::setNsdPublishListenerObj(jobject listener)
{
	m_publish_listener_obj = listener;
}


jobject AJServer::getPhotoListenerObj()
{
	return m_photo_listener_obj;
}

jobject AJServer::getVideoListenerObj()
{
	return m_video_listener_obj;
}

jobject AJServer::getAudioListenerObj()
{
	return m_audio_listener_obj;
}

jobject AJServer::getMediaListenerObj()
{
	return m_media_listener_obj;
}


jobject AJServer::getNsdDiscoveryListenerObj()
{
	return m_discovery_listener_obj;
}

jobject AJServer::getNsdPublishListenerObj()
{
	return m_publish_listener_obj;
}



AirJoyServer * AJServer::getAirJoyServer()
{
	return m_AirJoyServer;
}


AirJoyServiceInfo * AJServer::getAirJoyServerInfo()
{
	return m_AirJoyServerInfo;
}


AirJoyNsd * AJServer::getAirJoyNsd()
{
	return m_AirJoyNsd;
}


AJDiscoveryListener * AJServer::getAJDiscoveryListener()
{
	return m_AJDiscoveryListener;
}



#if 0
AJPublishListener * AJServer::getAJPublishListener()
{
	return m_AJPublishListener;
}
#endif


AJMediaListener * AJServer::getAJMediaListener()
{
	return m_AJMediaListener;
}

AJState *AJServer::getAJState()
{
	return m_AJState;
}

jclass AJServer::getServerinfoClass()
{
	return m_serverinfo_cls;
}
