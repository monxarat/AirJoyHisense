/*
 * AirMediaClientVideo.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __AIRMEDIA_CLIENT_VIDEO_H__
#define __AIRMEDIA_CLIENT_VIDEO_H__


#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AirMediaClient;

    class AIRJOY_API AirMediaClientVideo
    {
    public:
        AirMediaClientVideo(AirMediaClient *sender);
        virtual ~AirMediaClientVideo();

        /**
         * 发送视频流
         *  @videoUrl       [in] 网络视频流地址
         *  @videoId        [in] 标识视频的唯一字符串，建议采用MD5，或UUID
         *  @videoNameUtf8  [in] 视频名称(可选)
         *  @startPositon   [in] 开始播放视频的时间点，单位为秒
         *  @return 会话ID
         */
        AirJoySessionId    playVideo                      (const std::string &videoUrl,
                                                           const std::string &videoId,
                                                           const std::string &videoNameUtf8 = "",
                                                           float startPosition = 0.0);

        /**
         * 设置播放速率
         *  @videoId        [in] 标识视频的唯一字符串
         *  @rate           [in] 0: 暂停，1: 正常速率播放
         *  @return 会话ID
         */
        AirJoySessionId    setPlayVideoRate               (const std::string &videoId,
                                                           float rate);

        /**
         * 停止播放视频
         *  @videoId        [in] 标识视频的唯一字符串
         *  @return 会话ID
         */
        AirJoySessionId    stopPlayVideo                  (const std::string &videoId);

        /**
         * 设置播放进度
         *  @videoId        [in] 标识视频的唯一字符串
         *  @playPosition   [in] 当前播放视频的时间点，单位为秒
         *  @return 会话ID
         */
        AirJoySessionId    setPlayVideoProgress           (const std::string &videoId,
                                                           float playPosition);

        /**
         * 获取播放进度
         *  @videoId        [in] 标识视频的唯一字符串
         *  @return 会话ID
         */
        AirJoySessionId    getPlayVideoProgress           (const std::string &videoId);

        /**
         * 获取播放信息
         *  @return 会话ID
         */
        AirJoySessionId    getPlayVideoInfo               (void);

    private:
        AirMediaClient  *m_sender;
    };

}


#endif // __AIRMEDIA_CLIENT_VIDEO_H__
