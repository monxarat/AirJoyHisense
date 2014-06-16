/*
 * coding.tom@gmail.com
 * 
 * AirJoyDefault.cpp
 *
 * 2011-5-14
 *
 */

#include "AirJoyDefault.hpp"

namespace airjoy
{

    // Query主题
    const std::string AIRJOY_TOPIC_AIRQUERY             = "http://www.airjoy.cn/airquery";
    const std::string AIRJOY_TOPIC_AIRFILE              = "http://www.airjoy.cn/airfile";
    const std::string AIRJOY_TOPIC_AIRMEDIA             = "http://www.airjoy.cn/airmedia";
    const std::string AIRJOY_TOPIC_PUBSUB               = "http://www.airjoy.cn/pubsub";

    // Action for pubsub
    const std::string PUBSUB_SUBSCRIBE                  = "subscribe";
    const std::string PUBSUB_UNSUBSCRIBE                = "unsubscribe";
    const std::string PUBSUB_PUBLISH                    = "publish";

    // Action for AirQuery
    const std::string AIRQUERY_GETFEATURES              = "getfeatures";

    // Action for AirFile
    const std::string AIRFILE_GETFILELIST               = "getfilelist";
    const std::string AIRFILE_GETFILECOUNT              = "getfilecount";
    const std::string AIRFILE_PUTFILE                   = "getlist";

    // Action for AirMedia

    // Action for photo
    const std::string AIRMEDIA_PHOTO_PUT                = "photo#put";
    const std::string AIRMEDIA_PHOTO_DISPLAY            = "photo#display";
    const std::string AIRMEDIA_PHOTO_STOP               = "photo#stop";
    const std::string AIRMEDIA_PHOTO_ROTATE             = "photo#rotate";
    const std::string AIRMEDIA_PHOTO_ZOOM               = "photo#zoom";
    const std::string AIRMEDIA_PHOTO_MOVE               = "photo#move";

    // Action for video
    const std::string AIRMEDIA_VIDEO_PUT                = "video#put";
    const std::string AIRMEDIA_VIDEO_RATE               = "video#rate";
    const std::string AIRMEDIA_VIDEO_STOP               = "video#stop";
    const std::string AIRMEDIA_VIDEO_SETPROGRESS        = "video#setprogress";
    const std::string AIRMEDIA_VIDEO_GETPROGRESS        = "video#getprogress";
    const std::string AIRMEDIA_VIDEO_GETPLAYBACKINFO    = "video#getplaybackinfo";

    // Action for audio
    const std::string AIRMEDIA_AUDIO_PUT                = "audio#put";
    const std::string AIRMEDIA_AUDIO_RATE               = "audio#rate";
    const std::string AIRMEDIA_AUDIO_STOP               = "audio#stop";
    const std::string AIRMEDIA_AUDIO_SETPROGRESS        = "audio#setprogress";
    const std::string AIRMEDIA_AUDIO_GETPROGRESS        = "audio#getprogress";
    const std::string AIRMEDIA_AUDIO_GETPLAYBACKINFO    = "audio#getplaybackinfo";

    // Action for volume
    const std::string AIRMEDIA_VOLUME_SET               = "volume#set";
    const std::string AIRMEDIA_VOLUME_GET               = "volume#get";

    // 自动发现端口
    const int AIRJOY_NSD_PORT                           = 5959;

    // 服务宣告有效时间
    const int AIRJOY_SERVICE_NOTIFY_TIMEOUT                = 30;

    // 服务发现有效时间
    const int AIRJOY_SERVICE_DISCOVERY_TIMEOUT             = 5;

    // AirJoy 服务端口，由系统分配
    const int AIRJOY_SERVER_PORT                        = 0;

    // AirJoy 服务类型
    const std::string AIRJOY_SERVICE_TYPE               = "airjoy";
}
