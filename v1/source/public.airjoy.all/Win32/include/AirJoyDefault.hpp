/*
 * coding.tom@gmail.com 
 *
 * AirJoyDefault.hpp
 *
 * 2012-6-16
 */

#ifndef __AIRJOY_DEFAULT_H__
#define __AIRJOY_DEFAULT_H__

#include <string>

namespace airjoy
{

    //----------------------------------------------------------------------------
    //
    // Query-Topic
    //
    //----------------------------------------------------------------------------
    extern const std::string AIRJOY_TOPIC_AIRQUERY;                      // AirQuery
    extern const std::string AIRJOY_TOPIC_AIRFILE;                       // AirFile
    extern const std::string AIRJOY_TOPIC_AIRMEDIA;                      // AirMedia
    extern const std::string AIRJOY_TOPIC_PUBSUB;                        // AirMedia

    //----------------------------------------------------------------------------
    //
    // Action for AirQuery
    //
    //----------------------------------------------------------------------------
    extern const std::string PUBSUB_SUBSCRIBE;                      // subscribe
    extern const std::string PUBSUB_UNSUBSCRIBE;                    // unsubscribe
    extern const std::string PUBSUB_PUBLISH;                        // publish

    //----------------------------------------------------------------------------
    //
    // Action for AirQuery
    //
    //----------------------------------------------------------------------------
    extern const std::string AIRQUERY_GETFEATURES;                  // getfeatures

    //----------------------------------------------------------------------------
    //
    // Action for AirFile
    //
    //----------------------------------------------------------------------------
    extern const std::string AIRFILE_GETFILELIST;                   // getfilelist
    extern const std::string AIRFILE_GETFILECOUNT;                  // getfilecount
    extern const std::string AIRFILE_PUTFILE;                       // putfile

    //----------------------------------------------------------------------------
    //
    // Action for AirMedia
    //
    //----------------------------------------------------------------------------

    // Action for photo
    extern const std::string AIRMEDIA_PHOTO_PUT;
    extern const std::string AIRMEDIA_PHOTO_DISPLAY;
    extern const std::string AIRMEDIA_PHOTO_STOP;
    extern const std::string AIRMEDIA_PHOTO_ROTATE;
    extern const std::string AIRMEDIA_PHOTO_ZOOM;
    extern const std::string AIRMEDIA_PHOTO_MOVE;

    // Action for video
    extern const std::string AIRMEDIA_VIDEO_PUT;
    extern const std::string AIRMEDIA_VIDEO_RATE;
    extern const std::string AIRMEDIA_VIDEO_STOP;
    extern const std::string AIRMEDIA_VIDEO_SETPROGRESS;
    extern const std::string AIRMEDIA_VIDEO_GETPROGRESS;
    extern const std::string AIRMEDIA_VIDEO_GETPLAYBACKINFO;

    // Action for audio
    extern const std::string AIRMEDIA_AUDIO_PUT;
    extern const std::string AIRMEDIA_AUDIO_RATE;
    extern const std::string AIRMEDIA_AUDIO_STOP;
    extern const std::string AIRMEDIA_AUDIO_SETPROGRESS;
    extern const std::string AIRMEDIA_AUDIO_GETPROGRESS;
    extern const std::string AIRMEDIA_AUDIO_GETPLAYBACKINFO;

    // Action for volume
    extern const std::string AIRMEDIA_VOLUME_SET;
    extern const std::string AIRMEDIA_VOLUME_GET;

    //----------------------------------------------------------------------------
    // 
    // AirJoy相关参数
    //
    //----------------------------------------------------------------------------
    extern const int AIRJOY_NSD_PORT;                       // 自动发现端口

    extern const int AIRJOY_SERVICE_NOTIFY_TIMEOUT;         // 服务宣告有效时间
    extern const int AIRJOY_SERVICE_DISCOVERY_TIMEOUT;      // 服务发现有效时间
    extern const int AIRJOY_SERVER_PORT;                    // AirJoy 服务端口
    extern const std::string AIRJOY_SERVICE_TYPE;           // 服务类型
    
    //----------------------------------------------------------------------------
    // 
    // AirPlay相关参数
    //
    //----------------------------------------------------------------------------
    //extern const int AIRPLAY_SERVER_PORT;

}

#endif // __AIRJOY_DEFAULT_H__
