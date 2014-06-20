
#include "JNIGlobalParam.h"


JNIGlobalParam *p_JNIGlobalParam;

JNIGlobalParam::JNIGlobalParam(JNIEnv* jni_env, jobject jni_obj)
{
	jni_env->GetJavaVM(&m_jvm); 
	m_obj = jni_env->NewGlobalRef(jni_obj);
	m_PlaybackInfoCls = jni_env->FindClass(CLASS_PLAYBACKINFO);

	mClientIp = "";
	mPlayingMediaType = AirPlayServer::EventUnknown;
	mAirPlayServer = NULL;
	m_airtunes_listener_obj = NULL;
	m_result_listener_obj = NULL;

//	p_JNIGlobalParam = this;
}

JNIGlobalParam::~JNIGlobalParam()
{
	m_env->DeleteGlobalRef(m_obj);
}

JavaVM * JNIGlobalParam::GetGlobalJavaVM()
{
	return m_jvm;
}

jobject JNIGlobalParam::GetJobject()
{
	return m_obj;
}

jclass JNIGlobalParam::GetPlaybackInfoClass()
{
	return m_PlaybackInfoCls;
}


void JNIGlobalParam::setAirTunesListenerObj(jobject listener)
{
	m_airtunes_listener_obj = listener;
}
jobject JNIGlobalParam::getAirTunesListenerObj()
{
	return m_airtunes_listener_obj;
}


void JNIGlobalParam::setResultListenerObj(jobject listener)
{
	m_result_listener_obj = listener;
}

jobject JNIGlobalParam::getResultListenerObj()
{
	return m_result_listener_obj;
}

void JNIGlobalParam::setMediaVideo()
{
	mPlayingMediaType = AirPlayServer::EventVideo;
}

void JNIGlobalParam::setMediaPhoto()
{
	mPlayingMediaType = AirPlayServer::EventPhoto;
}

bool JNIGlobalParam::isMediaPhoto()
{
	if(mPlayingMediaType == AirPlayServer::EventPhoto)
		return true;
	else
		return false;
}

bool JNIGlobalParam::isMediaVideo()
{
	if(mPlayingMediaType == AirPlayServer::EventVideo)
		return true;
	else
		return false;
}

bool JNIGlobalParam::isMyDevIP(const std::string &clientIp)
{
	if(mClientIp == clientIp)
		return true;
	else
		return false;
}

void JNIGlobalParam::setClientIP(const std::string &clientIp)
{
	mClientIp = clientIp;
}

void JNIGlobalParam::setAirPlayServer(AirPlayServer  *ap_server)
{
	mAirPlayServer = ap_server;
}

void JNIGlobalParam::publishEvt(int type) 
{
#if 0
	if(mAirPlayServer == NULL) 
		return;
	mAirPlayServer->publishEvent(mPlayingMediaType, (AirPlayServer::EventAction)type, mClientIp);
	LOGI("publishEvent type=%d evt=%d", mPlayingMediaType, type);
#endif
}

void JNIGlobalParam::tryStopClient(const std::string &clientIp)
{
#if 0
	if(mClientIp == "") 
	{
		mClientIp = clientIp;
		return;
	}

	if(mAirPlayServer == NULL) 
		return;

	if(mClientIp != clientIp) 
	{
		LOGE("tryStopClient start %d", mPlayingMediaType);
		mAirPlayServer->publishEvent(mPlayingMediaType, AirPlayServer::EventActionStopped, mClientIp);
		mClientIp = clientIp;
		LOGE("tryStopClient ok");
	}
#endif
}


