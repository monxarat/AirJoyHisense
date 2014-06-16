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
    // 订阅类接口
    //------------------------------------------------------------------------------------	

    /**
    * 订阅事件
    *  @fromIp         [in] 订阅者IP
    *  @topic          [in] 订阅主题
    */
    virtual void didSubscribeEvent(const std::string &fromIp, const std::string &topic)
    {
        std::cout << "didSubscribeEvent fromIp: " << fromIp << " topic: " << topic << std::endl;
    }

    /**
    * 取消订阅事件
    *  @fromIp         [in] 订阅者IP
    *  @topic          [in] 订阅主题
    */        
    virtual void didUnsubscribeEvent(const std::string &fromIp, const std::string &topic)
    {
        std::cout << "didUnsubscribeEvent fromIp: " << fromIp << " topic: " << topic << std::endl;
    }

    //------------------------------------------------------------------------------------
    // 照片类接口
    //------------------------------------------------------------------------------------

    /**
    * 缓存照片流
    *  @url            [in] 照片流地址
    *  @photoId        [in] 标识照片的唯一字符串
    *  @name           [in] 照片名称(可选)
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 显示照片流
    *  @url            [in] 照片流地址
    *  @photoId        [in] 标识照片的唯一字符串
    *  @name           [in] 照片名称(可选)
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 显示已经缓存的照片流
    *  @photoId        [in] 标识照片的唯一字符串
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
    */
    virtual ResultCode displayCachedPhoto   (const std::string &photoId, const std::string &fromIp)
    { 
        std::cout << "displayCachedPhoto: " << photoId << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * 停止显示照片
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
    */
    virtual ResultCode stopPhoto            (const std::string &fromIp)
    { 
        std::cout << "stopPhoto: " << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * 旋转照片
    *  @photoId        [in] 标识照片的唯一字符串
    *  @direction      [in] 旋转方向
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 缩放照片
    *  @photoId        [in] 标识照片的唯一字符串
    *  @rate           [in] 缩放比例
    *                       rate = 0: 缩放至最合适大小
    *                       rate = 1: 缩放至原大小
    *                       rate > 1: 放大
    *                       0 < rate < 1, 缩小
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
    */
    virtual ResultCode zoomPhoto            (const std::string &photoId,
                                             float rate,
                                             const std::string &fromIp)
    {
        std::cout << "zoomPhoto: " << photoId << " rate: " << rate << std::endl;

        return ResultCodeOk; 
    }

	/**
     * 移动照片
     *  @photoId        [in] 标识照片的唯一字符串
     *  @x              [in] x轴移动比例，有符号浮点数，单位：接收方屏幕宽度
　   *                       x值域范围：(-1 < x < 1) 
　   *                       x = 0: 不移动
　   *                       0 < x < 1: 向右移动
　   *                       　-1 < x < 0: 向左移动
　   *                       假如接收方屏幕宽度为800，则x = 0.3，代表图片向屏幕右方移动（0.3 x 800）个像素点。
　   *  @y              [in] y轴移动比例，有符号浮点数，单位：接收方屏幕高度
　   *                     　y值域范围：(-1 < y < 1) 
　   *                     　y = 0: 不移动
　   *                       0 < y < 1: 向下移动
　   *                     　-1 < y < 0: 向上移动
　   *                     　假如接收方屏幕宽度为600，则y = 0.3，代表图片向屏幕下方移动（0.3 x 600）个像素点。
     *  @return
     *     ResultCodeOk - 成功
     *     ResultCodeErrorNotSupport - 不支持此功能
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
    // 视频类接口
    //------------------------------------------------------------------------------------

    /**
    * 播放视频流
    *  @url            [in] 视频流地址
    *  @videoId        [in] 标识视频的唯一字符串
    *  @name           [in] 视频名称(可选)
    *  @position       [in] 播放时间点，单位为秒
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 设置回放速度
    *  @url            [in] 视频流地址
    *  @videoId        [in] 标识视频的唯一字符串
    *  @rate           [in] 0: 暂停，1: 正常速率播放
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 停止播放视频
    *  @videoId        [in] 标识视频的唯一字符串
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
    */
    virtual ResultCode stopPlayVideo        (const std::string &videoId, const std::string &fromIp)
    {
        std::cout << "stopPlayVideo: " << videoId << std::endl;
        
        m_isPlaying = false;

        return ResultCodeOk; 
    }

    /**
    * 设置播放进度
    *  @videoId        [in] 标识视频的唯一字符串
    *  @playPosition   [in] 当前播放视频的时间点，单位为秒
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 获取播放进度
    *  @videoId        [in] 标识视频的唯一字符串
    *  @rate           [out] 0:暂停，1:播放
    *  @position       [out] 当前播放/暂停时间点，单位为秒
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 获取播放信息
    *  @info           [out] 视频信息
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    // 音频类接口
    //------------------------------------------------------------------------------------

    /**
    * 播放音频流
    *  @url            [in] 音频流地址
    *  @videoId        [in] 标识音频的唯一字符串
    *  @name           [in] 视频名称(可选)
    *  @position       [in] 播放时间点，单位为秒
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 设置回放速度
    *  @url            [in] 音频流地址
    *  @videoId        [in] 标识视频的唯一字符串
    *  @rate           [in] 0: 暂停，1: 正常速率播放
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 停止播放音频
    *  @videoId        [in] 标识音频的唯一字符串
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
    */
    virtual ResultCode stopPlayAudio        (const std::string &audioId, const std::string &fromIp)
    {
        std::cout << "stopPlayAudio: " << audioId << std::endl;
        
        return ResultCodeOk; 
    }

    /**
    * 设置播放进度
    *  @videoId        [in] 标识音频的唯一字符串
    *  @playPosition   [in] 当前播放音频的时间点，单位为秒
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 获取播放进度
    *  @videoId        [in] 标识音频的唯一字符串
    *  @rate           [out] 0:暂停，1:播放
    *  @position       [out] 当前播放/暂停时间点，单位为秒
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    * 获取播放信息
    *  @info           [out] 音频信息
    *  @return
    *     ResultCodeOk - 成功
    *     ResultCodeErrorNotSupport - 不支持此功能
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
    // 音量接口
    //------------------------------------------------------------------------------------

    /**
    * 设置音量
    *  @volume         [in] 当前音量(1-100)
    *  @return 会话ID
    */
    virtual ResultCode setVolume               (int volume, const std::string &fromIp)
    {
        std::cout << "setVolume: " << volume << std::endl;

        return ResultCodeOk;
    }

    /**
    * 获取音量
    *  @volume         [out] 当前音量(1-100)
    *  @return 会话ID
    */
    virtual ResultCode getVolume               (int * volume, const std::string &fromIp)
    {
        std::cout << "getVolume: " << std::endl;
        
        *volume = 66;

        return ResultCodeOk;
    }

	/**
	* 收到一条自定义请求
	*  @appAction		[in] 自定义行为
	*  @appParam		[in] 自定义参数
	*  @appResult		[out] 结果
	*  @appResultLen	[in] appResult长度
	*  @return
	*		true  - 支持此自定义请求
	*		false - 不支持此自定义请求
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
// 0. 实例化AirJoy
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
            // 停止AirJoy自动发现服务
            gAirJoyNsd.stop();

            // 停止AirJoy服务
            gAirJoyServer.stop();

            return;
        }

        if (cmd == "start")
        {
            //------------------------------------------------------------------------
            // 1 启动AirJoy服务
            //------------------------------------------------------------------------

            // 1.1 启动AirJoy服务线程
            gAirJoyServer.start(gAirJoyServiceInfo);

            // 1.2 加载AirMedia模块
            gAirJoyServer.loadAirMedia(&gMyAirMediaListener);

            //------------------------------------------------------------------------
            // 2 发布AirJoy服务
            //------------------------------------------------------------------------

            // 2.1 启动AirJoy服务线程
            gAirJoyNsd.start();

            // 2.2 读取服务端口，准备发布服务 
            gAirJoyServiceInfo.setPort(gAirJoyServer.port());

            // 2.3 发布AirJoy服务
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
            // 停止AirJoy自动发现服务
            gAirJoyNsd.stop();

            // 停止AirJoy服务
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
    // 1 启动AirJoy服务
    //------------------------------------------------------------------------
    gAirJoyServiceInfo.setDeviceId(deviceId);
    gAirJoyServiceInfo.setDeviceName(deviceName);
    gAirJoyServiceInfo.setDeviceType(deviceType);

    // 1.1 启动AirJoy服务线程
    gAirJoyServer.start(gAirJoyServiceInfo);

    // 1.2 加载AirMedia模块
    gAirJoyServer.loadAirMedia(&gMyAirMediaListener);

    //------------------------------------------------------------------------
    // 2 发布AirJoy服务
    //------------------------------------------------------------------------

    // 2.1 启动AirJoy自动发现功能
    gAirJoyNsd.start();

    // 2.2 读取服务端口，准备发布服务
    gAirJoyServiceInfo.setPort(gAirJoyServer.port());

    // 2.3 发布AirJoy服务
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
