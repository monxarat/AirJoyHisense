
#include "AJServer.h"
#include "AJPublishListener.h"



 /**
 * 发布服务成功
 *  @serviceInfo    [in] 服务信息
 */
void AJPublishListener::onServicePublished(AirJoyServiceInfo & serviceInfo)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getNsdPublishListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
//		p_vm->DetachCurrentThread();
		return;
	}
	
	mid = env->GetMethodID(
			env_class, 
			"onServicePublished", 
			"(Lcom/fqx/airjoy/server/ServerInfo;)V");
    if (mid == NULL)
    {
//		p_vm->DetachCurrentThread();
        return;
    }

	jobject serverinfo_obj = g_AJServer->newServerinfoObj(env, serviceInfo);

	env->CallVoidMethod(obj, mid, serverinfo_obj);

//	p_vm->DetachCurrentThread();
    
    return;
}

/**
 * 发布服务失败
 *  @serviceInfo    [in] 服务信息
 *  @errorCode      [in] 错误代码
 */
void AJPublishListener::onPublishFailed(AirJoyServiceInfo & serviceInfo, int errorCode)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getNsdPublishListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL) {
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
//		p_vm->DetachCurrentThread();
		return;
	}
	
	mid = env->GetMethodID(
			env_class, 
			"onPublishFailed", 
			"(Lcom/fqx/airjoy/server/ServerInfo;I)V");
    if (mid == NULL)
    {
//		p_vm->DetachCurrentThread();
        return;
    }

	jobject serverinfo_obj = g_AJServer->newServerinfoObj(env, serviceInfo);

	env->CallVoidMethod(obj, mid, serverinfo_obj, errorCode);

//	p_vm->DetachCurrentThread();
    
    return;
}

/**
 * 取消发布服务：成功
 *  @serviceInfo    [in] 服务信息
 *  @errorCode      [in] 错误代码
 */
void AJPublishListener::onServiceUnPublished(AirJoyServiceInfo & serviceInfo)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getNsdPublishListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL){
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	
	mid = env->GetMethodID(
			env_class, 
			"onServiceUnPublished", 
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

/**
 * 取消发布服务：失败
 *  @serviceInfo    [in] 服务信息
 *  @errorCode      [in] 错误代码
 */
void AJPublishListener::onUnPulishFailed(AirJoyServiceInfo & serviceInfo, int errorCode)
{
	JNIEnv *env;
	jmethodID mid;
	JavaVM* p_vm = g_AJServer->getVM();
	jobject obj = g_AJServer->getNsdPublishListenerObj();
	if(obj == NULL)
		return;

    p_vm->AttachCurrentThread(&env, NULL);
	if(env == NULL){
		return;
	}

	jclass env_class = env->GetObjectClass(obj);
	if(env_class == NULL) {
		p_vm->DetachCurrentThread();
		return;
	}
	
	mid = env->GetMethodID(
			env_class, 
			"onUnPulishFailed", 
			"(Lcom/fqx/airjoy/server/ServerInfo;I)V");
    if (mid == NULL)
    {
		p_vm->DetachCurrentThread();
        return;
    }

	jobject serverinfo_obj = g_AJServer->newServerinfoObj(env, serviceInfo);

	env->CallVoidMethod(obj, mid, serverinfo_obj, errorCode);

	p_vm->DetachCurrentThread();
    
    return;
}

