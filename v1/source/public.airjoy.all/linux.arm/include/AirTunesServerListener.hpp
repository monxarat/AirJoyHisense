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
         * ��ʼ������Ƶ
         */
        virtual void didStartPlayAudio(const std::string &clientIp) = 0;

        /*
         * ��ͣ������Ƶ
         */
        virtual void didPause(const std::string &clientIp) = 0;
        
        /*
         * ֹͣ������Ƶ
         */
        virtual void didStop(const std::string &clientIp) = 0;

        /*
         * ��������
         *  @value              [in] ����ֵ
         *  @note
         *    ����ֵ��16����0Ϊ���������-144Ϊ������������ɻ�û�����
         *  ---------------------------------------
         *          ����ֵ     ��������
         *  ---------------------------------------
         *           0          [16] �����������
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
         *          -144        ����
         */
        virtual void didSetVolume(float value, const std::string &clientIp) = 0;

        /*
         * ���÷���ͼƬ
         *  @data               [in] ͼƬ���ݣ�jpeg��ʽ��
         *  @length             [in] ͼƬ���ݳ���
         */        
        virtual void didSetImage(const char *data, std::size_t length, const std::string &clientIp) = 0;

        /*
         * ������Ƶ��Ϣ
         *  @name               [in] ������
         *  @artist             [in] ����
         *  @album              [in] ר����
         */
        virtual void didSetAudioInfo(const std::string &name, const std::string &artist, const std::string &album, const std::string &clientIp) = 0;

    #ifndef __USE_LIB_AO__
        /*
         * ��ʼ����Ƶ�����Ϣ
         *  @sampleRateInHz         [in] ��Ƶ���ʣ�һ��Ϊ44100
         *  @channelConfig          [in] ��ƵƵ����1=��������2=������
         *  @audioFormat            [in] ��Ƶ��ʽ��16λ��32λ
         *  @fmtp                   [in] fmtpֵ��osxϵͳ��Ҫ�õ�
         *  @fmtpLen                [in] fmtp���ȣ�һ��Ϊ12
         *  @note
         *    Java���루��ʼ��AudioTrack��
		      AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 
                                                sampleRateInHz,                         // 44100����
                                                AudioFormat.CHANNEL_OUT_STEREO,         // 2: ������
		                                        AudioFormat.ENCODING_PCM_16BIT,         // 16λ
		                                        sampleRateInHz * 2 * 4,
		                                        AudioTrack.MODE_STREAM);
         */
        virtual void didAudioDriverInit(int sampleRateInHz, int channelConfig, int audioFormat, int fmtp[12], size_t fmtpLen) = 0;

        /*
         * �����Ƶ���ݣ�PCM��ʽ
         *  @outbuf         [in] ��Ƶ����
         *  @size           [in] ��Ƶ���ݳ���
         *  @note
         *    Java���루�����Ƶ��
         *    track.write(outbuf, 0, size);
         */
        virtual void didAudioDriverPlay(char *outbuf, int size) = 0;
    #endif // __USE_LIB_AO__
    };

}

#endif // __AIRTUNES_SERVER_LISTENER_H__
