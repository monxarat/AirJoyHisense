/*
* NsdServiceInfo.h
*
* Copyright (c) 2012 jxfengzi@gmail.com 
*
*
*
*
*/

#ifndef __NSD_SERVICE_INFO_H__
#define __NSD_SERVICE_INFO_H__

#include <string>
#include <map>

namespace airjoy
{

	class TXTRecord 
	{
	private:
		#define MAX_KEY_LEN 128
		char mKey[MAX_KEY_LEN + 1];

		#define MAX_VALUE_LEN 128
		char mValue[MAX_VALUE_LEN + 1];

	public:
		TXTRecord(const char *key, const char *value)
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

    class NsdServiceInfo
    {
    public:
		typedef boost::shared_ptr<TXTRecord>	TXTRecordPointer;
		typedef std::list<TXTRecordPointer>		TXTRecordMap;

    private:
        std::string     m_domain;
        std::string     m_type;
        std::string     m_name;
        std::string     m_fullResolveName;

        std::string     m_hostName;
        uint16_t        m_port;

        unsigned long   m_ip;
        std::string     m_ipString;

        TXTRecordMap    m_TXTRecords;

        uint32_t        m_updateNumber;

    public:
        NsdServiceInfo(const std::string &name, const std::string &type, const std::string &domain)
            : m_domain(domain)
            , m_type(type)
            , m_name(name)
            , m_port(0)
            , m_ip(0)
            , m_updateNumber(0)
        {
            m_fullResolveName = name;
            m_fullResolveName.append(".");
            m_fullResolveName.append(type);
            m_fullResolveName.append(domain);
        }

        ~NsdServiceInfo()
        {
        }

        uint32_t getUpdateNumber(void)          { return m_updateNumber; }
        void increaseUpdateNumber(void)          { m_updateNumber++; }

        std::string & getName(void)             { return m_name; }
        std::string & getType(void)             { return m_type; }
        std::string & getDomain(void)           { return m_domain; }

        std::string & getHostName(void)         { return m_hostName; }
        uint16_t      getPort(void)             { return m_port; }
        unsigned long getIp(void)               { return m_ip; }
        std::string & getIPWithString(void)     { return m_ipString; }
        std::string & getFullResolveName(void)  { return m_fullResolveName; }

        void setName(const char *name)          { m_name = name; }
        void setType(const char *type)          { m_type = type; }
        void setDomain(const char *domain)      { m_domain = domain; }

        void setHostName(const char *hostName)  { m_hostName = hostName; }
        void setPort(uint16_t port)             { m_port = port; }

        void setIPAddress(const struct sockaddr *address, uint32_t ttl)
        {
            const char * ipChar = (const char *)inet_ntoa (((struct sockaddr_in *)address)->sin_addr);
            unsigned long ip = inet_addr (ipChar);

            m_ipString = ipChar;
            m_ip       = ip;
        }

        void setTXTRecordData(uint16_t txtLen, const unsigned char *txtRecord)
        {
            m_TXTRecords.clear();

            if (txtLen < 2)
                return;

            int i = 0;
            char buf[256];
            char key[256];
            char value[256];
            
            //std::cout << "------------------"<< std::endl;

            unsigned char length = txtRecord[i];
            while (i < txtLen)
            {
                const unsigned char *start = txtRecord + (i+1);

                memset(buf, 0, 256);
                memcpy(buf, start, length);

                i = i + length + 1;
                length = txtRecord[i];

                char *p = strstr(buf, "=");
                if (p)
                {
                    memset(key, 0, 256);
                    memcpy(key,  buf, p - buf);

                    p++;
                    memset(value, 0, 256);
                    memcpy(value,  p, strlen(p));

                    m_TXTRecords.push_back(boost::make_shared<TXTRecord>(key, value));
                    //std::cout << key << "=" << value << std::endl;
                }
            }
            //std::cout << "------------------"<< std::endl;
        }

        std::string & getTXTRecordValueWithKey(const char *key)
        {
            static std::string value;

            TXTRecordMap::iterator p = m_TXTRecords.begin();
		    for (; p != m_TXTRecords.end(); ++p)
            {
                if (strcmp((*p)->getKey(), key) == 0)
                {
                    value = (*p)->getValue();
                    break;
                }
            }

            return value;
        }

        void setTXTRecordValueWithKey(const char *key, const char *value)
        {
#if 0
            TXTRecordMap::iterator p = m_TXTRecords.begin();
		    for (; p != m_TXTRecords.end(); ++p)
            {
                if (strcmp((*p)->getKey(), key) == 0)
                {
                    return;
                }
            }
#endif
            m_TXTRecords.push_back(boost::make_shared<TXTRecord>(key, value));
        }

        TXTRecordMap & txtRecord(void)
        {
            return m_TXTRecords;
        }
    };

}


#endif // __NSD_SERVICE_INFO_H__
