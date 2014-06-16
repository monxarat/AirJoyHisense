/*
* AirPlayClientListener.h
*
* coding.tom@gmail.com
* 2011-8-30
*
*/

#ifndef __AIRPLAY_CLIENT_LISTENER_H__
#define __AIRPLAY_CLIENT_LISTENER_H__

#include "AnyPlayTypeDef.hpp"
#include "AnyPlayApi.hpp"
#include "AirPlayServiceInfo.hpp"

namespace anyplay
{

    class ANYPLAY_API AirPlayClientListener
    {
    public:
        AirPlayClientListener() {}
        virtual ~AirPlayClientListener() {}

        /**
         * ���ӳɹ�
         */
        virtual void didConnectToServerOk(void) = 0;

        /**
         * ����ʧ��
         */
        virtual void didConnectToServerFailed(void) = 0;

        /**
         * �Ͽ�����
         */
        virtual void didDisconnected(void) = 0;

        /**
         * ��ȡ�������Ϣ
         */
        virtual void didGetServerInfo(AirPlayServiceInfo &info) = 0;

        /**
         * ��ȡ����˻õ�Ƭ����
         * δʵ��
         */
        virtual void didGetSlideshowFeatures() {}

        /**
         * ���ͷ���HTTP����
         * δʵ��
         */
        virtual void didPostReverse() {}

        /**
         * ��ȡ���Ž���
         */
        virtual void didGetPlayVideoProgress() = 0;

        /**
         * ��ȡ������Ϣ
         */
        virtual void didGetPlayVideoInfo(float startPosition, float duration) = 0;

        /**
         * ��ȡ��������
         * δʵ��
         */
        virtual void didGetProperty() {}
    };

}

#endif // __AIRPLAY_CLIENT_LISTENER_H__