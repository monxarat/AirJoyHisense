
#include <stdio.h>
#include <stdlib.h>

#include "AJServer.h"
#include "AirMediaListener.hpp"
#include "AJMediaListener.h"

// �Ự��ӿ�

/**
 * �����¼�
 *  @fromIp         [in] ������IP
 *  @topic          [in] ��������
 */
void AJMediaListener::didSubscribeEvent(const std::string &fromIp, const std::string &topic)
{
	LOGI("didSubscribeEvent ip=%s topic=%s", fromIp.c_str(), topic.c_str());
}


/**
 * ȡ�������¼�
 *  @fromIp         [in] ������IP
 *  @topic          [in] ��������
 */        
void AJMediaListener::didUnsubscribeEvent(const std::string &fromIp, const std::string &topic)
{
	LOGI("didUnsubscribeEvent ip=%s topic=%s", fromIp.c_str(), topic.c_str());
}

// ��Ƭ��ӿ�
/**
 * ������Ƭ��
 *  @url            [in] ��Ƭ����ַ
 *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
 *  @name           [in] ��Ƭ����(��ѡ)
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::cachePhoto(const std::string &photoId,
										 const std::string &url,
										 const std::string &name,const std::string &fromIp)
{ 
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getPhotoListenerObj();
	LOGE("Start->(cachePhoto)");
	if(obj == NULL) {
		LOGE("ERR-> ResultCodeErrorNotSupport(cachePhoto)");
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("ERR-> AttachCurrentThread(cachePhoto)");
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		LOGE("ERR-> GetObjectClass<cachePhoto> .\n");
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "cachePhoto", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if (mid == NULL)
    {
		LOGE("ERR-> GetMethodID<cachePhoto> .\n");
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(photoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	jstring jurl = env->NewStringUTF(url.c_str());
	if(jurl == NULL) {
		env->DeleteLocalRef(jid);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	jstring jname = env->NewStringUTF(name.c_str());
	if(jname == NULL) {
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jurl);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	jstring jfromIp = env->NewStringUTF(fromIp.c_str());
	if(jfromIp == NULL) {
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jurl);
		env->DeleteLocalRef(jname);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	int ret = env->CallIntMethod(obj, mid, jid, jurl, jname, jfromIp);

	env->DeleteLocalRef(jid);
	env->DeleteLocalRef(jurl);
	env->DeleteLocalRef(jname);
	env->DeleteLocalRef(jfromIp);
	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

/**
 * ��ʾ��Ƭ��
 *  @url            [in] ��Ƭ����ַ
 *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
 *  @name           [in] ��Ƭ����(��ѡ)
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::displayPhoto(const std::string &photoId,
										 const std::string &url,
										 const std::string &name,const std::string &fromIp)
{ 
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getPhotoListenerObj();
	LOGE("Start->(displayPhoto)");
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "displayPhoto", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)I");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(photoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	jstring jurl = env->NewStringUTF(url.c_str());
	if(jurl == NULL) {
		env->DeleteLocalRef(jid);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	jstring jname = env->NewStringUTF(name.c_str());
	if(jname == NULL) {
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jurl);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	jstring jfromIp = env->NewStringUTF(fromIp.c_str());
	if(jfromIp == NULL) {
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jurl);
		env->DeleteLocalRef(jname);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret = env->CallIntMethod(obj, mid, jid, jurl, jname, jfromIp);

	g_AJServer->getAJState()->initState(photoId, url, name);
	g_AJServer->getAJState()->setMediaTypePhoto();
	g_AJServer->getAJState()->setStatePlay(fromIp);

	env->DeleteLocalRef(jid);
	env->DeleteLocalRef(jurl);
	env->DeleteLocalRef(jname);
	env->DeleteLocalRef(jfromIp);
	p_vm->DetachCurrentThread();

    
	return (ResultCode)ret; 
}

/**
 * ��ʾ�Ѿ��������Ƭ��
 *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::displayCachedPhoto(const std::string &photoId,const std::string &fromIp)
{ 
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getPhotoListenerObj();
	if(obj == NULL) {
		return ResultCodeErrorParam; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "displayCachedPhoto", "(Ljava/lang/String;)I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(photoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	int ret = env->CallIntMethod(obj, mid, jid);

#if 1
	g_AJServer->getAJState()->initState(photoId, " ", "  ");
	g_AJServer->getAJState()->setMediaTypePhoto();
	g_AJServer->getAJState()->setStatePlay(fromIp);
#endif

	env->DeleteLocalRef(jid);
	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

/**
 * ֹͣ��ʾ��Ƭ
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::stopPhoto(const std::string &fromIp)
{ 
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getPhotoListenerObj();
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "stopPhoto", "()I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	int ret = env->CallIntMethod(obj, mid);

	
	g_AJServer->getAJState()->setStateStop(fromIp);

	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

/**
 * ��ת��Ƭ
 *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
 *  @direction      [in] ��ת����
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::rotatePhoto(const std::string &photoId,
										 PhotoDirectionType direction,const std::string &fromIp)
{ 
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getPhotoListenerObj();
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "rotatePhoto", "(Ljava/lang/String;I)I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(photoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret = env->CallIntMethod(obj, mid, jid, int(direction));

	env->DeleteLocalRef(jid);
	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

/**
 * ������Ƭ
 *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
 *  @rate           [in] ���ű���
 *                       rate = 0: ����������ʴ�С
 *                       rate = 1: ������ԭ��С
 *                       rate > 1: �Ŵ�
 *                       0 < rate < 1, ��С
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::zoomPhoto(const std::string &photoId,
										 float rate,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getPhotoListenerObj();
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "zoomPhoto", "(Ljava/lang/String;F)I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(photoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret = env->CallIntMethod(obj, mid, jid, rate);

	env->DeleteLocalRef(jid);
	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

ResultCode AJMediaListener::movePhoto(const std::string &photoId, float x, float y,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getPhotoListenerObj();
	if(obj == NULL) {
		LOGE("ERR: movePhoto getPhotoListenerObj=null");
		return ResultCodeErrorParam; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "movePhoto", "(Ljava/lang/String;FF)I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(photoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret = env->CallIntMethod(obj, mid, jid, x, y);

	env->DeleteLocalRef(jid);
	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}


// ��Ƶ��ӿ�

/**
 * ������Ƶ��
 *  @url            [in] ��Ƶ����ַ
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @name           [in] ��Ƶ����(��ѡ)
 *  @position       [in] ����ʱ��㣬��λΪ��
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::playVideo(const std::string &videoId,
										 const std::string &url,
										 const std::string &name,
										 float position,const std::string &fromIp)
{ 
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getVideoListenerObj();
	LOGI("Start->(playVideo)");
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "playVideo", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;FLjava/lang/String;)I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(videoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	jstring jurl = env->NewStringUTF(url.c_str());
	if(jurl == NULL) {
		env->DeleteLocalRef(jid);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	jstring jname = env->NewStringUTF(name.c_str());
	if(jname == NULL) {
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jurl);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	jstring jfromIp = env->NewStringUTF(fromIp.c_str());
	if(jfromIp== NULL) {
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jurl);
		env->DeleteLocalRef(jname);
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret = env->CallIntMethod(obj, mid, jid, jurl, jname, position, jfromIp);

	env->DeleteLocalRef(jid);
	env->DeleteLocalRef(jurl);
	env->DeleteLocalRef(jname);
	env->DeleteLocalRef(jfromIp);

	p_vm->DetachCurrentThread();
    
	g_AJServer->getAJState()->initState(videoId, url, name);
	g_AJServer->getAJState()->setMediaTypeVideo();
	g_AJServer->getAJState()->setStatePlay(fromIp);

	return (ResultCode)ret; 
}

/**
 * ���ûط��ٶ�
 *  @url            [in] ��Ƶ����ַ
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @rate           [in] 0: ��ͣ��1: �������ʲ���
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::setPlayVideoRate(const std::string &videoId,
										 float rate,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getVideoListenerObj();
	LOGE("Start->(setPlayVideoRate)");
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "setPlayVideoRate", "(Ljava/lang/String;F)I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(videoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret = env->CallIntMethod(obj, mid, jid, rate);

	int m_rate = (int) rate;
	if(m_rate == 0) {
		g_AJServer->getAJState()->setStatePause(fromIp);
	}else{
		g_AJServer->getAJState()->setStatePlay(fromIp);
	}
	env->DeleteLocalRef(jid);

	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

/**
 * ֹͣ������Ƶ
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::stopPlayVideo(const std::string &videoId,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getVideoListenerObj();
	LOGE("Start->(stopPlayVideo)");
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "stopPlayVideo", "(Ljava/lang/String;)I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(videoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret = env->CallIntMethod(obj, mid, jid);

	g_AJServer->getAJState()->setStateStop(fromIp);

	env->DeleteLocalRef(jid);

	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

/**
 * ���ò��Ž���
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::setPlayVideoProgress(const std::string &videoId,
										  float playPosition,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getVideoListenerObj();
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "setPlayVideoProgress", "(Ljava/lang/String;F)I");
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(videoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret = env->CallIntMethod(obj, mid, jid, playPosition);

	env->DeleteLocalRef(jid);

	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

bool getVideoInfo2C(JNIEnv *env, jobject j_obj, float *p_position, float *p_rate)
{
	jstring jstr;
	jclass cls = env->GetObjectClass(j_obj);

	jfieldID jplayPosition = env->GetFieldID(cls, "m_postion", "F"); 
	jfloat position = env->GetFloatField(j_obj, jplayPosition);
	*p_position = position;

	jfieldID jrate = env->GetFieldID(cls, "m_rate", "F"); 
	jfloat rate = env->GetFloatField(j_obj, jrate);
	*p_rate = rate;

	return true;
}

#if 0
bool getAJPlaybackInfo2C(JNIEnv *env, jobject j_obj, AirJoyPlayBackInfo *p_PlaybackInfo)
{
	jstring jstr;
	jclass cls = env->GetObjectClass(j_obj);

#if 0
	jfieldID jname = env->GetFieldID(cls, "mname", "Ljava/lang/String;");
	jstr = (jstring)env->GetObjectField(j_obj, jname);
	const char *pname = env->GetStringUTFChars(jstr, NULL);
    if (pname == NULL) {  // Out of memory
        return false;
    }
	p_PlaybackInfo->name = pname;
	env->ReleaseStringUTFChars(jstr, pname);

	jfieldID jid = env->GetFieldID(cls, "mid", "Ljava/lang/String;");
	jstr = (jstring)env->GetObjectField(j_obj, jid);
	const char *pid = env->GetStringUTFChars(jstr, NULL);
    if (pid == NULL) {  // Out of memory
        return false;
    }
	p_PlaybackInfo->id = pid ;
	env->ReleaseStringUTFChars(jstr, pid);

	jfieldID jurl = env->GetFieldID(cls, "murl", "Ljava/lang/String;");
	jstr = (jstring)env->GetObjectField(j_obj, jurl);
	const char *purl = env->GetStringUTFChars(jstr, NULL);
    if (purl == NULL) {  // Out of memory
        return false;
    }
	p_PlaybackInfo->url = purl;
	env->ReleaseStringUTFChars(jstr, purl);
#endif

	jfieldID jplayPosition = env->GetFieldID(cls, "mposition", "F"); 
	jfloat position = env->GetIntField(j_obj, jplayPosition);
	p_PlaybackInfo->position = position;

	jfieldID jduration = env->GetFieldID(cls, "mduration", "F"); 
	jfloat duration = env->GetIntField(j_obj, jduration);
	p_PlaybackInfo->duration = duration;

	jfieldID jrate = env->GetFieldID(cls, "mrate", "F"); 
	jfloat rate = env->GetIntField(j_obj, jrate);
	p_PlaybackInfo->rate = rate;

	return true;
}
#else
bool getAJPlaybackInfo2C(JNIEnv *env, jobject j_obj, AirJoyPlayBackInfo &p_PlaybackInfo)
{
	jstring jstr;
	jclass cls = env->GetObjectClass(j_obj);

#if 0
	jfieldID jname = env->GetFieldID(cls, "mname", "Ljava/lang/String;");
	jstr = (jstring)env->GetObjectField(j_obj, jname);
	const char *pname = env->GetStringUTFChars(jstr, NULL);
    if (pname == NULL) {  // Out of memory
        return false;
    }
	p_PlaybackInfo->name = pname;
	env->ReleaseStringUTFChars(jstr, pname);

	jfieldID jid = env->GetFieldID(cls, "mid", "Ljava/lang/String;");
	jstr = (jstring)env->GetObjectField(j_obj, jid);
	const char *pid = env->GetStringUTFChars(jstr, NULL);
    if (pid == NULL) {  // Out of memory
        return false;
    }
	p_PlaybackInfo->id = pid ;
	env->ReleaseStringUTFChars(jstr, pid);

	jfieldID jurl = env->GetFieldID(cls, "murl", "Ljava/lang/String;");
	jstr = (jstring)env->GetObjectField(j_obj, jurl);
	const char *purl = env->GetStringUTFChars(jstr, NULL);
    if (purl == NULL) {  // Out of memory
        return false;
    }
	p_PlaybackInfo->url = purl;
	env->ReleaseStringUTFChars(jstr, purl);
#endif

	jfieldID jplayPosition = env->GetFieldID(cls, "mposition", "F"); 
	jfloat position = env->GetFloatField(j_obj, jplayPosition);
	p_PlaybackInfo.position = position;

	jfieldID jduration = env->GetFieldID(cls, "mduration", "F"); 
	jfloat duration = env->GetFloatField(j_obj, jduration);
	p_PlaybackInfo.duration = duration;

	jfieldID jrate = env->GetFieldID(cls, "mrate", "F"); 
	jfloat rate = env->GetFloatField(j_obj, jrate);
	p_PlaybackInfo.rate = rate;

	return true;
}

#endif

#define  LINK_PLAYBACKINFO 	"()Lcom/fqx/airjoy/server/AJPlayBackInfo;"
#define  LINK_VIDEOINFO 	"(Ljava/lang/String;)Lcom/fqx/airjoy/server/AJVideoInfo;"


/**
 * ��ȡ���Ž���
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @rate           [out] 0:��ͣ��1:����
 *  @position       [out] ��ǰ����/��ͣʱ��㣬��λΪ��
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::getPlayVideoProgress(const std::string &videoId,
										  float *rate,
										  float *position,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getVideoListenerObj();
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "getPlayVideoProgress", LINK_VIDEOINFO);
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jstring jid = env->NewStringUTF(videoId.c_str());
	if(jid == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}

	int ret;
	jobject videoinfo_obj = env->CallObjectMethod(obj, mid, jid);
	if(videoinfo_obj == NULL)
		ret = ResultCodeError;
	else {
		getVideoInfo2C(env, videoinfo_obj, position, rate);
		ret = ResultCodeOk;
	}
	

	env->DeleteLocalRef(jid);

	p_vm->DetachCurrentThread();
    
	return (ResultCode)ret; 
}

#if 1
/**
 * ��ȡ������Ϣ
 *  @info           [out] ��Ƶ��Ϣ
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::getPlayVideoInfo(AirJoyPlayBackInfo &info,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getVideoListenerObj();
	if(obj == NULL) {
		return ResultCodeErrorNotSupport; 
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "getPlayVideoInfo", LINK_PLAYBACKINFO);
    if(mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }

	jobject videoinfo_obj = env->CallObjectMethod(obj, mid);
	bool ret = getAJPlaybackInfo2C(env, videoinfo_obj, info);
	LOGI("getPlayVideoInfo dur=%f, pos=%f", info.duration, info.position);

	p_vm->DetachCurrentThread();

	if(!ret)
		return ResultCodeError;

	return ResultCodeOk;
}
#endif

// ��Ƶ��ӿ�

/**
 * ������Ƶ��
 *  @url            [in] ��Ƶ����ַ
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @name           [in] ��Ƶ����(��ѡ)
 *  @position       [in] ����ʱ��㣬��λΪ��
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::playAudio(const std::string &audioId,
										 const std::string &url,
										 const std::string &name,
										 float position,const std::string &fromIp)
{ 
	return ResultCodeErrorNotSupport; 
}

/**
 * ���ûط��ٶ�
 *  @url            [in] ��Ƶ����ַ
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @rate           [in] 0: ��ͣ��1: �������ʲ���
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::setPlayAudioRate(const std::string &audioId,
										 float rate,const std::string &fromIp)
{ 
	return ResultCodeErrorNotSupport; 
}

/**
 * ֹͣ������Ƶ
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::stopPlayAudio(const std::string &audioId,const std::string &fromIp)
{
	return ResultCodeErrorNotSupport;
}

/**
 * ���ò��Ž���
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::setPlayAudioProgress(const std::string &audioId,
										  float playPosition,const std::string &fromIp)
{
	return ResultCodeErrorNotSupport;
}

/**
 * ��ȡ���Ž���
 *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
 *  @rate           [out] 0:��ͣ��1:����
 *  @position       [out] ��ǰ����/��ͣʱ��㣬��λΪ��
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::getPlayAudioProgress(const std::string &audioId,
										  float *rate,
										  float *position,const std::string &fromIp)
{
	return ResultCodeErrorNotSupport;
}

/**
 * ��ȡ������Ϣ
 *  @info           [out] ��Ƶ��Ϣ
 *  @return
 *     ResultCodeOk - �ɹ�
 *     ResultCodeErrorNotSupport - ��֧�ִ˹���
 */
ResultCode AJMediaListener::getPlayAudioInfo(AirJoyPlayBackInfo *info,const std::string &fromIp)
{
	return ResultCodeErrorNotSupport;
}

// �����ӿ�

/**
 * ��������
 *  @volume         [in] ��ǰ����(1-100)
 *  @return �ỰID
 */
ResultCode AJMediaListener::setVolume(int volume,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getMediaListenerObj();
	if(obj == NULL)
		return ResultCodeErrorParam;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		LOGE("ERR-> initSession <init> .\n");
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "setVolume", "(I)I");
    if (mid == NULL)
    {
		LOGE("GetMethodID: ERR-> initSession");
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }


	int ret = env->CallIntMethod(obj, mid, volume);

	p_vm->DetachCurrentThread();
    
    return (ResultCode)ret;

}


/**
 * ��ȡ����
 *  @volume         [out] ��ǰ����(1-100)
 *  @return �ỰID
 */
ResultCode AJMediaListener::getVolume(int * volume,const std::string &fromIp)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getMediaListenerObj();
	if(obj == NULL)
		return ResultCodeErrorParam;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return ResultCodeErrorParam;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
	}
	
	mid = env->GetMethodID(env_class, "getVolume", "()I");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return ResultCodeErrorParam;
    }


	int v = env->CallIntMethod(obj, mid);
	 
	*volume = v;

	p_vm->DetachCurrentThread();
    
    return ResultCodeOk;
}
