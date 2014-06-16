/*
* AirMediaListener.hpp
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
*/

#ifndef __AIR_MEDIA_LISTENER_H__
#define __AIR_MEDIA_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AIRJOY_API AirMediaListener
    {
    public:
        AirMediaListener() {}
        virtual ~AirMediaListener() {}

    public:
	
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
        }

	    /**
         * 取消订阅事件
         *  @fromIp         [in] 订阅者IP
         *  @topic          [in] 订阅主题
         */        
        virtual void didUnsubscribeEvent(const std::string &fromIp, const std::string &topic)
        {
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
            return ResultCodeErrorNotSupport; 
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
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 显示已经缓存的照片流
         *  @photoId        [in] 标识照片的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode displayCachedPhoto   (const std::string &photoId,
                                                 const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 停止显示照片
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPhoto            (const std::string &fromIp)
        { 
            return ResultCodeErrorNotSupport; 
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
            return ResultCodeErrorNotSupport; 
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
            return ResultCodeErrorNotSupport; 
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
　       *  @y             [ in] y轴移动比例，有符号浮点数，单位：接收方屏幕高度
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
            return ResultCodeErrorNotSupport; 
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
            return ResultCodeErrorNotSupport; 
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
            return ResultCodeErrorNotSupport;
        }

        /**
         * 停止播放视频
         *  @videoId        [in] 标识视频的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPlayVideo        (const std::string &videoId,
                                                 const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
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
            return ResultCodeErrorNotSupport;
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
            return ResultCodeErrorNotSupport;
        }

        /**
         * 获取播放信息
         *  @info           [out] 视频信息
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode getPlayVideoInfo       (AirJoyPlayBackInfo &info,
                                                   const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
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
            return ResultCodeErrorNotSupport; 
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
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 停止播放音频
         *  @videoId        [in] 标识音频的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPlayAudio        (const std::string &audioId,
                                                 const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
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
            return ResultCodeErrorNotSupport;
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
            return ResultCodeErrorNotSupport;
        }

        /**
         * 获取播放信息
         *  @info           [out] 音频信息
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode getPlayAudioInfo       (AirJoyPlayBackInfo &info,
                                                   const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // 音量接口
        //------------------------------------------------------------------------------------

        /**
         * 设置音量
         *  @volume         [in] 当前音量(1-100)
         *  @return 处理代码
         */
        virtual ResultCode setVolume               (int volume,
                                                    const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }


        /**
         * 获取音量
         *  @volume         [out] 当前音量(1-100)
         *  @return 处理代码
         */
        virtual ResultCode getVolume               (int * volume,
                                                    const std::string &fromIp)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // 自定义消息
        //------------------------------------------------------------------------------------

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
                                                    size_t appResultLen,
                                                    const std::string &fromIp)
        {
            return false;
        }
    };

}

#endif // __AIR_MEDIA_LISTENER_H__

