/*
*
* testAirPlayServer.cpp
*
*
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "AirTunesServiceInfo.hpp"
#include "AirTunesServer.hpp"
#include "AirTunesServerListener.hpp"

#include "AirPlayNsd.hpp"
#include "AirPlayNsdDiscoveryListener.hpp"
#include "AirPlayNsdPublishListener.hpp"

using namespace airjoy;

//------------------------------------------------------------------
// 0. 实例化AirTunes
//------------------------------------------------------------------
AirTunesServiceInfo				gAirTunesServiceInfo;
AirTunesServer                  gAirTunesServer;
std::string                     gClientIp;

#ifdef __APPLE_BONJOUR__
class BonjourAirTunesPublishListener : public AirPlayNsdPublishListener
{
public:
    BonjourAirTunesPublishListener() {}
    
    virtual ~BonjourAirTunesPublishListener() {}

    virtual void onUnPulishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode)
    {
        std::cout << "BonjourAirTunes onUnPulishFailed" << std::endl;
    }

    virtual void onServiceUnPublished(const std::string & servicetype, const std::string & serviceName)
    {
        std::cout << "BonjourAirTunes onServiceUnPublished" << std::endl;
    }

    virtual void onServicePublished(const std::string & servicetype, const std::string & serviceName)
    {
        std::cout << "BonjourAirTunes onServicePublished" << std::endl;
    }

    virtual void onPublishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode)
    {
        std::cout << "BonjourAirTunes onPublishFailed" << std::endl;
    }
};

class BonjourAirTunesDiscoveryListener : public AirPlayNsdDiscoveryListener
{
public:
    BonjourAirTunesDiscoveryListener() {}
    
    virtual ~BonjourAirTunesDiscoveryListener() {}

    virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode)
    {
        std::cout << "BonjourAirTunes onStopDiscoveryFailed" << std::endl;
    }

    virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode) 
    {
        std::cout << "BonjourAirTunes onStartDiscoveryFailed" << std::endl;
    }

    virtual void onServiceLost(const std::string serviceType, 
                               const std::string serviceName, 
                               const std::string serviceIp,
                               uint16_t servicePort)
    {
        std::cout << "---------------------- AirTunes onServiceLost ----------------------" << std::endl;
        std::cout << "serviceType: " << serviceType << std::endl;
        std::cout << "serviceName: " << serviceName << std::endl;
        std::cout << "serviceIp  : " << serviceIp << std::endl;
        std::cout << "servicePort: " << servicePort << std::endl;
    }

    virtual void onServiceFound(const std::string serviceType, 
                                const std::string serviceName, 
                                const std::string serviceIp,
                                uint16_t servicePort) 
    {
        std::cout << "---------------------- AirTunes onServiceFound ----------------------" << std::endl;
        std::cout << "serviceType: " << serviceType << std::endl;
        std::cout << "serviceName: " << serviceName << std::endl;
        std::cout << "serviceIp  : " << serviceIp << std::endl;
        std::cout << "servicePort: " << servicePort << std::endl;
    }

    virtual void onDiscoveryStopped(const std::string serviceType) 
    {
        std::cout << "BonjourAirTunes onDiscoveryStopped" << std::endl;
    }

    virtual void onDiscoveryStarted(const std::string serviceType) 
    {
        std::cout << "BonjourAirTunes onDiscoveryStarted" << std::endl;
    }
};


class BonjourDacpDiscoveryListener : public AirPlayNsdDiscoveryListener
{
public:
    BonjourDacpDiscoveryListener() {}
    
    virtual ~BonjourDacpDiscoveryListener() {}

    virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode)
    {
        std::cout << "BonjourDacp onStopDiscoveryFailed" << std::endl;
    }

    virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode) 
    {
        std::cout << "BonjourDacp onStartDiscoveryFailed" << std::endl;
    }

    virtual void onServiceLost(const std::string serviceType, 
                               const std::string serviceName, 
                               const std::string serviceIp,
                               uint16_t servicePort)
    {
        std::cout << "---------------------- BonjourDacp onServiceLost ----------------------" << std::endl;
        std::cout << "serviceType: " << serviceType << std::endl;
        std::cout << "serviceName: " << serviceName << std::endl;
        std::cout << "serviceIp  : " << serviceIp << std::endl;
        std::cout << "servicePort: " << servicePort << std::endl;

        // DACP服务下线，清除DACP信息
        gAirTunesServer.cleanDacpInfo(serviceIp);
    }

    virtual void onServiceFound(const std::string serviceType, 
                                const std::string serviceName, 
                                const std::string serviceIp,
                                uint16_t servicePort) 
    {
        std::cout << "---------------------- BonjourDacp onServiceFound ----------------------" << std::endl;
        std::cout << "serviceType: " << serviceType << std::endl;
        std::cout << "serviceName: " << serviceName << std::endl;
        std::cout << "serviceIp  : " << serviceIp << std::endl;
        std::cout << "servicePort: " << servicePort << std::endl;

        // DACP服务上线，设置DACP服务IP和端口
        gAirTunesServer.setDacpInfo(serviceIp, servicePort);
    }

    virtual void onDiscoveryStopped(const std::string serviceType) 
    {
        std::cout << "BonjourDacp onDiscoveryStopped" << std::endl;
    }

    virtual void onDiscoveryStarted(const std::string serviceType) 
    {
        std::cout << "BonjourDacp onDiscoveryStarted" << std::endl;
    }
};
#endif // __APPLE_BONJOUR__


class MyAirTunesServerListener : public AirTunesServerListener
{
private:
    std::string     m_imgFile;

public:
    MyAirTunesServerListener() 
        : m_imgFile("e:/AirTunesImage.jpg")
    {}

    virtual ~MyAirTunesServerListener() 
    {}

    /*
    * 开始播放音频
    */
    virtual void didStartPlayAudio(const std::string &clientIp)
    {
        std::cout << "MyAirTunesServerListener::didStartPlayAudio() : " << clientIp << std::endl;
        gClientIp = clientIp;
    }

    /*
    * 暂停播放音频
    */
    virtual void didPause(const std::string &clientIp)
    {
        std::cout << "MyAirTunesServerListener::didPause() : " << clientIp << std::endl;
    }

    /*
    * 停止播放音频
    */
    virtual void didStop(const std::string &clientIp)
    {
        std::cout << "MyAirTunesServerListener::didStop() : " << clientIp << std::endl;
        gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_STOP);
    }

    /*
    * 设置音量
    *  @value              [in] 音量值
    */
    virtual void didSetVolume(float value, const std::string &clientIp)
    {
        std::cout << "MyAirTunesServerListener::didSetVolume: " << value  << " ip: " << clientIp << std::endl;
    }

    /*
    * 设置封面图片
    *  @data               [in] 图片数据（jpeg格式）
    *  @length             [in] 图片数据长度
    */        
    virtual void didSetImage(const char *data, std::size_t length, const std::string &clientIp)
    {
        std::cout << "MyAirTunesServerListener::didSetImage: " << length << " ip: " << clientIp << std::endl;
        
        // C++
        std::ofstream jpgFile;
        jpgFile.open(m_imgFile.c_str(), std::ios_base::out | std::ios_base::binary);
        for (std::size_t i = 0; i < length; ++i)
            jpgFile.put(data[i]);
        jpgFile.close();
    }

    /*
    * 设置音频信息
    *  @name               [in] 歌曲名
    *  @artist             [in] 作者
    *  @album              [in] 专辑名
    */
    virtual void didSetAudioInfo(const std::string &name, const std::string &artist, const std::string &album, const std::string &clientIp)
    {
        std::cout << "MyAirTunesServerListener::didSetAudioInfo()" << " ip: " << clientIp << std::endl;
        std::cout << "name   : " << name << std::endl;
        std::cout << "artist : " << artist << std::endl;
        std::cout << "album  : " << album << std::endl;
    }

    #ifndef __USE_LIB_AO__
    /*
    * 初始化音频输出信息
    *  @sampleRateInHz         [in] 音频码率，一般为44100
    *  @channelConfig          [in] 音频频道，1=单声道，2=立体声
    *  @audioFormat            [in] 音频格式：16位，32位
    *  @fmtp                   [in] fmtp for osx
    *  @fmtpLen                [in] length os fmtp
    *  @note
    *    Java代码（初始化AudioTrack）
        AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 
                                          sampleRateInHz,                         // 44100赫兹
                                          AudioFormat.CHANNEL_OUT_STEREO,         // 2: 立体声
                                          AudioFormat.ENCODING_PCM_16BIT,         // 16位
                                          sampleRateInHz * 2 * 4,
                                          AudioTrack.MODE_STREAM);
    */
    virtual void didAudioDriverInit(int sampleRateInHz, int channelConfig, int audioFormat, int fmtp[12], size_t fmtpLen)
    {
        std::cout << "MyAirTunesServerListener::didAudioDriverInit" << std::endl;
        std::cout << "sampleRateInHz: " << sampleRateInHz << std::endl;
        std::cout << "channelConfig: " << channelConfig << std::endl;
        std::cout << "audioFormat: " << audioFormat << std::endl;
    }

    /*
    * 输出音频数据，PCM格式
    *  @outbuf         [in] 音频数据
    *  @size           [in] 音频数据长度
    *  @note
    *    Java代码（输出音频）
    *    track.write(outbuf, 0, size);
    */
    virtual void didAudioDriverPlay(char *outbuf, int size)
    {
        //std::cout << "MyAirTunesServerListener::didAudioDriverPlay" << std::endl;
        //std::cout << "size: " << size << std::endl;
    }
    #endif // __USE_LIB_AO__
};

//------------------------------------------------------------------
// 1. 实例化AirTunes
//------------------------------------------------------------------
MyAirTunesServerListener        gMyAirTunesServerListener;

#ifdef __APPLE_BONJOUR__

/*
名词解释：
Bonjour：苹果自动发现服务
AirTunes：AirPlay中音频服务
DACP：AirTunes服务中用来控制音频的协议（Digital Audio Control Protocol）

注意：在AirTunes中，需要启动2个Bonjour服务，一个发布AirTunes服务，另一个监听iPhone端发布的Dacp服务
*/

// AirTunes自动发现服务
AirPlayNsd                          gBonjourAirTunes;                   
BonjourAirTunesDiscoveryListener    gBonjourAirTunesDiscoveryListener;
BonjourAirTunesPublishListener      gBonjourAirTunesPublishListener;

// AirTunes控制服务
AirPlayNsd                          gBonjourDacp;
BonjourDacpDiscoveryListener        gBonjourDacpDiscoveryListener;
#endif


class MyCommand
{
public:
    MyCommand() 
        : m_eventSessionId(0)
        , m_playing(false)
    {
    }

    ~MyCommand()
    {
    }
    
    void run()
    {
        std::string buf;
        m_loop = true;

        while (m_loop)
        {
            std::cout << "> ";
            buf.clear();
            std::cin >> buf;
            parseCommand(buf);
        }
    }

    void setEventSessionId(int id) { m_eventSessionId = id; }

private:
    void parseCommand(const std::string &cmd)
    {

        if (cmd == "help" || cmd == "h")
        {
            std::cout << "---------- help ----------" << std::endl;
            std::cout << "help  -- help infomation" << std::endl;
            std::cout << "exit  -- exit" << std::endl;
            std::cout << "reset -- restart AirJoy" << std::endl;
            std::cout << "stop  -- stop" << std::endl;
            std::cout << "start -- start" << std::endl;
            std::cout << "p1    -- next item" << std::endl;
            std::cout << "p2    -- prev item" << std::endl;
            std::cout << "p3    -- pause" << std::endl;
            std::cout << "p4    -- play | pause" << std::endl;
            std::cout << "p5    -- play" << std::endl;
            std::cout << "p6    -- stop" << std::endl;
            std::cout << "p7    -- volume down" << std::endl;
            std::cout << "p8    -- volume up" << std::endl;
            return;
        }

        if (cmd == "p1")
        {
            gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_NEXTITEM);
            return;
        }

        if (cmd == "p2")
        {
            gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_PREVITEM);
            return;
        }

        if (cmd == "p3")
        {
            gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_PAUSE);
            return;
        }
        
        if (cmd == "p4")
        {
            gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_PLAYPAUSE);
            return;
        }
        
        if (cmd == "p5")
        {
            gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_PLAY);
            return;
        }
        
        if (cmd == "p6")
        {
            gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_STOP);
            return;
        }
        
        if (cmd == "p7")
        {
            gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_VOLUMEDOWN);
            return;
        }
        
        if (cmd == "p8")
        {
            gAirTunesServer.sendDacpCommand(gClientIp, AirTunesServer::DACP_CMD_VOLUMEUP);
            return;
        }

        if (cmd == "stop")
        {
            // 停止AirTunes服务
#ifdef __APPLE_BONJOUR__
            gBonjourAirTunes.stopServiceDiscovery();
            gBonjourDacp.stopServiceDiscovery();
#endif
            gAirTunesServer.stop();

            return;
        }

        if (cmd == "start")
        {
            gAirTunesServer.start(gAirTunesServiceInfo, &gMyAirTunesServerListener);
                
            gAirTunesServiceInfo.setPort(gAirTunesServer.port());

#ifdef __APPLE_BONJOUR__
            gBonjourAirTunes.discoveryService(gAirTunesServiceInfo.type(), &gBonjourAirTunesDiscoveryListener);
            gBonjourAirTunes.publishService(gAirTunesServiceInfo, &gBonjourAirTunesPublishListener);

            gBonjourDacp.discoveryService("_dacp._tcp", &gBonjourDacpDiscoveryListener);
#endif

            return;
        }

        if (cmd == "reset" || cmd == "r")
        {
            for (int i = 0; i < 1; i++)
            {
                //stop();
                //start();
                std::cout << "reset: " << i << std::endl;
            }

            return;
        }

        if (cmd == "exit" || cmd == "x")
        {
#ifdef __APPLE_BONJOUR__
            gBonjourAirTunes.stopServiceDiscovery();
            gBonjourDacp.stopServiceDiscovery();
#endif
            gAirTunesServer.stop();

            m_loop = false;
            return;
        }

    }


private:
    bool               m_loop;
    int                m_eventSessionId;
    bool               m_playing;
};

int main(int argc, char *argv[])
{
    // 0 设置硬件信息
    // 设置网卡地址，和将网卡地址设置为DeviceID效果一样
#if 0
    unsigned char macAddress[] = {0x00, 0x21, 0x6a, 0xb8, 0xa7, 0x34};
    gAirTunesServiceInfo.setMacAddress(macAddress, 6);                      // 本机网卡MAC地址
#else
    gAirTunesServiceInfo.setDeviceId("00:21:6a:b8:a7:34");                  // 设备ID
    //gAirTunesServiceInfo.setDeviceId("11:3f:FF:01:60:F9");                  // 设备ID
#endif
    gAirTunesServiceInfo.setDeviceName("HP-WIN7");                             // 机器名称

    // 打印AirTunes发布时的名称
    std::cout << "AirTunes published Name: " << gAirTunesServiceInfo.publishedName() << std::endl;

    // 1 启动AirTunes服务
    gAirTunesServer.start(gAirTunesServiceInfo, &gMyAirTunesServerListener);

    // 2 启动Bonjour服务
#ifdef __APPLE_BONJOUR__

    // 2.1 启动Bonjour模块
    bool ret = gBonjourAirTunes.discoveryService(gAirTunesServiceInfo.type(), &gBonjourAirTunesDiscoveryListener);
    if (! ret)
    {
        std::cout << "AirTunes discoveryService failed!" << std::endl;
    }
    else
    {
        std::cout << "AirTunes discoveryService ok!" << std::endl;
    }

    // 2.2 发布AirTunes服务
    gAirTunesServiceInfo.setPort(gAirTunesServer.port());                   // 更新服务端口，发布服务需要
    ret = gBonjourAirTunes.publishService(gAirTunesServiceInfo, &gBonjourAirTunesPublishListener);
    if (! ret)
    {
        std::cout << "AirTunes publishService failed!" << std::endl;
    }
    else
    {
        std::cout << "AirTunes publishService ok!" << std::endl;
    }

    // 2.3 启动DACP自动发现功能
    ret = gBonjourDacp.discoveryService("_dacp._tcp", &gBonjourDacpDiscoveryListener);
    if (! ret)
    {
        std::cout << "DACP discoveryService failed!" << std::endl;
    }
    else
    {
        std::cout << "DACP discoveryService ok!" << std::endl;
    }
#endif

    MyCommand cmd;
    cmd.run();

    std::cout << "byebye! " << std::endl;

    return 0;
}
