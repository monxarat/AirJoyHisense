/*
* AirJoyServiceInfo.hpp
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

#ifndef __AIRJOY_SERVICE_INFO_H__
#define __AIRJOY_SERVICE_INFO_H__

#include <string>

#include "AirJoyTypeDef.hpp"
#include "AirJoyDefault.hpp"
#include "AirJoyApi.hpp"

#ifdef _WIN32 
#pragma warning(disable:4251)
#endif

namespace airjoy
{

    class AIRJOY_API AirJoyServiceInfo
    {
    public:
        AirJoyServiceInfo()
            : m_type(AIRJOY_SERVICE_TYPE)
            , m_version("1.0")
            , m_port(0)
        {
        }

        virtual ~AirJoyServiceInfo()
        {
        }

        // service type
        const std::string & type(void) { return m_type; }
        
        // service version
        const std::string & version(void) { return m_version; }

		// ip
		void setIp(const std::string &ip) { m_ip = ip; }
		const std::string &ip(void) { return m_ip; }

        // service port 
        void setPort(int port) { m_port = port; }
        int port(void) { return m_port; }

        // device id
        void setDeviceId(const std::string &id) { m_deviceId = id; }

        const std::string & deviceId(void) { return m_deviceId; }
     
        // device name
        void setDeviceName(const std::string &n) { m_deviceName = n; }
        const std::string & deviceName(void) { return m_deviceName; }
          
        // device type
        void setDeviceType(DeviceType t) { m_deviceType = t; }
        DeviceType deviceType(void) { return m_deviceType; }

    private:
        std::string     m_type;
        std::string     m_version;
		std::string		m_ip;
		int             m_port;

        std::string     m_deviceId;
        std::string     m_deviceName;
        DeviceType      m_deviceType;
    };


}


#endif // __AIRJOY_SERVICE_INFO_H__
