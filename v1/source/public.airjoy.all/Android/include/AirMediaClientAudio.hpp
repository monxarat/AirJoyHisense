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
         * ������Ƶ��
         *  @audioUrl       [in] ������Ƶ����ַ
         *  @audioId        [in] ��ʶ��Ƶ��Ψһ�ַ������������MD5����UUID
         *  @audioNameUtf8  [in] ��Ƶ����(��ѡ)
         *  @startPositon   [in] ��ʼ������Ƶ��ʱ��㣬��λΪ��
         *  @return �ỰID
         */
        AirJoySessionId    playAudio                      (const std::string &audioUrl,
                                                           const std::string &audioId,
                                                           const std::string &audioNameUtf8 = "",
                                                           float startPosition = 0.0);

        /**
         * ���ò�������
         *  @audioId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @return �ỰID
         */
        AirJoySessionId    setPlayAudioRate               (const std::string &audioId,
                                                           float rate);

        /**
         * ֹͣ������Ƶ
         *  @audioId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return �ỰID
         */
        AirJoySessionId    stopPlayAudio                  (const std::string &audioId);

        /**
         * ���ò��Ž���
         *  @audioId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return �ỰID
         */
        AirJoySessionId    setPlayAudioProgress           (const std::string &audioId,
                                                           float playPosition);

        /**
         * ��ȡ���Ž���
         *  @audioId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return �ỰID
         */
        AirJoySessionId    getPlayAudioProgress           (const std::string &audioId);

        /**
         * ��ȡ������Ϣ
         *  @return �ỰID
         */
        AirJoySessionId    getPlayAudioInfo               (void);

    private:
        AirMediaClient  *m_sender;
    };

}

#endif // __AIRMEDIA_CLIENT_AUDIO_H__
