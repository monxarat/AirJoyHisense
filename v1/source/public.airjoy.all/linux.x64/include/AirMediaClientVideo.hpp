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
         * ������Ƶ��
         *  @videoUrl       [in] ������Ƶ����ַ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ������������MD5����UUID
         *  @videoNameUtf8  [in] ��Ƶ����(��ѡ)
         *  @startPositon   [in] ��ʼ������Ƶ��ʱ��㣬��λΪ��
         *  @return �ỰID
         */
        AirJoySessionId    playVideo                      (const std::string &videoUrl,
                                                           const std::string &videoId,
                                                           const std::string &videoNameUtf8 = "",
                                                           float startPosition = 0.0);

        /**
         * ���ò�������
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @return �ỰID
         */
        AirJoySessionId    setPlayVideoRate               (const std::string &videoId,
                                                           float rate);

        /**
         * ֹͣ������Ƶ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return �ỰID
         */
        AirJoySessionId    stopPlayVideo                  (const std::string &videoId);

        /**
         * ���ò��Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return �ỰID
         */
        AirJoySessionId    setPlayVideoProgress           (const std::string &videoId,
                                                           float playPosition);

        /**
         * ��ȡ���Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return �ỰID
         */
        AirJoySessionId    getPlayVideoProgress           (const std::string &videoId);

        /**
         * ��ȡ������Ϣ
         *  @return �ỰID
         */
        AirJoySessionId    getPlayVideoInfo               (void);

    private:
        AirMediaClient  *m_sender;
    };

}


#endif // __AIRMEDIA_CLIENT_VIDEO_H__
