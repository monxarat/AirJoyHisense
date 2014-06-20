
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
 * ��ʼ������Ƶ
 */
void APAirTunesListener::didStartPlayAudio(const std::string &clientIp)
{
	doAirTunsFun("didStartPlayAudio", clientIp);
}

/*
 * ��ͣ������Ƶ
 */
void APAirTunesListener::didPause(const std::string &clientIp)
{
	doAirTunsFun("didPause", clientIp);
}

/*
 * ֹͣ������Ƶ
 */
void APAirTunesListener::didStop(const std::string &clientIp)
{
	doAirTunsFun("didStop", clientIp);
}

/*
 * ��������
 *  @value              [in] ����ֵ
 *  @note
 *    ����ֵ��16����0Ϊ���������-144Ϊ������������ɻ�û�����
 *  ---------------------------------------
 *          ����ֵ     ��������
 *  ---------------------------------------
 *           0          [16] �����������
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
 *          -144        ����
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
 * ���÷���ͼƬ
 *  @data               [in] ͼƬ���ݣ�jpeg��ʽ��
 *  @length             [in] ͼƬ���ݳ���
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
 * ������Ƶ��Ϣ
 *  @name               [in] ������
 *  @artist             [in] ����
 *  @album              [in] ר����
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
 * ��ʼ����Ƶ�����Ϣ
 *  @sampleRateInHz         [in] ��Ƶ���ʣ�һ��Ϊ44100
 *  @channelConfig          [in] ��ƵƵ����1=��������2=������
 *  @audioFormat            [in] ��Ƶ��ʽ��16λ��32λ
 *  @fmtp                   [in] fmtpֵ��osxϵͳ��Ҫ�õ�
 *  @fmtpLen                [in] fmtp���ȣ�һ��Ϊ12
 *  @note
 *    Java���루��ʼ��AudioTrack��
	  AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 
										sampleRateInHz,                         // 44100����
										AudioFormat.CHANNEL_OUT_STEREO,         // 2: ������
										AudioFormat.ENCODING_PCM_16BIT,         // 16λ
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
 * �����Ƶ���ݣ�PCM��ʽ
 *  @outbuf         [in] ��Ƶ����
 *  @size           [in] ��Ƶ���ݳ���
 *  @note
 *    Java���루�����Ƶ��
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
