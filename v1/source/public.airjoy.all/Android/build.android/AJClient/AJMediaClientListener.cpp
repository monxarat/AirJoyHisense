
#include"AJClientEntry.h"

static void connectToServerResult(const char *fun)
{
	JNIEnv *env;
	jmethodID mid;

	LOGI("connectToServerResult fun=%d", fun);

	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getEventListenerObj();
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
	
	mid = env->GetMethodID(env_class, fun, "()V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid);

	p_vm->DetachCurrentThread();
    
    return;
}

/**
 * ���ӳɹ�
 */
void AJMediaClientListener::didSubscribeEvent(const std::string &serverIp)
{
	LOGI("didSubscribeEvent ip=%s", serverIp.c_str());
	connectToServerResult("didSubscribeEvent");
}


/**
 * ����ʧ��
 */
void AJMediaClientListener::didUnsubscribeEvent(const std::string &serverIp)
{
	LOGI("didUnsubscribeEvent ip=%s", serverIp.c_str());
	connectToServerResult("didUnsubscribeEvent");
}



/**
 * ��ʱ
 */
void AJMediaClientListener::didTimeout(const AirJoySessionId sessionId)
{
	connectToServerResult("didTimeout");
}


/**
 * �������
 */
void AJMediaClientListener::didErrorNet(const AirJoySessionId sessionId)
{
	connectToServerResult("didErrorNet");
}

/**
 * �յ��¼�
 *  @type           [in] ���ͣ�photo/audio/video
 *  @action         [in] ��Ϊ��play/pause/stop
 *  @id             [in] id for photo/audio/video
 *  @url            [in, option] url for photo/audio/video
 *  @name           [in, option] name for photo/audio/video
 */
void AJMediaClientListener::recvEvent(const std::string &type,
					   const std::string &action,
					   const std::string &id,
					   const std::string &url,
					   const std::string &name,
					   const std::string &fromIp) 
{
	JNIEnv *env;
	jmethodID mid;

	LOGI("AJClientEventListener fun=recvEvent");

	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getEventListenerObj();
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
	
	mid = env->GetMethodID(env_class, "recvEvent", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V");
    if (mid == NULL) {
		p_vm->DetachCurrentThread();
        return;
    }

	jstring jtype = env->NewStringUTF(type.c_str());
	if(jtype == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}

	jstring jaction = env->NewStringUTF(action.c_str());
	if(jaction== NULL) {
		env->DeleteLocalRef(jtype);
		p_vm->DetachCurrentThread();
		return;
	}

	jstring jid = env->NewStringUTF(id.c_str());
	if(jid == NULL) {
		env->DeleteLocalRef(jtype);
		env->DeleteLocalRef(jaction);
		p_vm->DetachCurrentThread();
		return;
	}

	jstring jurl = env->NewStringUTF(url.c_str());
	if(jurl == NULL) {
		env->DeleteLocalRef(jtype);
		env->DeleteLocalRef(jaction);
		env->DeleteLocalRef(jid);
		p_vm->DetachCurrentThread();
		return;
	}

	jstring jname = env->NewStringUTF(name.c_str());
	if(jname == NULL) {
		env->DeleteLocalRef(jtype);
		env->DeleteLocalRef(jaction);
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jurl);
		p_vm->DetachCurrentThread();
		return;
	}

	jstring jfromIp = env->NewStringUTF(fromIp.c_str());
	if(jname == NULL) {
		env->DeleteLocalRef(jtype);
		env->DeleteLocalRef(jaction);
		env->DeleteLocalRef(jid);
		env->DeleteLocalRef(jurl);
		env->DeleteLocalRef(jname);
		p_vm->DetachCurrentThread();
		return;
	}

	env->CallVoidMethod(obj, mid, jtype, jaction, jid, jurl, jname, jfromIp);

	env->DeleteLocalRef(jtype);
	env->DeleteLocalRef(jaction);
	env->DeleteLocalRef(jid);
	env->DeleteLocalRef(jurl);
	env->DeleteLocalRef(jname);
	env->DeleteLocalRef(jfromIp);

	p_vm->DetachCurrentThread();
    
    return;
}




#if 0
static void sessionResult(const char *fun)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getMediaResultListenerObj();
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
	
	mid = env->GetMethodID(env_class, fun, "()V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid);

	p_vm->DetachCurrentThread();
    
    return;
}


/**
 * ��ʼ���Ự�ɹ�
 */
void AJMediaClientListener::didInitSessionOk(void)
{
	sessionResult("didInitSessionOk");
}


/**
 * �رջỰ�ɹ�
 */
void AJMediaClientListener::didCloseSessionOk(void)
{
	sessionResult("didCloseSessionOk");
}
#endif


/**
 * ������
 *  @sessionId     [in] �ỰID
 *  @code          [in] ���ش���
 *      ResultCodeOk����ʾ���շ��Ѿ��յ������Ҵ���ɹ���
 *      ResultCodeErrorNotSupport: �Է���֧�ִ�����
 *      ResultCodeErrorTimeout: ��ʱ��
 */
void AJMediaClientListener::handleResult(const AirJoySessionId sessionId, ResultCode code)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getMediaResultListenerObj();
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
	
	mid = env->GetMethodID(env_class, "handleResult", "(II)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, (int)sessionId, (int)code);

	p_vm->DetachCurrentThread();
    
    return;
}


/**
 * ������
 *  @sessionId      [in] �ỰID
 *  @appAction      [in] ��Ϊ
 *  @appResult      [in] ����
 */
void AJMediaClientListener::handleResult(const AirJoySessionId sessionId, const std::string &appAction, const std::string &appResult)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getMediaResultListenerObj();
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
	
	mid = env->GetMethodID(env_class, "handleResult", "(ILjava/lang/String;Ljava/lang/String;)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	jstring jappAction = env->NewStringUTF(appAction.c_str());
	if(jappAction == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}

	jstring jappResult = env->NewStringUTF(appResult.c_str());
	if(jappResult == NULL) {
		env->DeleteLocalRef(jappAction);
		p_vm->DetachCurrentThread();
		return;
	}


	env->CallVoidMethod(obj, mid, (int)sessionId, jappAction, jappResult);

	env->DeleteLocalRef(jappAction);
	env->DeleteLocalRef(jappResult);

	p_vm->DetachCurrentThread();
    
    return;
}


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

#define  LINK_PLAYBACKINFO 	"(II)Lcom/fqx/airjoy/clientlistener/AJPlayBackInfo;"


/**
 * �����ȡ��Ƶ���Ž��Ƚ��
 *  @sessionId      [in] �ỰID
 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
 *  @rate           [in] 0: ��ͣ��1: �������ʲ���
 *  @position       [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
 *  @return �ỰID
 */
void AJMediaClientListener::handleGetPlayVideoProgressResult(const AirJoySessionId sessionId, ResultCode code, float rate, float position)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getMediaResultListenerObj();
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
	
	mid = env->GetMethodID(env_class, "handleGetPlayVideoProgressResult", "(IIFF)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, (int)sessionId, (int)code, rate, position);

	p_vm->DetachCurrentThread();
    
    return;
}


/**
 * �����ȡ��Ƶ������Ϣ���
 *  @sessionId      [in] �ỰID
 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
 *  @info           [in] �ط���Ϣ���� PlayInfo ����
 *  @return �ỰID
 */
void AJMediaClientListener::handleGetPlayVideoInfoResult(const AirJoySessionId sessionId, ResultCode code, AirJoyPlayBackInfo &info)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getMediaResultListenerObj();
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
	
#if 0
	mid = env->GetMethodID(env_class, "handleGetPlayVideoInfoResult", LINK_PLAYBACKINFO);
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	jobject playback_obj = g_AJClientEntry->newPlaybackObj(env, info);

	env->CallVoidMethod(obj, mid, (int)sessionId, (int)code, playback_obj);

#else
	mid = env->GetMethodID(env_class, "handleGetVideoInfoResult", "(IFF)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, (int)code, info.duration, info.position);
	LOGI("handleGetVideoInfoResult dur=%f, pos=%f", info.duration, info.position);

#endif
	p_vm->DetachCurrentThread();
    
    return;

}


/**
 * �����ȡ��Ƶ���Ž��Ƚ��
 *  @sessionId      [in] �ỰID
 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
 *  @rate           [in] 0: ��ͣ��1: �������ʲ���
 *  @position       [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
 *  @return �ỰID
 */
void AJMediaClientListener::handleGetPlayAudioProgressResult(const AirJoySessionId sessionId, ResultCode code, float rate, float position)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getMediaResultListenerObj();
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
	
	mid = env->GetMethodID(env_class, "handleGetPlayAudioProgressResult", "(IIFF)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, (int)sessionId, (int)code, rate, position);

	p_vm->DetachCurrentThread();
    
    return;
}


/**
 * �����ȡ��Ƶ������Ϣ���
 *  @sessionId      [in] �ỰID
 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾinfoΪ��Чֵ��
 *  @info           [in] �ط���Ϣ���� PlayInfo ����
 *  @return �ỰID
 */
void AJMediaClientListener::handleGetPlayAudioInfoResult(const AirJoySessionId sessionId, ResultCode code, AirJoyPlayBackInfo &info)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getMediaResultListenerObj();
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
	
#if 0
	mid = env->GetMethodID(env_class, "handleGetPlayAudioInfoResult", LINK_PLAYBACKINFO);
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	jobject playback_obj = g_AJClientEntry->newPlaybackObj(env, info);

	env->CallVoidMethod(obj, mid, (int)sessionId, (int)code, playback_obj);
#endif

	p_vm->DetachCurrentThread();
    
    return;

}


/**
 * �����ȡ�������
 *  @sessionId      [in] �ỰID
 *  @code           [in] ���ش��룬�����ResultCodeOk����ʾvolumeΪ��Чֵ��
 *  @volume         [in] ����ֵ��0 -> 100��
 *  @return �ỰID
 */
void AJMediaClientListener::handleGetVolumeResult(const AirJoySessionId sessionId, ResultCode code, int volume)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getMediaResultListenerObj();
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
	
	mid = env->GetMethodID(env_class, "handleGetVolumeResult", "(III)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	env->CallVoidMethod(obj, mid, (int)sessionId, (int)code, volume);

	p_vm->DetachCurrentThread();
    
    return;
}
