
#include "AJClientEntry.h"
#include "com_fqx_airjoy_client_MediaPhoto.h"

/*
 * Class:     com_fqx_airjoy_client_MediaPhoto
 * Method:    displayPhoto
 * Signature: (Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaPhoto_displayPhoto(JNIEnv *env, jobject obj, jstring url, jstring id, jint action, jstring name)
{
	const char *purl = env->GetStringUTFChars(url, NULL);
    if (purl == NULL) {  // Out of memory
        return ResultCodeErrorParam;
    }

	const char *pid = env->GetStringUTFChars(id, NULL);
    if (pid == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(url, purl);
        return ResultCodeErrorParam;
    }

	const char *p_name = env->GetStringUTFChars(name, NULL);
    if (p_name == NULL) {  // Out of memory
		env->ReleaseStringUTFChars(url, purl);
		env->ReleaseStringUTFChars(id, pid);
        return ResultCodeErrorParam;
    }

	std::string murl = g_AJClientEntry->getAirFileServer()->addSharedFile(p_name, purl);
	LOGI("name=%s, action=%d, getUrl=%s", p_name, action, murl.c_str());

	int ret = g_AJClientEntry->getAirMediaClient()->photo()->displayPhoto(
			murl, 
			pid, 
			(PhotoActionType)action, 
			p_name);


	env->ReleaseStringUTFChars(url, purl);
	env->ReleaseStringUTFChars(id, pid);
	env->ReleaseStringUTFChars(name, p_name);

	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaPhoto
 * Method:    displayCachedPhoto
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaPhoto_displayCachedPhoto(JNIEnv *env, jobject obj, jstring id)
{
	const char *pid = env->GetStringUTFChars(id, NULL);
    if (pid == NULL) {
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->photo()->displayCachedPhoto(pid);

	env->ReleaseStringUTFChars(id, pid);

	return ret;

}


/*
 * Class:     com_fqx_airjoy_client_MediaPhoto
 * Method:    stopDisplayPhoto
 * Signature: ()I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaPhoto_stopDisplayPhoto(JNIEnv *env, jobject obj)
{
	int ret = g_AJClientEntry->getAirMediaClient()->photo()->stopDisplayPhoto();
	return ret;
}


/*
 * Class:     com_fqx_airjoy_client_MediaPhoto
 * Method:    rotatePhoto
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaPhoto_rotatePhoto(JNIEnv *env, jobject obj, jstring id, jint direction)
{
	const char *pid = env->GetStringUTFChars(id, NULL);
    if (pid == NULL) {
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->photo()->rotatePhoto( pid, (PhotoDirectionType)direction);

	env->ReleaseStringUTFChars(id, pid);

	return ret;

}


/*
 * Class:     com_fqx_airjoy_client_MediaPhoto
 * Method:    zoomPhoto
 * Signature: (Ljava/lang/String;F)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaPhoto_zoomPhoto(JNIEnv *env, jobject obj, jstring id, jfloat rate)
{
	const char *pid = env->GetStringUTFChars(id, NULL);
    if (pid == NULL) {
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->photo()->zoomPhoto( pid, rate);

	env->ReleaseStringUTFChars(id, pid);

	return ret;

}


/*
 * Class:     com_fqx_airjoy_client_MediaPhoto
 * Method:    movePhoto
 * Signature: (Ljava/lang/String;FF)I
 */
JNIEXPORT jint JNICALL 
Java_com_fqx_airjoy_client_MediaPhoto_movePhoto(JNIEnv *env, jobject obj, jstring id, jfloat x, jfloat y)
{
	const char *pid = env->GetStringUTFChars(id, NULL);
    if (pid == NULL) {
        return ResultCodeErrorParam;
    }

	int ret = g_AJClientEntry->getAirMediaClient()->photo()->movePhoto( pid, x, y);

	env->ReleaseStringUTFChars(id, pid);

	return ret;

}

