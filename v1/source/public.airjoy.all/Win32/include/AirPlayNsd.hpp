/*
 * AirPlayNsd.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#ifndef __AIRPLAY_NSD_H__
#define __AIRPLAY_NSD_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

#include "AirPlayServiceInfo.hpp"
#include "AirTunesServiceInfo.hpp"

#include "AirPlayNsdDiscoveryListener.hpp"
#include "AirPlayNsdPublishListener.hpp"

namespace airjoy
{

    class AirPlayNsdCore;

    class AIRJOY_API AirPlayNsd
    {
    public:
        AirPlayNsd();
        virtual ~AirPlayNsd();
        
        /**
         * �����Զ����ַ���
		 * @serviceType       [in] ��������
         * @listener          [in] ������
         * @return
         *      true  - �����ɹ�
         *      false - ����ʧ��
         */
        bool discoveryService(const std::string &serviceType, AirPlayNsdDiscoveryListener *listener);
        
        /**
         * ֹͣ�Զ����ַ���
         */    
        bool stopServiceDiscovery(void);
   
        /**
         * ��������
         *  @serviceInfo    [in] AirPlay�������Ϣ
         *  @listener       [in] ������
         *  @return
         *      true  - �����ɹ�
         *      false - ����ʧ��
         */
        bool publishService(AirPlayServiceInfo &serviceInfo, AirPlayNsdPublishListener *listener);
        
		/**
         * ��������
         *  @serviceInfo    [in] AirTunes�������Ϣ
         *  @listener       [in] ������
         *  @return
         *      true  - �����ɹ�
         *      false - ����ʧ��
         */
        bool publishService(AirTunesServiceInfo &serviceInfo, AirPlayNsdPublishListener *listener);

    private:
        AirPlayNsdCore     * m_core;
        bool                 m_isStart;
    };

}

#endif // __AIRPLAY_NSD_H__
