/*
 * AirPlayClientVideo.hpp
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

#ifndef __AIRPLAY_CLIENT_VIDEO_H__
#define __AIRPLAY_CLIENT_VIDEO_H__


#include <string>
#include "AnyPlayTypeDef.hpp"
#include "AnyPlayApi.hpp"
#include "AnyPlayURL.hpp"

namespace anyplay
{

    class AirPlayClient;

    class ANYPLAY_API AirPlayClientVideo
    {
    public:
        AirPlayClientVideo(AirPlayClient *sender);
        virtual ~AirPlayClientVideo();

        /**
         * 发送视频流
         *  @url            [in] 视频流地址, （本地或者网络），见AnyPlayURL
         *  @videoId        [in] 标识视频的唯一字符串，建议采用MD5，或UUID
         *  @startPositon   [in] 开始播放视频的时间点，单位为秒
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool playVideo                                     (AnyPlayURL &url,
                                                            const std::string &videoId,
                                                            float startPosition = 0.0);

        /**
         * 设置播放速率
         *  @videoId        [in] 标识视频的唯一字符串
         *  @rate           [in] 0: 暂停，1: 正常速率播放
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool setPlayVideoRate                              (float rate);

        /**
         * 停止播放视频
         *  @videoId        [in] 标识视频的唯一字符串
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool stopPlayVideo                                 (void);

        /**
         * 设置播放进度
         *  @videoId        [in] 标识视频的唯一字符串
         *  @playPosition   [in] 当前播放视频的时间点，单位为秒
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool setPlayVideoProgress                          (float playPosition);
        /**
         * 获取播放进度
         *  @videoId        [in] 标识视频的唯一字符串
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool getPlayVideoProgress                          (void);

        /**
         * 获取播放信息
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool getPlayVideoInfo                              (void);

        /**
         * 获取播放属性
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool getProperty                                   (void);

        /**
         * 设置播放属性
         *  @return
         *      true  - 发送成功
         *      false - 发送失败
         */
        bool setProperty                                   (void);

    private:
        AirPlayClient  *m_sender;
    };

}


#endif // __AIRPLAY_CLIENT_VIDEO_H__