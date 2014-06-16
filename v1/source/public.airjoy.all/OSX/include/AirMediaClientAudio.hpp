/*
 * AirMediaClientAudio.hpp
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

#ifndef __AIRMEDIA_CLIENT_AUDIO_H__
#define __AIRMEDIA_CLIENT_AUDIO_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AirMediaClient;

    class AIRJOY_API AirMediaClientAudio
    {
    public:
        AirMediaClientAudio(AirMediaClient *sender);
        virtual ~AirMediaClientAudio();

        /**
         * 发送音频流
         *  @audioUrl       [in] 网络音频流地址
         *  @audioId        [in] 标识音频的唯一字符串，建议采用MD5，或UUID
         *  @audioNameUtf8  [in] 音频名称(可选)
         *  @startPositon   [in] 开始播放音频的时间点，单位为秒
         *  @return 会话ID
         */
        AirJoySessionId    playAudio                      (const std::string &audioUrl,
                                                           const std::string &audioId,
                                                           const std::string &audioNameUtf8 = "",
                                                           float startPosition = 0.0);

        /**
         * 设置播放速率
         *  @audioId        [in] 标识音频的唯一字符串
         *  @rate           [in] 0: 暂停，1: 正常速率播放
         *  @return 会话ID
         */
        AirJoySessionId    setPlayAudioRate               (const std::string &audioId,
                                                           float rate);

        /**
         * 停止播放音频
         *  @audioId        [in] 标识音频的唯一字符串
         *  @return 会话ID
         */
        AirJoySessionId    stopPlayAudio                  (const std::string &audioId);

        /**
         * 设置播放进度
         *  @audioId        [in] 标识音频的唯一字符串
         *  @playPosition   [in] 当前播放音频的时间点，单位为秒
         *  @return 会话ID
         */
        AirJoySessionId    setPlayAudioProgress           (const std::string &audioId,
                                                           float playPosition);

        /**
         * 获取播放进度
         *  @audioId        [in] 标识音频的唯一字符串
         *  @return 会话ID
         */
        AirJoySessionId    getPlayAudioProgress           (const std::string &audioId);

        /**
         * 获取播放信息
         *  @return 会话ID
         */
        AirJoySessionId    getPlayAudioInfo               (void);

    private:
        AirMediaClient  *m_sender;
    };

}

#endif // __AIRMEDIA_CLIENT_AUDIO_H__
