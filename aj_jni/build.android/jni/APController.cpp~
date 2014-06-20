
/*
 * Copyright (C), 2011-2212, AnyPlay
 *
 * File name:  	APController.cpp
 * Author:     	SangWencheng
 * Version:    	V1.0
 * Date: 		2012-08-06
 *
 * Description:    
 * 		APController实现了核心库Java到C++的JNI调用
 *
 * History:        
 * 	1. 	Date:
 * 		Author:
 * 		Modification:
 * 	2. ...
 */


#include <stdio.h>
#include <stdlib.h>

#include "AirPlayNsd.hpp"
#include "AirPlayNsdDiscoveryListener.hpp"
#include "AirPlayNsdPublishListener.hpp"

//#include "AnyPlay.hpp"
#include "Bonjour.h"
#include "AirPlayServer.hpp"
#include "JNIGlobalParam.h"
#include "com_fqx_anyplay_service_APController.h"
#include "AirplayJniListener.h"

//#include "MyTcpServer.hpp"
#include "AirTunesServer.hpp"
#include "AirTunesServiceInfo.hpp"
#include "AirTunesServerListener.hpp"
#include "APAirTunesListener.h"


//using namespace anyplay;
using namespace airjoy;

struct fields_t {
    jfieldID  context;
};
static fields_t fields;

//AnyPlay sg_anyplay;

AirPlayServer      				gAirPlayServer;
AirPlayServiceInfo 				airplayServiceInfo;
AirplayJniListener 				myAirPlayServerListener;
AirPlayNsd 						gAirPlayNsd;


AirTunesServer                  gAirTunesServer;
AirPlayNsd                      gBonjourAirTunes;                   
AirTunesServiceInfo				gAirTunesServiceInfo;



class APController
{
	public:
		APController(JNIEnv *env, jobject obj)
		{
			LOGI("APController: V1.0.\n");
		}

		virtual ~APController()
		{
		}

 	private:

};


class BonjourAirTunesPublishListener : public AirPlayNsdPublishListener
{
public:
    BonjourAirTunesPublishListener() {}
    
    virtual ~BonjourAirTunesPublishListener() {}

    virtual void onUnPulishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode)
    {
        LOGI("BonjourAirTunes onUnPulishFailed");
    }

    virtual void onServiceUnPublished(const std::string & servicetype, const std::string & serviceName)
    {
        LOGI("BonjourAirTunes onServiceUnPublished");
    }

    virtual void onServicePublished(const std::string & servicetype, const std::string & serviceName)
    {
        LOGI("BonjourAirTunes onServicePublished");
    }

    virtual void onPublishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode)
    {
        LOGI("BonjourAirTunes onPublishFailed");
    }
};

class BonjourAirTunesDiscoveryListener : public AirPlayNsdDiscoveryListener
{
public:
    BonjourAirTunesDiscoveryListener() {}
    
    virtual ~BonjourAirTunesDiscoveryListener() {}

    virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode)
    {
        LOGI("BonjourAirTunes onStopDiscoveryFailed");
    }

    virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode) 
    {
        LOGI("BonjourAirTunes onStartDiscoveryFailed");
    }

    virtual void onServiceLost(const std::string serviceType, 
                               const std::string serviceName, 
                               const std::string serviceIp,
                               uint16_t servicePort)
    {
        LOGI("---------------------- AirTunes onServiceLost ----------------------");
        LOGI("serviceType: %s" ,serviceType.c_str());
        LOGI("serviceName: %s" ,serviceName.c_str());
        LOGI("serviceIp  : %s" ,serviceIp.c_str());
        LOGI("servicePort: %d" ,servicePort);
    }

    virtual void onServiceFound(const std::string serviceType, 
                                const std::string serviceName, 
                                const std::string serviceIp,
                                uint16_t servicePort) 
    {
        LOGI("---------------------- AirTunes onServiceFound ----------------------");
        LOGI("serviceType: %s" ,serviceType.c_str());
        LOGI("serviceName: %s" ,serviceName.c_str());
        LOGI("serviceIp  : %s" ,serviceIp.c_str());
        LOGI("servicePort: %d" ,servicePort);
    }

    virtual void onDiscoveryStopped(const std::string serviceType) 
    {
        LOGI("BonjourAirTunes onDiscoveryStopped");
    }

    virtual void onDiscoveryStarted(const std::string serviceType) 
    {
        LOGI("BonjourAirTunes onDiscoveryStarted");
    }
};


//------------------------------------------------------------------
// 1. 实例化AirTunes
//------------------------------------------------------------------
APAirTunesListener gMyAirTunesServerListener;
BonjourAirTunesDiscoveryListener    gBonjourAirTunesDiscoveryListener;
BonjourAirTunesPublishListener      gBonjourAirTunesPublishListener;



class MyAirPlayNsdPublishListener : public AirPlayNsdPublishListener
{
public:
    MyAirPlayNsdPublishListener() {}
    
    virtual ~MyAirPlayNsdPublishListener() {}

    virtual void onUnPulishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode)
    {
        LOGI("AirPlay onUnPulishFailed");
    }

    virtual void onServiceUnPublished(const std::string & servicetype, const std::string & serviceName)
    {
        LOGI("AirPlay onServiceUnPublished");
    }

    virtual void onServicePublished(const std::string & servicetype, const std::string & serviceName)
    {
        LOGI("AirPlay onServicePublished");
    }

    virtual void onPublishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode)
    {
        LOGI("AirPlay onPublishFailed");
    }
};

class MyAirPlayNsdDiscoveryListener : public AirPlayNsdDiscoveryListener
{
public:
    MyAirPlayNsdDiscoveryListener() {}
    
    virtual ~MyAirPlayNsdDiscoveryListener() {}

    virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode)
    {
        LOGI("AirPlay onStopDiscoveryFailed" );
    }

    virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode) 
    {
        LOGI("AirPlay onStartDiscoveryFailed" );
    }

	virtual void onServiceLost(const std::string serviceType, 
                               const std::string serviceName, 
                               const std::string serviceIp,
                               uint16_t servicePort)
    {
        LOGI("---------------------- AirPlay onServiceLost ----------------------" );
    }

	virtual void onServiceFound(const std::string serviceType, 
                                const std::string serviceName, 
                                const std::string serviceIp,
                                uint16_t servicePort) 
    {
        LOGI("---------------------- AirPlay onServiceFound ----------------------" );
        LOGI("deviceType: %s", serviceType.c_str());
        LOGI("deviceName: %s" ,serviceName.c_str() );
        LOGI("ip: %s" ,serviceIp.c_str() );
        LOGI("port: %d" ,servicePort);
    }

    virtual void onDiscoveryStopped(const std::string serviceType) 
    {
        LOGI("AirPlay onDiscoveryStopped" );
    }

    virtual void onDiscoveryStarted(const std::string serviceType) 
    {
        LOGI("AirPlay onDiscoveryStarted" );
    }
};

MyAirPlayNsdDiscoveryListener   gMyAirPlayNsdDiscoveryListener;
MyAirPlayNsdPublishListener     gMyAirPlayNsdPublishListener;
/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    getAirplayPort
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_fqx_anyplay_service_APController_getAirplayPort
  (JNIEnv *env, jobject obj)
{
	jint port = gAirPlayServer.port();
	return port;
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    mdnsd_start
 * Signature: (I)V
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_anyplay_service_APController_mdnsd_1start
  (JNIEnv *env, jobject obj, jint debug)
{
	int ret = mdnsd_start(debug);
	if(ret > 0) {
		return true;
	}
	return false;
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    mdnsd_stop
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_anyplay_service_APController_mdnsd_1stop
  (JNIEnv *env, jobject obj)
{
	int ret = mdnsd_stop();
	if(ret > 0) {
		return true;
	}
	return false;
}




/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirTunesStart
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_fqx_anyplay_service_APController_AirTunesStart
  (JNIEnv *env, jobject obj, jstring devId, jstring devName, jstring devModel)
{
	const char *pdeviceId = env->GetStringUTFChars(devId, NULL);
    if (pdeviceId == NULL) {  // Out of memory
        return -1;
    }

	const char *pname = env->GetStringUTFChars(devName, NULL);
    if (pname == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(devId, pdeviceId);
        return -1;
    }

	const char *pdevModel = env->GetStringUTFChars(devModel, NULL);
    if (pdevModel == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(devId, pdeviceId);
		env->ReleaseStringUTFChars(devName, pname);
        return -1;
    }
	LOGI("AirTunesStart: id=%s  name=%s  model=%s", pdeviceId, pname, pdevModel);

	gAirTunesServiceInfo.setDeviceId(pdeviceId);
    gAirTunesServiceInfo.setDeviceName(pname);                             // 机器名称

    // 1 启动AirTunes服务
    gAirTunesServer.start(gAirTunesServiceInfo, &gMyAirTunesServerListener);

    
	env->ReleaseStringUTFChars(devId, pdeviceId);
	env->ReleaseStringUTFChars(devName, pname);
	env->ReleaseStringUTFChars(devModel, pdevModel);

	return 1;
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirTunesStop
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_anyplay_service_APController_AirTunesStop
  (JNIEnv *env, jobject obj) 
{
    gBonjourAirTunes.unpublishService();
	jboolean ret = gBonjourAirTunes.stopServiceDiscovery();
	gAirTunesServer.stop();

	return ret;
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirplayStart
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT int JNICALL Java_com_fqx_anyplay_service_APController_AirplayStart
  (JNIEnv *env, jobject obj, jstring devId, jstring devName, jstring devModel)
{
	const char *pdeviceId = env->GetStringUTFChars(devId, NULL);
    if (pdeviceId == NULL) {  // Out of memory
        return -1;
    }

	const char *pname = env->GetStringUTFChars(devName, NULL);
    if (pname == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(devId, pdeviceId);
        return -1;
    }

	const char *pdevModel = env->GetStringUTFChars(devModel, NULL);
    if (pdevModel == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(devId, pdeviceId);
		env->ReleaseStringUTFChars(devName, pname);
        return -1;
    }
	LOGI("AirplayStart: id=%s  name=%s  model=%s", pdeviceId, pname, pdevModel);

	// 启动AirPlay服务
    airplayServiceInfo.setDeviceId(pdeviceId);
    airplayServiceInfo.setDeviceName(pname);
    airplayServiceInfo.setDeviceModel(pdevModel);

    gAirPlayServer.start(airplayServiceInfo, &myAirPlayServerListener);

	env->ReleaseStringUTFChars(devId, pdeviceId);
	env->ReleaseStringUTFChars(devName, pname);
	env->ReleaseStringUTFChars(devModel, pdevModel);

//	sg_airplay.start();
	LOGI("AirplayStart: ok");
	return 1;
}


/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirplayStop
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_anyplay_service_APController_AirplayStop
(JNIEnv *env, jobject obj) 
{
 	gAirPlayNsd.unpublishService();
	jboolean ret = gAirPlayNsd.stopServiceDiscovery();
 	gAirPlayServer.stop();

	return ret;
}
/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirTunesPublishService
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_anyplay_service_APController_AirTunesPublishService
  (JNIEnv *env, jobject obj, jboolean isTrue) 
{
	jboolean ret = false;
	if(isTrue) {
		ret = gBonjourAirTunes.publishService(gAirTunesServiceInfo, &gBonjourAirTunesPublishListener);
	}else{
		ret = gBonjourAirTunes.unpublishService();
	}

	return ret;
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirplayPublishService
 * Signature: (Z)Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_anyplay_service_APController_AirplayPublishService
  (JNIEnv *env, jobject obj, jboolean isTrue) 
{
	jboolean ret = false;
	if(isTrue) {
		ret = gAirPlayNsd.publishService(airplayServiceInfo, &gMyAirPlayNsdPublishListener);
	}else{
		ret = gAirPlayNsd.unpublishService();
	}
	return ret;
}


/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    PublishAirTunesService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_anyplay_service_APController_PublishAirTunesService
  (JNIEnv *env, jobject obj) 
{
	jboolean ret = false;
	// 1 启动Bonjour模块
    ret = gBonjourAirTunes.discoveryService(gAirTunesServiceInfo.type(), &gBonjourAirTunesDiscoveryListener);

    // 2 发布AirTunes服务
	if(ret) {
		gAirTunesServiceInfo.setPort(gAirTunesServer.port());    // 更新服务端口，发布服务需要
		ret = gBonjourAirTunes.publishService(gAirTunesServiceInfo, &gBonjourAirTunesPublishListener);
	}

	return ret;
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    PublishAirplayService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_anyplay_service_APController_PublishAirplayService
  (JNIEnv *env, jobject obj) 
{
	jboolean ret = false;
    // 1 启动AirPlay自动发现功能
    ret = gAirPlayNsd.discoveryService(airplayServiceInfo.type(), &gMyAirPlayNsdDiscoveryListener);
    // 2 发布AirPlay服务
	if(ret) {
		ret = gAirPlayNsd.publishService(airplayServiceInfo, &gMyAirPlayNsdPublishListener);
	}

	return ret;

}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    SetAirPlayListener
 * Signature: (Lcom/fqx/anyplay/service/APResultListener;)V
 */
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_SetAirPlayListener
  (JNIEnv *env, jobject obj, jobject listener_obj)
{
	if(listener_obj == NULL) {
		return;
	}

	p_JNIGlobalParam->setResultListenerObj(env->NewGlobalRef(listener_obj));
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    SetAirTunesServerListener
 * Signature: (Lcom/fqx/anyplay/service/APAirTunesServerListener;)V
 */
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_SetAirTunesServerListener
   (JNIEnv *env, jobject obj, jobject listener_obj)
{
	if(listener_obj == NULL) {
		return;
	}

	p_JNIGlobalParam->setAirTunesListenerObj(env->NewGlobalRef(listener_obj));
}



/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    PublishEvent
 * Signature: (IILjava/lang/String;)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_anyplay_service_APController_PublishEvent(JNIEnv *env, jclass cls, jint mediaType, jint evtType, jstring fromIp)
{
	const char *pfromIp = env->GetStringUTFChars(fromIp, NULL);
    if (pfromIp == NULL) {  // Out of memory
		LOGE("#### Err:(PublishEvent pfromIp=null)");
        return;
    }

	LOGI("publishEvent: mediaType=%d  evtType=%d  ip=%s", mediaType, evtType, pfromIp);
	gAirPlayServer.publishEvent((AirPlayServer::EventCategory)mediaType, (AirPlayServer::EventAction)evtType, pfromIp);
	env->ReleaseStringUTFChars(fromIp, pfromIp);
	return;
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    getSlideshowsPicture
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_anyplay_service_APController_getSlideshowsPicture(JNIEnv *env, jobject obj, jstring clientIp) 
{
	const char *pclientIp = env->GetStringUTFChars(clientIp, NULL);
    if (pclientIp == NULL) {  // Out of memory
		LOGE("#### Err:(PublishEvent pfromIp=null)");
        return;
    }

	LOGI("getSlideshowsPicture: ip=%s", pclientIp);
	gAirPlayServer.getSlideshowsPicture(pclientIp);
	env->ReleaseStringUTFChars(clientIp, pclientIp);
	return;
}



#if 0
/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirplaySendEvt
 * Signature: (IILjava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_AirplaySendEvt
  (JNIEnv *env, jobject obj, jint mediaType, jint evt, jstring category, jstring xAppleSessionId)
{
//	LOGI("AirplaySendEvt: id=%d  evt=%d", mediaType, evt);
	p_JNIGlobalParam->publishEvt(evt);
}
#endif

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    native_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_native_1init
  (JNIEnv *env, jclass pcls)
{
	jclass cls;
	cls = env->FindClass(CLASS_APCONTROLLER);
	if(cls == NULL)
		return;

	fields.context = env->GetFieldID(cls, "APControllerFlag", "I");
    if (fields.context == NULL) {
        return;
    }
	LOGI("native_init: OK.\n");
}


/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    native_setup
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_native_1setup
  (JNIEnv *env, jobject obj) 
{
	APController *p_APController = new APController(env, obj);
    if (p_APController == NULL) {
        return;
    }
    env->SetIntField(obj, fields.context, (int)p_APController);

	p_JNIGlobalParam = new JNIGlobalParam(env, obj);
	p_JNIGlobalParam->setAirPlayServer(&gAirPlayServer);
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    native_finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_native_1finalize
  (JNIEnv *env, jobject obj)
{
	APController *p_APController = (APController*)env->GetIntField(obj, fields.context);
	if(p_APController == NULL)
		return;

	delete p_APController;
}


/*
* Set some test stuff up.
* Returns the JNI version on success, -1 on failure.
*/
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		return -1;
	}

	result = JNI_VERSION_1_4;

	return result;
}
