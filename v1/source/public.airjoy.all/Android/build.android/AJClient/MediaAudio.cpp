

#include "AJClientEntry.h"
#include "com_fqx_airjoy_client_MediaAudio.h"

/*
 * Class:     com_fqx_airjoy_client_MediaAudio
 * Method:    playAudio
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;F)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaAudio_playAudio(JNIEnv *env, jobject obj, jstring url, jstring id, jstring encoding, jfloat position)
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

	ret = g_AJClientEntry->getAirMediaClient()->audio()->playAudio( p_url, p_id, p_encoding, position);


	env->ReleaseStringUTFChars(url, p_url);
	env->ReleaseStringUTFChars(id, p_id);
	env->ReleaseStringUTFChars(encoding, p_encoding);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaAudio
 * Method:    setPlayAudioRate
 * Signature: (Ljava/lang/String;F)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaAudio_setPlayAudioRate(JNIEnv *env, jobject obj, jstring id, jfloat rate)
{
	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->audio()->setPlayAudioRate( p_id, rate);

	env->ReleaseStringUTFChars(id, p_id);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaAudio
 * Method:    stopPlayAudio
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaAudio_stopPlayAudio(JNIEnv *env, jobject obj, jstring id)
{
	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->audio()->stopPlayAudio( p_id);

	env->ReleaseStringUTFChars(id, p_id);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaAudio
 * Method:    setPlayAudioProgress
 * Signature: (Ljava/lang/String;F)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaAudio_setPlayAudioProgress(JNIEnv *env, jobject obj, jstring id, jfloat position)
{
	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->audio()->setPlayAudioProgress( p_id, position);

	env->ReleaseStringUTFChars(id, p_id);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaAudio
 * Method:    getPlayAudioProgress
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaAudio_getPlayAudioProgress(JNIEnv *env, jobject obj, jstring id)
{
	const char *p_id = env->GetStringUTFChars(id, NULL);
    if (p_id == NULL) { 
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->audio()->getPlayAudioProgress( p_id);

	env->ReleaseStringUTFChars(id, p_id);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaAudio
 * Method:    getPlayAudioInfo
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaAudio_getPlayAudioInfo(JNIEnv *env, jobject obj)
{
	int ret = g_AJClientEntry->getAirMediaClient()->audio()->getPlayAudioInfo();

	return ret;
}

