/*
* AirPlayNsdDiscoveryListener.hpp
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

#ifndef __AIRPLAY_NSD_DISCOVERY_LISTENER_H__
#define __AIRPLAY_NSD_DISCOVERY_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

#include "AirPlayServiceInfo.hpp"

namespace airjoy
{

    /**
     * AirPlay��������ּ�����
     */

    class AIRJOY_API AirPlayNsdDiscoveryListener
    {
    public:
        AirPlayNsdDiscoveryListener() {}
        virtual ~AirPlayNsdDiscoveryListener() {}
        
        /**
         * �����Զ����ַ��񣺳ɹ�
         *  @serviceType    [in] ��������
         */
        virtual void onDiscoveryStarted(const std::string serviceType) = 0;

        /**
         * �����Զ����ַ���ʧ��
         *  @serviceType    [in] ��������
         *  @errorCode      [in] �������
         */
        virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode) = 0;
                
        /**
         * ֹͣ�Զ����ַ��񣺳ɹ�
         *  @serviceType    [in] ��������
         */
        virtual void onDiscoveryStopped(const std::string serviceType) = 0;
  
        /**
         * ֹͣ�Զ����ַ���ʧ��
         *  @serviceType    [in] ��������
         *  @errorCode      [in] �������
         */
        virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode) = 0;

        /**
         * ĳ�������Ѿ�����
         *  @serviceInfo    [in] ������Ϣ
         */
        virtual void onServiceLost(const std::string serviceType, 
                                   const std::string serviceName, 
                                   const std::string serviceIp,
                                   uint16_t servicePort) = 0;
        
        /**
         * ĳ�������Ѿ�����
         *  @serviceInfo    [in] ������Ϣ
         */
        virtual void onServiceFound(const std::string serviceType,
                                    const std::string serviceName,
                                    const std::string serviceIp,
                                    uint16_t servicePort) = 0;

    };

}



#endif // __AIRPLAY_NSD_DISCOVERY_LISTENER_H__
