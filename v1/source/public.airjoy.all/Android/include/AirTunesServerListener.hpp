/*
* AirTunesServerListener.hpp
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

#ifndef __AIRTUNES_SERVER_LISTENER_H__
#define __AIRTUNES_SERVER_LISTENER_H__

#include <string>
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AIRJOY_API AirTunesServerListener
    {
    public:
        AirTunesServerListener() {}
        virtual ~AirTunesServerListener() {}

        /*
         * 开始播放音频
         */
        virtual void didStartPlayAudio(const std::string &clientIp) = 0;

        /*
         * 暂停播放音频
         */
        virtual void didPause(const std::string &clientIp) = 0;
        
        /*
         * 停止播放音频
         */
        virtual void didStop(const std::string &clientIp) = 0;

        /*
         * 设置音量
         *  @value              [in] 音量值
         *  @note
         *    音量值分16级，0为最大音量，-144为静音，具体规律还没搞清楚
         *  ---------------------------------------
         *          音量值     音量级别
         *  ---------------------------------------
         *           0          [16] （最大音量）
         *          -1.875      [15]
         *          -3.75       [14]
         *          -5.625      [13]
         *          -7.5        [12]
         *          -9.375      [11]
         *          -11.25      [10]
         *          -13.12      [9]
         *          -15         [8]
         *          -16.87      [7]
         *          -18.75      [6]
         *          -20.62      [5]
         *          -22.5       [4]
         *          -24.37      [3]
         *          -26.25      [2]
         *          -28.12      [1]
         *          -144        静音
         */
        virtual void didSetVolume(float value, const std::string &clientIp) = 0;

        /*
         * 设置封面图片
         *  @data               [in] 图片数据（jpeg格式）
         *  @length             [in] 图片数据长度
         */        
        virtual void didSetImage(const char *data, std::size_t length, const std::string &clientIp) = 0;

        /*
         * 设置音频信息
         *  @name               [in] 歌曲名
         *  @artist             [in] 作者
         *  @album              [in] 专辑名
         */
        virtual void didSetAudioInfo(const std::string &name, const std::string &artist, const std::string &album, const std::string &clientIp) = 0;

    #ifndef __USE_LIB_AO__
        /*
         * 初始化音频输出信息
         *  @sampleRateInHz         [in] 音频码率，一般为44100
         *  @channelConfig          [in] 音频频道，1=单声道，2=立体声
         *  @audioFormat            [in] 音频格式：16位，32位
         *  @fmtp                   [in] fmtp值，osx系统需要用到
         *  @fmtpLen                [in] fmtp长度，一般为12
         *  @note
         *    Java代码（初始化AudioTrack）
		      AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 
                                                sampleRateInHz,                         // 44100赫兹
                                                AudioFormat.CHANNEL_OUT_STEREO,         // 2: 立体声
		                                        AudioFormat.ENCODING_PCM_16BIT,         // 16位
		                                        sampleRateInHz * 2 * 4,
		                                        AudioTrack.MODE_STREAM);
         */
        virtual void didAudioDriverInit(int sampleRateInHz, int channelConfig, int audioFormat, int fmtp[12], size_t fmtpLen) = 0;

        /*
         * 输出音频数据，PCM格式
         *  @outbuf         [in] 音频数据
         *  @size           [in] 音频数据长度
         *  @note
         *    Java代码（输出音频）
         *    track.write(outbuf, 0, size);
         */
        virtual void didAudioDriverPlay(char *outbuf, int size) = 0;
    #endif // __USE_LIB_AO__
    };

}

#endif // __AIRTUNES_SERVER_LISTENER_H__
