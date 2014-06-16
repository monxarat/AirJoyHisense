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
#include <sstream>

#include "AirPlayServer.hpp"
#include "AirPlayServerListener.hpp"

#ifdef __APPLE_BONJOUR__
#include "AirPlayNsd.hpp"
#include "AirPlayNsdDiscoveryListener.hpp"
#include "AirPlayNsdPublishListener.hpp"
#endif // __APPLE_BONJOUR__


#ifdef _WIN32
#include <windows.h>
#endif

using namespace airjoy;

//------------------------------------------------------------------
// 0. 实例化AirJoy
//------------------------------------------------------------------
AirPlayServer                   gAirPlayServer;
AirPlayServiceInfo              gAirPlayServiceInfo;

bool parseParam(int argc, char *argv[], std::string *deviceId, std::string *deviceName)
{
    //----------------------------------------------------------------------------
    // 第 0 步：读取设备信息参数：设备ID，设备昵称，设备类型，设备超时时间
    //----------------------------------------------------------------------------
    if (argc >= 3)
    {
        *deviceId = argv[1];
        *deviceName = argv[2];
    }
    else
    {
        std::cout << "Usage: " << argv[0] << " id name"  << std::endl;
        std::cout << "id         - device id" << std::endl;
        std::cout << "name       - device name" << std::endl;
        std::cout << "example: " << std::endl;
        std::cout << "       " << argv[0] << " 00:27:13:B6:0B:84 W500" << std::endl;
        std::cout << "       " << argv[0] << " 00:27:13:B6:0B:99 iMac" << "" << std::endl;
        return false;
    }

    std::cout << "Init: deviceId:" << *deviceId << " name:" << *deviceName << std::endl;

    return true;
}
    
// 全局变量，publishEvent() 需要这些参数
std::string                     gClientIp;              // 客户端ip
bool                            gIsPlaying = false;     // 播放与否
AirPlayServer::EventCategory    gPlayingMediaType;      // 正在播放的媒体类型

std::string                     gSlideshowsClientIp;              // 幻灯片客户端ip



class MyAirPlayServerListener : public AirPlayServerListener
{
private:
    std::string     m_rootPath;
    float           m_playPosition;

public:
    MyAirPlayServerListener() 
        : m_rootPath("e:/")
        , m_playPosition(0.0)
    {}

    virtual ~MyAirPlayServerListener() {}

public:

    /**
    * 订阅事件
    *  @clientIp           [in] 客户端ip
    */
    virtual void didSubscribeEvent(const std::string &clientIp) 
    {
        std::cout << "didSubscribeEvent: " << clientIp << std::endl;
    }

    /**
    * 取消订阅事件
    *  @clientIp           [in] 客户端ip
    */
    virtual void didUnsubscribeEvent(const std::string &clientIp) 
    {
        std::cout << "didUnsubscribeEvent: " << clientIp << std::endl;
    }

    /**
     * 即将接收照片
    *  @clientIp           [in] 客户端ip
     */
    virtual void willPutPhoto(const std::string &photoId, const std::string &clientIp)
    {
        std::cout << "willPutPhoto: " << photoId << " clientIp: " << clientIp << std::endl;

        // 0. 如果客户端IP和原来的不一样，认为这是一个新的iPhone发送了图片
        //    需要发布事件，通知上一个iPhone，照片或视频已经停止。
        if (gClientIp != clientIp)
        {
            gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
            gAirPlayServer.closeConnection(gClientIp);
            gClientIp = clientIp;
        }
    
        // 1. 保存信息, publishEvent() 需要用到
        gPlayingMediaType = AirPlayServer::EventPhoto;
    }

    /*
    * 接收照片（需要马上显示）
    *  @photoId            [in] 照片id
    *  @data               [in] 照片原始数据(jpg格式）
    *  @length             [in] 数据大小
    *  @clientIp           [in] 客户端ip
    *  @note
    *      如果接收方有如下事件：
    *        1、停止显示照片
    *        2、停止播放视频
    *        3、暂停视频
    *        4、播放视频
    *      则需要发布事件: AirPlayServer::publishEvent();
    */
    virtual void didPutPhoto(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp)
    {
        std::cout << "didPutPhoto: " << photoId << " len: " << length  << " clientIp: " << clientIp << std::endl;

        gIsPlaying = true;

        std::string path;
        path.append(m_rootPath);
        path.append(photoId);
        path.append(".jpg");

        // 用二进制格式保存文件
#if 1
        // C++
        std::ofstream jpgFile;
        jpgFile.open(path.c_str(), std::ios_base::out | std::ios_base::binary);
        for (std::size_t i = 0; i < length; ++i)
            jpgFile.put(data[i]);
        jpgFile.close();
#else
        // C
        FILE  *fp = ::fopen(path.c_str(), "wb");
        if (fp != NULL)
        {
            size_t count = ::fwrite(data, length, 1, fp);
            ::fclose(fp);
        }
#endif
    }

    /**
    * 即将缓存照片
    *  @photoId            [in] 照片id
    *  @clientIp           [in] 客户端ip
    */
    virtual void willPutPhotoCacheOnly(const std::string &photoId, const std::string &clientIp)
    {
        std::cout << "willPutPhotoCacheOnly: " << photoId << " clientIp: " << clientIp << std::endl;
        
        // 这里的逻辑与willPutPhoto一样，因为willPutPhoto和willPutPhotoCacheOnly调用顺序不确定

        // 0. 如果客户端IP和原来的不一样，认为这是一个新的iPhone发送了图片
        //    需要发布事件，通知上一个iPhone，照片或视频已经停止。
        if (gClientIp != clientIp)
        {
            gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
            gAirPlayServer.closeConnection(gClientIp);
            gClientIp = clientIp;
        }

        // 1. 保存信息, publishEvent() 需要用到
        gPlayingMediaType = AirPlayServer::EventPhoto;
    }

    /**
    * 接收照片并缓存（不马上显示，缓存起来）
    *  @photoId            [in] 照片id
    *  @data               [in] 照片原始数据(jpg格式）
    *  @length             [in] 数据大小
    *  @clientIp           [in] 客户端ip
    */
    virtual void didPutPhotoCacheOnly(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp)
    {
        std::cout << "didPutPhotoCacheOnly: " << photoId << " len: " << length << " clientIp: " << clientIp << std::endl;

        std::string path;
        path.append(m_rootPath);
        path.append(photoId);
        path.append("-cache");
        path.append(".jpg");

        // 用二进制格式保存文件
#if 1
        // C++
        std::ofstream jpgFile;
        jpgFile.open(path.c_str(), std::ios_base::out | std::ios_base::binary);
        for (std::size_t i = 0; i < length; ++i)
            jpgFile.put(data[i]);
        jpgFile.close();
#else   
        // C
        FILE  *fp = ::fopen(path.c_str(), "wb");
        if (fp != NULL)
        {
            size_t count = ::fwrite(data, length, 1, fp);
            ::fclose(fp);
        }
#endif
    }

    /**
    * 显示已经缓存的照片
    *  @photoId            [in] 照片id
    *  @clientIp           [in] 客户端ip
    */
    virtual void didDisplayCachedPhoto(const std::string &photoId, const std::string &clientIp)
    {
        // 1. 保存信息，publishEvent() 需要用到
        gClientIp = clientIp;
        gIsPlaying = true;
        gPlayingMediaType = AirPlayServer::EventPhoto;

        std::cout << "didDisplayCachedPhoto: " << photoId << " clientIp: " << clientIp << std::endl;
    }

    /**
    * 开始播放视频
    *  @contentLocation    [in] 视频位置（单位为百分比: 当前位置/总长度）
    *  @startPosition      [in] 从startPostion所指的时间点开始播放
    *  @clientIp           [in] 客户端ip
    */
    virtual void didStartPlayVideo(const std::string &contentLocation, double startPosition, const std::string &clientIp)
    {
        std::cout << "didStartPlayVideo: " << contentLocation << " startPostion: " << startPosition << " clientIp: " << clientIp << std::endl;
       
        // 0. 如果客户端IP和原来的不一样，认为这是一个新的iPhone发送了视频
        //    需要发布事件，通知上一个iPhone，照片或视频已经停止。
        if (gClientIp != clientIp)
        {
            gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
            gAirPlayServer.closeConnection(gClientIp);
            gClientIp = clientIp;
        }
        else
        {        
            if (gIsPlaying)
                gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
        }

        // 1. 保存信息, publishEvent() 需要用到
        gPlayingMediaType = AirPlayServer::EventVideo;

        // 2. 发布事件，告诉iPhone视频已经开始播放
        gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionPlaying, gClientIp);

        gIsPlaying = true;

        double videoLength = 285.0;                        // 假设视频长度
        m_playPosition = startPosition * videoLength;   // 计算当前时间点

        std::cout << "m_playPosition: " << m_playPosition << std::endl;
    }

    /**
    * 开始播放音频
    *  @contentLocation    [in] 音频位置，如：http://10.0.1.2:7002/123.mp3
    *  @startPosition      [in] 从startPostion所指的时间点开始播放（单位为百分比: 当前位置/总长度）
    *  @clientIp           [in] 客户端ip
    */
    virtual void didStartPlayMusic(const std::string &contentLocation, double startPosition, const std::string &clientIp)
    {
        std::cout << "didStartPlayMusic: " << contentLocation << " startPostion: " << startPosition << " clientIp: " << clientIp << std::endl;
       
        // 0. 如果客户端IP和原来的不一样，认为这是一个新的iPhone发送了音频
        //    需要发布事件，通知上一个iPhone，照片或视频已经停止。
        if (gClientIp != clientIp)
        {
            gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
            gAirPlayServer.closeConnection(gClientIp);
            gClientIp = clientIp;
        }
        else
        {
            if (gIsPlaying)
                gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
        }

        // 1. 保存信息, publishEvent() 需要用到
        gPlayingMediaType = AirPlayServer::EventVideo;

        // 2. 发布事件，告诉iPhone视频已经开始播放
        gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionPlaying, gClientIp);

        gIsPlaying = true;

        double musicLength = 285.0;                    // 假设音频长度
        m_playPosition = startPosition * musicLength;   // 计算当前时间点

        std::cout << "m_playPosition: " << m_playPosition << std::endl;
    }

    /**
    * 设置回放速度
    *  @rate: 
    *     0 - 暂停
    *     1 - 正常速率播放
    *  @clientIp           [in] 客户端ip
    */
    virtual void didSetPlaybackRate(float rate, const std::string &clientIp) 
    {
        std::cout << "didSetPlaybackRate: " << rate << " clientIp: " << clientIp << std::endl;

        if (rate == 1.0)
        {
            gIsPlaying = true;
        }
        else
        {
            gIsPlaying = false;
        }
    }


    /**
    * 停止回放或停止显示照片
    *  @clientIp           [in] 客户端ip
    */
    virtual void didStop(const std::string &clientIp) 
    {
        std::cout << "didStop" << " clientIp: " << clientIp << std::endl;

        gIsPlaying = false;
    }

    /**
    * 设置当前播放进度
    *  @playPostion        [in] 当前播放进度（单位：秒）
    *  @clientIp           [in] 客户端ip
    */
    virtual void setCurrentPlaybackProgress(float playPosition, const std::string &clientIp) 
    {
        std::cout << "setCurrentPlaybackProgress: " <<  playPosition << " clientIp: " << clientIp << std::endl;

        m_playPosition = playPosition;
    }

    /**
    * 获取当前播放进度（如果还不能读取视频长度，则填0处理）
    *  @time               [out] position : 当前播放点（单位：秒）
    *                      [out] duration : 视频总长度（单位：秒）
    *  @clientIp           [in] 客户端ip
    */
    virtual void getCurrentPlaybackProgress(PlaybackTime &time, const std::string &clientIp) 
    {
        m_playPosition += 1.0;

        std::cout << "getCurrentPlaybackProgress" << " clientIp: " << clientIp << std::endl;

        time.playPosition = m_playPosition;

        time.duration = 285.0;             // 视频长度为30s
    }

    /**
    * 获取回放信息
    *  @info               [out] 回放信息
    *  @clientIp           [in] 客户端ip
    */
    virtual void getPlaybackInfo(PlaybackInfo &info, const std::string &clientIp) 
    {
        /*
        typedef struct AIRJOY_API _PlaybackInfo
        {
        PlaybackTime playbackTime;      // 视频总长度 & 正在播放的时间点
        float rate;                     // 0.0 - 暂停, 1.0 - 正常速率播放
        bool readyToPlay;	            // true - 已经准备播放, false - 还未准备播放，（缺省值：true）
        bool playbackBufferEmpty;       // true - 播放缓冲空，false - 播放缓冲非空，（缺省值：true）
        bool playbackBufferFull;        // true - 播放缓冲已满，false - 播放缓冲未满，（缺省值：false）
        bool playbackLikelyToKeepUp;    // true（缺省值：true）
        TimeRanges loadedTimeRanges;    // 视频加载长度 & 视频开始播放的时间点
        TimeRanges seekableTimeRanges;  // iPhone/iPad端可以滑动视频的时间范围（视频长度 & 起始点）
        } PlaybackInfo;
        */

        std::cout << "getPlaybackInfo" << " clientIp: " << clientIp << std::endl;

        m_playPosition += 1.0;

        std::cout << "m_playPosition: " << m_playPosition << std::endl;

        info.playbackTime.duration = 285.0;                // 视频长度
        info.playbackTime.playPosition = m_playPosition;

        if (gIsPlaying)
        {
            info.rate = 1.0;

            std::cout << "gIsPlaying " << std::endl;
        }
        else
            info.rate = 0.0;

        info.readyToPlay = true;

        info.playbackBufferEmpty = true;
        info.playbackBufferFull = false;
        info.playbackLikelyToKeepUp = true;

        info.loadedTimeRanges.duration = 285.0;
        info.loadedTimeRanges.startPosition = 0.0;

        info.seekableTimeRanges.duration = 285.0;
        info.seekableTimeRanges.startPosition = 0.0;
    }

    /**
    * 设置音量（百分比: 当前音量/总音量）
    *  @value              [in] 百分比: 当前音量/总音量）
    *  @clientIp           [in] 客户端ip
    */
    virtual void didSetVolume(float value, const std::string &clientIp)
    {
        std::cout << "didSetVolume: " << value << " clientIp: " << clientIp << std::endl;
    }

    /**
    * 开始幻灯片播放
    *  @slideDuration      [in] 每张图片停留的时间
    *  @clientIp           [in] 客户端ip
    */
    virtual void didStartSlideshows(int slideDuration, const std::string &clientIp)
    {
        std::cout << "didStartSlideshows: " << slideDuration << " clientIp: " << clientIp << std::endl;
        
        gSlideshowsClientIp = clientIp;

        // 1. 保存信息, publishEvent() 需要用到
        gPlayingMediaType = AirPlayServer::EventSlideshow;
    }

    /**
    * 停止幻灯片播放
    *  @clientIp           [in] 客户端ip
    */
    virtual void didStopSlideshows(const std::string &clientIp)
    {
        std::cout << "didStopSlideshows: " << clientIp << std::endl;
    }

    /**
    * 获取到一张幻灯片播放的图片
    *  @index              [in] 照片索引，第1张，第2张，第3张...
    *  @data               [in] 照片原始数据(jpg格式）
    *  @length             [in] 数据大小
    *  @clientIp           [in] 客户端ip
    *  @note
    *      1. 调用AirPlayServer::getSlideshowsPicture(clientIp)才能收到此消息
    */
    virtual void didGetSlideshowsPicture(int index, const char *data, std::size_t length, const std::string &clientIp)
    {
        std::cout << "didGetSlideshowsPicture: " << length << " from: " << clientIp << std::endl;
        
        std::stringstream ss;
        ss << index;

        std::string path;
        path.append(m_rootPath);
        path.append("slideshows-");
        path.append(ss.str());
        path.append(".jpg");

        // 用二进制格式保存文件
#if 1
        // C++
        std::ofstream jpgFile;
        jpgFile.open(path.c_str(), std::ios_base::out | std::ios_base::binary);
        for (std::size_t i = 0; i < length; ++i)
            jpgFile.put(data[i]);
        jpgFile.close();
#else   
        // C
        FILE  *fp = ::fopen(path.c_str(), "wb");
        if (fp != NULL)
        {
            size_t count = ::fwrite(data, length, 1, fp);
            ::fclose(fp);
        }
#endif
    }

    /**
    * 获取幻灯片图片失败
    *  @clientIp           [in] 客户端ip
    *  @note
    *      1. 调用AirPlayServer::getSlideshowsPicture(clientIp)才能收到此消息
    *      2. 收到这个消息，表示幻灯片的照片已经播放完毕
    */
    virtual void didGetSlideshowsPictureFailed(const std::string &clientIp)
    {
        std::cout << "didGetSlideshowsPictureFailed: " << clientIp << std::endl;
    }
};

#ifdef __APPLE_BONJOUR__
class MyAirPlayNsdPublishListener : public AirPlayNsdPublishListener
{
public:
    MyAirPlayNsdPublishListener() {}
    
    virtual ~MyAirPlayNsdPublishListener() {}

    virtual void onUnPulishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode)
    {
        std::cout << "AirPlay onUnPulishFailed" << std::endl;
    }

    virtual void onServiceUnPublished(const std::string & servicetype, const std::string & serviceName)
    {
        std::cout << "AirPlay onServiceUnPublished" << std::endl;
    }

    virtual void onServicePublished(const std::string & servicetype, const std::string & serviceName)
    {
        std::cout << "AirPlay onServicePublished" << std::endl;
    }

    virtual void onPublishFailed(const std::string & servicetype, const std::string & serviceName, int errorCode)
    {
        std::cout << "AirPlay onPublishFailed" << std::endl;
    }
};

class MyAirPlayNsdDiscoveryListener : public AirPlayNsdDiscoveryListener
{
public:
    MyAirPlayNsdDiscoveryListener() {}
    
    virtual ~MyAirPlayNsdDiscoveryListener() {}

    virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode)
    {
        std::cout << "AirPlay onStopDiscoveryFailed" << std::endl;
    }

    virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode) 
    {
        std::cout << "AirPlay onStartDiscoveryFailed" << std::endl;
    }

    virtual void onServiceLost(const std::string serviceType, 
                               const std::string serviceName, 
                               const std::string serviceIp,
                               uint16_t servicePort)
    {
        std::cout << "---------------------- AirPlay onServiceLost ----------------------" << std::endl;
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
        std::cout << "---------------------- AirPlay onServiceFound ----------------------" << std::endl;
        std::cout << "serviceType: " << serviceType << std::endl;
        std::cout << "serviceName: " << serviceName << std::endl;
        std::cout << "serviceIp  : " << serviceIp << std::endl;
        std::cout << "servicePort: " << servicePort << std::endl;
    }

    virtual void onDiscoveryStopped(const std::string serviceType) 
    {
        std::cout << "AirPlay onDiscoveryStopped" << std::endl;
    }

    virtual void onDiscoveryStarted(const std::string serviceType) 
    {
        std::cout << "AirPlay onDiscoveryStarted" << std::endl;
    }
};
#endif // __APPLE_BONJOUR__


//------------------------------------------------------------------
// 1. 实例化MyAirPlayServerListener
//------------------------------------------------------------------
MyAirPlayServerListener         gMyAirPlayServerListener;

#ifdef __APPLE_BONJOUR__
AirPlayNsd                      gAirPlayNsd;
MyAirPlayNsdDiscoveryListener   gMyAirPlayNsdDiscoveryListener;
MyAirPlayNsdPublishListener     gMyAirPlayNsdPublishListener;
#endif


class MyCommand
{
public:
    MyCommand() 
        : m_playing(false)
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
            std::cout << "pause     -- pause" << std::endl;
            std::cout << "playing   -- playing" << std::endl;
            std::cout << "stopped   -- stopped" << std::endl;
            std::cout << "close     -- close connection" << std::endl;
            std::cout << "g         -- get a picture for slideshows" << std::endl;
            return;
        }

        if (cmd == "stop")
        {
            // 停止AirPlay服务
#ifdef __APPLE_BONJOUR__

            // to 阿三：2014.3.3 停止服务，iPhone就看不到TV了
            gAirPlayNsd.unpublishService();

            gAirPlayNsd.stopServiceDiscovery();
#endif
            gAirPlayServer.stop();

            return;
        }

        if (cmd == "start")
        {
            // 启动AirPlay服务 & 发布AirPlay服务
            gAirPlayServer.start(gAirPlayServiceInfo, &gMyAirPlayServerListener);

#ifdef __APPLE_BONJOUR__
            gAirPlayNsd.discoveryService(gAirPlayServiceInfo.type(), &gMyAirPlayNsdDiscoveryListener);
            gAirPlayNsd.publishService(gAirPlayServiceInfo, &gMyAirPlayNsdPublishListener);
#endif

            return;
        }

        if (cmd == "exit" || cmd == "x")
        {
#ifdef __APPLE_BONJOUR__
            gAirPlayNsd.unpublishService();
            gAirPlayNsd.stopServiceDiscovery();
#endif
            gAirPlayServer.stop();

            m_loop = false;
            return;
        }

        if (cmd == "pause")
        {
            gAirPlayServer.publishEvent(AirPlayServer::EventVideo, AirPlayServer::EventActionPaused, gClientIp);
            return;
        }

        if (cmd == "playing")
        {
            gAirPlayServer.publishEvent(AirPlayServer::EventVideo, AirPlayServer::EventActionPlaying, gClientIp);

            return;
        }

        if (cmd == "stopped")
        {
            gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
            return;
        }

        if (cmd == "close")
        {
            gAirPlayServer.closeConnection(gClientIp);
            return;
        }

        if (cmd == "g")
        {
            for (int i=0; i<300; ++i)
            {
                std::cout << "get slideshow picture: " << i << std::endl;

                // 获取一张幻灯片用的图片
                gAirPlayServer.getSlideshowsPicture(gClientIp);

            #ifdef _WIN32
                Sleep(1000 * 3);
            #else
                sleep(3);
            #endif
            }

            return;
        }
    }


private:
    bool               m_loop;
    bool               m_playing;
};

int main(int argc, char *argv[])
{
    // 0 设置硬件信息
    // 设置网卡地址，和将网卡地址设置为DeviceID效果一样
#if 0
    unsigned char macAddress[] = {0x00, 0x21, 0x6a, 0xb8, 0xa7, 0x34};
    gAirPlayServiceInfo.setMacAddress(macAddress, 6);                      // 本机网卡MAC地址
#else
    gAirPlayServiceInfo.setDeviceId("EC:88:8F:C0:44:9E");       // hp-win7
    //gAirPlayServiceInfo.setDeviceId("00:21:6a:b8:a7:34");
    //gAirPlayServiceInfo.setDeviceId("68:DF:DD:64:02:D8");     // mi3
    //gAirPlayServiceInfo.setDeviceId("70:25:59:62:69:44");
    //gAirPlayServiceInfo.setDeviceId("00:15:18:01:81:31");     // TV
    //gAirPlayServiceInfo.setDeviceId("70:25:59:62:69:44");       // TV wifi
#endif
    gAirPlayServiceInfo.setDeviceName("ViddaTV");                          // 机器名称

    // 不管上面是 setMacAddress() 还是 setDeviceId()，都能正确取到deviceId()
    std::cout << "AirPlay published DeviceID: " << gAirPlayServiceInfo.deviceId() << std::endl;

    // 1 启动AirPlay服务
    gAirPlayServer.start(gAirPlayServiceInfo, &gMyAirPlayServerListener);

    // 2 启动AirPlay自动发现功能，并发布AirPlay服务
#ifdef __APPLE_BONJOUR__
    // 2.1 启动AirPlay自动发现功能
    bool ret = gAirPlayNsd.discoveryService(gAirPlayServiceInfo.type(), &gMyAirPlayNsdDiscoveryListener);
    if (! ret)
    {
        std::cout << "AirPlay discoveryService failed!" << std::endl;
    }
    else
    {
        std::cout << "AirPlay discoveryService ok!" << std::endl;
    }

    // 2.2 发布AirPlay服务
    ret = gAirPlayNsd.publishService(gAirPlayServiceInfo, &gMyAirPlayNsdPublishListener);
    if (! ret)
    {
        std::cout << "AirPlay publishService failed!" << std::endl;
    }
    else
    {
        std::cout << "AirPlay publishService ok!" << std::endl;
    }


#endif

    //std::cout << "features: 0x" << std::hex << gAirPlayServiceInfo.features() << std::dec << std::endl;

    MyCommand cmd;
    cmd.run();

    // 3. 退出之前，先停止所有服务
#ifdef __APPLE_BONJOUR__
    gAirPlayNsd.stopServiceDiscovery();
#endif

    gAirPlayServer.stop();

    std::cout << "byebye! " << std::endl;

    return 0;
}
