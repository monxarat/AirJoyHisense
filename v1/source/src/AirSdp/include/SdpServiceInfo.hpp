/*
* SdpServiceInfo.hpp
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

#ifndef __SDP_SERVICE_INFO_H__
#define __SDP_SERVICE_INFO_H__

#include <string>
#include "TheBoost.hpp"
#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class SdpServiceInfo
    {
    public:
        SdpServiceInfo()
        {
            m_deviceType = DeviceTypeUnknown;
            m_servicePort = 0;
            m_notifyTimeout = 0;
            m_port = 0;
            m_retry = 0;
            m_nextNotifyTime = 0;
        }

        SdpServiceInfo(SdpServiceInfo &d)
        {
            if (this == &d)
                return;

            m_deviceId = d.m_deviceId;
            m_deviceName = d.m_deviceName;
            m_deviceType = d.m_deviceType;
            m_serviceType = d.m_serviceType;
            m_servicePort = d.m_servicePort;
            m_serviceVersion = d.m_serviceVersion;

            m_notifyTimeout = d.m_notifyTimeout;
            m_nextNotifyTime = d.m_nextNotifyTime;
            m_mac = d.m_mac;
            m_ip = d.m_ip;
            m_servicePort = d.m_servicePort;
            m_retry = d.m_retry;
        }

        virtual ~SdpServiceInfo()
        {
        }

        void setDeviceId(const std::string &id) { m_deviceId = id; }
        const std::string & deviceId(void) { return m_deviceId; }

        void setDeviceName(const std::string &name) { m_deviceName = name; }
        const std::string & deviceName(void) { return m_deviceName; }

        void setDeviceType(DeviceType type) { m_deviceType = type; }
        DeviceType deviceType(void) { return m_deviceType; }

        void setServiceType(const std::string & t) { m_serviceType = t; }
        const std::string & serviceType(void) { return m_serviceType; }

        void setServicePort(int port) { m_servicePort = port; }
        int servicePort(void) { return m_servicePort; }

        void setServiceVersion(const std::string & v) { m_serviceVersion = v; }
        const std::string serviceVersion(void) { return m_serviceVersion; }

        void setNotifyTimeout(uint64_t timeout) { m_notifyTimeout = timeout; }
        uint64_t notifyTimeout(void) { return m_notifyTimeout; }

        void setNextNotifyTime(uint64_t time) { m_nextNotifyTime = time; }
        uint64_t nextNotifyTime(void) { return m_nextNotifyTime; }

        void setMac(const std::string &mac) { m_mac = mac; }
        const std::string & mac(void) { return m_mac; }

        void setIp(const std::string &ip) { m_ip = ip; }
        const std::string & ip(void) { return m_ip; }

        void setPort(int port) { m_port = port; }
        int port(void) { return m_port; }

        void setRetry(int retry) { m_retry = retry; }
        int retry(void) { return m_retry; }

    private:
        std::string     m_deviceId;         // �豸ID
        std::string     m_deviceName;       // �豸����
        DeviceType      m_deviceType;       // �豸����

        std::string     m_serviceType;      // ��������
        int             m_servicePort;      // ����˿�
        std::string     m_serviceVersion;   // ����汾

        uint64_t        m_notifyTimeout;    // ����������Чʱ��
        uint64_t        m_nextNotifyTime;   // ��һ�����������ʱ���

        std::string     m_mac;              // mac��ַ
        std::string     m_ip;               // ip��ַ

        int             m_port;             // �Զ����ֶ˿ڡ���HTC G7���޷������鲥���豸ʹ�ô˲�����
        int             m_retry;            // ���Դ���
    };

    typedef boost::shared_ptr<SdpServiceInfo>       SdpServiceInfoPointer;
}

#endif // __SDP_SERVICE_INFO_H__