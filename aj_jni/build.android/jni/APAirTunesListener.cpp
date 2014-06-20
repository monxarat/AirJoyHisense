
#include "JNIGlobalParam.h"
#include "APAirTunesListener.h"

using namespace airjoy;


static void doAirTunsFun(const char *fun, const std::string &clientIp)
{
	JNIEnv *env;
	jmethodID mid;

	JavaVM *p_vm = p_JNIGlobalParam->GetGlobalJavaVM();
	jobject obj = p_JNIGlobalParam->getAirTunesListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	jstring jclientIp = env->NewStringUTF(clientIp.c_str());
	if(jclientIp == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}

	mid = env->GetMethodID(env_class, fun, "(Ljava/lang/String;)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, jclientIp);

	env->DeleteLocalRef(jclientIp);
	p_vm->DetachCurrentThread();
    
    return;
}

/*
 * 开始播放音频
 */
void APAirTunesListener::didStartPlayAudio(const std::string &clientIp)
{
	doAirTunsFun("didStartPlayAudio", clientIp);
}

/*
 * 暂停播放音频
 */
void APAirTunesListener::didPause(const std::string &clientIp)
{
	doAirTunsFun("didPause", clientIp);
}

/*
 * 停止播放音频
 */
void APAirTunesListener::didStop(const std::string &clientIp)
{
	doAirTunsFun("didStop", clientIp);
}

/*
 * 设置音量
 *  @value              [in] 音量值
 *  @note
 *    音量值分16级，0为最大音量，-144为静音，具体规律还没搞清楚
 *  ---------------------------------------
 *          音量值     音量级别
 *  ---------------------------------------
 *           0          [16] （最大音量）
 *          -1.875      [15]
 *          -3.75       [14]
 *          -5.625      [13]
 *          -7.5        [12]
 *          -9.375      [11]
 *          -11.25      [10]
 *          -13.12      [9]
 *          -15         [8]
 *          -16.87      [7]
 *          -18.75      [6]
 *          -20.62      [5]
 *          -22.5       [4]
 *          -24.37      [3]
 *          -26.25      [2]
 *          -28.12      [1]
 *          -144        静音
 */
void APAirTunesListener::didSetVolume(float value, const std::string &clientIp)
{
	JNIEnv *env;
	jmethodID mid;

	JavaVM *p_vm = p_JNIGlobalParam->GetGlobalJavaVM();
	jobject obj = p_JNIGlobalParam->getAirTunesListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(env_class, "didSetVolume", "(F)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, (jfloat)value);

	p_vm->DetachCurrentThread();

}

/*
 * 设置封面图片
 *  @data               [in] 图片数据（jpeg格式）
 *  @length             [in] 图片数据长度
 */        
void APAirTunesListener::didSetImage(const char *data, std::size_t length, const std::string &clientIp)
{
	JNIEnv *env;
	jmethodID mid;

	JavaVM *p_vm = p_JNIGlobalParam->GetGlobalJavaVM();
	jobject obj = p_JNIGlobalParam->getAirTunesListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(env_class, "didSetImage", "([BJ)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	jbyte *jpbyte = (jbyte *)data;
	jlong jlength = (jlong)length;
	jbyteArray jbarray = env->NewByteArray(jlength);
	env->SetByteArrayRegion(jbarray, 0, jlength, jpbyte);


	env->CallVoidMethod(obj, mid, jbarray, jlength);

	p_vm->DetachCurrentThread();

	return;
}

/*
 * 设置音频信息
 *  @name               [in] 歌曲名
 *  @artist             [in] 作者
 *  @album              [in] 专辑名
 */
void APAirTunesListener::didSetAudioInfo(const std::string &name, const std::string &artist, const std::string &album, const std::string &clientIp)
{
	JNIEnv *env;
	jmethodID mid;

	JavaVM *p_vm = p_JNIGlobalParam->GetGlobalJavaVM();
	jobject obj = p_JNIGlobalParam->getAirTunesListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	jstring jname = env->NewStringUTF(name.c_str());
	jstring jartist = env->NewStringUTF(artist.c_str());
	jstring jalbum = env->NewStringUTF(album.c_str());

	

	mid = env->GetMethodID(env_class, "didSetAudioInfo", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, jname, jartist, jalbum);

	if(jname != NULL) {
		env->DeleteLocalRef(jname);
	}
	if(jartist != NULL) {
		env->DeleteLocalRef(jartist);
	}
	if(jalbum != NULL) {
		env->DeleteLocalRef(jalbum);
	}

	p_vm->DetachCurrentThread();
    
    return;
}

#ifndef __USE_LIB_AO__
/*
 * 初始化音频输出信息
 *  @sampleRateInHz         [in] 音频码率，一般为44100
 *  @channelConfig          [in] 音频频道，1=单声道，2=立体声
 *  @audioFormat            [in] 音频格式：16位，32位
 *  @fmtp                   [in] fmtp值，osx系统需要用到
 *  @fmtpLen                [in] fmtp长度，一般为12
 *  @note
 *    Java代码（初始化AudioTrack）
	  AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 
										sampleRateInHz,                         // 44100赫兹
										AudioFormat.CHANNEL_OUT_STEREO,         // 2: 立体声
										AudioFormat.ENCODING_PCM_16BIT,         // 16位
										sampleRateInHz * 2 * 4,
										AudioTrack.MODE_STREAM);
 */
void APAirTunesListener::didAudioDriverInit(int sampleRateInHz, int channelConfig, int audioFormat, int fmtp[12], size_t fmtpLen)
{
	JNIEnv *env;
	jmethodID mid;

	JavaVM *p_vm = p_JNIGlobalParam->GetGlobalJavaVM();
	jobject obj = p_JNIGlobalParam->getAirTunesListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(env_class, "didAudioDriverInit", "(III)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, sampleRateInHz, channelConfig, audioFormat);

	p_vm->DetachCurrentThread();

}

/*
 * 输出音频数据，PCM格式
 *  @outbuf         [in] 音频数据
 *  @size           [in] 音频数据长度
 *  @note
 *    Java代码（输出音频）
 *    track.write(outbuf, 0, size);
 */
void APAirTunesListener::didAudioDriverPlay(char *outbuf, int size)
{
	JNIEnv *env;
	jmethodID mid;

	JavaVM *p_vm = p_JNIGlobalParam->GetGlobalJavaVM();
	jobject obj = p_JNIGlobalParam->getAirTunesListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	mid = env->GetMethodID(env_class, "didAudioDriverPlay", "([BJ)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	jbyte *jpbyte = (jbyte *)outbuf;
	jlong jlength = (jlong)size;
	jbyteArray jbarray = env->NewByteArray(jlength);
	env->SetByteArrayRegion(jbarray, 0, jlength, jpbyte);


	env->CallVoidMethod(obj, mid, jbarray, jlength);

	p_vm->DetachCurrentThread();

    return;
}
#endif // __USE_LIB_AO__
