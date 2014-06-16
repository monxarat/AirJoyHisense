/*
*
* testAirJoyClient.cpp
*
*
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>


#include "AirJoyNsd.hpp"
#include "AirJoyNsdDiscoveryListener.hpp"

#include "AirJoyClient.hpp"
#include "AirMediaClient.hpp"
#include "AirMediaClientQueryListener.hpp"
#include "AirMediaClientAudio.hpp"
#include "AirMediaClientPhoto.hpp"
#include "AirMediaClientVideo.hpp"
#include "AirMediaClientVolume.hpp"
#include "AirFileServer.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

using namespace airjoy;


class MyAirJoyNsdDiscoveryListener : public AirJoyNsdDiscoveryListener
{
public:
    MyAirJoyNsdDiscoveryListener() 
    {
    }
    
    virtual ~MyAirJoyNsdDiscoveryListener() 
    {
    }

    /**
    * ĳ�������Ѿ�����
    *  @serviceInfo    [in] ������Ϣ
    */
    virtual void onServiceLost(AirJoyServiceInfo &serviceInfo)
    {
		std::cout << "onServiceLost: " << serviceInfo.deviceName() << " " << serviceInfo.deviceId() << " " << serviceInfo.deviceType() << std::endl;
    }

    /**
    * ĳ�������Ѿ�����
    *  @serviceInfo    [in] ������Ϣ
    */
    virtual void onServiceFound(AirJoyServiceInfo &serviceInfo)
    {
		std::cout << "onServiceFound: " << serviceInfo.deviceName() << " " << serviceInfo.deviceId() << " " << serviceInfo.deviceType();
		std::cout << " --> ip: " << serviceInfo.ip() << ":" << serviceInfo.port() << std::endl;
    }
};

class MyAirMediaClientQueryListener : public AirMediaClientQueryListener
{
public:
    MyAirMediaClientQueryListener() {}
    virtual ~MyAirMediaClientQueryListener() {}

    /**
    * �����¼��ɹ�
    */
    virtual void didSubscribeEvent(const std::string &serverIp)
    {
        std::cout << "Event didSubscribeEvent: " << serverIp << std::endl;
    }

    /**
    * ȡ�������¼��ɹ�
    */
    virtual void didUnsubscribeEvent(const std::string &serverIp)
    {
        std::cout << "Event didUnsubscribeEvent: " << serverIp << std::endl;
    }

    /**
    * �յ��¼�
    *  @type           [in] ���ͣ�photo/audio/video
    *  @action         [in] ��Ϊ��play/pause/stop
    *  @id             [in] id for photo/audio/video
    *  @url            [in, option] url for photo/audio/video
    *  @name           [in, option] name for photo/audio/video
    */
    virtual void recvEvent(const std::string &type,
        const std::string &action,
        const std::string &id,
        const std::string &url,
        const std::string &name,
        const std::string &fromIp)
    {
        std::cout << "Receive Event: " << std::endl;
        std::cout << "type   : " << type << std::endl;
        std::cout << "action : " << action << std::endl;
        std::cout << "id     : " << id << std::endl;
        std::cout << "url    : " << url << std::endl;
        std::cout << "name   : " << name << std::endl;
        std::cout << "fromIp : " << fromIp << std::endl;
    }

	/**
	* ��ʱ
	*/
	virtual void didTimeout(const AirJoySessionId sessionId)
	{
		std::cout << "didTimeout ! sessionid: " << sessionId << std::endl;
	}

	/**
	* �������
	*/
	virtual void didErrorNet(const AirJoySessionId sessionId)
	{
		std::cout << "didErrorNet ! sessionId: " << sessionId << std::endl;
	}

    /**
    * ������
    *  @sessionId     [in] �ỰID
    *  @code          [in] ���ش���
    *      ResultCodeOk����ʾ���շ��Ѿ��յ������Ҵ���ɹ���
    *      ResultCodeErrorNotSupport: �Է���֧�ִ�����
    *      ResultCodeErrorTimeout: ��ʱ��
    */
    virtual void handleResult                  (const AirJoySessionId sessionId,
                                                ResultCode code)
    {
        static int count = 0;
        std::cout << "[ " << count++ << " ] " << "handleResult: " << sessionId << " code: " << code << std::endl;
    }

    /**
    * ������
    *  @sessionId      [in] �ỰID
    *  @appAction      [in] ��Ϊ
    *  @appResult      [in] ����
    */
    virtual void handleResult                  (const AirJoySessionId sessionId,
                                                const std::string &appAction,
                                                const std::string &appResult)
    {
        std::cout << "handleResult: " << sessionId << " appAction: " << appAction << " appResult: " << appResult << std::endl;
    }

    /**
    * �����ȡ��Ƶ���Ž��Ƚ��
    *  @sessionId      [in] �ỰID
    *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
    *  @rate           [in] 0: ��ͣ��1: �������ʲ���
    *  @position       [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
    *  @return �ỰID
    */
    virtual void handleGetPlayVideoProgressResult    (const AirJoySessionId sessionId,
                                                      ResultCode code,
                                                      float rate,
                                                      float position)
    {
        std::cout << "handleGetPlayVideoProgressResult: " << sessionId << " code: " << code << " rate: " << rate << " position: " << position << std::endl;
    }

    /**
    * �����ȡ��Ƶ������Ϣ���
    *  @sessionId      [in] �ỰID
    *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
    *  @info           [in] �ط���Ϣ���� PlayInfo ����
    *  @return �ỰID
    */
    virtual void handleGetPlayVideoInfoResult        (const AirJoySessionId sessionId,
                                                      ResultCode code,
                                                      AirJoyPlayBackInfo &info)
    {
        std::cout << "handleGetPlayVideoInfoResult: " << sessionId << " code: " << code << std::endl;
        std::cout << " id: " << info.id << std::endl;
        std::cout << " url: " << info.url << std::endl;
        std::cout << " name: " << info.name << std::endl;
        std::cout << " rate: " << info.rate << std::endl;
        std::cout << " position: " << info.position << std::endl;
        std::cout << " duration: " << info.duration << std::endl;  
    }

    /**
    * �����ȡ��Ƶ���Ž��Ƚ��
    *  @sessionId      [in] �ỰID
    *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
    *  @rate           [in] 0: ��ͣ��1: �������ʲ���
    *  @position       [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
    *  @return �ỰID
    */
    virtual void handleGetPlayAudioProgressResult    (const AirJoySessionId sessionId,
                                                      ResultCode code,
                                                      float rate,
                                                      float position)
    {
        std::cout << "handleGetPlayAudioProgressResult: " << sessionId << " code: " << code << " rate: " << rate << " position: " << position << std::endl;
    }

    /**
    * �����ȡ��Ƶ������Ϣ���
    *  @sessionId      [in] �ỰID
    *  @code           [in] ���ش��룬�����ResultCodeOk����ʾinfoΪ��Чֵ��
    *  @info           [in] �ط���Ϣ���� PlayInfo ����
    *  @return �ỰID
    */
    virtual void handleGetPlayAudioInfoResult        (const AirJoySessionId sessionId,
                                                      ResultCode code,
                                                      AirJoyPlayBackInfo &info)
    {
        std::cout << "handleGetPlayAudioInfoResult: " << sessionId << " code: " << code << std::endl;
        std::cout << " id: " << info.id << std::endl;
        std::cout << " url: " << info.url << std::endl;
        std::cout << " name: " << info.name << std::endl;
        std::cout << " rate: " << info.rate << std::endl;
        std::cout << " position: " << info.position << std::endl;  
        std::cout << " duration: " << info.duration << std::endl;  
    }

    /**
    * �����ȡ�������
    *  @sessionId      [in] �ỰID
    *  @code           [in] ���ش��룬�����ResultCodeOk����ʾvolumeΪ��Чֵ��
    *  @volume         [in] ����ֵ��0 -> 100��
    *  @return �ỰID
    */
    virtual void handleGetVolumeResult               (const AirJoySessionId sessionId,
                                                      ResultCode code,
                                                      int volume)
    {
        std::cout << "handleGetVolumeResult: " << sessionId << " code: " << code  << " volume:" << volume << std::endl;
    }
};


//------------------------------------------------------------------
// 0. ʵ����AirJoy�ͻ���
//------------------------------------------------------------------
AirMediaClient                 gAirMediaClient;
MyAirMediaClientQueryListener  gMyAirMediaClientQueryListener;

AirJoyNsd                      gAirJoyNsd;
MyAirJoyNsdDiscoveryListener   gMyAirJoyNsdDiscoveryListener;

AirFileServer				   gAirFileServer;

#define SERVER_IP           "10.0.1.6"
#define SERVER_PORT         50535

class MyCommand
{
private:
    bool               m_loop;
    int                m_eventSessionId;
    bool               m_playing;

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
            std::cout << "stop  -- stop" << std::endl;
            std::cout << "start -- start" << std::endl;
			
			std::cout << "sub    -- subscribe event" << std::endl;
			std::cout << "unsub  -- unsubscribe event" << std::endl;

			std::cout << "    ---  photo ---" << std::endl;
			std::cout << "p1    -- display photo" << std::endl;
			std::cout << "p2    -- cache photo" << std::endl;
			std::cout << "p3    -- display cached photo" << std::endl;
			std::cout << "p4    -- stop display photo" << std::endl;
			std::cout << "p5    -- rotate photo" << std::endl;
			std::cout << "p6    -- zoom photo" << std::endl;
            std::cout << "p7    -- move photo" << std::endl;

			std::cout << "    ---  audio ---" << std::endl;
			std::cout << "a1    -- play audio" << std::endl;
			std::cout << "a2    -- set play audio rate" << std::endl;
			std::cout << "a3    -- stop play audio" << std::endl;
			std::cout << "a4    -- set play audio progress" << std::endl;
			std::cout << "a5    -- get play audio progress" << std::endl;
			std::cout << "a6    -- get play audio info" << std::endl;
			
			std::cout << "    ---  video ---" << std::endl;
			std::cout << "v1    -- play video" << std::endl;
			std::cout << "v2    -- set play video rate" << std::endl;
			std::cout << "v3    -- stop play video" << std::endl;
			std::cout << "v4    -- set play video progress" << std::endl;
			std::cout << "v5    -- get play video progress" << std::endl;
			std::cout << "v6    -- get play video info" << std::endl;
			
			std::cout << "    ---  volume ---" << std::endl;
			std::cout << "s1    -- set volume" << std::endl;
			std::cout << "s2    -- get volume" << std::endl;

		    std::cout << "    ---  3rdpart message ---" << std::endl;
            std::cout << "sm    -- send user-defined message" << std::endl;

            return;
        }

		//-----------------------------------------------------------------------
		// ��Ƭ��
		//-----------------------------------------------------------------------
		if (cmd == "p1")
		{
			std::string url = gAirFileServer.addSharedFile("111.jpg", "F:\\TEST\\1.JPG");
			AirJoySessionId sessionId = gAirMediaClient.photo()->displayPhoto(url, "111", PhotoActionDisplay);
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		if (cmd == "p2")
		{
			std::string url = gAirFileServer.addSharedFile("222.jpg", "F:\\TEST\\1.JPG");
			AirJoySessionId sessionId = gAirMediaClient.photo()->displayPhoto(url, "222", PhotoActionCache);
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		if (cmd == "p3")
		{
			AirJoySessionId sessionId = gAirMediaClient.photo()->displayCachedPhoto("222");
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		if (cmd == "p4")
		{
			AirJoySessionId sessionId = gAirMediaClient.photo()->stopDisplayPhoto();
			std::cout << "sessionId: " << sessionId << std::endl;
		}
				
		if (cmd == "p5")
		{
            for (int i = 0; i < 200; ++i)
            {
			    AirJoySessionId sessionId = gAirMediaClient.photo()->rotatePhoto("222", PhotoDirectionLeft);
			    std::cout << "sessionId: " << sessionId << std::endl;

#ifdef _WIN32
                Sleep(1 * 50);
#else
                //usleep(1000 * 100);
                //sleep(1);
#endif
            }
        }
		
		if (cmd == "p6")
		{
			AirJoySessionId sessionId = gAirMediaClient.photo()->zoomPhoto("222", 2.0);
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
        if (cmd == "p7")
		{
            AirJoySessionId sessionId = gAirMediaClient.photo()->movePhoto("222", 0.5, 0.0);
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		//-----------------------------------------------------------------------
		// ��Ƶ��
		//-----------------------------------------------------------------------
		if (cmd == "a1")
		{
			std::string url = gAirFileServer.addSharedFile("111222.mp3", "F:\\TEST\\1.MP3");
			AirJoySessionId sessionId = gAirMediaClient.audio()->playAudio(url, "111222");
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		if (cmd == "a2")
		{
			AirJoySessionId sessionId = gAirMediaClient.audio()->setPlayAudioRate("111222", 0.0);
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		if (cmd == "a3")
		{
			AirJoySessionId sessionId = gAirMediaClient.audio()->stopPlayAudio("111222");
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		if (cmd == "a4")
		{
			AirJoySessionId sessionId = gAirMediaClient.audio()->setPlayAudioProgress("111222", 30);
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		if (cmd == "a5")
		{
			AirJoySessionId sessionId = gAirMediaClient.audio()->getPlayAudioProgress("111222");
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		if (cmd == "a6")
		{
			AirJoySessionId sessionId = gAirMediaClient.audio()->getPlayAudioInfo();
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		//-----------------------------------------------------------------------
		// ��Ƶ��
		//-----------------------------------------------------------------------
		if (cmd == "v1")
		{
			std::string url = gAirFileServer.addSharedFile("111222.mp3", "F:\\TEST\\1.MPG");
			AirJoySessionId sessionId = gAirMediaClient.video()->playVideo(url, "111222");
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		if (cmd == "v2")
		{
			AirJoySessionId sessionId = gAirMediaClient.video()->setPlayVideoRate("111222", 0.0);
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		if (cmd == "v3")
		{
			AirJoySessionId sessionId = gAirMediaClient.video()->stopPlayVideo("111222");
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		if (cmd == "v4")
		{
			AirJoySessionId sessionId = gAirMediaClient.video()->setPlayVideoProgress("111222", 30);
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		if (cmd == "v5")
		{
			AirJoySessionId sessionId = gAirMediaClient.video()->getPlayVideoProgress("111222");
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		if (cmd == "v6")
		{
			AirJoySessionId sessionId = gAirMediaClient.video()->getPlayVideoInfo();
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		//-----------------------------------------------------------------------
		// ����
		//-----------------------------------------------------------------------
		if (cmd == "s1")
		{
			AirJoySessionId sessionId = gAirMediaClient.volume()->setVolume(50);
			std::cout << "sessionId: " << sessionId << std::endl;
		}

		if (cmd == "s2")
		{
			AirJoySessionId sessionId = gAirMediaClient.volume()->getVolume();
			std::cout << "sessionId: " << sessionId << std::endl;
		}
		
		// �����¼�
		if (cmd == "sub")
		{
            gAirMediaClient.subscribeEvent(SERVER_IP, SERVER_PORT);
		}
		
		// ȡ������
		if (cmd == "unsub")
		{
            gAirMediaClient.unsubscribeEvent();
		}

        if (cmd == "stop")
        {
            // ֹͣ�Զ�����
            gAirJoyNsd.stop();

            // ֹͣAirMedia�ͻ���
            gAirMediaClient.stop();
		
			// ֹͣ�ļ�������
			gAirFileServer.stop();

            return;
        }
		
        //-----------------------------------------------------------------------
		// �����û��Զ�����Ϣ
		//-----------------------------------------------------------------------
		if (cmd == "sm")
		{
            std::string appAction("UserDefinedAction"); // ��Ϊ
            std::string appParam("UserDefinedParam");   // ����

			AirJoySessionId sessionId = gAirMediaClient.send(appAction, appParam);

			std::cout << "sessionId: " << sessionId << std::endl;
		}

        if (cmd == "start")
        {
            // �����Զ�����
            gAirJoyNsd.start();
            gAirJoyNsd.discoveryService(&gMyAirJoyNsdDiscoveryListener);

            // ����AirMedia�ͻ���
            gAirMediaClient.start();
            gAirMediaClient.setListener(&gMyAirMediaClientQueryListener);
			
			// �����ļ�������
			gAirFileServer.start();

            return;
        }

        if (cmd == "exit" || cmd == "x")
        {
            // ֹͣ�Զ�����
            gAirJoyNsd.stop();

            // ֹͣAirMedia�ͻ���
            gAirMediaClient.stop();

			// ֹͣ�ļ�������
			gAirFileServer.stop();

            m_loop = false;
            return;
        }

    }
};

int main(int argc, char *argv[])
{
    // �����Զ�����
    gAirJoyNsd.start();
    bool isDiscoveryStarted = gAirJoyNsd.discoveryService(&gMyAirJoyNsdDiscoveryListener);
    if (isDiscoveryStarted)
        std::cout << "discoveryService started! " << std::endl;
    else
        std::cout << "discoveryService failed! " << std::endl;


    // ����AirMedia�ͻ���
    gAirMediaClient.start();
    gAirMediaClient.setListener(&gMyAirMediaClientQueryListener);

	// �����ļ�������
	gAirFileServer.start();

    MyCommand cmd;
    cmd.run();

    std::cout << "byebye! " << std::endl;

    return 0;
}
