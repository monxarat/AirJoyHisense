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
// 0. ʵ����AirTunes
//------------------------------------------------------------------
AirTunesServiceInfo				gAirTunesServiceInfo;
AirTunesServer                  gAirTunesServer;

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

        // DACP�������ߣ����DACP��Ϣ
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

        // DACP�������ߣ�����DACP����IP�Ͷ˿�
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
    * ��ʼ������Ƶ
    */
    virtual void didStartPlayAudio(void)
    {
        std::cout << "MyAirTunesServerListener::didStartPlayAudio() " << std::endl;
    }

    /*
    * ��ͣ������Ƶ
    */
    virtual void didPause(void)
    {
        std::cout << "MyAirTunesServerListener::didPause() " << std::endl;
    }

    /*
    * ֹͣ������Ƶ
    */
    virtual void didStop(void)
    {
        std::cout << "MyAirTunesServerListener::didStop() " << std::endl;
    }

    /*
    * ��������
    *  @value              [in] ����ֵ
    */
    virtual void didSetVolume(float value)
    {
        std::cout << "MyAirTunesServerListener::didSetVolume: " << value << std::endl;
    }

    /*
    * ���÷���ͼƬ
    *  @data               [in] ͼƬ���ݣ�jpeg��ʽ��
    *  @length             [in] ͼƬ���ݳ���
    */        
    virtual void didSetImage(const char *data, std::size_t length)
    {
        std::cout << "MyAirTunesServerListener::didSetImage: " << length << std::endl;
        
        // C++
        std::ofstream jpgFile;
        jpgFile.open(m_imgFile.c_str(), std::ios_base::out | std::ios_base::binary);
        for (std::size_t i = 0; i < length; ++i)
            jpgFile.put(data[i]);
        jpgFile.close();
    }

    /*
    * ������Ƶ��Ϣ
    *  @name               [in] ������
    *  @artist             [in] ����
    *  @album              [in] ר����
    */
    virtual void didSetAudioInfo(const std::string &name, const std::string &artist, const std::string &album)
    {
        std::cout << "MyAirTunesServerListener::didSetAudioInfo()" << std::endl;
        std::cout << "name   : " << name << std::endl;
        std::cout << "artist : " << artist << std::endl;
        std::cout << "album  : " << album << std::endl;
    }

    #ifdef __ANDROID__
    /*
    * ��ʼ����Ƶ�����Ϣ
    *  @sampleRateInHz         [in] ��Ƶ���ʣ�һ��Ϊ44100
    *  @channelConfig          [in] ��ƵƵ����1=��������2=������
    *  @audioFormat            [in] ��Ƶ��ʽ��16λ��32λ
    *  @note
    *    Java���루��ʼ��AudioTrack��
        AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 
                                          sampleRateInHz,                         // 44100����
                                          AudioFormat.CHANNEL_OUT_STEREO,         // 2: ������
                                          AudioFormat.ENCODING_PCM_16BIT,         // 16λ
                                          sampleRateInHz * 2 * 4,
                                          AudioTrack.MODE_STREAM);
    */
    virtual void didAudioDriverInit(int sampleRateInHz, int channelConfig, int audioFormat)
    {
        std::cout << "MyAirTunesServerListener::didAudioDriverInit" << std::endl;
        std::cout << "sampleRateInHz: " << sampleRateInHz << std::endl;
        std::cout << "channelConfig: " << channelConfig << std::endl;
        std::cout << "audioFormat: " << audioFormat << std::endl;
    }

    /*
    * �����Ƶ���ݣ�PCM��ʽ
    *  @outbuf         [in] ��Ƶ����
    *  @size           [in] ��Ƶ���ݳ���
    *  @note
    *    Java���루�����Ƶ��
    *    track.write(outbuf, 0, size);
    */
    virtual void didAudioDriverPlay(char *outbuf, int size)
    {
        std::cout << "MyAirTunesServerListener::didAudioDriverPlay" << std::endl;
        std::cout << "size: " << size << std::endl;
    }
    #endif // __ANDROID__
};

//------------------------------------------------------------------
// 1. ʵ����AirTunes
//------------------------------------------------------------------
MyAirTunesServerListener        gMyAirTunesServerListener;

#ifdef __APPLE_BONJOUR__

/*
���ʽ��ͣ�
Bonjour��ƻ���Զ����ַ���
AirTunes��AirPlay����Ƶ����
DACP��AirTunes����������������Ƶ��Э�飨Digital Audio Control Protocol��

ע�⣺��AirTunes�У���Ҫ����2��Bonjour����һ������AirTunes������һ������iPhone�˷�����Dacp����
*/

// AirTunes�Զ����ַ���
AirPlayNsd                          gBonjourAirTunes;                   
BonjourAirTunesDiscoveryListener    gBonjourAirTunesDiscoveryListener;
BonjourAirTunesPublishListener      gBonjourAirTunesPublishListener;

// AirTunes���Ʒ���
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
            gAirTunesServer.sendDacpCommand(AirTunesServer::DACP_CMD_NEXTITEM);
            return;
        }

        if (cmd == "p2")
        {
            gAirTunesServer.sendDacpCommand(AirTunesServer::DACP_CMD_PREVITEM);
            return;
        }

        if (cmd == "p3")
        {
            gAirTunesServer.sendDacpCommand(AirTunesServer::DACP_CMD_PAUSE);
            return;
        }
        
        if (cmd == "p4")
        {
            gAirTunesServer.sendDacpCommand(AirTunesServer::DACP_CMD_PLAYPAUSE);
            return;
        }
        
        if (cmd == "p5")
        {
            gAirTunesServer.sendDacpCommand(AirTunesServer::DACP_CMD_PLAY);
            return;
        }
        
        if (cmd == "p6")
        {
            gAirTunesServer.sendDacpCommand(AirTunesServer::DACP_CMD_STOP);
            return;
        }
        
        if (cmd == "p7")
        {
            gAirTunesServer.sendDacpCommand(AirTunesServer::DACP_CMD_VOLUMEDOWN);
            return;
        }
        
        if (cmd == "p8")
        {
            gAirTunesServer.sendDacpCommand(AirTunesServer::DACP_CMD_VOLUMEUP);
            return;
        }

        if (cmd == "stop")
        {
            // ֹͣAirTunes����
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
    // 0 ����Ӳ����Ϣ
    unsigned char macAddress[] = {0x00, 0x21, 0x6a, 0xb8, 0xa7, 0x34};
    gAirTunesServiceInfo.setMacAddress(macAddress, 6);                      // ��������MAC��ַ
    gAirTunesServiceInfo.setDeviceName("W500");                             // ��������

    // 1 ����AirTunes����
    gAirTunesServer.start(gAirTunesServiceInfo, &gMyAirTunesServerListener);

    // 2 ����Bonjour����
#ifdef __APPLE_BONJOUR__

    // 2.1 ����Bonjourģ��
    gBonjourAirTunes.discoveryService(gAirTunesServiceInfo.type(), &gBonjourAirTunesDiscoveryListener);

    // 2.2 ����AirTunes����
    gAirTunesServiceInfo.setPort(gAirTunesServer.port());                   // ���·���˿ڣ�����������Ҫ
    gBonjourAirTunes.publishService(gAirTunesServiceInfo, &gBonjourAirTunesPublishListener);

    // 2.3 ����DACP�Զ����ֹ���
    gBonjourDacp.discoveryService("_dacp._tcp", &gBonjourDacpDiscoveryListener);
#endif

    MyCommand cmd;
    cmd.run();

    std::cout << "byebye! " << std::endl;

    return 0;
}
