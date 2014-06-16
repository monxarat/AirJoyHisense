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
         * ������Ƶ��
         *  @url            [in] ��Ƶ����ַ, �����ػ������磩����AnyPlayURL
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ������������MD5����UUID
         *  @startPositon   [in] ��ʼ������Ƶ��ʱ��㣬��λΪ��
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool playVideo                                     (AnyPlayURL &url,
                                                            const std::string &videoId,
                                                            float startPosition = 0.0);

        /**
         * ���ò�������
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool setPlayVideoRate                              (float rate);

        /**
         * ֹͣ������Ƶ
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool stopPlayVideo                                 (void);

        /**
         * ���ò��Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @playPosition   [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool setPlayVideoProgress                          (float playPosition);
        /**
         * ��ȡ���Ž���
         *  @videoId        [in] ��ʶ��Ƶ��Ψһ�ַ���
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool getPlayVideoProgress                          (void);

        /**
         * ��ȡ������Ϣ
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool getPlayVideoInfo                              (void);

        /**
         * ��ȡ��������
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool getProperty                                   (void);

        /**
         * ���ò�������
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool setProperty                                   (void);

    private:
        AirPlayClient  *m_sender;
    };

}


#endif // __AIRPLAY_CLIENT_VIDEO_H__