/*
* AirPlayServer.hpp
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

#ifndef __AIRPLAY_SERVER_H__
#define __AIRPLAY_SERVER_H__

#include <string>
#include "AirJoyApi.hpp"
#include "AirPlayServiceInfo.hpp"

namespace airjoy
{

    class AirPlayServerCore;
    class AirPlayServiceInfo;
    class AirPlayServerListener;

    class AIRJOY_API AirPlayServer
    {
    public:

        // �¼�����
        typedef enum _EventCategory
        {
            EventUnknown     = -1,
            EventPhoto       = 0,    // ͼƬ
            EventVideo       = 1,    // ��Ƶ
        } EventCategory;

        // �¼���Ϊ
        typedef enum _EventAction
        {
            EventActionNone      = -1,
            EventActionPlaying   = 0,    // ����
            EventActionPaused    = 1,    // ��ͣ
            EventActionLoading   = 2,    // ���ڼ���
            EventActionStopped   = 3,    // ֹͣ����/ֹͣ��ʾ
        } EventAction;

    public:
        AirPlayServer();
        virtual ~AirPlayServer();

        /**
         * ���������
         *  @serviceInfo    [in] AirPlay�������Ϣ
         *  @listener       [in] ������
         */  
        void start(AirPlayServiceInfo &serviceInfo, AirPlayServerListener *listener);

        /**
         * ֹͣ�����
         */
        void stop(void);

        /**
         * ��ȡ����˶˿�
         *  @return
         *      AirPlay����˶˿ں�
         */
        uint16_t port(void);

        /**
         * ����ý���¼�
         *  @type       [in] ����
         *  @action     [in] ����
         *  @clientIp   [in] �ͻ���IP
         */
        void publishEvent(EventCategory type, EventAction action, const std::string &clientIp);

        /**
         * �ر���ͻ��˵�����
         *  @clientIp       [in] �ͻ���ip
         */ 
        void closeConnection(const std::string &clientIp);
        
        /**
         * ��ͻ��˷������󣬻�ȡһ�Żõ�Ƭ�õ�ͼƬ
         *  @clientIp       [in] �ͻ���ip
         */
        void getSlideshowsPicture(const std::string &clientIp);

    private:
        AirPlayServerCore         * m_core;
        bool                        m_isStart;
    };

}

#endif // __AIRPLAY_SERVER_H__

