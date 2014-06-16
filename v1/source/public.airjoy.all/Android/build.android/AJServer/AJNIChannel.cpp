
#include "AJServer.h"

#include "com_fqx_airjoy_server_AirJoy.h"

AJServer * g_AJServer;

/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    setPhotoListener
 * Signature: (Lcom/fqx/airjoy/listener/AirPhotoListener;)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_server_AirJoy_setPhotoListener(JNIEnv *env, jobject obj, jobject listener)
{
	if(listener == NULL)
		return;
	g_AJServer->setPhotoListenerObj(env->NewGlobalRef(listener));
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    setVideoListener
 * Signature: (Lcom/fqx/airjoy/listener/AirPhotoListener;)V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_setVideoListener(JNIEnv *env, jobject obj, jobject listener)
{
	if(listener == NULL)
		return;
	g_AJServer->setVideoListenerObj(env->NewGlobalRef(listener));
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    setAudioListener
 * Signature: (Lcom/fqx/airjoy/listener/AirPhotoListener;)V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_setAudioListener(JNIEnv *env, jobject obj, jobject listener)
{
	if(listener == NULL)
		return;
	g_AJServer->setAudioListenerObj(env->NewGlobalRef(listener));
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    setNsdDiscoveryListener
 * Signature: (Lcom/fqx/airjoy/listener/AirPhotoListener;)V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_setNsdDiscoveryListener(JNIEnv *env, jobject obj, jobject listener)
{
	if(listener == NULL)
		return;
	g_AJServer->setNsdDiscoveryListenerObj(env->NewGlobalRef(listener));
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    setNsdPublishListener
 * Signature: (Lcom/fqx/airjoy/listener/AirPhotoListener;)V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_setNsdPublishListener(JNIEnv *env, jobject obj, jobject listener)
{
	if(listener == NULL)
		return;
	g_AJServer->setNsdPublishListenerObj(env->NewGlobalRef(listener));
}

/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    setMediaListener
 * Signature: (Lcom/fqx/airjoy/listener/AirPhotoListener;)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_server_AirJoy_setMediaListener(JNIEnv *env, jobject obj, jobject listener)
{
	if(listener == NULL)
		return;
	g_AJServer->setMediaListenerObj(env->NewGlobalRef(listener));
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    clsListenner
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_clsListenner(JNIEnv *env, jobject obj)
{
	if(g_AJServer->getPhotoListenerObj() != NULL)
		env->DeleteGlobalRef(g_AJServer->getPhotoListenerObj());

	if(g_AJServer->getVideoListenerObj() != NULL)
		env->DeleteGlobalRef(g_AJServer->getVideoListenerObj());

	if(g_AJServer->getAudioListenerObj() != NULL)
		env->DeleteGlobalRef(g_AJServer->getAudioListenerObj());

	if(g_AJServer->getNsdPublishListenerObj() != NULL)
		env->DeleteGlobalRef(g_AJServer->getNsdPublishListenerObj());

	if(g_AJServer->getNsdDiscoveryListenerObj() != NULL)
		env->DeleteGlobalRef(g_AJServer->getNsdDiscoveryListenerObj());

	if(g_AJServer->getMediaListenerObj() != NULL)
		env->DeleteGlobalRef(g_AJServer->getMediaListenerObj());
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    config
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_config(JNIEnv *env, jobject obj, jstring name, jstring id, jint type)
{
	const char *pnickname = env->GetStringUTFChars(name, NULL);
    if (pnickname == NULL) {  // Out of memory
        return;
    }

	const char *pdeviceId = env->GetStringUTFChars(id, NULL);
    if (pdeviceId== NULL) {  // Out of memory
		env->ReleaseStringUTFChars(name, pnickname);
        return;
    }

	AirJoyServiceInfo *p_server_info = g_AJServer->getAirJoyServerInfo();

	p_server_info->setDeviceName(pnickname);
	p_server_info->setDeviceId(pdeviceId);
	p_server_info->setDeviceType((DeviceType)type);

	env->ReleaseStringUTFChars(name, pnickname);
	env->ReleaseStringUTFChars(id, pdeviceId);
}

/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    publishMediaEvent
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_server_AirJoy_publishMediaEvent(JNIEnv *env, jclass cls, jstring mediaType, jstring action, jstring id, jstring url, jstring name, jstring fromIp)
{
	LOGI("publishMediaEvent start");
	const char *pmediaType = env->GetStringUTFChars(mediaType, NULL);
    if (pmediaType == NULL) {  // Out of memory
        return false;
    }

	const char *paction = env->GetStringUTFChars(action, NULL);
    if (paction == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(mediaType, pmediaType);
        return false;
    }

	const char *pid = env->GetStringUTFChars(id, NULL);
    if (pid == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(mediaType, pmediaType);
		env->ReleaseStringUTFChars(action, paction);
        return false;
    }

	const char *purl = env->GetStringUTFChars(url, NULL);
    if (purl == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(mediaType, pmediaType);
		env->ReleaseStringUTFChars(action, paction);
		env->ReleaseStringUTFChars(id, pid);
        return false;
    }

	const char *pname = env->GetStringUTFChars(name, NULL);
    if (pname == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(mediaType, pmediaType);
		env->ReleaseStringUTFChars(action, paction);
		env->ReleaseStringUTFChars(id, pid);
		env->ReleaseStringUTFChars(url, purl);
        return false;
    }

	const char *pfromIp = env->GetStringUTFChars(fromIp, NULL);
    if (pfromIp == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(mediaType, pmediaType);
		env->ReleaseStringUTFChars(action, paction);
		env->ReleaseStringUTFChars(id, pid);
		env->ReleaseStringUTFChars(url, purl);
		env->ReleaseStringUTFChars(url, pname);
        return false;
    }


	LOGI("publishMediaEvent %s %s %s %s %s %s", pmediaType, paction, pid, purl, pname, pfromIp);
	jboolean ret = g_AJServer->getAirJoyServer()->publishMediaEvent(pmediaType, paction, pid, purl, pname, pfromIp);

	env->ReleaseStringUTFChars(mediaType, pmediaType);
	env->ReleaseStringUTFChars(action, paction);
	env->ReleaseStringUTFChars(id, pid);
	env->ReleaseStringUTFChars(url, purl);
	env->ReleaseStringUTFChars(name, pname);
	env->ReleaseStringUTFChars(name, pfromIp);

	return ret;

}

/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    ServerStart
 * Signature: ()V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_server_AirJoy_ServerStart(JNIEnv *env, jobject obj)
{
	AirJoyServer * p_AirJoyServer = g_AJServer->getAirJoyServer();
	AirJoyServiceInfo *p_AirJoyServiceInfo = g_AJServer->getAirJoyServerInfo();

	p_AirJoyServer->start(*p_AirJoyServiceInfo);
	p_AirJoyServer->loadAirMedia(g_AJServer->getAJMediaListener());
}

/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    nsdStart
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_nsdStart(JNIEnv *env, jobject obj)
{
	g_AJServer->getAirJoyNsd()->start();
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    publishService
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_publishService(JNIEnv *env, jobject obj)
{
	AirJoyServer * p_AirJoyServer = g_AJServer->getAirJoyServer();
	AirJoyServiceInfo *p_AirJoyServiceInfo = g_AJServer->getAirJoyServerInfo();

	p_AirJoyServiceInfo->setPort(p_AirJoyServer->port());
#if 0
	AJPublishListener * p_AJPublishListener;
	LOGI("AirJoy publishService Test4");
	p_AJPublishListener = g_AJServer->getAJPublishListener();
	LOGI("AirJoy publishService Test5");
	if(p_AJPublishListener == NULL) {
		LOGE("AirJoy publishService Fail");
		return;
	}
#endif
	jboolean ret = g_AJServer->getAirJoyNsd()->publishService(*p_AirJoyServiceInfo);
	if(ret) {
		LOGI("AirJoy publishService ok");
	} else {
		LOGE("AirJoy publishService fail");
	}
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    stop
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_stop(JNIEnv *env, jobject obj)
{
	g_AJServer->getAirJoyNsd()->stop();
	g_AJServer->getAirJoyServer()->stop();
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    native_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_native_1init(JNIEnv *env, jclass cls)
{
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    native_setup
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_native_1setup(JNIEnv *env, jobject obj)
{
	g_AJServer = new AJServer(env, obj);
}


/*
 * Class:     com_fqx_airjoy_server_AirJoy
 * Method:    native_finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL
Java_com_fqx_airjoy_server_AirJoy_native_1finalize(JNIEnv *env, jobject obj)
{
	if(g_AJServer != NULL) 
	{
		delete g_AJServer;
	}
}
