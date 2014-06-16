/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_fqx_airjoy_client_AJClient */

#ifndef _Included_com_fqx_airjoy_client_AJClient
#define _Included_com_fqx_airjoy_client_AJClient
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setDiscoveryListener
 * Signature: (Lcom/fqx/airjoy/clientlistener/AirJoyNsdDiscoveryListener;)V
 */
JNIEXPORT void JNICALL Java_com_fqx_airjoy_client_AJClient_setDiscoveryListener
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setMediaClientListener
 * Signature: (Lcom/fqx/airjoy/clientlistener/AirMediaClientListener;)V
 */
JNIEXPORT void JNICALL Java_com_fqx_airjoy_client_AJClient_setMediaClientListener
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setConnectServerListener
 * Signature: (Lcom/fqx/airjoy/clientlistener/ConnectServerListener;)V
 */
JNIEXPORT void JNICALL Java_com_fqx_airjoy_client_AJClient_setConnectServerListener
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setEventListener
 * Signature: (Lcom/fqx/airjoy/clientlistener/AirClientEventListener;)V
 */
JNIEXPORT void JNICALL Java_com_fqx_airjoy_client_AJClient_setEventListener
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    startDiscoriverService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_airjoy_client_AJClient_startDiscoriverService
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    startMeidaService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_airjoy_client_AJClient_startMeidaService
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    startFileService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_airjoy_client_AJClient_startFileService
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    stopDiscoriverService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_airjoy_client_AJClient_stopDiscoriverService
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    stopMeidaService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_airjoy_client_AJClient_stopMeidaService
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    stopFileService
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_airjoy_client_AJClient_stopFileService
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    subscribeEvent
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_airjoy_client_AJClient_subscribeEvent
  (JNIEnv *, jobject, jstring, jint);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    unsubscribeEvent
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_com_fqx_airjoy_client_AJClient_unsubscribeEvent
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setVolume
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_com_fqx_airjoy_client_AJClient_setVolume
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    getVolume
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_fqx_airjoy_client_AJClient_getVolume
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    setStatus
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_com_fqx_airjoy_client_AJClient_setStatus
  (JNIEnv *, jobject, jint);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    status
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_com_fqx_airjoy_client_AJClient_status
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    send
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_com_fqx_airjoy_client_AJClient_send
  (JNIEnv *, jobject, jstring, jstring, jint);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    native_init
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fqx_airjoy_client_AJClient_native_1init
  (JNIEnv *, jclass);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    native_setup
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fqx_airjoy_client_AJClient_native_1setup
  (JNIEnv *, jobject);

/*
 * Class:     com_fqx_airjoy_client_AJClient
 * Method:    native_finalize
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_fqx_airjoy_client_AJClient_native_1finalize
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
