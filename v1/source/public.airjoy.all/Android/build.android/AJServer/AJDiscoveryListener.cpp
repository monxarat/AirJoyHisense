

#include "AJServer.h"
#include "AJDiscoveryListener.h"

#if 0
void discoveryBodyString(const std::string fun, const std::string serviceType)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getNsdDiscoveryListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
//		p_vm->DetachCurrentThread();
		return;
	}
	
	mid = env->GetMethodID(env_class, fun.c_str(), "(Ljava/lang/String;)V");
    if (mid == NULL)
    {
//		p_vm->DetachCurrentThread();
        return;
    }

	jstring jtype = env->NewStringUTF(serviceType.c_str());
	if(jtype == NULL) {
//		p_vm->DetachCurrentThread();
		return;
	}

	env->CallVoidMethod(obj, mid, jtype);
	env->DeleteLocalRef(jtype);

//	p_vm->DetachCurrentThread();
    
    return;
}

void discoveryBodyStringInt(const std::string fun, const std::string serviceType, int errorCode)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getNsdDiscoveryListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL)
	{
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	
	mid = env->GetMethodID(env_class, fun.c_str(), "(Ljava/lang/String;I)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	jstring jtype = env->NewStringUTF(serviceType.c_str());
	if(jtype == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}

	env->CallVoidMethod(obj, mid, jtype, errorCode);
	env->DeleteLocalRef(jtype);

	p_vm->DetachCurrentThread();
    
    return;
}

 /**
 * 启动监听：成功
 *  @serviceType    [in] 服务类型
 */
void AJDiscoveryListener::onDiscoveryStarted(const std::string serviceType)
{
	discoveryBodyString("onDiscoveryStarted", serviceType);
}


/**
 * 启动监听：失败
 *  @serviceType    [in] 服务类型
 *  @errorCode      [in] 错误代码
 */
void AJDiscoveryListener::onStartDiscoveryFailed(const std::string serviceType, int errorCode)
{
	discoveryBodyStringInt("onStartDiscoveryFailed", serviceType, errorCode);
}


/**
 * 停止监听：成功
 *  @serviceType    [in] 服务类型
 */
void AJDiscoveryListener::onDiscoveryStopped(const std::string serviceType)
{
	discoveryBodyString("onDiscoveryStopped", serviceType);
}


/**
 * 停止监听：失败
 *  @serviceType    [in] 服务类型
 *  @errorCode      [in] 错误代码
 */
void AJDiscoveryListener::onStopDiscoveryFailed(const std::string serviceType, int errorCode)
{
	discoveryBodyStringInt("onStopDiscoveryFailed", serviceType, errorCode);
}


void onServerLostOrFound(const std::string fun, AirJoyServiceInfo &serviceInfo)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getNsdDiscoveryListenerObj();
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
	
	mid = env->GetMethodID(
			env_class, 
			fun.c_str(),
			"(Lcom/fqx/airjoy/server/ServerInfo;)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	jobject serverinfo_obj = g_AJServer->newServerinfoObj(env, serviceInfo);

	env->CallVoidMethod(obj, mid, serverinfo_obj);

	p_vm->DetachCurrentThread();
    
    return;
}

#endif

/**
 * 某个服务已经下线
 *  @serviceInfo    [in] 服务信息
 */
void AJDiscoveryListener::onServiceLost(AirJoyServiceInfo &serviceInfo)
{
	//onServerLostOrFound("onServiceLost", serviceInfo);
}


/**
 * 某个服务已经上线
 *  @serviceInfo    [in] 服务信息
 */
void AJDiscoveryListener::onServiceFound(AirJoyServiceInfo &serviceInfo)
{
	//onServerLostOrFound("onServiceFound", serviceInfo);
}
