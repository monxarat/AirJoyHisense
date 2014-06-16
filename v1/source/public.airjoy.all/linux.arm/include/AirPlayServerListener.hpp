/*
* AirPlayServerListener.hpp
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
*
*/

#ifndef __AIRPLAY_SERVER_LISTENER_H__
#define __AIRPLAY_SERVER_LISTENER_H__

#include <string>
#include "AirJoyApi.hpp"

namespace airjoy
{

    // 时间范围
    typedef struct AIRJOY_API _TimeRanges
    {
        float startPosition;    // 起始时间点，单位: seconds
        float duration;         // 持续时间长度，单位: seconds
    } TimeRanges;
    
    // 播放时间点
    typedef struct AIRJOY_API _PlaybackTime
    {
        float playPosition;     // 正在播放的时间点，单位: seconds
        float duration;         // 视频总长度，单位: seconds
    } PlaybackTime;

    // 回放信息
    typedef struct AIRJOY_API _PlaybackInfo
    {
        PlaybackTime playbackTime;      // 视频总长度 & 正在播放的时间点
        float rate;                     // 0 - 暂停, 1 - 正常速率播放
        bool readyToPlay;	            // true - 已经准备播放, false - 还未准备播放，（缺省值：true）
        bool playbackBufferEmpty;       // true - 播放缓冲空，false - 播放缓冲非空，（缺省值：true）
        bool playbackBufferFull;        // true - 播放缓冲满，false - 播放缓冲未满，（缺省值：false）
        bool playbackLikelyToKeepUp;    // true（缺省值：true）
        TimeRanges loadedTimeRanges;    // 视频加载长度 & 视频开始播放的时间点
        TimeRanges seekableTimeRanges;  // iPhone/iPad端可以滑动视频的时间范围（视频长度 & 起始点）
    } PlaybackInfo;

    class AIRJOY_API AirPlayServerListener
    {
    public:
        AirPlayServerListener() {}
        virtual ~AirPlayServerListener() {}

    public:

        /**
         * 订阅事件
         *  @clientIp           [in] 客户端ip
         */
        virtual void didSubscribeEvent(const std::string &clientIp) {}

        /**
         * 取消订阅事件
         *  @clientIp           [in] 客户端ip
         */
        virtual void didUnsubscribeEvent(const std::string &clientIp) {}

        /**
         * 即将接收照片
         *  @photoId            [in] 照片id
         *  @clientIp           [in] 客户端ip
         */
        virtual void willPutPhoto(const std::string &photoId, const std::string &clientIp) = 0;

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
        virtual void didPutPhoto(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp) = 0;
        
        /**
         * 即将缓存照片
         *  @photoId            [in] 照片id
         *  @clientIp           [in] 客户端ip
         */
        virtual void willPutPhotoCacheOnly(const std::string &photoId, const std::string &clientIp) = 0;

        /**
         * 接收照片并缓存（不马上显示，缓存起来）
         *  @photoId            [in] 照片id
         *  @data               [in] 照片原始数据(jpg格式）
         *  @length             [in] 数据大小
         *  @clientIp           [in] 客户端ip
         */
        virtual void didPutPhotoCacheOnly(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp) = 0;

        /**
         * 显示已经缓存的照片
         *  @photoId            [in] 照片id
         *  @clientIp           [in] 客户端ip
         */
        virtual void didDisplayCachedPhoto(const std::string &photoId, const std::string &clientIp) = 0;
        
        /**
         * 开始播放视频
         *  @contentLocation    [in] 视频位置（单位为百分比: 当前位置/总长度）
         *  @startPosition      [in] 从startPostion所指的时间点开始播放
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStartPlayVideo(const std::string &contentLocation, double startPosition, const std::string &clientIp) = 0;
        
        /**
         * 开始播放音频
         *  @contentLocation    [in] 音频位置，如：http://10.0.1.2:7002/123.mp3
         *  @startPosition      [in] 从startPostion所指的时间点开始播放（单位为百分比: 当前位置/总长度）
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStartPlayMusic(const std::string &contentLocation, double startPosition, const std::string &clientIp) = 0;

        /**
         * 设置回放速度
         *  @rate: 
         *     0 - 暂停
         *     1 - 正常速率播放
         *  @clientIp           [in] 客户端ip
         */
        virtual void didSetPlaybackRate(float rate, const std::string &clientIp) = 0;

        /**
         * 停止回放或停止显示照片
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStop(const std::string &clientIp) = 0;
        
        /**
         * 设置当前播放进度
         *  @playPostion        [in] 当前播放进度（单位：秒）
         *  @clientIp           [in] 客户端ip
         */
        virtual void setCurrentPlaybackProgress(float playPosition, const std::string &clientIp) = 0;

        /**
         * 获取当前播放进度（如果还不能读取视频长度，则填0处理）
         *  @time               [out] position : 当前播放点（单位：秒）
         *                      [out] duration : 视频总长度（单位：秒）
         *  @clientIp           [in] 客户端ip
         */
        virtual void getCurrentPlaybackProgress(PlaybackTime &time, const std::string &clientIp) = 0;

        /**
         * 获取回放信息
         *  @info               [out] 回放信息
         *  @clientIp           [in] 客户端ip
         */
        virtual void getPlaybackInfo(PlaybackInfo &info, const std::string &clientIp) = 0;

        /**
         * 设置音量（百分比: 当前音量/总音量）
         *  @value              [in] 百分比: 当前音量/总音量）
         *  @clientIp           [in] 客户端ip
         */
        virtual void didSetVolume(float value, const std::string &clientIp) = 0;

        /**
         * 开始幻灯片播放
         *  @slideDuration      [in] 每张图片停留的时间
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStartSlideshows(int slideDuration, const std::string &clientIp) = 0;
        
        /**
         * 停止幻灯片播放
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStopSlideshows(const std::string &clientIp) = 0;
        
        /**
         * 获取到一张幻灯片播放的图片
         *  @index              [in] 照片索引，第1张，第2张，第3张...
         *  @data               [in] 照片原始数据(jpg格式）
         *  @length             [in] 数据大小
         *  @clientIp           [in] 客户端ip
         *  @note
         *      1. 调用AirPlayServer::getSlideshowsPicture(clientIp)才能收到此消息
         */
        virtual void didGetSlideshowsPicture(int index, const char *data, std::size_t length, const std::string &clientIp) = 0;
        
        /**
         * 获取幻灯片图片失败
         *  @clientIp           [in] 客户端ip
         *  @note
         *      1. 调用AirPlayServer::getSlideshowsPicture(clientIp)才能收到此消息
         *      2. 收到这个消息，表示幻灯片的照片已经播放完毕
         */
        virtual void didGetSlideshowsPictureFailed(const std::string &clientIp) = 0;

    };

}

#endif // __AIRPLAY_SERVER_LISTENER_H__