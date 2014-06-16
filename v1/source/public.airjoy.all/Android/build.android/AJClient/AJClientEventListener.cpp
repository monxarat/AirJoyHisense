

#include"AJClientEntry.h"

AJClientEventListener::AJClientEventListener()
{
	LOGI("new AJClientEventListener");
}

AJClientEventListener::~AJClientEventListener()
{
	LOGI("del AJClientEventListener");
}

static void evtListenerResult(const char *fun)
{
	JNIEnv *env;
	jmethodID mid;

	LOGI("evtListenerResult fun=%d", fun);

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
 * 超时
 */
void AJClientEventListener::didTimeout(void) 
{
	evtListenerResult("didTimeout");
}


/**
 * 网络错误
 */
void AJClientEventListener::didErrorNet(void) 
{
	evtListenerResult("didErrorNet");
}


/**
 * 订阅事件成功
 */
void AJClientEventListener::didSubscribeEvent(void) 
{
	evtListenerResult("didSubscribeEvent");
}


/**
 * 取消订阅事件成功
 */
void AJClientEventListener::didUnsubscribeEvent(void) 
{
	evtListenerResult("didUnsubscribeEvent");
}


/**
 * 收到事件
 *  @type           [in] 类型：photo/audio/video
 *  @action         [in] 行为：play/pause/stop
 *  @id             [in] id for photo/audio/video
 *  @url            [in, option] url for photo/audio/video
 *  @name           [in, option] name for photo/audio/video
 */
void AJClientEventListener::recvEvent(const std::string &type,
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
	if(jfromIp == NULL) {
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


