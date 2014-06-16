/*
* AirPlayClient.hpp
*
*
* coding.tom@gmail.com
*
* 2012-5-23
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __AIR_PLAY_CLIENT_H__
#define __AIR_PLAY_CLIENT_H__

#include <string>
#include "AnyPlayTypeDef.hpp"
#include "AnyPlayApi.hpp"

#include "AirPlayClientPhoto.hpp"
#include "AirPlayClientVideo.hpp"

namespace anyplay
{

    class AirPlayClientListener;
    class AnyPlayCore;

    class ANYPLAY_API AirPlayClient
    {
    public:
        typedef enum _AirPlayClientRequestType
        {
            RequestTypeUnknown                 = 0,
            RequestTypeGetServerInfo           = 1,
            RequestTypePostReverse             = 2,
            RequestTypeGetSlideshowFeatures    = 3,
            RequestTypePutPhoto                = 5,
            RequestTypePutSlideshows           = 6,
            RequestTypePostStop                = 7,
            RequestTypePostPlay                = 8,
            RequestTypePostScrub               = 9,
            RequestTypePostRate                = 10,
            RequestTypeGetScrub                = 11,
            RequestTypeGetPlaybackInfo         = 12,
            RequestTypePutSetProperty          = 13,
            RequestTypePutGetProperty          = 14,
        } AirPlayClientRequestType;

    private:
        AnyPlayCore                 * m_core;
        AirPlayClientPhoto          * m_photo;
        AirPlayClientVideo          * m_video;
        AirPlayClientRequestType      m_requestType;
        bool                          m_isStart;
        bool                          m_isConnected;

    public:
        AirPlayClientRequestType requestType(void);
        void setRequestType(AirPlayClientRequestType t);
        void resetRequestType(void);

    public:
        AirPlayClient(AnyPlayCore * core);
        virtual ~AirPlayClient();

        /**
         * 启动AirPlay客户端
         *  @listener       [in] 监听者
         */
        void start(AirPlayClientListener *listener);

        /**
         * 停止客户端
         */
        void stop(void);

        /**
         * 连接到AirPlayServer
         *  @ip             [in] AirPlay服务端地址
         *  @port           [in] AirPlay服务端口
         */
        void connectToServer(const std::string &ip, uint16_t port);

        /**
         * 断开连接
         */
        void disconnect(void);

        /**
         * 发送消息到已经连接的服务端
         *  @message    [in] 准备发送的消息
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool send(const std::string &message);

        /**
         * 获取服务端信息
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool getServerInfo(void);

        /**
         * 获取服务端幻灯片特性
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool getSlideshowFeatures(void);

        /**
         * 发送反向HTTP请求
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool postReverse(void);

        /**
         * 照片功能模块
         */
        AirPlayClientPhoto * photo(void);

        /**
         * 视频功能模块
         */
        AirPlayClientVideo * video(void);
    };

}

#endif // __AIR_PLAY_CLIENT_H__