/*
* AirTunesServiceInfo.hpp
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

#ifndef __AIRTUNES_SERVICE_INFO_H__
#define __AIRTUNES_SERVICE_INFO_H__

#include <string>
#include <list>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"

namespace airjoy
{


	class AIRJOY_API KeyValue 
	{
	private:
		#define MAX_KEY_LEN 128
		char mKey[MAX_KEY_LEN + 1];

		#define MAX_VALUE_LEN 128
		char mValue[MAX_VALUE_LEN + 1];

	public:
		KeyValue(const char *key, const char *value)
		{
			strncpy(mKey, key, MAX_KEY_LEN);
			strncpy(mValue, value, MAX_VALUE_LEN);
		}

		const char * getKey() 
		{
			return mKey; 
		}

		const char * getValue()
		{
			return mValue; 
		}
	};

    class AIRJOY_API AirTunesServiceInfo
    {
    public:
        //typedef std::map<std::string, std::string> TxtRecordType;
        typedef std::list<KeyValue *>   TxtRecordType;

    private:
        #define         MAC_LEN     6
        unsigned char   m_mac[MAC_LEN];
        size_t          m_macAddressLen;
        std::string    *m_publishedName;

        std::string    *m_deviceName;
        std::string    *m_type;
        uint16_t        m_port;

        // txt
        std::string    *m_txtVersion;       // TXT record Version
        uint32_t        m_audioChannels;    // audio channels
        std::string    *m_audioCodecs;      // audio codecs
        std::string    *m_encryptionTypes;  // supported encryption types
        std::string    *m_metadataTypes;    // supported metadata types
        bool            m_requirePassword;  // does the speaker require a password
        std::string    *m_audioRate;        // audio sample rate(Hz)
        std::string    *m_audioSize;        // audio sample size(bit)
        std::string    *m_transportType;    // supported transport(TCP or UDP)
        std::string    *m_serverVersion;    // server version
        std::string    *m_deviceModel;      // device model

        TxtRecordType  *m_txt;

    public:
        AirTunesServiceInfo();
        AirTunesServiceInfo(const AirTunesServiceInfo &info);
		virtual ~AirTunesServiceInfo();

        const std::string & type(void);

        void setPort(uint16_t port);
        uint16_t port(void);

        TxtRecordType & txt(void);

        void setMacAddress(unsigned char mac[6], size_t len = 6);
        const unsigned char *macAddress(void);
        size_t macAddressLen(void);

        void setDeviceId(const std::string &id);

        void setDeviceName(const std::string &name);
        const std::string & deviceName(void);

        const std::string & publishedName(void);

    private:
        int hexStrToInt(const char *str);
    };

}


#endif // __AIRTUNES_SERVICE_INFO_H__
