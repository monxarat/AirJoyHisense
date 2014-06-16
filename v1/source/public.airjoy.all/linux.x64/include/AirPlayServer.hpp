/*
* AirPlayServer.hpp
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

#ifndef __AIRPLAY_SERVER_H__
#define __AIRPLAY_SERVER_H__

#include <string>
#include "AirJoyApi.hpp"
#include "AirPlayServiceInfo.hpp"

namespace airjoy
{

    class AirPlayServerCore;
    class AirPlayServiceInfo;
    class AirPlayServerListener;

    class AIRJOY_API AirPlayServer
    {
    public:

        // 事件分类
        typedef enum _EventCategory
        {
            EventUnknown     = -1,
            EventPhoto       = 0,    // 图片
            EventVideo       = 1,    // 视频
        } EventCategory;

        // 事件行为
        typedef enum _EventAction
        {
            EventActionNone      = -1,
            EventActionPlaying   = 0,    // 播放
            EventActionPaused    = 1,    // 暂停
            EventActionLoading   = 2,    // 正在加载
            EventActionStopped   = 3,    // 停止播放/停止显示
        } EventAction;

    public:
        AirPlayServer();
        virtual ~AirPlayServer();

        /**
         * 启动服务端
         *  @serviceInfo    [in] AirPlay服务端信息
         *  @listener       [in] 监听者
         */  
        void start(AirPlayServiceInfo &serviceInfo, AirPlayServerListener *listener);

        /**
         * 停止服务端
         */
        void stop(void);

        /**
         * 读取服务端端口
         *  @return
         *      AirPlay服务端端口号
         */
        uint16_t port(void);

        /**
         * 发布媒体事件
         *  @type       [in] 类型
         *  @action     [in] 动作
         *  @clientIp   [in] 客户端IP
         */
        void publishEvent(EventCategory type, EventAction action, const std::string &clientIp);

        /**
         * 关闭与客户端的连接
         *  @clientIp       [in] 客户端ip
         */ 
        void closeConnection(const std::string &clientIp);
        
        /**
         * 向客户端发送请求，获取一张幻灯片用的图片
         *  @clientIp       [in] 客户端ip
         */
        void getSlideshowsPicture(const std::string &clientIp);

    private:
        AirPlayServerCore         * m_core;
        bool                        m_isStart;
    };

}

#endif // __AIRPLAY_SERVER_H__

