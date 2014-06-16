
#ifndef __AJDISCOVERYLISTENER_H__
#define __AJDISCOVERYLISTENER_H__


#include "AirJoyNsdDiscoveryListener.hpp"


using namespace airjoy;

class AJDiscoveryListener: public AirJoyNsdDiscoveryListener 
{
	public:
#if 0
		 /**
         * �����������ɹ�
         *  @serviceType    [in] ��������
         */
        virtual void onDiscoveryStarted(const std::string serviceType);
        
        /**
         * ����������ʧ��
         *  @serviceType    [in] ��������
         *  @errorCode      [in] �������
         */
        virtual void onStartDiscoveryFailed(const std::string serviceType, int errorCode);

        /**
         * ֹͣ�������ɹ�
         *  @serviceType    [in] ��������
         */
        virtual void onDiscoveryStopped(const std::string serviceType);

        /**
         * ֹͣ������ʧ��
         *  @serviceType    [in] ��������
         *  @errorCode      [in] �������
         */
        virtual void onStopDiscoveryFailed(const std::string serviceType, int errorCode);
#endif
        
        /**
         * ĳ�������Ѿ�����
         *  @serviceInfo    [in] ������Ϣ
         */
         virtual void onServiceLost(AirJoyServiceInfo &serviceInfo);

        /**
         * ĳ�������Ѿ�����
         *  @serviceInfo    [in] ������Ϣ
         */
        virtual void onServiceFound(AirJoyServiceInfo &serviceInfo);

};

#endif
