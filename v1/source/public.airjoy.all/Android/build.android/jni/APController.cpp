
/*
 * Copyright (C), 2011-2212, AnyPlay
 *
 * File name:  	APController.cpp
 * Author:     	SangWencheng
 * Version:    	V1.0
 * Date: 		2012-08-06
 *
 * Description:    
 * 		APControllerʵ���˺��Ŀ�Java��C++��JNI����
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

#include "AirFileServer.hpp"

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
AirPlayNsd                          gBonjourAirTunes;                   
AirTunesServiceInfo					gAirTunesServiceInfo;



class APController
{
	public:
		APController(JNIEnv *env, jobject obj)
		{
			LOGI("APController: V1.0.\n");
            m_server = new AirFileServer();
		}

		virtual ~APController()
		{
            delete m_server;
		}

 	private:
        AirFileServer *m_server;
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


class MyAirTunesServerListener : public AirTunesServerListener
{
private:
    std::string     m_imgFile;

public:
    MyAirTunesServerListener() 
    {}

    virtual ~MyAirTunesServerListener() 
    {}

    /*
    * ��ʼ������Ƶ
    */
    virtual void didStartPlayAudio(const std::string &clientIp)
    {
        LOGI( "MyAirTunesServerListener::didStartPlayAudio() " );
    }

    /*
    * ��ͣ������Ƶ
    */
    virtual void didPause(const std::string &clientIp)
    {
        LOGI( "MyAirTunesServerListener::didPause() " );
    }

    /*
    * ֹͣ������Ƶ
    */
    virtual void didStop(const std::string &clientIp)
    {
        LOGI( "MyAirTunesServerListener::didStop() " );
    }

    /*
    * ��������
    *  @value              [in] ����ֵ
    */
    virtual void didSetVolume(float value, const std::string &clientIp)
    {
        LOGI( "MyAirTunesServerListener::didSetVolume: %f", value );
    }

    /*
    * ���÷���ͼƬ
    *  @data               [in] ͼƬ���ݣ�jpeg��ʽ��
    *  @length             [in] ͼƬ���ݳ���
    */        
    virtual void didSetImage(const char *data, std::size_t length, const std::string &clientIp)
    {
        LOGI( "MyAirTunesServerListener::didSetImage: %d" ,length );
    }

    /*
    * ������Ƶ��Ϣ
    *  @name               [in] ������
    *  @artist             [in] ����
    *  @album              [in] ר����
    */
    virtual void didSetAudioInfo(const std::string &name, const std::string &artist, const std::string &album, const std::string &clientIp)
    {
        LOGI( "MyAirTunesServerListener::didSetAudioInfo()" );
    }

    #ifndef __USE_LIB_AO__
    /*
    * ��ʼ����Ƶ�����Ϣ
    *  @sampleRateInHz         [in] ��Ƶ���ʣ�һ��Ϊ44100
    *  @channelConfig          [in] ��ƵƵ����1=��������2=������
    *  @audioFormat            [in] ��Ƶ��ʽ��16λ��32λ
    *  @fmtp                   [in] fmtp for osx
    *  @fmtpLen                [in] length os fmtp
    *  @note
    *    Java���루��ʼ��AudioTrack��
        AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 
                                          sampleRateInHz,                         // 44100����
                                          AudioFormat.CHANNEL_OUT_STEREO,         // 2: ������
                                          AudioFormat.ENCODING_PCM_16BIT,         // 16λ
                                          sampleRateInHz * 2 * 4,
                                          AudioTrack.MODE_STREAM);
    */
    virtual void didAudioDriverInit(int sampleRateInHz, int channelConfig, int audioFormat, int fmtp[12], size_t fmtpLen)
    {
        LOGI( "MyAirTunesServerListener::didAudioDriverInit" );
    }

    /*
    * �����Ƶ���ݣ�PCM��ʽ
    *  @outbuf         [in] ��Ƶ����
    *  @size           [in] ��Ƶ���ݳ���
    *  @note
    *    Java���루�����Ƶ��
    *    track.write(outbuf, 0, size);
    */
    virtual void didAudioDriverPlay(char *outbuf, int size)
    {
        LOGI( "MyAirTunesServerListener::didAudioDriverPlay" );
    }
    #endif // __USE_LIB_AO__
};

//------------------------------------------------------------------
// 1. ʵ����AirTunes
//------------------------------------------------------------------
MyAirTunesServerListener   		    gMyAirTunesServerListener;
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
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_mdnsd_1start
  (JNIEnv *env, jobject obj, jint debug)
{
	mdnsd_start(debug);
}

/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirplayStart
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_AirplayStart
  (JNIEnv *env, jobject obj, jstring devId, jstring devName, jstring devModel)
{
	const char *pdeviceId = env->GetStringUTFChars(devId, NULL);
    if (pdeviceId == NULL) {  // Out of memory
        return;
    }

	const char *pname = env->GetStringUTFChars(devName, NULL);
    if (pname == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(devId, pdeviceId);
        return;
    }

	const char *pdevModel = env->GetStringUTFChars(devModel, NULL);
    if (pdevModel == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(devId, pdeviceId);
		env->ReleaseStringUTFChars(devName, pname);
        return;
    }
	LOGI("AirplayStart: id=%s  name=%s  model=%s", pdeviceId, pname, pdevModel);

	// ����AirPlay����
    airplayServiceInfo.setDeviceId(pdeviceId);
    airplayServiceInfo.setDeviceName(pname);
    airplayServiceInfo.setDeviceModel(pdevModel);

    gAirPlayServer.start(airplayServiceInfo, &myAirPlayServerListener);

    // 4.1 ����AirPlay�Զ����ֹ���
    gAirPlayNsd.discoveryService(airplayServiceInfo.type(), &gMyAirPlayNsdDiscoveryListener);

    // 4.2 ����AirPlay����
    gAirPlayNsd.publishService(airplayServiceInfo, &gMyAirPlayNsdPublishListener);

#if 1

//  gAirTunesServiceInfo.setMacAddress(macAddress, 6);                      // ��������MAC��ַ
	gAirTunesServiceInfo.setDeviceId(pdeviceId);
    gAirTunesServiceInfo.setDeviceName(pname);                             // ��������

    // 1 ����AirTunes����
    gAirTunesServer.start(gAirTunesServiceInfo, &gMyAirTunesServerListener);

    // 2.1 ����Bonjourģ��
    gBonjourAirTunes.discoveryService(gAirTunesServiceInfo.type(), &gBonjourAirTunesDiscoveryListener);

    // 2.2 ����AirTunes����
    gAirTunesServiceInfo.setPort(gAirTunesServer.port());                   // ���·���˿ڣ�����������Ҫ
    gBonjourAirTunes.publishService(gAirTunesServiceInfo, &gBonjourAirTunesPublishListener);

#endif


	env->ReleaseStringUTFChars(devId, pdeviceId);
	env->ReleaseStringUTFChars(devName, pname);
	env->ReleaseStringUTFChars(devModel, pdevModel);

//	sg_airplay.start();
	LOGI("AirplayStart: ok");
}

#if 0
void StartTunesService()
{
}
#endif



/*
 * Class:     com_fqx_anyplay_service_APController
 * Method:    AirplayStop
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fqx_anyplay_service_APController_AirplayStop
(JNIEnv *env, jobject obj) 
{
	gAirPlayNsd.stopServiceDiscovery();
    gAirPlayServer.stop();
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
