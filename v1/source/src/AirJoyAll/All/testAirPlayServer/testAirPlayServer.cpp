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

#include "AirPlayServer.hpp"
#include "AirPlayServerListener.hpp"

#ifdef __APPLE_BONJOUR__
#include "AirPlayNsd.hpp"
#include "AirPlayNsdDiscoveryListener.hpp"
#include "AirPlayNsdPublishListener.hpp"
#endif // __APPLE_BONJOUR__


using namespace airjoy;

//------------------------------------------------------------------
// 0. ʵ����AirJoy
//------------------------------------------------------------------
AirPlayServer                   gAirPlayServer;
AirPlayServiceInfo              gAirPlayServiceInfo;

bool parseParam(int argc, char *argv[], std::string *deviceId, std::string *deviceName)
{
    //----------------------------------------------------------------------------
    // �� 0 ������ȡ�豸��Ϣ�������豸ID���豸�ǳƣ��豸���ͣ��豸��ʱʱ��
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
    
// ȫ�ֱ�����publishEvent() ��Ҫ��Щ����
std::string                     gClientIp;              // �ͻ���ip
bool                            gIsPlaying = false;     // �������
AirPlayServer::EventCategory    gPlayingMediaType;      // ���ڲ��ŵ�ý������

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
    * �����¼�
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void didSubscribeEvent(const std::string &clientIp) 
    {
        std::cout << "didSubscribeEvent: " << clientIp << std::endl;
    }

    /**
    * ȡ�������¼�
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void didUnsubscribeEvent(const std::string &clientIp) 
    {
        std::cout << "didUnsubscribeEvent: " << clientIp << std::endl;
    }

    /**
     * ����������Ƭ
    *  @clientIp           [in] �ͻ���ip
     */
    virtual void willPutPhoto(const std::string &photoId, const std::string &clientIp)
    {
        std::cout << "willPutPhoto: " << photoId << " clientIp: " << clientIp << std::endl;

        // 0. ����ͻ���IP��ԭ���Ĳ�һ������Ϊ����һ���µ�iPhone������ͼƬ
        //    ��Ҫ�����¼���֪ͨ��һ��iPhone����Ƭ����Ƶ�Ѿ�ֹͣ��
        if (gClientIp != clientIp)
        {
            gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
            gAirPlayServer.closeConnection(gClientIp);
            gClientIp = clientIp;
        }
    
        // 1. ������Ϣ, publishEvent() ��Ҫ�õ�
        gPlayingMediaType = AirPlayServer::EventPhoto;
    }

    /*
    * ������Ƭ����Ҫ������ʾ��
    *  @photoId            [in] ��Ƭid
    *  @data               [in] ��Ƭԭʼ����(jpg��ʽ��
    *  @length             [in] ���ݴ�С
    *  @clientIp           [in] �ͻ���ip
    *  @note
    *      ������շ��������¼���
    *        1��ֹͣ��ʾ��Ƭ
    *        2��ֹͣ������Ƶ
    *        3����ͣ��Ƶ
    *        4��������Ƶ
    *      ����Ҫ�����¼�: AirPlayServer::publishEvent();
    */
    virtual void didPutPhoto(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp)
    {
        std::cout << "didPutPhoto: " << photoId << " len: " << length  << " clientIp: " << clientIp << std::endl;

        gIsPlaying = true;

        std::string path;
        path.append(m_rootPath);
        path.append(photoId);
        path.append(".jpg");

        // �ö����Ƹ�ʽ�����ļ�
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
    * ����������Ƭ
    *  @photoId            [in] ��Ƭid
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void willPutPhotoCacheOnly(const std::string &photoId, const std::string &clientIp)
    {
        std::cout << "willPutPhotoCacheOnly: " << photoId << " clientIp: " << clientIp << std::endl;
        
        // ������߼���willPutPhotoһ������ΪwillPutPhoto��willPutPhotoCacheOnly����˳��ȷ��

        // 0. ����ͻ���IP��ԭ���Ĳ�һ������Ϊ����һ���µ�iPhone������ͼƬ
        //    ��Ҫ�����¼���֪ͨ��һ��iPhone����Ƭ����Ƶ�Ѿ�ֹͣ��
        if (gClientIp != clientIp)
        {
            gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionStopped, gClientIp);
            gAirPlayServer.closeConnection(gClientIp);
            gClientIp = clientIp;
        }

        // 1. ������Ϣ, publishEvent() ��Ҫ�õ�
        gPlayingMediaType = AirPlayServer::EventPhoto;
    }

    /**
    * ������Ƭ�����棨��������ʾ������������
    *  @photoId            [in] ��Ƭid
    *  @data               [in] ��Ƭԭʼ����(jpg��ʽ��
    *  @length             [in] ���ݴ�С
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void didPutPhotoCacheOnly(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp)
    {
        std::cout << "didPutPhotoCacheOnly: " << photoId << " len: " << length << " clientIp: " << clientIp << std::endl;

        std::string path;
        path.append(m_rootPath);
        path.append(photoId);
        path.append("-cache");
        path.append(".jpg");

        // �ö����Ƹ�ʽ�����ļ�
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
    * ��ʾ�Ѿ��������Ƭ
    *  @photoId            [in] ��Ƭid
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void didDisplayCachedPhoto(const std::string &photoId, const std::string &clientIp)
    {
        // 1. ������Ϣ��publishEvent() ��Ҫ�õ�
        gClientIp = clientIp;
        gIsPlaying = true;
        gPlayingMediaType = AirPlayServer::EventPhoto;

        std::cout << "didDisplayCachedPhoto: " << photoId << " clientIp: " << clientIp << std::endl;
    }

    /**
    * ��ʼ������Ƶ
    *  @contentLocation    [in] ��Ƶλ�ã���λΪ�ٷֱ�: ��ǰλ��/�ܳ��ȣ�
    *  @startPosition      [in] ��startPostion��ָ��ʱ��㿪ʼ����
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void didStartPlayVideo(const std::string &contentLocation, double startPosition, const std::string &clientIp)
    {
        std::cout << "didStartPlayVideo: " << contentLocation << " startPostion: " << startPosition << " clientIp: " << clientIp << std::endl;
       
        // 0. ����ͻ���IP��ԭ���Ĳ�һ������Ϊ����һ���µ�iPhone��������Ƶ
        //    ��Ҫ�����¼���֪ͨ��һ��iPhone����Ƭ����Ƶ�Ѿ�ֹͣ��
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

        // 1. ������Ϣ, publishEvent() ��Ҫ�õ�
        gPlayingMediaType = AirPlayServer::EventVideo;

        // 2. �����¼�������iPhone��Ƶ�Ѿ���ʼ����
        gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionPlaying, gClientIp);

        gIsPlaying = true;

        double videoLength = 30;                        // ������Ƶ����
        m_playPosition = startPosition * videoLength;   // ���㵱ǰʱ���

        std::cout << "m_playPosition: " << m_playPosition << std::endl;
    }

    /**
    * ��ʼ������Ƶ
    *  @contentLocation    [in] ��Ƶλ�ã��磺http://10.0.1.2:7002/123.mp3
    *  @startPosition      [in] ��startPostion��ָ��ʱ��㿪ʼ���ţ���λΪ�ٷֱ�: ��ǰλ��/�ܳ��ȣ�
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void didStartPlayMusic(const std::string &contentLocation, double startPosition, const std::string &clientIp)
    {
        std::cout << "didStartPlayMusic: " << contentLocation << " startPostion: " << startPosition << " clientIp: " << clientIp << std::endl;
       
        // 0. ����ͻ���IP��ԭ���Ĳ�һ������Ϊ����һ���µ�iPhone��������Ƶ
        //    ��Ҫ�����¼���֪ͨ��һ��iPhone����Ƭ����Ƶ�Ѿ�ֹͣ��
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

        // 1. ������Ϣ, publishEvent() ��Ҫ�õ�
        gPlayingMediaType = AirPlayServer::EventVideo;

        // 2. �����¼�������iPhone��Ƶ�Ѿ���ʼ����
        gAirPlayServer.publishEvent(gPlayingMediaType, AirPlayServer::EventActionPlaying, gClientIp);

        gIsPlaying = true;

        double musicLength = 30.0;                    // ������Ƶ����
        m_playPosition = startPosition * musicLength;   // ���㵱ǰʱ���

        std::cout << "m_playPosition: " << m_playPosition << std::endl;
    }

    /**
    * ���ûط��ٶ�
    *  @rate: 
    *     0 - ��ͣ
    *     1 - �������ʲ���
    *  @clientIp           [in] �ͻ���ip
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
    * ֹͣ�طŻ�ֹͣ��ʾ��Ƭ
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void didStop(const std::string &clientIp) 
    {
        std::cout << "didStop" << " clientIp: " << clientIp << std::endl;

        gIsPlaying = false;
    }

    /**
    * ���õ�ǰ���Ž���
    *  @playPostion        [in] ��ǰ���Ž��ȣ���λ���룩
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void setCurrentPlaybackProgress(float playPosition, const std::string &clientIp) 
    {
        std::cout << "setCurrentPlaybackProgress: " <<  playPosition << " clientIp: " << clientIp << std::endl;

        m_playPosition = playPosition;
    }

    /**
    * ��ȡ��ǰ���Ž��ȣ���������ܶ�ȡ��Ƶ���ȣ�����0����
    *  @time               [out] position : ��ǰ���ŵ㣨��λ���룩
    *                      [out] duration : ��Ƶ�ܳ��ȣ���λ���룩
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void getCurrentPlaybackProgress(PlaybackTime &time, const std::string &clientIp) 
    {
        m_playPosition += 1.0;

        std::cout << "getCurrentPlaybackProgress" << " clientIp: " << clientIp << std::endl;

        time.playPosition = m_playPosition;

        time.duration = 30.0;             // ��Ƶ����Ϊ30s
    }

    /**
    * ��ȡ�ط���Ϣ
    *  @info               [out] �ط���Ϣ
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void getPlaybackInfo(PlaybackInfo &info, const std::string &clientIp) 
    {
        /*
        typedef struct AIRJOY_API _PlaybackInfo
        {
        PlaybackTime playbackTime;      // ��Ƶ�ܳ��� & ���ڲ��ŵ�ʱ���
        float rate;                     // 0.0 - ��ͣ, 1.0 - �������ʲ���
        bool readyToPlay;	            // true - �Ѿ�׼������, false - ��δ׼�����ţ���ȱʡֵ��true��
        bool playbackBufferEmpty;       // true - ���Ż���գ�false - ���Ż���ǿգ���ȱʡֵ��true��
        bool playbackBufferFull;        // true - ���Ż���������false - ���Ż���δ������ȱʡֵ��false��
        bool playbackLikelyToKeepUp;    // true��ȱʡֵ��true��
        TimeRanges loadedTimeRanges;    // ��Ƶ���س��� & ��Ƶ��ʼ���ŵ�ʱ���
        TimeRanges seekableTimeRanges;  // iPhone/iPad�˿��Ի�����Ƶ��ʱ�䷶Χ����Ƶ���� & ��ʼ�㣩
        } PlaybackInfo;
        */

        std::cout << "getPlaybackInfo" << " clientIp: " << clientIp << std::endl;

        m_playPosition += 1.0;

        std::cout << "m_playPosition: " << m_playPosition << std::endl;

        info.playbackTime.duration = 30.0;                // ��Ƶ����
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

        info.loadedTimeRanges.duration = 30.0;
        info.loadedTimeRanges.startPosition = 0.0;

        info.seekableTimeRanges.duration = 30.0;
        info.seekableTimeRanges.startPosition = 0.0;
    }

    /**
    * �����������ٷֱ�: ��ǰ����/��������
    *  @value              [in] �ٷֱ�: ��ǰ����/��������
    *  @clientIp           [in] �ͻ���ip
    */
    virtual void didSetVolume(float value, const std::string &clientIp)
    {
        std::cout << "didSetVolume: " << value << " clientIp: " << clientIp << std::endl;
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
// 1. ʵ����MyAirPlayServerListener
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
            return;
        }

        if (cmd == "stop")
        {
            // ֹͣAirPlay����
#ifdef __APPLE_BONJOUR__
            gAirPlayNsd.stopServiceDiscovery();
#endif
            gAirPlayServer.stop();

            return;
        }

        if (cmd == "start")
        {
            // ����AirPlay���� & ����AirPlay����
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

    }


private:
    bool               m_loop;
    bool               m_playing;
};

int main(int argc, char *argv[])
{
    // 0 ����Ӳ����Ϣ
    // ����������ַ���ͽ�������ַ����ΪDeviceIDЧ��һ��
#if 0
    unsigned char macAddress[] = {0x00, 0x21, 0x6a, 0xb8, 0xa7, 0x34};
    gAirPlayServiceInfo.setMacAddress(macAddress, 6);                      // ��������MAC��ַ
#else
    gAirPlayServiceInfo.setDeviceId("00:21::6a:b8:a7:34");                 // �豸ID
#endif
    gAirPlayServiceInfo.setDeviceName("W500");                             // ��������
    
    // 1 ����AirPlay����
    gAirPlayServer.start(gAirPlayServiceInfo, &gMyAirPlayServerListener);

    // 2 ����AirPlay�Զ����ֹ��ܣ�������AirPlay����
#ifdef __APPLE_BONJOUR__
    // 2.1 ����AirPlay�Զ����ֹ���
    gAirPlayNsd.discoveryService(gAirPlayServiceInfo.type(), &gMyAirPlayNsdDiscoveryListener);

    // 2.2 ����AirPlay����
    gAirPlayNsd.publishService(gAirPlayServiceInfo, &gMyAirPlayNsdPublishListener);
#endif

    std::cout << "features: 0x" << std::hex << gAirPlayServiceInfo.features() << std::dec << std::endl;

    MyCommand cmd;
    cmd.run();

    // 3. �˳�֮ǰ����ֹͣ���з���
#ifdef __APPLE_BONJOUR__
    gAirPlayNsd.stopServiceDiscovery();
#endif

    gAirPlayServer.stop();

    std::cout << "byebye! " << std::endl;

    return 0;
}
