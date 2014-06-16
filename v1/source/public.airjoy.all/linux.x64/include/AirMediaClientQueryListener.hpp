/* 
* AirMediaClientQueryListener.h
*
* coding.tom@gmail.com
* 2011-8-30
*
*/

#ifndef __AIRMEDIA_CLIENT_QUERY_LISTENER_H__
#define __AIRMEDIA_CLIENT_QUERY_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{

    class AIRJOY_API AirMediaClientQueryListener
    {
    public:
        AirMediaClientQueryListener() {}
        virtual ~AirMediaClientQueryListener() {}

        /**
         * �����¼��ɹ�
         */
        virtual void didSubscribeEvent(const std::string &serverIp) = 0;

        /**
         * ȡ�������¼��ɹ�
         */
        virtual void didUnsubscribeEvent(const std::string &serverIp) = 0;

        /**
         * �յ��¼�
         *  @type           [in] ���ͣ�photo/audio/video/init
         *  @action         [in] ��Ϊ��play/pause/stop/init
         *  @id             [in] id for photo/audio/video
         *  @url            [in, option] url for photo/audio/video
         *  @name           [in, option] name for photo/audio/video
         *  @fromIp         [in] ��Դ��Դ��ַ
         */
        virtual void recvEvent(const std::string &type,
                               const std::string &action,
                               const std::string &id,
                               const std::string &url,
                               const std::string &name,
                               const std::string &fromIp) = 0;

        /**
         * ��ʱ
         */
		virtual void didTimeout(const AirJoySessionId sessionId) = 0;
		
		/**
         * �������
         */
		virtual void didErrorNet(const AirJoySessionId sessionId) = 0;

        /**
         * ������
         *  @sessionId     [in] �ỰID
         *  @code          [in] ���ش���
         *      ResultCodeOk����ʾ���շ��Ѿ��յ������Ҵ���ɹ���
         *      ResultCodeErrorNotSupport: �Է���֧�ִ�����
         *      ResultCodeErrorTimeout: ��ʱ��
         */
        virtual void handleResult                  (const AirJoySessionId sessionId,
                                                    ResultCode code) = 0;

        /**
         * ������
         *  @sessionId      [in] �ỰID
         *  @appAction      [in] ��Ϊ
         *  @appResult      [in] ����
         */
        virtual void handleResult                  (const AirJoySessionId sessionId,
                                                    const std::string &appAction,
                                                    const std::string &appResult) = 0;

        /**
         * �����ȡ��Ƶ���Ž��Ƚ��
         *  @sessionId      [in] �ỰID
         *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @position       [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return �ỰID
         */
        virtual void handleGetPlayVideoProgressResult    (const AirJoySessionId sessionId,
                                                          ResultCode code,
                                                          float rate,
                                                          float position) = 0;

        /**
         * �����ȡ��Ƶ������Ϣ���
         *  @sessionId      [in] �ỰID
         *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
         *  @info           [in] �ط���Ϣ���� PlayInfo ����
         *  @return �ỰID
         */
        virtual void handleGetPlayVideoInfoResult        (const AirJoySessionId sessionId,
                                                          ResultCode code,
                                                          AirJoyPlayBackInfo &info) = 0;

        /**
         * �����ȡ��Ƶ���Ž��Ƚ��
         *  @sessionId      [in] �ỰID
         *  @code           [in] ���ش��룬�����ResultCodeOk����ʾrate��positionΪ��Чֵ��
         *  @rate           [in] 0: ��ͣ��1: �������ʲ���
         *  @position       [in] ��ǰ������Ƶ��ʱ��㣬��λΪ��
         *  @return �ỰID
         */
        virtual void handleGetPlayAudioProgressResult    (const AirJoySessionId sessionId,
                                                          ResultCode code,
                                                          float rate,
                                                          float position) = 0;

        /**
         * �����ȡ��Ƶ������Ϣ���
         *  @sessionId      [in] �ỰID
         *  @code           [in] ���ش��룬�����ResultCodeOk����ʾinfoΪ��Чֵ��
         *  @info           [in] �ط���Ϣ���� PlayInfo ����
         *  @return �ỰID
         */
        virtual void handleGetPlayAudioInfoResult        (const AirJoySessionId sessionId,
                                                          ResultCode code,
                                                          AirJoyPlayBackInfo &info) = 0;

        /**
         * �����ȡ�������
         *  @sessionId      [in] �ỰID
         *  @code           [in] ���ش��룬�����ResultCodeOk����ʾvolumeΪ��Чֵ��
         *  @volume         [in] ����ֵ��0 -> 100��
         *  @return �ỰID
         */
        virtual void handleGetVolumeResult               (const AirJoySessionId sessionId,
                                                          ResultCode code,
                                                          int volume) = 0;
    };

}

#endif // __AIRMEDIA_CLIENT_QUERY_LISTENER_H__
