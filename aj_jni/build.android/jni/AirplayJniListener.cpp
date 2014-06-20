
#include "JNIGlobalParam.h"
#include "AirplayJniListener.h"

extern AirPlayServer gAirPlayServer;
/**
 * �����¼�
 *  @clientIp��        �ͻ���ip
 */
void AirplayJniListener::didSubscribeEvent(const std::string &clientIp)
{
	LOGI("didSubscribeEvent ip= %s", clientIp.c_str());
}

/**
 * ȡ�������¼�
 *  @clientIp��        �ͻ���ip
 */
void AirplayJniListener::didUnsubscribeEvent(const std::string &clientIp)
{
	LOGI("didUnsubscribeEvent ip= %s", clientIp.c_str());
}

// ����������Ƭ
void AirplayJniListener::willPutPhoto(const std::string &photoId, const std::string &clientIp)
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;


	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	p_JNIGlobalParam->tryStopClient(clientIp);
	p_JNIGlobalParam->setMediaPhoto();


	LOGI("++++++++++++ willPutPhoto = %s", photoId.c_str());
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> willPutPhoto <GetObjectClass> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "willPutPhoto", "(Ljava/lang/String;Ljava/lang/String;)V");


	jstring jphotoId = env->NewStringUTF(photoId.c_str());
	if(jphotoId == NULL) {
		LOGE("CJNIDEBUG: ERR-> willPutPhoto <photoId> .\n");
		jvm->DetachCurrentThread();
		return;
	}

	jstring jclientIp = env->NewStringUTF(clientIp.c_str());
	if(jclientIp == NULL) {
		env->DeleteLocalRef(jphotoId);
		LOGE("CJNIDEBUG: ERR-> willPutPhoto <clientIp> .\n");
		jvm->DetachCurrentThread();
		return;
	}


	env->CallVoidMethod(obj, mid, jphotoId, jclientIp);

	env->DeleteLocalRef(jphotoId);
	env->DeleteLocalRef(jclientIp);

	jvm->DetachCurrentThread();
}

// ������Ƭ����Ҫ������ʾ��
void AirplayJniListener::didPutPhoto(const std::string &photoId,  const char *data, std::size_t length, const std::string &clientIp) 
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didPutPhoto = %s", photoId.c_str());
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didPutPhoto <GetObjectClass> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didPutPhoto", "(Ljava/lang/String;[BJ)V");


	jstring jphotoId = env->NewStringUTF(photoId.c_str());
	if(jphotoId == NULL) {
		LOGE("CJNIDEBUG: ERR-> didPutPhoto <photoId> .\n");
		jvm->DetachCurrentThread();
		return;
	}


	jbyte *jpbyte = (jbyte *)data;
	jlong jlength = (jlong)length;
	jbyteArray jbarray = env->NewByteArray(jlength);
	env->SetByteArrayRegion(jbarray, 0, jlength, jpbyte);

	env->CallVoidMethod(obj, mid, jphotoId, jbarray, jlength);
	env->DeleteLocalRef(jphotoId);

	jvm->DetachCurrentThread();
}

// ����������Ƭ
void AirplayJniListener::willPutPhotoCacheOnly(const std::string &photoId, const std::string &clientIp)
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	p_JNIGlobalParam->tryStopClient(clientIp);
	p_JNIGlobalParam->setMediaPhoto();

	LOGI("willPutPhotoCacheOnly = %s", photoId.c_str());
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> willPutPhotoCacheOnly <GetObjectClass> .\n");
		jvm->DetachCurrentThread();
		return;
	}

	mid = env->GetMethodID(envcls, "willPutPhotoCacheOnly", "(Ljava/lang/String;Ljava/lang/String;)V");


	jstring jphotoId = env->NewStringUTF(photoId.c_str());
	if(jphotoId == NULL) {
		LOGE("CJNIDEBUG: ERR-> willPutPhotoCacheOnly <photoId> .\n");
		jvm->DetachCurrentThread();
		return;
	}

	jstring jclientIp = env->NewStringUTF(clientIp.c_str());
	if(jclientIp == NULL) {
		LOGE("CJNIDEBUG: ERR-> willPutPhotoCacheOnly <clientIp> .\n");
		env->DeleteLocalRef(jphotoId);
		jvm->DetachCurrentThread();
		return;
	}


	env->CallVoidMethod(obj, mid, jphotoId, jclientIp);

	env->DeleteLocalRef(jphotoId);
	env->DeleteLocalRef(jclientIp);

	jvm->DetachCurrentThread();
}


// ������Ƭ�����棨��������ʾ������������
void AirplayJniListener::didPutPhotoCacheOnly(const std::string &photoId,   const char *data, std::size_t length, const std::string &clientIp) 
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didPutPhotoCacheOnly = %s", photoId.c_str());
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didPutPhoto <GetObjectClass> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didPutPhotoCacheOnly", "(Ljava/lang/String;[BJ)V");


	jstring jphotoId = env->NewStringUTF(photoId.c_str());
	if(jphotoId == NULL) {
		LOGE("CJNIDEBUG: ERR-> didPutPhoto <photoId> .\n");
		jvm->DetachCurrentThread();
		return;
	}


	jbyte *jpbyte = (jbyte *)data;
	jlong jlength = (jlong)length;
	jbyteArray jbarray = env->NewByteArray(jlength);
	env->SetByteArrayRegion(jbarray, 0, jlength, jpbyte);

	env->CallVoidMethod(obj, mid, jphotoId, jbarray, jlength);
	env->DeleteLocalRef(jphotoId);

	jvm->DetachCurrentThread();
}

// ��ʾ�Ѿ��������Ƭ
void AirplayJniListener::didDisplayCachedPhoto(const std::string &photoId,   const std::string &clientIp) 
{
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didDisplayCachedPhoto = %s", photoId.c_str());
	JNIEnv *env;
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didPutPhoto <GetObjectClass> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didDisplayCachedPhoto", "(Ljava/lang/String;)V");


	jstring jphotoId = env->NewStringUTF(photoId.c_str());
	if(jphotoId == NULL) {
		LOGE("CJNIDEBUG: ERR-> didPutPhoto <photoId> .\n");
		jvm->DetachCurrentThread();
		return;
	}

	env->CallVoidMethod(obj, mid, jphotoId);
	env->DeleteLocalRef(jphotoId);

	jvm->DetachCurrentThread();
}

#if 0
// ֹͣ��ʾͼƬ��ֹͣ�õ�Ƭ��ʾ
void AirplayJniListener::didStopPhotoOrSlideshow()
{
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGE("++++++++++++ didStopPhotoOrSlideshow ");
	JNIEnv *env;
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didPutPhoto <GetObjectClass> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didStopPhotoOrSlideshow", "()V");


	env->CallVoidMethod(obj, mid);

	jvm->DetachCurrentThread();
}
#endif

/**
 * ֹͣ�طŻ�ֹͣ��ʾ��Ƭ
 *  @clientIp           [in] �ͻ���ip
 */
void AirplayJniListener::didStop(const std::string &clientIp)
{
	jclass envcls;
	jmethodID mid;
	JNIEnv *env;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didPutPhoto <GetObjectClass> .\n");
		jvm->DetachCurrentThread();
		return;
	}

	if(p_JNIGlobalParam->isMediaPhoto()) {
		mid = env->GetMethodID(envcls, "didStopPhotoOrSlideshow", "()V");
	}else if(p_JNIGlobalParam->isMediaVideo()) {
		mid = env->GetMethodID(envcls, "didStopPlayback", "()V");
	}else{
		LOGE("CJNIDEBUG: ERR-> didStop <Type> .\n");
		jvm->DetachCurrentThread();
		return;
	}

	env->CallVoidMethod(obj, mid);

	jvm->DetachCurrentThread();
}


// ��ʼ������Ƶ
// startPosition:   ��startPostion��ָ��ʱ��㿪ʼ����
// contentLocation: ��Ƶλ��
//     1: http://192.168.0.116:7001/1/1a59ea26-1861-5206-aa61-ce3960ae6cb1.mp4
//     2: http://v.youku.com/player/getRealM3U8/vid/XNDA3MjEyMTMy/type/flv/ts/1340956945/video.m3u8
void AirplayJniListener::didStartPlayVideo(const std::string &contentLocation, double startPosition,  const std::string &clientIp) 
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	LOGI("didStartPlayVideo");
	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	p_JNIGlobalParam->tryStopClient(clientIp);
	p_JNIGlobalParam->setMediaVideo();

	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> JniVirDoudou <init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didStartPlayVideo", "(Ljava/lang/String;ILjava/lang/String;)V");

	LOGI("Video URL:%s", contentLocation.c_str());

	jstring jcontentLocation = env->NewStringUTF(contentLocation.c_str());
	if(jcontentLocation == NULL) {
		jvm->DetachCurrentThread();
		return;
	}

	jstring jclientIp = env->NewStringUTF(clientIp.c_str());
	if(jclientIp == NULL) {
		env->DeleteLocalRef(jcontentLocation);
		jvm->DetachCurrentThread();
		return;
	}


	jint pos = startPosition * 1000;
	env->CallVoidMethod(obj, mid, jcontentLocation, pos, jclientIp);

	env->DeleteLocalRef(jcontentLocation);
	env->DeleteLocalRef(jclientIp);

	jvm->DetachCurrentThread();
}

/**
 * ��ʼ������Ƶ
 *  @contentLocation    [in] ��Ƶλ�ã��磺http://10.0.1.2:7002/123.mp3
 *  @startPosition      [in] ��startPostion��ָ��ʱ��㿪ʼ���ţ���λΪ�ٷֱ�: ��ǰλ��/�ܳ��ȣ�
 *  @clientIp           [in] �ͻ���ip
 */
void AirplayJniListener::didStartPlayMusic(const std::string &contentLocation, double startPosition, const std::string &clientIp)
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	LOGI("didStartPlayMusic");
	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	p_JNIGlobalParam->tryStopClient(clientIp);
	p_JNIGlobalParam->setMediaVideo();

	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> JniVirDoudou <init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didStartPlayMusic", "(Ljava/lang/String;ILjava/lang/String;)V");

	LOGI("Music URL:%s", contentLocation.c_str());

	jstring jcontentLocation = env->NewStringUTF(contentLocation.c_str());
	if(jcontentLocation == NULL) {
		jvm->DetachCurrentThread();
		return;
	}

	jstring jclientIp = env->NewStringUTF(clientIp.c_str());
	if(jclientIp == NULL) {
		env->DeleteLocalRef(jcontentLocation);
		jvm->DetachCurrentThread();
		return;
	}


	jint pos = startPosition * 1000;
	env->CallVoidMethod(obj, mid, jcontentLocation, pos, jclientIp);

	env->DeleteLocalRef(jcontentLocation);
	env->DeleteLocalRef(jclientIp);

	jvm->DetachCurrentThread();
}


// ������Ƶ�ط��ٶ�
// rate: 
//     0 - ��ͣ
//     1 - �������ʲ���
void AirplayJniListener::didSetPlaybackRate(float rate, const std::string &clientIp) 
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didSetPlaybackRate rate=%f", rate);
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> JniVirDoudou <init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didSetPlaybackRate", "(J)V");


	env->CallVoidMethod(obj, mid, (jlong)rate);

	jvm->DetachCurrentThread();
}

#if 0
// ֹͣ�ط�
void AirplayJniListener::didStopPlayback()
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("++++++++++++ didStopPlayback");
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> JniVirDoudou <init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didStopPlayback", "()V");

	env->CallVoidMethod(obj, mid);

	jvm->DetachCurrentThread();
}
#endif

// ���õ�ǰ���Ž���
void AirplayJniListener::setCurrentPlaybackProgress(float playPosition, const std::string &clientIp) 
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("setCurrentPlaybackProgress playPosition=%f", playPosition);
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> JniVirDoudou <init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "setCurrentPlaybackProgress", "(J)V");


	env->CallVoidMethod(obj, mid, (jlong)playPosition);

	jvm->DetachCurrentThread();
}



jobject NewPlaybackInfo(JNIEnv *env)
{
	jclass cls = p_JNIGlobalParam->GetPlaybackInfoClass();
	jmethodID mid = env->GetMethodID(cls, "<init>","()V");
	jobject PlaybackInfoObj = env->NewObject(cls, mid); 	 		  //����java����

	return PlaybackInfoObj;
}

bool getPlaybackTime2C(JNIEnv *env, jobject PlaybackTimeObj, PlaybackTime &p_PlaybackTime)
{
	jclass cls = env->GetObjectClass(PlaybackTimeObj);

	jfieldID jplayPosition = env->GetFieldID(cls, "playPosition", "J"); 
	jlong playPosition = env->GetLongField(PlaybackTimeObj, jplayPosition);
	p_PlaybackTime.playPosition = (float)playPosition;

	jfieldID jduration = env->GetFieldID(cls, "duration", "J"); 
	jlong duration = env->GetLongField(PlaybackTimeObj, jduration);
	p_PlaybackTime.duration = (float)duration;

	return true;
}

bool getPlaybackInfo2C(JNIEnv *env, jobject PlaybackInfoObj, PlaybackInfo &p_PlaybackInfo)
{
	//jclass cls = p_JNIGlobalParam->GetPlaybackInfoClass();
	jclass cls = env->GetObjectClass(PlaybackInfoObj);


	jfieldID jplayPosition = env->GetFieldID(cls, "playPosition", "I"); 
	jint playPosition = env->GetIntField(PlaybackInfoObj, jplayPosition);
	p_PlaybackInfo.playbackTime.playPosition = (float)playPosition;

	jfieldID jduration = env->GetFieldID(cls, "duration", "I"); 
	jint duration = env->GetIntField(PlaybackInfoObj, jduration);
	p_PlaybackInfo.playbackTime.duration = (float)duration;

	jfieldID jrate = env->GetFieldID(cls, "rate", "I"); 
	jint rate = env->GetIntField(PlaybackInfoObj, jrate);
	p_PlaybackInfo.rate = (float)rate;


	jfieldID jreadyToPlay = env->GetFieldID(cls, "readyToPlay", "Z"); 
	jboolean readyToPlay = env->GetBooleanField(PlaybackInfoObj, jreadyToPlay);
	p_PlaybackInfo.readyToPlay = readyToPlay;

	jfieldID jplaybackBufferEmpty = env->GetFieldID(cls, "playbackBufferEmpty", "Z"); 
	jboolean playbackBufferEmpty = env->GetBooleanField(PlaybackInfoObj, jplaybackBufferEmpty);
	p_PlaybackInfo.playbackBufferEmpty = playbackBufferEmpty;

	jfieldID jplaybackBufferFull = env->GetFieldID(cls, "playbackBufferFull", "Z"); 
	jboolean playbackBufferFull = env->GetBooleanField(PlaybackInfoObj, jplaybackBufferFull);
	p_PlaybackInfo.playbackBufferFull = playbackBufferFull;

	jfieldID jplaybackLikelyToKeepUp = env->GetFieldID(cls, "playbackLikelyToKeepUp", "Z"); 
	jboolean playbackLikelyToKeepUp = env->GetBooleanField(PlaybackInfoObj, jplaybackLikelyToKeepUp);
	p_PlaybackInfo.playbackLikelyToKeepUp = playbackLikelyToKeepUp;


	jfieldID jloaded_startPosition = env->GetFieldID(cls, "loaded_startPosition", "I"); 
	jint loaded_startPosition = env->GetIntField(PlaybackInfoObj, jloaded_startPosition);
	p_PlaybackInfo.loadedTimeRanges.startPosition = (float)loaded_startPosition;

	jfieldID jloaded_duration = env->GetFieldID(cls, "loaded_duration", "I"); 
	jint loaded_duration = env->GetIntField(PlaybackInfoObj, jloaded_duration);
	p_PlaybackInfo.loadedTimeRanges.duration = (float)loaded_duration;

	jfieldID jseek_startPosition = env->GetFieldID(cls, "seek_startPosition", "I"); 
	jint seek_startPosition = env->GetIntField(PlaybackInfoObj, jseek_startPosition);
	p_PlaybackInfo.seekableTimeRanges.startPosition = (float)seek_startPosition;

	jfieldID jseek_duration = env->GetFieldID(cls, "seek_duration", "I"); 
	jint seek_duration = env->GetIntField(PlaybackInfoObj, jseek_duration);
	p_PlaybackInfo.seekableTimeRanges.duration = (float)seek_duration;

	return true;
}

// ��ȡ��ǰ���Ž���
//      position : �Ӵ�ʱ��㿪ʼ����
//      duration : �Ѿ��������ŵ�ʱ�䳤��
void AirplayJniListener::getCurrentPlaybackProgress(PlaybackTime &time, const std::string &clientIp) 
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("getCurrentPlaybackProgress ");
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> JniVirDoudou <init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	jlong position, duration;

	mid = env->GetMethodID(envcls, "getCurrentPlaybackProgress", LINK_PLAYBACKTIME); 
	jobject objPlaybackTime = env->CallObjectMethod(obj, mid);

	getPlaybackTime2C(env, objPlaybackTime, time);

#if 0
	LOGE("getCurrentPlaybackProgress playPosition = %f", time.playPosition);
	LOGE("getCurrentPlaybackProgress duration = %f", time.duration);
#endif

	jvm->DetachCurrentThread();
}


// ��ȡ�ط���Ϣ
//      duration : ��������ʱ��
//      position : ��ǰʱ���
void AirplayJniListener::getPlaybackInfo(PlaybackInfo &info, const std::string &clientIp) 
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("getPlaybackInfo ");
//	LOGE("call: getPlaybackInfo");
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> JniVirDoudou <init> .\n");
		jvm->DetachCurrentThread();
		return;
	}

	mid = env->GetMethodID(envcls, "getPlaybackInfo2", LINK_PLAYBACKINFO); 
	if(mid == NULL) 
	{
		LOGE("CJNIDEBUG: ERR-> GetMethodID <getPlaybackInfo> .\n");
		jvm->DetachCurrentThread();
		return;
	}

	jobject objPlaybackInfo = env->CallObjectMethod(obj, mid);

	getPlaybackInfo2C(env, objPlaybackInfo, info);

	jvm->DetachCurrentThread();
}


#if 0
// �Ѿ������¼��Ự������˿������������¼�֪ͨ��
void AirplayJniListener::didCreateEventSession(int eventSessionId, const std::string &senderIp)
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGE("++++++++++++ didCreateEventSession=%d", eventSessionId);
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didCreateEventSession <init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didCreateEventSession", "(ILjava/lang/String;)V");

	jstring jsenderIp = env->NewStringUTF(senderIp.c_str());
	if(jsenderIp == NULL) {
		LOGE("CJNIDEBUG: ERR-> didCreateEventSession <senderIp> .\n");
		return;
	}


	env->CallVoidMethod(obj, mid, (jint)eventSessionId, jsenderIp);

	jvm->DetachCurrentThread();
}
#endif


// ��������
void AirplayJniListener::didSetVolume(float value, const std::string &clientIp) {
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didSetVolume vlaue=%f", value);
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didSetVolume<init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didSetVolume", "(F)V");

	env->CallVoidMethod(obj, mid, (jfloat)value);

	jvm->DetachCurrentThread();
}



/**
 * ��ʼ�õ�Ƭ����
 *  @slideDuration      [in] ÿ��ͼƬͣ����ʱ��
 *  @clientIp           [in] �ͻ���ip
 */
void AirplayJniListener::didStartSlideshows(int slideDuration, const std::string &clientIp)
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didStartSlideshows slideDuration=%d", slideDuration);
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didStartSlideshows<init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didStartSlideshows", "(ILjava/lang/String;)V");

	jstring jclientIp = env->NewStringUTF(clientIp.c_str());
	if(jclientIp == NULL) {
		LOGE("CJNIDEBUG: ERR-> didStartSlideshows<clientIp> .\n");
		jvm->DetachCurrentThread();
		return;
	}


	env->CallVoidMethod(obj, mid, slideDuration, jclientIp);

	env->DeleteLocalRef(jclientIp);
	jvm->DetachCurrentThread();
}

/**
 * ֹͣ�õ�Ƭ����
 *  @clientIp           [in] �ͻ���ip
 */
void AirplayJniListener::didStopSlideshows(const std::string &clientIp)
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didStopSlideshows");
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didStopSlideshows<init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didStopSlideshows", "(Ljava/lang/String;)V");

	jstring jclientIp = env->NewStringUTF(clientIp.c_str());
	if(jclientIp == NULL) {
		LOGE("CJNIDEBUG: ERR-> didStopSlideshows<clientIp> .\n");
		jvm->DetachCurrentThread();
		return;
	}


	env->CallVoidMethod(obj, mid, jclientIp);

	env->DeleteLocalRef(jclientIp);
	jvm->DetachCurrentThread();
}

/**
 * ��ȡ��һ�Żõ�Ƭ���ŵ�ͼƬ
 *  @index              [in] ��Ƭ��������1�ţ���2�ţ���3��...
 *  @data               [in] ��Ƭԭʼ����(jpg��ʽ��
 *  @length             [in] ���ݴ�С
 *  @clientIp           [in] �ͻ���ip
 *  @note
 *      1. ����AirPlayServer::getSlideshowsPicture(clientIp)�����յ�����Ϣ
 */
void AirplayJniListener::didGetSlideshowsPicture(int index, const char *data, std::size_t length, const std::string &clientIp)
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didGetSlideshowsPicture= %d", index);
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didGetSlideshowsPicture <GetObjectClass> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didGetSlideshowsPicture", "(I[BJ)V");


	jbyte *jpbyte = (jbyte *)data;
	jlong jlength = (jlong)length;
	jbyteArray jbarray = env->NewByteArray(jlength);
	env->SetByteArrayRegion(jbarray, 0, jlength, jpbyte);

	env->CallVoidMethod(obj, mid, (jint)index, jbarray, jlength);

	jvm->DetachCurrentThread();
}

/**
 * ��ȡ�õ�ƬͼƬʧ��
 *  @clientIp           [in] �ͻ���ip
 *  @note
 *      1. ����AirPlayServer::getSlideshowsPicture(clientIp)�����յ�����Ϣ
 *      2. �յ������Ϣ����ʾ�õ�Ƭ����Ƭ�Ѿ��������
 */
void AirplayJniListener::didGetSlideshowsPictureFailed(const std::string &clientIp)
{
	JNIEnv *env;
	jclass envcls;
	jmethodID mid;

	jobject obj = p_JNIGlobalParam->GetJobject();
	JavaVM *jvm = p_JNIGlobalParam->GetGlobalJavaVM();

	LOGI("didGetSlideshowsPictureFailed");
	jvm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		LOGE("callback_handler: failed to attach " "current thread");
		return;
	}
		
	envcls = env->GetObjectClass(obj);
	if(envcls == NULL) {
		LOGE("CJNIDEBUG: ERR-> didGetSlideshowsPictureFailed<init> .\n");
		jvm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(envcls, "didGetSlideshowsPictureFailed", "()V");

	env->CallVoidMethod(obj, mid);

	jvm->DetachCurrentThread();
}

