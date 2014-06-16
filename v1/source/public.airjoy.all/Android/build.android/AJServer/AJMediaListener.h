
#ifndef __AJMEDIALISTENER_H__
#define __AJMEDIALISTENER_H__

#include <stdio.h>
#include <stdlib.h>

#include "AirMediaListener.hpp"


using namespace airjoy;

class AJMediaListener: public AirMediaListener
{
	    public:
	
        // 会话类接口

	    /**
         * 订阅事件
         *  @fromIp         [in] 订阅者IP
         *  @topic          [in] 订阅主题
         */
        virtual void didSubscribeEvent(const std::string &fromIp, const std::string &topic);

	    /**
         * 取消订阅事件
         *  @fromIp         [in] 订阅者IP
         *  @topic          [in] 订阅主题
         */        
        virtual void didUnsubscribeEvent(const std::string &fromIp, const std::string &topic);

        // 照片类接口

        /**
         * 缓存照片流
         *  @url            [in] 照片流地址
         *  @photoId        [in] 标识照片的唯一字符串
         *  @name           [in] 照片名称(可选);
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode cachePhoto           (const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name,const std::string &fromIp);

        /**
         * 显示照片流
         *  @url            [in] 照片流地址
         *  @photoId        [in] 标识照片的唯一字符串
         *  @name           [in] 照片名称(可选);
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode displayPhoto         (const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name,const std::string &fromIp);
        /**
         * 显示已经缓存的照片流
         *  @photoId        [in] 标识照片的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode displayCachedPhoto   (const std::string &photoId,const std::string &fromIp);

        /**
         * 停止显示照片
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPhoto            (const std::string &fromIp);

        /**
         * 旋转照片
         *  @photoId        [in] 标识照片的唯一字符串
         *  @direction      [in] 旋转方向
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode rotatePhoto          (const std::string &photoId,
                                                 PhotoDirectionType direction,const std::string &fromIp);

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
                                                 float rate,const std::string &fromIp);



        virtual ResultCode movePhoto            (const std::string &photoId,
                                                 float x,
												 float y,const std::string &fromIp);

        // 视频类接口

        /**
         * 播放视频流
         *  @url            [in] 视频流地址
         *  @videoId        [in] 标识视频的唯一字符串
         *  @name           [in] 视频名称(可选);
         *  @position       [in] 播放时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode playVideo            (const std::string &videoId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position,const std::string &fromIp);

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
                                                 float rate,const std::string &fromIp);

        /**
         * 停止播放视频
         *  @videoId        [in] 标识视频的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPlayVideo        (const std::string &videoId,const std::string &fromIp);

        /**
         * 设置播放进度
         *  @videoId        [in] 标识视频的唯一字符串
         *  @playPosition   [in] 当前播放视频的时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode setPlayVideoProgress  (const std::string &videoId,
                                                  float playPosition,const std::string &fromIp);

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
                                                  float *position,const std::string &fromIp);

        /**
         * 获取播放信息
         *  @info           [out] 视频信息
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
//        virtual ResultCode getPlayVideoInfo       (AirJoyPlayBackInfo *info,const std::string &fromIp);

        virtual ResultCode getPlayVideoInfo       (AirJoyPlayBackInfo &info, const std::string &fromIp);
        // 音频类接口

        /**
         * 播放音频流
         *  @url            [in] 音频流地址
         *  @videoId        [in] 标识音频的唯一字符串
         *  @name           [in] 视频名称(可选);
         *  @position       [in] 播放时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode playAudio            (const std::string &audioId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position,const std::string &fromIp);

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
                                                 float rate,const std::string &fromIp);

        /**
         * 停止播放音频
         *  @videoId        [in] 标识音频的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPlayAudio        (const std::string &audioId,const std::string &fromIp);

        /**
         * 设置播放进度
         *  @videoId        [in] 标识音频的唯一字符串
         *  @playPosition   [in] 当前播放音频的时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode setPlayAudioProgress  (const std::string &audioId,
                                                  float playPosition,const std::string &fromIp);

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
                                                  float *position,const std::string &fromIp);

        /**
         * 获取播放信息
         *  @info           [out] 音频信息
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode getPlayAudioInfo       (AirJoyPlayBackInfo *info,const std::string &fromIp);

        // 音量接口

        /**
         * 设置音量
         *  @volume         [in] 当前音量(1-100);
         *  @return 会话ID
         */
        virtual ResultCode setVolume               (int volume,const std::string &fromIp);


        /**
         * 获取音量
         *  @volume         [out] 当前音量(1-100);
         *  @return 会话ID
         */
        virtual ResultCode getVolume               (int * volume,const std::string &fromIp);

};

#endif
