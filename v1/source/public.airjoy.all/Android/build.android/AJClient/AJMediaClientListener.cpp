
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
 * 连接成功
 */
void AJMediaClientListener::didSubscribeEvent(const std::string &serverIp)
{
	LOGI("didSubscribeEvent ip=%s", serverIp.c_str());
	connectToServerResult("didSubscribeEvent");
}


/**
 * 连接失败
 */
void AJMediaClientListener::didUnsubscribeEvent(const std::string &serverIp)
{
	LOGI("didUnsubscribeEvent ip=%s", serverIp.c_str());
	connectToServerResult("didUnsubscribeEvent");
}



/**
 * 超时
 */
void AJMediaClientListener::didTimeout(const AirJoySessionId sessionId)
{
	connectToServerResult("didTimeout");
}


/**
 * 网络错误
 */
void AJMediaClientListener::didErrorNet(const AirJoySessionId sessionId)
{
	connectToServerResult("didErrorNet");
}

/**
 * 收到事件
 *  @type           [in] 类型：photo/audio/video
 *  @action         [in] 行为：play/pause/stop
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
 * 初始化会话成功
 */
void AJMediaClientListener::didInitSessionOk(void)
{
	sessionResult("didInitSessionOk");
}


/**
 * 关闭会话成功
 */
void AJMediaClientListener::didCloseSessionOk(void)
{
	sessionResult("didCloseSessionOk");
}
#endif


/**
 * 处理结果
 *  @sessionId     [in] 会话ID
 *  @code          [in] 返回代码
 *      ResultCodeOk：表示接收方已经收到，并且处理成功。
 *      ResultCodeErrorNotSupport: 对方不支持此请求。
 *      ResultCodeErrorTimeout: 超时。
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
 * 处理结果
 *  @sessionId      [in] 会话ID
 *  @appAction      [in] 行为
 *  @appResult      [in] 数据
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
 * 处理获取视频播放进度结果
 *  @sessionId      [in] 会话ID
 *  @code           [in] 返回代码，如果是ResultCodeOk：表示rate和position为有效值。
 *  @rate           [in] 0: 暂停，1: 正常速率播放
 *  @position       [in] 当前播放视频的时间点，单位为秒
 *  @return 会话ID
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
 * 处理获取视频播放信息结果
 *  @sessionId      [in] 会话ID
 *  @code           [in] 返回代码，如果是ResultCodeOk：表示rate和position为有效值。
 *  @info           [in] 回放信息，见 PlayInfo 定义
 *  @return 会话ID
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
 * 处理获取音频播放进度结果
 *  @sessionId      [in] 会话ID
 *  @code           [in] 返回代码，如果是ResultCodeOk：表示rate和position为有效值。
 *  @rate           [in] 0: 暂停，1: 正常速率播放
 *  @position       [in] 当前播放视频的时间点，单位为秒
 *  @return 会话ID
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
 * 处理获取音频播放信息结果
 *  @sessionId      [in] 会话ID
 *  @code           [in] 返回代码，如果是ResultCodeOk：表示info为有效值。
 *  @info           [in] 回放信息，见 PlayInfo 定义
 *  @return 会话ID
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
 * 处理获取音量结果
 *  @sessionId      [in] 会话ID
 *  @code           [in] 返回代码，如果是ResultCodeOk：表示volume为有效值。
 *  @volume         [in] 音量值（0 -> 100）
 *  @return 会话ID
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
