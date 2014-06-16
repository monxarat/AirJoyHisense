
#include "AJClientEntry.h"
#include "com_fqx_airjoy_client_MediaVideo.h"


/*
 * Class:     com_fqx_airjoy_client_MediaVideo
 * Method:    playVideo
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;F)I
 */
JNIEXPORT jint JNICALL
Java_com_fqx_airjoy_client_MediaVideo_playVideo(JNIEnv *env, jobject obj, jstring url, jstring id, jstring encoding, jfloat position)
{
	int ret;

	const char *p_url = env->GetStringUTFChars(url, NULL);
    if (p_url == NULL) { 
        return ResultCodeErrorParam;
    }

	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
		env->ReleaseStringUTFChars(url, p_url);
        return ResultCodeErrorParam;
    }

	const char *p_encoding = env->GetStringUTFChars(encoding, NULL);
    if (p_encoding == NULL) { 
		env->ReleaseStringUTFChars(url, p_url);
		env->ReleaseStringUTFChars(id, p_id);
        return ResultCodeErrorParam;
	}

	ret = g_AJClientEntry->getAirMediaClient()->video()->playVideo( p_url, p_id, p_encoding, position);


	env->ReleaseStringUTFChars(url, p_url);
	env->ReleaseStringUTFChars(id, p_id);
	env->ReleaseStringUTFChars(encoding, p_encoding);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaVideo
 * Method:    setPlayVideoRate
 * Signature: (Ljava/lang/String;F)I
 */
JNIEXPORT jint JNICALL
Java_com_fqx_airjoy_client_MediaVideo_setPlayVideoRate(JNIEnv *env, jobject obj, jstring id, jfloat rate)
{
	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->video()->setPlayVideoRate( p_id, rate);

	env->ReleaseStringUTFChars(id, p_id);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaVideo
 * Method:    stopPlayVideo
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_fqx_airjoy_client_MediaVideo_stopPlayVideo(JNIEnv *env, jobject obj, jstring id)
{
	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->video()->stopPlayVideo( p_id);

	env->ReleaseStringUTFChars(id, p_id);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaVideo
 * Method:    setPlayVideoProgress
 * Signature: (Ljava/lang/String;F)I
 */
JNIEXPORT jint JNICALL
Java_com_fqx_airjoy_client_MediaVideo_setPlayVideoProgress(JNIEnv *env, jobject obj, jstring id, jfloat position)
{
	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->video()->setPlayVideoProgress( p_id, position);

	env->ReleaseStringUTFChars(id, p_id);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaVideo
 * Method:    getPlayVideoProgress
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_com_fqx_airjoy_client_MediaVideo_getPlayVideoProgress(JNIEnv *env, jobject obj, jstring id)
{
	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->video()->getPlayVideoProgress( p_id);

	env->ReleaseStringUTFChars(id, p_id);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaVideo
 * Method:    getPlayVideoInfo
 * Signature: ()I
 */
JNIEXPORT jint JNICALL
Java_com_fqx_airjoy_client_MediaVideo_getPlayVideoInfo(JNIEnv *env, jobject obj)
{
	int ret = g_AJClientEntry->getAirMediaClient()->video()->getPlayVideoInfo();

	return ret;
}
