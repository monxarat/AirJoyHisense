/*
*
* testAirJoyServer.cpp
*
*
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>


#include "AirJoyServer.hpp"
#include "AirMediaListener.hpp"


#include "AirJoyNsd.hpp"
#include "AirJoyNsdDiscoveryListener.hpp"


using namespace airjoy;

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

class MyAirMediaListener : public AirMediaListener
{
private:
    bool            m_isPlaying;
    float           m_playPosition;

public:
    MyAirMediaListener()
        : m_isPlaying(false)
        , m_playPosition(0.0)
    {
    }

    virtual ~MyAirMediaListener()
    {
	}

    //------------------------------------------------------------------------------------
    // ������ӿ�
    //------------------------------------------------------------------------------------	

    /**
    * �����¼�
    *  @fromIp         [in] ������IP
    *  @topic          [in] ��������
    */
    virtual void didSubscribeEvent(const std::string &fromIp, const std::string &topic)
    {
        std::cout << "didSubscribeEvent fromIp: " << fromIp << " topic: " << topic << std::endl;
    }

    /**
    * ȡ�������¼�
    *  @fromIp         [in] ������IP
    *  @topic          [in] ��������
    */        
    virtual void didUnsubscribeEvent(const std::string &fromIp, const std::string &topic)
    {
        std::cout << "didUnsubscribeEvent fromIp: " << fromIp << " topic: " << topic << std::endl;
    }

    //------------------------------------------------------------------------------------
    // ��Ƭ��ӿ�
    //------------------------------------------------------------------------------------

    /**
    * ������Ƭ��
    *  @url            [in] ��Ƭ����ַ
    *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
    *  @name           [in] ��Ƭ����(��ѡ)
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode cachePhoto           (const std::string &photoId,
                                             const std::string &url,
                                             const std::string &name,
                                             const std::string &fromIp)
    {
        std::cout << "cachePhoto: " << photoId << " url: " << url << " name: " << name << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * ��ʾ��Ƭ��
    *  @url            [in] ��Ƭ����ַ
    *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
    *  @name           [in] ��Ƭ����(��ѡ)
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode displayPhoto         (const std::string &photoId,
                                             const std::string &url,
                                             const std::string &name,
                                             const std::string &fromIp)
    { 
        std::cout << "displayPhoto: " << photoId << " url: " << url << " name: " << name << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * ��ʾ�Ѿ��������Ƭ��
    *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode displayCachedPhoto   (const std::string &photoId, const std::string &fromIp)
    { 
        std::cout << "displayCachedPhoto: " << photoId << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * ֹͣ��ʾ��Ƭ
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode stopPhoto            (const std::string &fromIp)
    { 
        std::cout << "stopPhoto: " << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * ��ת��Ƭ
    *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
    *  @direction      [in] ��ת����
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode rotatePhoto          (const std::string &photoId,
                                             PhotoDirectionType direction,
                                             const std::string &fromIp)
    {
		static int i = 0;

        std::cout << "rotatePhoto: " << i++ << " photoid: " << photoId << " direction: " << direction << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * ������Ƭ
    *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
    *  @rate           [in] ���ű���
    *                       rate = 0: ����������ʴ�С
    *                       rate = 1: ������ԭ��С
    *                       rate > 1: �Ŵ�
    *                       0 < rate < 1, ��С
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode zoomPhoto            (const std::string &photoId,
                                             float rate,
                                             const std::string &fromIp)
    {
        std::cout << "zoomPhoto: " << photoId << " rate: " << rate << std::endl;

        return ResultCodeOk; 
    }

	/**
     * �ƶ���Ƭ
     *  @photoId        [in] ��ʶ��Ƭ��Ψһ�ַ���
     *  @x              [in] x���ƶ��������з��Ÿ���������λ�����շ���Ļ���
��   *                       xֵ��Χ��(-1 < x < 1) 
��   *                       x = 0: ���ƶ�
��   *                       0 < x < 1: �����ƶ�
��   *                       ��-1 < x < 0: �����ƶ�
��   *                       ������շ���Ļ���Ϊ800����x = 0.3������ͼƬ����Ļ�ҷ��ƶ���0.3 x 800�������ص㡣
��   *  @y              [in] y���ƶ��������з��Ÿ���������λ�����շ���Ļ�߶�
��   *                     ��yֵ��Χ��(-1 < y < 1) 
��   *                     ��y = 0: ���ƶ�
��   *                       0 < y < 1: �����ƶ�
��   *                     ��-1 < y < 0: �����ƶ�
��   *                     ��������շ���Ļ���Ϊ600����y = 0.3������ͼƬ����Ļ�·��ƶ���0.3 x 600�������ص㡣
     *  @return
     *     ResultCodeOk - �ɹ�
     *     ResultCodeErrorNotSupport - ��֧�ִ˹���
     */
	virtual ResultCode movePhoto            (const std::string &photoId,
		                                     float x,
		                                     float y,
                                             const std::string &fromIp)
	{
		std::cout << "movePhoto: " << photoId << " x: " << x << " y: " << y << std::endl;

		return ResultCodeOk; // return ResultCodeErrorNotSupport; 
	}

    //------------------------------------------------------------------------------------
    // ��Ƶ��ӿ�
    //------------------------------------------------------------------------------------

    /**
    * ������Ƶ��
    *  @url            [in] ��Ƶ����ַ
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @name           [in] ��Ƶ����(��ѡ)
    *  @position       [in] ����ʱ��㣬��λΪ��
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode playVideo            (const std::string &videoId,
                                             const std::string &url,
                                             const std::string &name,
                                             float position,
                                             const std::string &fromIp)
    { 
        std::cout << "playVideo: " << videoId << " url: " << url << " name: " << name  << " position: " << position << std::endl;
        
        m_isPlaying = true;
        m_playPosition = position;

        return ResultCodeOk; 
    }

    /**
    * ���ûط��ٶ�
    *  @url            [in] ��Ƶ����ַ
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @rate           [in] 0: ��ͣ��1: �������ʲ���
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode setPlayVideoRate     (const std::string &videoId,
                                             float rate,
                                             const std::string &fromIp)
    {
        std::cout << "setPlayVideoRate: " << videoId << " rate: " << rate << std::endl;
        
        if (rate == 1.0)
        {
            m_isPlaying = true;
        }
        else
        {
            m_isPlaying = false;
        }

        return ResultCodeOk; 
    }

    /**
    * ֹͣ������Ƶ
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode stopPlayVideo        (const std::string &videoId, const std::string &fromIp)
    {
        std::cout << "stopPlayVideo: " << videoId << std::endl;
        
        m_isPlaying = false;

        return ResultCodeOk; 
    }

    /**
    * ���ò��Ž���
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode setPlayVideoProgress  (const std::string &videoId,
                                              float playPosition,
                                              const std::string &fromIp)
    {
        std::cout << "setPlayVideoProgress: " << videoId << " playPosition: " << playPosition << std::endl;
        
        m_playPosition = playPosition;

        return ResultCodeOk; 
    }

    /**
    * ��ȡ���Ž���
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @rate           [out] 0:��ͣ��1:����
    *  @position       [out] ��ǰ����/��ͣʱ��㣬��λΪ��
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode getPlayVideoProgress  (const std::string &videoId,
                                              float *rate,
                                              float *position,
                                              const std::string &fromIp)
    {
        std::cout << "getPlayVideoProgress: " << videoId << std::endl;
        
        *rate = m_isPlaying ? 1.0 : 0.0;

        *position = m_playPosition;

        return ResultCodeOk; 
    }

    /**
    * ��ȡ������Ϣ
    *  @info           [out] ��Ƶ��Ϣ
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode getPlayVideoInfo       (AirJoyPlayBackInfo &info, const std::string &fromIp)
    {
        std::cout << "getPlayVideoInfo" << std::endl;

        info.rate = 1.0;
        info.position = 20.0;
        info.duration = 100.0;

#if 1
        strncpy(info.id, "111", 128);
        strncpy(info.name, "test", 128);
        strncpy(info.url, "http://www.baidu.com/1.mp4", 1024);
#endif

        return ResultCodeOk; 
    }

    //------------------------------------------------------------------------------------
    // ��Ƶ��ӿ�
    //------------------------------------------------------------------------------------

    /**
    * ������Ƶ��
    *  @url            [in] ��Ƶ����ַ
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @name           [in] ��Ƶ����(��ѡ)
    *  @position       [in] ����ʱ��㣬��λΪ��
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode playAudio            (const std::string &audioId,
                                             const std::string &url,
                                             const std::string &name,
                                             float position,
                                             const std::string &fromIp)
    { 
        std::cout << "playAudio: " << audioId << " url: " << url << " name: " << name  << " position: " << position << std::endl;
        
        m_isPlaying = true;
        m_playPosition = position;

        return ResultCodeOk;
    }

    /**
    * ���ûط��ٶ�
    *  @url            [in] ��Ƶ����ַ
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @rate           [in] 0: ��ͣ��1: �������ʲ���
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode setPlayAudioRate     (const std::string &audioId,
                                             float rate,
                                             const std::string &fromIp)
    { 
        std::cout << "setPlayAudioRate: " << audioId << " rate: " << rate << std::endl;
        
        if (rate == 1.0)
        {
            m_isPlaying = true;
        }
        else
        {
            m_isPlaying = false;
        }

        return ResultCodeOk; 
    }

    /**
    * ֹͣ������Ƶ
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode stopPlayAudio        (const std::string &audioId, const std::string &fromIp)
    {
        std::cout << "stopPlayAudio: " << audioId << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * ���ò��Ž���
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode setPlayAudioProgress  (const std::string &audioId,
                                              float playPosition,
                                              const std::string &fromIp)
    {
        std::cout << "setPlayAudioProgress: " << audioId << " playPosition: " << playPosition << std::endl;
        
        m_playPosition = playPosition;

        return ResultCodeOk; 
    }

    /**
    * ��ȡ���Ž���
    *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
    *  @rate           [out] 0:��ͣ��1:����
    *  @position       [out] ��ǰ����/��ͣʱ��㣬��λΪ��
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode getPlayAudioProgress  (const std::string &audioId,
                                              float *rate,
                                              float *position,
                                              const std::string &fromIp)
    {
        std::cout << "getPlayAudioProgress: " << audioId << std::endl;
        
        *rate = m_isPlaying ? 1.0 : 0.0;
        *position = m_playPosition;

        return ResultCodeOk;
    }

    /**
    * ��ȡ������Ϣ
    *  @info           [out] ��Ƶ��Ϣ
    *  @return
    *     ResultCodeOk - �ɹ�
    *     ResultCodeErrorNotSupport - ��֧�ִ˹���
    */
    virtual ResultCode getPlayAudioInfo       (AirJoyPlayBackInfo &info, const std::string &fromIp)
    {
        std::cout << "getPlayAudioInfo: " << std::endl;
        
        info.rate = 1.0;
        info.position = 20.0;
        info.duration = 100.0;

#if 1
        strncpy(info.id, "111", 128);
        strncpy(info.name, "test", 128);
        strncpy(info.url, "http://www.baidu.com/1.mp3", 1024);
#endif

        return ResultCodeOk;
    }

    //------------------------------------------------------------------------------------
    // �����ӿ�
    //------------------------------------------------------------------------------------

    /**
    * ��������
    *  @volume         [in] ��ǰ����(1-100)
    *  @return �ỰID
    */
    virtual ResultCode setVolume               (int volume, const std::string &fromIp)
    {
        std::cout << "setVolume: " << volume << std::endl;

        return ResultCodeOk;
    }

    /**
    * ��ȡ����
    *  @volume         [out] ��ǰ����(1-100)
    *  @return �ỰID
    */
    virtual ResultCode getVolume               (int * volume, const std::string &fromIp)
    {
        std::cout << "getVolume: " << std::endl;
        
        *volume = 66;

        return ResultCodeOk;
    }

	/**
	* �յ�һ���Զ�������
	*  @appAction		[in] �Զ�����Ϊ
	*  @appParam		[in] �Զ������
	*  @appResult		[out] ���
	*  @appResultLen	[in] appResult����
	*  @return
	*		true  - ֧�ִ��Զ�������
	*		false - ��֧�ִ��Զ�������
	*/
	virtual bool didReceiveRequest			   (const std::string &appAction,
		                                        const std::string &appParam,
		                                        char *appResult,
                                                size_t apResultLen,
                                                const std::string &fromIp)
	{
		std::cout << "didReceiveRequest: " << std::endl;
		std::cout << "appAction: " << appAction << std::endl;
		std::cout << "appParam: " << appParam << std::endl;
		
		strncpy(appResult, "<result>hello</result>", apResultLen);

		return true;
	}
};


//------------------------------------------------------------------
// 0. ʵ����AirJoy
//------------------------------------------------------------------
AirJoyServer                    gAirJoyServer;
AirJoyServiceInfo               gAirJoyServiceInfo;
MyAirMediaListener              gMyAirMediaListener;

AirJoyNsd                       gAirJoyNsd;


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
            std::cout << "pv    -- publish event" << std::endl;
            return;
        }

        if (cmd == "stop")
        {
            // ֹͣAirJoy�Զ����ַ���
            gAirJoyNsd.stop();

            // ֹͣAirJoy����
            gAirJoyServer.stop();

            return;
        }

        if (cmd == "start")
        {
            //------------------------------------------------------------------------
            // 1 ����AirJoy����
            //------------------------------------------------------------------------

            // 1.1 ����AirJoy�����߳�
            gAirJoyServer.start(gAirJoyServiceInfo);

            // 1.2 ����AirMediaģ��
            gAirJoyServer.loadAirMedia(&gMyAirMediaListener);

            //------------------------------------------------------------------------
            // 2 ����AirJoy����
            //------------------------------------------------------------------------

            // 2.1 ����AirJoy�����߳�
            gAirJoyNsd.start();

            // 2.2 ��ȡ����˿ڣ�׼���������� 
            gAirJoyServiceInfo.setPort(gAirJoyServer.port());

            // 2.3 ����AirJoy����
            gAirJoyNsd.publishService(gAirJoyServiceInfo);

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
            // ֹͣAirJoy�Զ����ַ���
            gAirJoyNsd.stop();

            // ֹͣAirJoy����
            gAirJoyServer.stop();

            m_loop = false;
            return;
        }

		if (cmd == "pv")
		{
			gAirJoyServer.publishMediaEvent("photo", "play", "12345", "http://www.baidu.com/logo.jpg", "Baidu Logo", "192.160.0.111");
		}

    }


private:
    bool               m_loop;
    int                m_eventSessionId;
    bool               m_playing;
};

int main(int argc, char *argv[])
{
    std::string deviceId;
    std::string deviceName;
    DeviceType  deviceType = DeviceTypePcWindows;

    if (! parseParam(argc, argv, &deviceId, &deviceName))
        return 0;

    //------------------------------------------------------------------------
    // 1 ����AirJoy����
    //------------------------------------------------------------------------
    gAirJoyServiceInfo.setDeviceId(deviceId);
    gAirJoyServiceInfo.setDeviceName(deviceName);
    gAirJoyServiceInfo.setDeviceType(deviceType);

    // 1.1 ����AirJoy�����߳�
    gAirJoyServer.start(gAirJoyServiceInfo);

    // 1.2 ����AirMediaģ��
    gAirJoyServer.loadAirMedia(&gMyAirMediaListener);

    //------------------------------------------------------------------------
    // 2 ����AirJoy����
    //------------------------------------------------------------------------

    // 2.1 ����AirJoy�Զ����ֹ���
    gAirJoyNsd.start();

    // 2.2 ��ȡ����˿ڣ�׼����������
    gAirJoyServiceInfo.setPort(gAirJoyServer.port());

    // 2.3 ����AirJoy����
    bool isPublish = gAirJoyNsd.publishService(gAirJoyServiceInfo);
    if (isPublish)
        std::cout << "publishService : " << gAirJoyServiceInfo.deviceName() << " type: " << gAirJoyServiceInfo.type() << " deviceId: " << gAirJoyServiceInfo.deviceId() << " port: " << gAirJoyServiceInfo.port() << std::endl;
    else
        std::cout << "publishService failed: " << gAirJoyServiceInfo.deviceName() << std::endl;

    MyCommand cmd;
    cmd.run();

    std::cout << "byebye! " << std::endl;

    return 0;
}
