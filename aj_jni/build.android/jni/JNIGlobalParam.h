
#ifndef __JNIGLOBAPARAM_H__
#define __JNIGLOBAPARAM_H__

#include <jni.h>
#include <android/log.h>

#include <stdio.h>
#include <stdlib.h>

#include "AirPlayServer.hpp"
#include "AirPlayServerListener.hpp"

#define  LOG_TAG    "libJNIAnyplay"
#if 0
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define  LOGI(...)  
#endif
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define  CLASS_APCONTROLLER  	"com/fqx/anyplay/service/APController"
#define  CLASS_PLAYBACKINFO 	"com/fqx/anyplay/service/PlaybackInfo"
#define  CLASS_AIRPLAYLISTENER 	"com/fqx/anyplay/service/AirplayListener"

#define  LINK_PLAYBACKINFO 	"()Lcom/fqx/anyplay/service/PlaybackInfo;"
#define  LINK_PLAYBACKTIME 	"()Lcom/fqx/anyplay/service/PlaybackTime;"

using namespace airjoy;


class JNIGlobalParam
{
	public:
		JNIGlobalParam(JNIEnv* jni_env, jobject jni_obj);

		virtual ~JNIGlobalParam();


		JavaVM * GetGlobalJavaVM();
		jobject GetJobject();
		jclass GetPlaybackInfoClass();


		void setMediaVideo();
		void setMediaPhoto();
		void setClientIP(const std::string &clientIp);
		void setAirPlayServer(AirPlayServer *ap_server);
		void tryStopClient(const std::string &clientIp);
		bool isMediaPhoto();
		bool isMediaVideo();
		bool isMyDevIP(const std::string &clientIp);
		void publishEvt(int type);

		void setAirTunesListenerObj(jobject listener);
		jobject getAirTunesListenerObj();

		void setResultListenerObj(jobject listener);
		jobject getResultListenerObj();

	private:
		JavaVM *m_jvm;
		JNIEnv *m_env;
		jobject m_obj;

		jclass 	m_PlaybackInfoCls;
		jobject m_airtunes_listener_obj;
		jobject m_result_listener_obj;


		AirPlayServer   				*mAirPlayServer;
		std::string   					mClientIp;              // 客户端ip
		bool   		  					mIsPlaying;     		// 播放与否
		AirPlayServer::EventCategory    mPlayingMediaType;      // 正在播放的媒体类型
};

extern JNIGlobalParam *p_JNIGlobalParam;


#endif
