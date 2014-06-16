/*
* AirJoyServer.hpp
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

#ifndef __AIRJOY_SERVER_H__
#define __AIRJOY_SERVER_H__

#include "AirJoyApi.hpp"
#include "AirJoyServiceInfo.hpp"
#include "AirFileListener.hpp"
#include "AirMediaListener.hpp"

namespace airjoy
{

    class AirJoyServerCore;

    class AIRJOY_API AirJoyServer
    {
    public:
        AirJoyServer();
        virtual ~AirJoyServer();
        
        /**
         * 启动服务端
         *  @info           [in] AirJoy服务端信息
         */  
        bool start(AirJoyServiceInfo &info);
        
        /**
         * 停止服务端
         */ 
        bool stop(void);

        /**
         * 读取服务端端口
         *  @return
         *      AirJoy服务端端口号
         */
        uint16_t port(void);

        /**
         * 加载模块：AirFile
         *  @listener       [in] 监听者
         */ 
        bool loadAirFile(AirFileListener *listener);
        
        /**
         * 卸载模块：AirFile
         */ 
        bool unloadAirFile(void);

        /**
         * 加载模块：AirMedia
         *  @listener       [in] 监听者
         */ 
        bool loadAirMedia(AirMediaListener *listener);
        
        /**
         * 卸载模块：AirMedia
         */ 
        bool unloadAirMedia(void);

        /**
         * 发布媒体事件
         *  @mediaType  [in] 事件类型，必须为: [photo, audio, video, init]
         *  @action     [in] 事件动作, 必须为：[play, pause, stop, init]
         *  @id         [in, option] 图片ID、音频流ID、视频流ID
         *  @url        [in, option] 图片地址、音频地址、视频地址
         *  @name       [in, option] 图片名称、音频名称、视频名称
         *  @fromIp     [in] 媒体来源地址
         */
        bool publishMediaEvent(const std::string &mediaType, 
                               const std::string &action, 
                               const std::string &id,
                               const std::string &url,
                               const std::string &name,
                               const std::string &fromIp);

    private:
        AirJoyServerCore    * m_core;
    };

}


#endif // __AIRJOY_SERVER_H__