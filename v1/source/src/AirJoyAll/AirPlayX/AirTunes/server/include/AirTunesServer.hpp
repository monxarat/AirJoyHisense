/*
* AirTunesServer.hpp
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

#ifndef __AIR_TUNES_SERVER_H__
#define __AIR_TUNES_SERVER_H__

#include <string>
#include "AirJoyApi.hpp"
#include "AirTunesServiceInfo.hpp"

namespace airjoy
{

    class AirTunesServerCore;
    class AirTunesServerListener;
    class AirTunesServiceInfo;
    class DacpClient;

    class AIRJOY_API AirTunesServer
    {
    public:
        typedef enum _DacpCommand
        {
            DACP_CMD_NEXTITEM   = 0x01,     // 下一首歌曲
            DACP_CMD_PREVITEM   = 0x02,     // 上一首歌曲
            DACP_CMD_PAUSE      = 0x03,     // 暂停
            DACP_CMD_PLAYPAUSE  = 0x04,     // 暂停播放之间切换
            DACP_CMD_PLAY       = 0x05,     // 播放
            DACP_CMD_STOP       = 0x06,     // 停止
            DACP_CMD_VOLUMEDOWN = 0x07,     // 音量-
            DACP_CMD_VOLUMEUP   = 0x08,     // 音量+
        } DacpCommand;

    public:
        AirTunesServer();
        virtual ~AirTunesServer();

        /**
         * 启动服务端
         *  @serviceInfo    [in] AirTunes服务端信息
         *  @listener       [in] 监听者
         */
        void start(AirTunesServiceInfo &serviceInfo, AirTunesServerListener *listener);

        /**
         * 停止服务端
         */  
        void stop(void);

        /**
         * 读取服务端端口
         *  @return
         *      AirTunes服务端端口号
         */  
        uint16_t port(void);

        /**
         * 设置DACP服务信息
         *  @ip             [in] iPhone/iPad IP地址
         *  @port           [in] iPhone/iPad 上接收音频控制命令的DACP端口
         */
        void setDacpInfo(const std::string &ip, uint16_t port);
        
        /**
         * 清除DACP服务信息
         *  @ip             [in] iPhone/iPad IP地址
         */
        void cleanDacpInfo(const std::string &ip);

        /**
         * 发送音频控制命令
         *  @ip             [in] iPhone/iPad IP地址
         *  @cmd            [in] 音频控制命令
         */
        void sendDacpCommand(const std::string &ip, DacpCommand cmd);

    private:
        AirTunesServerCore      * m_core;
        DacpClient              * m_dacpClient;
        bool                      m_isStart;
    };

}

#endif // __AIR_TUNES_SERVER_H__