
#ifndef __AIRPLAYJNILISTENER_H__
#define __AIRPLAYJNILISTENER_H__

#include "AirPlayServer.hpp"
#include "AirPlayServerListener.hpp"
#include "AirPlayNsd.hpp"


using namespace airjoy;

class AirplayJniListener:public AirPlayServerListener 
{

    public:

        /**
         * 订阅事件
         *  @clientIp：        客户端ip
         */
        virtual void didSubscribeEvent(const std::string &clientIp);

        /**
         * 取消订阅事件
         *  @clientIp：        客户端ip
         */
        virtual void didUnsubscribeEvent(const std::string &clientIp);

        /**
         * 即将接收照片
         *  @photoId：         照片id
         */
        virtual void willPutPhoto(const std::string &photoId, const std::string &clientIp) ;

        /*
         * 接收照片（需要马上显示）
         *  @photoId:            照片id
         *  @sessionId:          会话id
         *  @data:               照片原始数据(jpg格式）
         *  @length:             数据大小
         *  @note:
         *       如果接收方主动关闭照片，则需要发布事件: AirPlayServer::publishEvent();
         */
        virtual void didPutPhoto(const std::string &photoId, const char *data, std::size_t length, const std::string &clientIp) ;
        
        /**
         * 即将缓存照片
         *  @photoId：         照片id
         */
        virtual void willPutPhotoCacheOnly(const std::string &photoId, const std::string &clientIp) ;

        /**
         * 接收照片并缓存（不马上显示，缓存起来）
         *  @photoId:            照片id
         *  @sessionId:          会话id
         *  @data:               照片原始数据(jpg格式）
         *  @length:             数据大小
         */
        virtual void didPutPhotoCacheOnly(const std::string &photoId,  const char *data, std::size_t length, const std::string &clientIp) ;

        /**
         * 显示已经缓存的照片
         *  @photoId:            照片id
         */
        virtual void didDisplayCachedPhoto(const std::string &photoId,  const std::string &clientIp) ;
        
        /**
         * 停止显示图片或停止幻灯片显示
         */
       // virtual void didStopPhotoOrSlideshow(void)  ;

        /**
         * 开始播放视频
         *  @startPosition:   从startPostion所指的时间点开始播放
         *  @contentLocation: 视频位置（百分比: 当前位置/总长度）
         *     1: http://192.168.0.116:7001/1/1a59ea26-1861-5206-aa61-ce3960ae6cb1.mp4
         *     2: http://v.youku.com/player/getRealM3U8/vid/XNDA3MjEyMTMy/type/flv/ts/1340956945/video.m3u8
         *  @sessionId：      会话ID
         */
        virtual void didStartPlayVideo(const std::string &contentLocation, double startPosition,  const std::string &clientIp) ;

        /**
         * 开始播放音频
         *  @contentLocation    [in] 音频位置，如：http://10.0.1.2:7002/123.mp3
         *  @startPosition      [in] 从startPostion所指的时间点开始播放（单位为百分比: 当前位置/总长度）
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStartPlayMusic(const std::string &contentLocation, double startPosition, const std::string &clientIp) ;


        /**
         * 设置视频回放速度
         *  @rate: 
         *     0 - 暂停
         *     1 - 正常速率播放
         */
        virtual void didSetPlaybackRate(float rate, const std::string &clientIp) ;

        /**
         * 停止回放
         */
        //virtual void didStopPlayback(void, const std::string &clientIp) ;

        /**
         * 停止回放或停止显示照片
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStop(const std::string &clientIp);

        
        /**
         * 设置当前播放进度
         *  @playPostion        当前播放进度（单位：秒）
         */
        virtual void setCurrentPlaybackProgress(float playPosition, const std::string &clientIp) ;

        /**
         * 获取当前播放进度（如果还不能读取视频长度，则填0处理）
         *  @time       [out] position : 当前播放点（单位：秒）
         *              [out] duration : 视频总长度（单位：秒）
         */
        virtual void getCurrentPlaybackProgress(PlaybackTime &time, const std::string &clientIp) ;

        /**
         * 获取回放信息
         *  @info       [out] 回放信息
         */
        virtual void getPlaybackInfo(PlaybackInfo &info, const std::string &clientIp) ;

        /**
         * 设置音量（百分比: 当前音量/总音量）
         *  @value      [in] 百分比: 当前音量/总音量）
         */
        virtual void didSetVolume(float value, const std::string &clientIp) ;





        /**
         * 开始幻灯片播放
         *  @slideDuration      [in] 每张图片停留的时间
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStartSlideshows(int slideDuration, const std::string &clientIp);
        
        /**
         * 停止幻灯片播放
         *  @clientIp           [in] 客户端ip
         */
        virtual void didStopSlideshows(const std::string &clientIp);
        
        /**
         * 获取到一张幻灯片播放的图片
         *  @index              [in] 照片索引，第1张，第2张，第3张...
         *  @data               [in] 照片原始数据(jpg格式）
         *  @length             [in] 数据大小
         *  @clientIp           [in] 客户端ip
         *  @note
         *      1. 调用AirPlayServer::getSlideshowsPicture(clientIp)才能收到此消息
         */
        virtual void didGetSlideshowsPicture(int index, const char *data, std::size_t length, const std::string &clientIp);
        
        /**
         * 获取幻灯片图片失败
         *  @clientIp           [in] 客户端ip
         *  @note
         *      1. 调用AirPlayServer::getSlideshowsPicture(clientIp)才能收到此消息
         *      2. 收到这个消息，表示幻灯片的照片已经播放完毕
         */
        virtual void didGetSlideshowsPictureFailed(const std::string &clientIp);

};


#endif
