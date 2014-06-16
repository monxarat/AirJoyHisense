/*
* AirPlayClientListener.h
*
* coding.tom@gmail.com
* 2011-8-30
*
*/

#ifndef __AIRPLAY_CLIENT_LISTENER_H__
#define __AIRPLAY_CLIENT_LISTENER_H__

#include "AnyPlayTypeDef.hpp"
#include "AnyPlayApi.hpp"
#include "AirPlayServiceInfo.hpp"

namespace anyplay
{

    class ANYPLAY_API AirPlayClientListener
    {
    public:
        AirPlayClientListener() {}
        virtual ~AirPlayClientListener() {}

        /**
         * 连接成功
         */
        virtual void didConnectToServerOk(void) = 0;

        /**
         * 连接失败
         */
        virtual void didConnectToServerFailed(void) = 0;

        /**
         * 断开连接
         */
        virtual void didDisconnected(void) = 0;

        /**
         * 获取服务端信息
         */
        virtual void didGetServerInfo(AirPlayServiceInfo &info) = 0;

        /**
         * 获取服务端幻灯片特性
         * 未实现
         */
        virtual void didGetSlideshowFeatures() {}

        /**
         * 发送反向HTTP请求
         * 未实现
         */
        virtual void didPostReverse() {}

        /**
         * 获取播放进度
         */
        virtual void didGetPlayVideoProgress() = 0;

        /**
         * 获取播放信息
         */
        virtual void didGetPlayVideoInfo(float startPosition, float duration) = 0;

        /**
         * 获取播放属性
         * 未实现
         */
        virtual void didGetProperty() {}
    };

}

#endif // __AIRPLAY_CLIENT_LISTENER_H__