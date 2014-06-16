
#include "AJClientEntry.h"

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setDiscoveryListener
 * Signature: (Lcom/fqx/airjoy/clientlistener/AirJoyNsdDiscoveryListener;)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_client_AJClient_setDiscoveryListener(JNIEnv *env, jobject obj, jobject listener_obj)
{
	if(listener_obj == NULL)
		return;
	g_AJClientEntry->setDiscoveryListenerObj(env->NewGlobalRef(listener_obj));
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setMediaClientListener
 * Signature: (Lcom/fqx/airjoy/clientlistener/AirMediaClientListener;)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_client_AJClient_setMediaClientListener(JNIEnv *env, jobject obj, jobject listener_obj)
{
	if(listener_obj == NULL)
		return;
	g_AJClientEntry->setMediaResultListenerObj(env->NewGlobalRef(listener_obj));
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setConnectServerListener
 * Signature: (Lcom/fqx/airjoy/clientlistener/ConnectServerListener;)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_client_AJClient_setConnectServerListener(JNIEnv *env, jobject obj, jobject listener_obj)
{
	if(listener_obj == NULL)
		return;
	g_AJClientEntry->setConnectServerListenerObj(env->NewGlobalRef(listener_obj));
}

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setEventListener
 * Signature: (Lcom/fqx/airjoy/clientlistener/AirClientEventListener;)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_client_AJClient_setEventListener(JNIEnv *env, jobject obj, jobject listener_obj)
{
	if(listener_obj == NULL)
		return;
	g_AJClientEntry->setEventListenerObj(env->NewGlobalRef(listener_obj));
}

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    startDiscoriverService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_startDiscoriverService(JNIEnv *env, jobject obj)
{
	AirJoyNsd * m_AirJoyNsd;
	m_AirJoyNsd = g_AJClientEntry->getAirJoyNsd();
	m_AirJoyNsd->start();
	jboolean ret = m_AirJoyNsd->discoveryService(g_AJClientEntry->getAJDiscoveryListener());
	LOGI("startMeidaService ok");

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    startMeidaService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_startMeidaService(JNIEnv *env, jobject obj)
{
	AirMediaClient * m_AirMediaClient;
	m_AirMediaClient = g_AJClientEntry->getAirMediaClient();
	m_AirMediaClient->start();
	m_AirMediaClient->setListener(g_AJClientEntry->getAJMediaClientListener());
	LOGI("startMeidaService ok");

	return true;
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    startFileService
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_startFileService(JNIEnv *env, jobject obj)
{
	g_AJClientEntry->getAirFileServer()->start();
	LOGI("startFileService ok");
	return true;
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    stopDiscoriverService
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_stopDiscoriverService(JNIEnv *env, jobject obj)
{
	g_AJClientEntry->getAirJoyNsd()->stop();
	return true;
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    stopMeidaService
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_stopMeidaService(JNIEnv *env, jobject obj)
{
	g_AJClientEntry->getAirMediaClient()->stop();
	return true;
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    stopFileService
 * Signature: ()V
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_stopFileService(JNIEnv *env, jobject obj)
{
	g_AJClientEntry->getAirFileServer()->stop();
	return true;
}


#if 0
/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    connectToServer
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_connectToServer(JNIEnv *env, jobject obj, jstring ip, jint port)
{
	const char *p_ip = env->GetStringUTFChars(ip, NULL);
    if (p_ip == NULL) {  // Out of memory
        return false;
    }

	jboolean ret = g_AJClientEntry->getAirMediaClient()->connectToServer(p_ip, (int)port);

	env->ReleaseStringUTFChars(ip, p_ip);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    disconnect
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_disconnect(JNIEnv *env, jobject obj)
{
	return g_AJClientEntry->getAirMediaClient()->disconnect();
}

#endif


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    subscribeEvent
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_subscribeEvent(JNIEnv *env, jobject obj, jstring ip, jint port)
{
	const char *p_ip = env->GetStringUTFChars(ip, NULL);
    if (p_ip == NULL) {  // Out of memory
        return false;
    }

	jboolean ret = g_AJClientEntry->getAirMediaClient()->subscribeEvent(p_ip, (int)port);

	g_AJClientEntry->setIPort(p_ip, (int)port);

	env->ReleaseStringUTFChars(ip, p_ip);

	return ret;
}

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    unsubscribeEvent
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_unsubscribeEvent(JNIEnv *env, jobject obj)
{
	return g_AJClientEntry->getAirMediaClient()->unsubscribeEvent();
}

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setVolume
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_AJClient_setVolume(JNIEnv *env, jobject obj, jint volume)
{
	return g_AJClientEntry->getAirMediaClient()->volume()->setVolume((int)volume);
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    getVolume
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_AJClient_getVolume(JNIEnv *env, jobject obj)
{
	return g_AJClientEntry->getAirMediaClient()->volume()->getVolume();
}


#if 0
/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setStatus
 * Signature: (I)V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_client_AJClient_setStatus(JNIEnv *env, jobject obj, jint state)
{
	g_AJClientEntry->getAirMediaClient()->setStatus((AirJoyStatus)state);
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    status
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_AJClient_status(JNIEnv *env, jobject obj)
{
	return (int)(g_AJClientEntry->getAirMediaClient()->status());
}
#endif


#if 0
/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    send
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL 
Java_com_fqx_airjoy_client_AJClient_send__Ljava_lang_String_2(JNIEnv *env, jobject obj, jstring msg)
{
	const char *p_msg = env->GetStringUTFChars(msg, NULL);
    if (p_msg == NULL) {  // Out of memory
        return false;
    }

	jboolean ret = g_AJClientEntry->getAirMediaClient()->send(p_msg);

	env->ReleaseStringUTFChars(msg, p_msg);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    send
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_AJClient_send__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2I(JNIEnv *env, jobject obj, 
		jstring xmlns, jstring appAction, jstring appParam, jint type)
{
	int ret = ResultCodeErrorNotSupport;   // 不支持的请求

	const char *p_xmlns = env->GetStringUTFChars(xmlns, NULL);
    if (p_xmlns == NULL) {  
        return ResultCodeErrorParam;
    }

	const char *p_appAction = env->GetStringUTFChars(appAction, NULL);
    if (p_appAction == NULL) {  
		env->ReleaseStringUTFChars(xmlns, p_xmlns);
        return ResultCodeErrorParam;
    }

	const char *p_appParam = env->GetStringUTFChars(appParam, NULL);
    if (p_appParam == NULL) {  
		env->ReleaseStringUTFChars(xmlns, p_xmlns);
		env->ReleaseStringUTFChars(appAction, p_appAction);
        return ResultCodeErrorParam;
    }

	//int ret = g_AJClientEntry->getAirMediaClient()->send(p_xmlns, p_appAction, p_appParam, (MessageType)type);

	env->ReleaseStringUTFChars(xmlns, p_xmlns);
	env->ReleaseStringUTFChars(appAction, p_appAction);
	env->ReleaseStringUTFChars(appParam, p_appParam);

	return ret;
}
#endif

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    send
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_AJClient_send(JNIEnv *env, jobject obj, jstring appAction, jstring appParam, jint type)
{
	int ret = ResultCodeErrorNotSupport;   // 不支持的请求

	const char *p_appAction = env->GetStringUTFChars(appAction, NULL);
    if (p_appAction == NULL) {  
        return ResultCodeErrorParam;
    }

	const char *p_appParam = env->GetStringUTFChars(appParam, NULL);
    if (p_appParam == NULL) {  
		env->ReleaseStringUTFChars(appAction, p_appAction);
        return ResultCodeErrorParam;
    }

	ret = g_AJClientEntry->getAirMediaClient()->send(p_appAction, p_appParam);

	env->ReleaseStringUTFChars(appAction, p_appAction);
	env->ReleaseStringUTFChars(appParam, p_appParam);

	return ret;
}

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    native_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_client_AJClient_native_1init(JNIEnv *env, jclass cls)
{
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    native_setup
 * Signature: ()V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_client_AJClient_native_1setup(JNIEnv *env, jobject obj)
{
	g_AJClientEntry = new AJClientEntry(env, obj);
	LOGI("native_setup env=%d", env);
}


/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    native_finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL 
Java_com_fqx_airjoy_client_AJClient_native_1finalize(JNIEnv *env, jobject obj)
{
}


/*
* Set some test stuff up.
* Returns the JNI version on success, -1 on failure.
*/
JNIEXPORT jint JNICALL 
JNI_OnLoad(JavaVM* vm, void* reserved)
{
	JNIEnv* env = NULL;
	jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("Err: JNI_OnLoad");
		return -1;
	}

	result = JNI_VERSION_1_4;

	LOGI("JNI_OnLoad: JNI_VERSION_1_4");
	return result;
}
