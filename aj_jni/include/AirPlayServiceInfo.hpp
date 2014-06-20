/*
* AirPlayServiceInfo.hpp
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

#ifndef __AIRPLAY_SERVICE_INFO_H__
#define __AIRPLAY_SERVICE_INFO_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"
#include "AirPlayServerDefault.hpp"

#ifdef _WIN32 
//#pragma warning(disable:4251)
#endif

namespace airjoy
{

    class AIRJOY_API AirPlayServiceInfo
    {
    public:
        AirPlayServiceInfo();
        AirPlayServiceInfo(const AirPlayServiceInfo &info);
        virtual ~AirPlayServiceInfo();

        // protovers
        const std::string & protovers(void);

        // service type 
        const std::string & type(void);

        // service version
        const std::string & version(void);

        // service features
        uint32_t features(void);
		void setFeatures(uint32_t f);

        // service port 
        void setPort(uint16_t port);
        uint16_t port(void);

        // device id
        void setDeviceId(const std::string &id);
        const std::string & deviceId(void);
        
        void setMacAddress(unsigned char mac[6], size_t len = 6);
        const unsigned char *macAddress(void);
        size_t macAddressLen(void);

        // device name
        void setDeviceName(const std::string &n);
        const std::string & deviceName(void);
          
        // device model 
        void setDeviceModel(const std::string &m);
        const std::string & deviceModel(void);

        // device ip
        void setIp(const std::string &ip);
        const std::string & ip(void);

    private:
        std::string    *m_protovers;

        std::string    *m_type;
        std::string    *m_version;
        uint32_t        m_features;
        uint16_t        m_port;

        #define         MAC_LEN     6
        unsigned char   m_mac[MAC_LEN];
        size_t          m_macAddressLen;

        std::string    *m_deviceId;
        std::string    *m_deviceName;
        std::string    *m_deviceModel;

        std::string    *m_ip;
    };

}


#endif // __AIRPLAY_SERVICE_INFO_H__
