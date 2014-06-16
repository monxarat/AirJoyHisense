/*
* AirPlayClient.hpp
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
*/

#ifndef __AIR_PLAY_CLIENT_H__
#define __AIR_PLAY_CLIENT_H__

#include <string>
#include "AnyPlayTypeDef.hpp"
#include "AnyPlayApi.hpp"

#include "AirPlayClientPhoto.hpp"
#include "AirPlayClientVideo.hpp"

namespace anyplay
{

    class AirPlayClientListener;
    class AnyPlayCore;

    class ANYPLAY_API AirPlayClient
    {
    public:
        typedef enum _AirPlayClientRequestType
        {
            RequestTypeUnknown                 = 0,
            RequestTypeGetServerInfo           = 1,
            RequestTypePostReverse             = 2,
            RequestTypeGetSlideshowFeatures    = 3,
            RequestTypePutPhoto                = 5,
            RequestTypePutSlideshows           = 6,
            RequestTypePostStop                = 7,
            RequestTypePostPlay                = 8,
            RequestTypePostScrub               = 9,
            RequestTypePostRate                = 10,
            RequestTypeGetScrub                = 11,
            RequestTypeGetPlaybackInfo         = 12,
            RequestTypePutSetProperty          = 13,
            RequestTypePutGetProperty          = 14,
        } AirPlayClientRequestType;

    private:
        AnyPlayCore                 * m_core;
        AirPlayClientPhoto          * m_photo;
        AirPlayClientVideo          * m_video;
        AirPlayClientRequestType      m_requestType;
        bool                          m_isStart;
        bool                          m_isConnected;

    public:
        AirPlayClientRequestType requestType(void);
        void setRequestType(AirPlayClientRequestType t);
        void resetRequestType(void);

    public:
        AirPlayClient(AnyPlayCore * core);
        virtual ~AirPlayClient();

        /**
         * ����AirPlay�ͻ���
         *  @listener       [in] ������
         */
        void start(AirPlayClientListener *listener);

        /**
         * ֹͣ�ͻ���
         */
        void stop(void);

        /**
         * ���ӵ�AirPlayServer
         *  @ip             [in] AirPlay����˵�ַ
         *  @port           [in] AirPlay����˿�
         */
        void connectToServer(const std::string &ip, uint16_t port);

        /**
         * �Ͽ�����
         */
        void disconnect(void);

        /**
         * ������Ϣ���Ѿ����ӵķ����
         *  @message    [in] ׼�����͵���Ϣ
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool send(const std::string &message);

        /**
         * ��ȡ�������Ϣ
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool getServerInfo(void);

        /**
         * ��ȡ����˻õ�Ƭ����
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool getSlideshowFeatures(void);

        /**
         * ���ͷ���HTTP����
         *  @return
         *      true  - ���ͳɹ�
         *      false - ����ʧ��
         */
        bool postReverse(void);

        /**
         * ��Ƭ����ģ��
         */
        AirPlayClientPhoto * photo(void);

        /**
         * ��Ƶ����ģ��
         */
        AirPlayClientVideo * video(void);
    };

}

#endif // __AIR_PLAY_CLIENT_H__