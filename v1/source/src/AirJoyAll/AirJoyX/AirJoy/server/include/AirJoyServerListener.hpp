/* 
* AirJoyServerListener.h
*
* coding.tom@gmail.com
* 2011-8-30
*
*/

#ifndef __AIRJOY_SERVER_LISTENER_H__
#define __AIRJOY_SERVER_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AIRJOY_API AirJoyServerListener
    {
    public:
        // 回放信息
        typedef struct _PlayInfo
        {
            std::string     id;         // 视频/音频ID
            std::string     url;        // 地址
            std::string     name;       // 名称（可选）
            float           rate;       // 0:暂停, 1:播放
            float           position;   // 当前播放/暂停时间点，单位为秒。
        } PlayInfo;

    public:
        AirJoyServerListener() {}
        virtual ~AirJoyServerListener() {}

    public:
        //------------------------------------------------------------------------------------
        // 如果支持某类功能，需要返回：true
        //------------------------------------------------------------------------------------
        virtual bool isSupportPhoto             (void)  { return false; }
        virtual bool isSupportVideo             (void)  { return false; }
        virtual bool isSupportAudio             (void)  { return false; }
        virtual bool isSupportVolume            (void)  { return false; }

        //------------------------------------------------------------------------------------
        // 照片类接口
        //------------------------------------------------------------------------------------

        /**
         * 缓存照片流
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @url            [in] 照片流地址
         *  @photoId        [in] 标识照片的唯一字符串
         *  @name           [in] 照片名称(可选)
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode cachePhoto           (const std::string &fromDeviceId,
                                                 const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 显示照片流
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @url            [in] 照片流地址
         *  @photoId        [in] 标识照片的唯一字符串
         *  @name           [in] 照片名称(可选)
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode displayPhoto         (const std::string &fromDeviceId,
                                                 const std::string &photoId,
                                                 const std::string &url,
                                                 const std::string &name)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 显示已经缓存的照片流
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @photoId        [in] 标识照片的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode displayCachedPhoto   (const std::string &fromDeviceId,
                                                 const std::string &photoId)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 停止显示照片
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPhoto            (const std::string &fromDeviceId)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 旋转照片
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @photoId        [in] 标识照片的唯一字符串
         *  @direction      [in] 旋转方向
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode rotatePhoto          (const std::string &fromDeviceId,
                                                 const std::string &photoId,
                                                 PhotoDirectionType direction)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 旋转照片
         *  @fromDeviceId   [in] 发送方的设备ID
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
        virtual ResultCode zoomPhoto            (const std::string &fromDeviceId,
                                                 const std::string &photoId,
                                                 float rate)
        {
            return ResultCodeErrorNotSupport; 
        }

        //------------------------------------------------------------------------------------
        // 视频类接口
        //------------------------------------------------------------------------------------

        /**
         * 播放视频流
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @url            [in] 视频流地址
         *  @videoId        [in] 标识视频的唯一字符串
         *  @name           [in] 视频名称(可选)
         *  @position       [in] 播放时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode playVideo            (const std::string &fromDeviceId,
                                                 const std::string &videoId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 设置回放速度
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @url            [in] 视频流地址
         *  @videoId        [in] 标识视频的唯一字符串
         *  @rate           [in] 0: 暂停，1: 正常速率播放
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode setPlayVideoRate     (const std::string &fromDeviceId,
                                                 const std::string &videoId,
                                                 float rate)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * 停止播放视频
         *  @toDeviceId     [in] 接收方的设备ID
         *  @videoId        [in] 标识视频的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPlayVideo        (const std::string &fromDeviceId,
                                                 const std::string &videoId)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * 设置播放进度
         *  @toDeviceId     [in] 接收方的设备ID
         *  @videoId        [in] 标识视频的唯一字符串
         *  @playPosition   [in] 当前播放视频的时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode setPlayVideoProgress  (const std::string &fromDeviceId,
                                                  const std::string &videoId,
                                                  float playPosition)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * 获取播放进度
         *  @toDeviceId     [in] 接收方的设备ID
         *  @videoId        [in] 标识视频的唯一字符串
         *  @rate           [out] 0:暂停，1:播放
         *  @position       [out] 当前播放/暂停时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode getPlayVideoProgress  (const std::string &fromDeviceId,
                                                  const std::string &videoId,
                                                  float *rate,
                                                  float *position)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * 获取播放信息
         *  @toDeviceId     [in] 接收方的设备ID
         *  @info           [out] 视频信息
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode getPlayVideoInfo       (const std::string &toDeviceId,
                                                   PlayInfo *info)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // 音频类接口
        //------------------------------------------------------------------------------------

        /**
         * 播放音频流
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @url            [in] 音频流地址
         *  @videoId        [in] 标识音频的唯一字符串
         *  @name           [in] 视频名称(可选)
         *  @position       [in] 播放时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode playAudio            (const std::string &fromDeviceId,
                                                 const std::string &audioId,
                                                 const std::string &url,
                                                 const std::string &name,
                                                 float position)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 设置回放速度
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @url            [in] 音频流地址
         *  @videoId        [in] 标识视频的唯一字符串
         *  @rate           [in] 0: 暂停，1: 正常速率播放
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode setPlayAudioRate     (const std::string &fromDeviceId,
                                                 const std::string &audioId,
                                                 float rate)
        { 
            return ResultCodeErrorNotSupport; 
        }

        /**
         * 停止播放音频
         *  @toDeviceId     [in] 接收方的设备ID
         *  @videoId        [in] 标识音频的唯一字符串
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode stopPlayAudio        (const std::string &fromDeviceId,
                                                 const std::string &audioId)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * 设置播放进度
         *  @toDeviceId     [in] 接收方的设备ID
         *  @videoId        [in] 标识音频的唯一字符串
         *  @playPosition   [in] 当前播放音频的时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode setPlayAudioProgress  (const std::string &fromDeviceId,
                                                  const std::string &audioId,
                                                  float playPosition)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * 获取播放进度
         *  @toDeviceId     [in] 接收方的设备ID
         *  @videoId        [in] 标识音频的唯一字符串
         *  @rate           [out] 0:暂停，1:播放
         *  @position       [out] 当前播放/暂停时间点，单位为秒
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode getPlayAudioProgress  (const std::string &fromDeviceId,
                                                  const std::string &audioId,
                                                  float *rate,
                                                  float *position)
        {
            return ResultCodeErrorNotSupport;
        }

        /**
         * 获取播放信息
         *  @toDeviceId     [in] 接收方的设备ID
         *  @info           [out] 音频信息
         *  @return
         *     ResultCodeOk - 成功
         *     ResultCodeErrorNotSupport - 不支持此功能
         */
        virtual ResultCode getPlayAudioInfo       (const std::string &fromDeviceId,
                                                   PlayInfo *info)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // 系统类接口
        //------------------------------------------------------------------------------------

        /**
         * 设置音量
         *  @toDeviceId     [in] 接收方的设备ID
         *  @volume         [in] 当前音量(1-100)
         *  @return 会话ID
         */
        virtual ResultCode setVolume               (const std::string &fromDeviceId,
                                                    int volume)
        {
            return ResultCodeErrorNotSupport;
        }


        /**
         * 获取音量
         *  @toDeviceId     [in] 接收方的设备ID
         *  @volume         [out] 当前音量(1-100)
         *  @return 会话ID
         */
        virtual ResultCode getVolume               (const std::string &fromDeviceId,
                                                    int * volume)
        {
            return ResultCodeErrorNotSupport;
        }

        //------------------------------------------------------------------------------------
        // 扩展消息接口
        //------------------------------------------------------------------------------------     
        /**
         * 处理扩展消息
         *  @fromDeviceId   [in] 发送方的设备ID
         *  @appXmlns       [in] 名字空间
         *  @appParam       [in] 参数
         *  @appResult      [out] 结果
         *  @return
         *     true  - 成功处理此消息
         *     false - 无法处理此消息
         */
        virtual bool handleMessage              (const std::string &fromDeviceId,
                                                 const std::string &appXmlns,
                                                 const std::string &appParam,
                                                 std::string &appResult)
        { 
            return false; 
        }
    };

}

#endif // __AIRJOY_SERVER_LISTENER_H__