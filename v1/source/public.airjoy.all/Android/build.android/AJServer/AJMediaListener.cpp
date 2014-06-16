
#include <stdio.h>
#include <stdlib.h>

#include "AJServer.h"
#include "AirMediaListener.hpp"
#include "AJMediaListener.h"

// 会话类接口

/**
 * 订阅事件
 *  @fromIp         [in] 订阅者IP
 *  @topic          [in] 订阅主题
 */
void AJMediaListener::didSubscribeEvent(const std::string &fromIp, const std::string &topic)
{
	LOGI("didSubscribeEvent ip=%s topic=%s", fromIp.c_str(), topic.c_str());
}


/**
 * 取消订阅事件
 *  @fromIp         [in] 订阅者IP
 *  @topic          [in] 订阅主题
 */        
void AJMediaListener::didUnsubscribeEvent(const std::string &fromIp, const std::string &topic)
{
	LOGI("didUnsubscribeEvent ip=%s topic=%s", fromIp.c_str(), topic.c_str());
}

// 照片类接口
/**
 * 缓存照片流
 *  @url            [in] 照片流地址
 *  @photoId        [in] 标识照片的唯一字符串
 *  @name           [in] 照片名称(可选)
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 显示照片流
 *  @url            [in] 照片流地址
 *  @photoId        [in] 标识照片的唯一字符串
 *  @name           [in] 照片名称(可选)
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 显示已经缓存的照片流
 *  @photoId        [in] 标识照片的唯一字符串
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 停止显示照片
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 旋转照片
 *  @photoId        [in] 标识照片的唯一字符串
 *  @direction      [in] 旋转方向
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 缩放照片
 *  @photoId        [in] 标识照片的唯一字符串
 *  @rate           [in] 缩放比例
 *                       rate = 0: 缩放至最合适大小
 *                       rate = 1: 缩放至原大小
 *                       rate > 1: 放大
 *                       0 < rate < 1, 缩小
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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


// 视频类接口

/**
 * 播放视频流
 *  @url            [in] 视频流地址
 *  @videoId        [in] 标识视频的唯一字符串
 *  @name           [in] 视频名称(可选)
 *  @position       [in] 播放时间点，单位为秒
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 设置回放速度
 *  @url            [in] 视频流地址
 *  @videoId        [in] 标识视频的唯一字符串
 *  @rate           [in] 0: 暂停，1: 正常速率播放
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 停止播放视频
 *  @videoId        [in] 标识视频的唯一字符串
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 设置播放进度
 *  @videoId        [in] 标识视频的唯一字符串
 *  @playPosition   [in] 当前播放视频的时间点，单位为秒
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 获取播放进度
 *  @videoId        [in] 标识视频的唯一字符串
 *  @rate           [out] 0:暂停，1:播放
 *  @position       [out] 当前播放/暂停时间点，单位为秒
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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
 * 获取播放信息
 *  @info           [out] 视频信息
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
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

// 音频类接口

/**
 * 播放音频流
 *  @url            [in] 音频流地址
 *  @videoId        [in] 标识音频的唯一字符串
 *  @name           [in] 视频名称(可选)
 *  @position       [in] 播放时间点，单位为秒
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
 */
ResultCode AJMediaListener::playAudio(const std::string &audioId,
										 const std::string &url,
										 const std::string &name,
										 float position,const std::string &fromIp)
{ 
	return ResultCodeErrorNotSupport; 
}

/**
 * 设置回放速度
 *  @url            [in] 音频流地址
 *  @videoId        [in] 标识视频的唯一字符串
 *  @rate           [in] 0: 暂停，1: 正常速率播放
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
 */
ResultCode AJMediaListener::setPlayAudioRate(const std::string &audioId,
										 float rate,const std::string &fromIp)
{ 
	return ResultCodeErrorNotSupport; 
}

/**
 * 停止播放音频
 *  @videoId        [in] 标识音频的唯一字符串
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
 */
ResultCode AJMediaListener::stopPlayAudio(const std::string &audioId,const std::string &fromIp)
{
	return ResultCodeErrorNotSupport;
}

/**
 * 设置播放进度
 *  @videoId        [in] 标识音频的唯一字符串
 *  @playPosition   [in] 当前播放音频的时间点，单位为秒
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
 */
ResultCode AJMediaListener::setPlayAudioProgress(const std::string &audioId,
										  float playPosition,const std::string &fromIp)
{
	return ResultCodeErrorNotSupport;
}

/**
 * 获取播放进度
 *  @videoId        [in] 标识音频的唯一字符串
 *  @rate           [out] 0:暂停，1:播放
 *  @position       [out] 当前播放/暂停时间点，单位为秒
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
 */
ResultCode AJMediaListener::getPlayAudioProgress(const std::string &audioId,
										  float *rate,
										  float *position,const std::string &fromIp)
{
	return ResultCodeErrorNotSupport;
}

/**
 * 获取播放信息
 *  @info           [out] 音频信息
 *  @return
 *     ResultCodeOk - 成功
 *     ResultCodeErrorNotSupport - 不支持此功能
 */
ResultCode AJMediaListener::getPlayAudioInfo(AirJoyPlayBackInfo *info,const std::string &fromIp)
{
	return ResultCodeErrorNotSupport;
}

// 音量接口

/**
 * 设置音量
 *  @volume         [in] 当前音量(1-100)
 *  @return 会话ID
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
 * 获取音量
 *  @volume         [out] 当前音量(1-100)
 *  @return 会话ID
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
