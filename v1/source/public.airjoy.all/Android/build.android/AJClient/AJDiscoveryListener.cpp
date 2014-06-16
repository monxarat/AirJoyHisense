

#include "AJClientEntry.h"


#if 0
jboolean onServerLostOrFound(const std::string fun, AirJoyServiceInfo &serviceInfo)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getDiscoveryListenerObj();
	if(obj == NULL) {
		return false;
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return false;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return false;
	}
	
	mid = env->GetMethodID(
			env_class, 
			fun.c_str(),
			"(Lcom/fqx/airjoy/clientlistener/ServerInfo;)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return false;
    }

	jobject serverinfo_obj = g_AJClientEntry->newServerinfoObj(env, serviceInfo);

	env->CallVoidMethod(obj, mid, serverinfo_obj);

	p_vm->DetachCurrentThread();
    
    return true;
}
#else
jboolean onServerLostOrFound(const std::string fun, AirJoyServiceInfo &serviceInfo)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJClientEntry->getVM();
	jobject obj = g_AJClientEntry->getDiscoveryListenerObj();
	if(obj == NULL) {
		return false;
	}

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return false;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return false;
	}
	
	mid = env->GetMethodID(
			env_class, 
			fun.c_str(),
			"(Ljava/lang/String;Ljava/lang/String;II)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
		return false;
    }

	jstring jname = env->NewStringUTF(serviceInfo.deviceName().c_str());
	if(jname == NULL) {
		p_vm->DetachCurrentThread();
		return false;
	}

	jstring jip = env->NewStringUTF(serviceInfo.ip().c_str());
	if(jname == NULL) {
		env->DeleteLocalRef(jname);
		p_vm->DetachCurrentThread();
		return false;
	}


	env->CallVoidMethod(obj, mid, jname, jip, serviceInfo.port(), serviceInfo.deviceType());

	env->DeleteLocalRef(jname);
	env->DeleteLocalRef(jip);
	p_vm->DetachCurrentThread();
    
    return true;
}

#endif


/**
 * 某个服务已经下线
 *  @serviceInfo    [in] 服务信息
 */
void AJDiscoveryListener::onServiceLost(AirJoyServiceInfo &serviceInfo)
{
	bool ret = onServerLostOrFound("onServiceLost", serviceInfo);
	if(!ret) {
		LOGE(" Err: onServiceLost");
	}
}


/**
 * 某个服务已经上线
 *  @serviceInfo    [in] 服务信息
 */
void AJDiscoveryListener::onServiceFound(AirJoyServiceInfo &serviceInfo)
{
	bool ret = onServerLostOrFound("onServiceFound", serviceInfo);
	if(!ret) {
		LOGE(" Err: onServiceFound");
	}
}
