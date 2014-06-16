/*
* AirTunesServiceInfo.cpp
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

#include "AirTunesServiceInfo.hpp"
#include <stdio.h>
#include <sstream>
#include <vector>
#include <cctype>
#include <iostream>
#include "TheBoost.hpp"
#include "boost/xpressive/xpressive_dynamic.hpp"

#define DEFAULT_DEVICE_ID "112233AABBCC"

// unused
namespace airjoy
{

    // Audio codecs
    typedef enum _AudioCodecs
    {
        AC_PCM      = 0,    // PCM
        AC_ALAC     = 1,    // Apple Lossless (ALAC)
        AC_AAC      = 2,    // AAC
        AC_AACELD   = 3,    // AAC ELD (Enhanced Low Delay)
    } AudioCodecs;

    // Encryption Types
    typedef enum _EncryptionTypes
    {
        ET_NoEncryption    = 0,    // no encryption
        ET_RSA             = 1,    // RSA (AirPort Express)
        ET_FairPlay        = 3,    // FairPlay
        ET_MFiSAP          = 4,    // MFiSAP (3rd-party devices)
        ET_FairPlaySAPV25  = 5,    // FairPlay SAPv2.5
    } EncryptionTypes;

    // Metadata Types
    typedef enum _MetadataType
    {
        MT_TEXT        = 0,     // text
        MT_ARTWORK     = 1,     // artwork
        MT_PROGRESS    = 2,     // progress
    } MetadataType;

    // TransportType
    typedef enum _TransportType
    {
        TP_TCP = 0,     // TCP
        TP_UDP = 1,     // UDP
    } TransportType;

}

using namespace airjoy;

// service infomation
#define AIRTUNES_SERVICE_DEFAULT_NAME       "AirSpeaker"
#define AIRTUNES_SERVICE_DEFAULT_TYPE       "_raop._tcp"
#define AIRTUNES_SERVICE_DEFAULT_PORT       5000

// TXT record version
#define DEFAULT_TXT_VERSION                 "1"

// 2 channels, 44100 Hz, 16-bit audio
#define DEFAULT_AUDIO_CHANNELS              2
#define DEFAULT_AUDIO_SAMPLE_RATE           "44100"
#define DEFAULT_AUDIO_SAMPLE_SIZE           "16"

#if 0
// audio codecs
//#define DEFAULT_AUDIO_CODECS                "0,1,2,3"
#define DEFAULT_AUDIO_CODECS                "0,1"

// encryption types
#define DEFAULT_ENCRYPTION_TYPES            "0,1" 
//#define DEFAULT_ENCRYPTION_TYPES            "0,3,5"

// metadata types
#define DEFAULT_METADATA_TYPES              "0,1,2"

// no password
#define DEFAULT_PASSWORD                    false

// transport protocols
//#define DEFAULT_TRANSPORT_TYPES             "TCP,UDP"
#define DEFAULT_TRANSPORT_TYPES             "UDP"

// service version
#define DEFAULT_SERVICE_VERSION             "130.14"
//#define DEFAULT_SERVICE_VERSION             "101.28"
//#define DEFAULT_SERVICE_VERSION             "104.29"
//#define DEFAULT_SERVICE_VERSION             "150.33"

// device model
//#define DEFAULT_DEVICE_MODEL                "AppleTV3,1"
#define DEFAULT_DEVICE_MODEL                "AppleTV2,1"
//#define DEFAULT_DEVICE_MODEL                "Xbmc,1"
#endif

// audio codecs
#define DEFAULT_AUDIO_CODECS                "0,1,3"

// encryption types
#define DEFAULT_ENCRYPTION_TYPES            "0,3,5"

// metadata types
#define DEFAULT_METADATA_TYPES              "0,1,2"

// no password
#define DEFAULT_PASSWORD                    false

// transport protocols
#define DEFAULT_TRANSPORT_TYPES             "UDP"

// service version
#define DEFAULT_SERVICE_VERSION             "150.33"

// device model
#define DEFAULT_DEVICE_MODEL                "AppleTV3,2"

AirTunesServiceInfo::AirTunesServiceInfo()
    : m_publishedName(new std::string)
    , m_deviceName(new std::string(AIRTUNES_SERVICE_DEFAULT_NAME))
    , m_type(new std::string(AIRTUNES_SERVICE_DEFAULT_TYPE))
    , m_port(AIRTUNES_SERVICE_DEFAULT_PORT)
    , m_txtVersion(new std::string(DEFAULT_TXT_VERSION))
    , m_audioChannels(DEFAULT_AUDIO_CHANNELS)
    , m_audioCodecs(new std::string(DEFAULT_AUDIO_CODECS))
    , m_audioRate(new std::string(DEFAULT_AUDIO_SAMPLE_RATE))
    , m_audioSize(new std::string(DEFAULT_AUDIO_SAMPLE_SIZE))
    , m_encryptionTypes(new std::string(DEFAULT_ENCRYPTION_TYPES))
    , m_metadataTypes(new std::string(DEFAULT_METADATA_TYPES))
    , m_requirePassword(DEFAULT_PASSWORD)
    , m_transportType(new std::string(DEFAULT_TRANSPORT_TYPES))
    , m_serverVersion(new std::string(DEFAULT_SERVICE_VERSION))
    , m_deviceModel(new std::string(DEFAULT_DEVICE_MODEL))
    , m_txt(new TxtRecordType)
    , m_macAddressLen(MAC_LEN)
{
}

AirTunesServiceInfo::AirTunesServiceInfo(const AirTunesServiceInfo &info)
{
	memcpy(m_mac, info.m_mac, MAC_LEN);
    m_macAddressLen = info.m_macAddressLen;

    m_publishedName = new std::string();

    m_deviceName = new std::string(*info.m_deviceName);
    m_type = new std::string(*info.m_type);
	m_port = info.m_port;

    m_txtVersion = new std::string(*info.m_txtVersion);
	m_audioChannels = info.m_audioChannels;
    m_audioCodecs = new std::string(*info.m_audioCodecs);
    m_audioRate = new std::string(*info.m_audioRate);
    m_audioSize = new std::string(*info.m_audioSize);
    m_encryptionTypes = new std::string(*info.m_encryptionTypes);
    m_metadataTypes = new std::string(*info.m_metadataTypes);
	m_requirePassword = info.m_requirePassword;
    m_transportType = new std::string(*info.m_transportType);
    m_serverVersion = new std::string(*info.m_serverVersion);
    m_deviceModel = new std::string(*info.m_deviceModel);
    m_txt = new TxtRecordType;
}

AirTunesServiceInfo::~AirTunesServiceInfo()
{
    delete m_publishedName;

    delete m_deviceName;
    delete m_type;

    delete m_txtVersion;
    delete m_audioCodecs;
    delete m_encryptionTypes;
    delete m_metadataTypes;
    delete m_audioRate;
    delete m_audioSize;
    delete m_transportType;
    delete m_serverVersion;
    delete m_deviceModel;

	TxtRecordType::iterator p = m_txt->begin();
    for (; p != m_txt->end(); ++p)
    {
		delete (*p);
    }

    delete m_txt;
}

const std::string & AirTunesServiceInfo::type(void)
{
    return *m_type;
}

void AirTunesServiceInfo::setPort(uint16_t port)
{
    m_port = port;
}

uint16_t AirTunesServiceInfo::port(void)
{
    return m_port;
}
        
void AirTunesServiceInfo::setMacAddress(unsigned char mac[6], size_t len)
{
#if 1
    size_t maclen = (len < MAC_LEN) ? len : MAC_LEN;
    m_macAddressLen = maclen;

    for (size_t i = 0; i < maclen; ++i)
	{
        //m_mac[i] = ::toupper(mac[i]);
        m_mac[i] = mac[i];
	}
#else
        m_mac[0] = 0x11;
        m_mac[1] = 0x22;
        m_mac[2] = 0x33;
        m_mac[3] = 0xaa;
        m_mac[4] = 0xbb;
        m_mac[5] = 0xcc;
#endif
}

const unsigned char * AirTunesServiceInfo::macAddress(void)
{
    return m_mac;
}

size_t AirTunesServiceInfo::macAddressLen(void)
{
    return m_macAddressLen;
}

void AirTunesServiceInfo::setDeviceId(const std::string &id)
{
#if 0
    unsigned char mac[MAC_LEN];
    memset(mac, 0, 6);

    std::string regex("(\\w+)");

    using namespace boost::xpressive;
    sregex reg = sregex::compile(regex.c_str(), icase);

    sregex_iterator pos(id.begin(), id.end(), reg);
    sregex_iterator end;

    size_t i = 0;
    while (pos != end)
    {
        std::string v = (*pos)[0];

        if (i >= MAC_LEN)
            break;
        
        mac[i] = this->hexStrToInt(v.c_str());
        
        std::cout << "v: " << v << std::endl;
        std::cout << "mac[" << i << "]: " << (int)mac[i] << std::endl;

        ++pos;
        ++i;
    }

    this->setMacAddress(mac, 6);
#else
    const char * m = "11:22:33:aa:bb:cc";
    if (id.length() < strlen(m))
        return;

    const char * p = id.c_str();

    char v[MAC_LEN][3];
    unsigned char mac[MAC_LEN];

    memset(v[0], 0, 3);
    v[0][0] = p[0];
    v[0][1] = p[1];

    memset(v[1], 0, 3);
    v[1][0] = p[3];
    v[1][1] = p[4];

    memset(v[2], 0, 3);
    v[2][0] = p[6];
    v[2][1] = p[7];

    memset(v[3], 0, 3);
    v[3][0] = p[9];
    v[3][1] = p[10];

    memset(v[4], 0, 3);
    v[4][0] = p[12];
    v[4][1] = p[13];

    memset(v[5], 0, 3);
    v[5][0] = p[15];
    v[5][1] = p[16];

    for (int i = 0; i < MAC_LEN; ++i)
        mac[i] = this->hexStrToInt(v[i]);

    this->setMacAddress(mac, 6);
#endif
}

void AirTunesServiceInfo::setDeviceName(const std::string &name)
{
    *m_deviceName = name;
}

const std::string & AirTunesServiceInfo::deviceName(void)
{
    return *m_deviceName;
}

const std::string & AirTunesServiceInfo::publishedName(void)
{
    char buf[32];
    memset(buf, 0, sizeof(buf));

#ifdef _WIN32
    _snprintf(buf, sizeof(buf), "%02X%02X%02X%02X%02X%02X", m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5]);
#else
    snprintf(buf, sizeof(buf), "%02X%02X%02X%02X%02X%02X", m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5]);
#endif

    //for(size_t i=0; i<strlen(buf); ++i)
    //    buf[i] = std::toupper(buf[i]);
    
    m_publishedName->clear();
    m_publishedName->append(buf);
    m_publishedName->append("@");
    m_publishedName->append(*m_deviceName);

    return *m_publishedName;
}

/*
name: 5855CA1AE288@Apple TV
type: _raop._tcp
port: 49152
txt:
 txtvers=1
 ch=2
 cn=0,1,2,3
 da=true        // must be true ?
 et=0,3,5
 md=0,1,2
 pw=false
 sv=false       // must be false ?
 sr=44100
 ss=16
 tp=UDP
 vn=65537       // must be 65537 ?
 vs=130.14
 am=AppleTV2,1
 sf=0x4         // must be 0x4 ?

txtvers	1	TXT record version 1
ch	2	audio channels: stereo
cn	0,1,2,3	audio codecs
et	0,3,5	supported encryption types
md	0,1,2	supported metadata types
pw	false	does the speaker require a password?
sr	44100	audio sample rate: 44100 Hz
ss	16	audio sample size: 16-bit
tp	UDP	supported transport: TCP or UDP
vs	130.14	server version 130.14
am	AppleTV2,1	device model
*/

/*
char* argv[] = { "tp=UDP", 
                 "sm=false", 
                 "sv=false", 
                 "ek=1", 
                 "et=0,1", 
                 "cn=0,1", 
                 "ch=2", 
                 "ss=16", 
                 "sr=44100", 
                 "pw=false",
                 "vn=3", 
                 "txtvers=1", 
                 NULL };
*/

AirTunesServiceInfo::TxtRecordType & AirTunesServiceInfo::txt(void)
{
	std::stringstream ssAudioChannels;
	ssAudioChannels << m_audioChannels;

    m_txt->clear();

	m_txt->push_back(new KeyValue("am", m_deviceModel->c_str()));
	m_txt->push_back(new KeyValue("ch", ssAudioChannels.str().c_str()));
	m_txt->push_back(new KeyValue("cn", m_audioCodecs->c_str()));
	m_txt->push_back(new KeyValue("da", "true"));

    // encryption tyeps;
    //  0: no encryption
    //  1: airport express (RSA+AES)
    //  3: apple tv (FairPlay+AES)
	m_txt->push_back(new KeyValue("et", m_encryptionTypes->c_str()));

	m_txt->push_back(new KeyValue("md", m_metadataTypes->c_str()));

    // password
	m_txt->push_back(new KeyValue("pw", m_requirePassword ? "true" : "false"));

	m_txt->push_back(new KeyValue("rhd", "1.9.8"));

	m_txt->push_back(new KeyValue("sf", "0x4"));
	m_txt->push_back(new KeyValue("sr", m_audioRate->c_str()));
	m_txt->push_back(new KeyValue("ss", m_audioSize->c_str()));
	m_txt->push_back(new KeyValue("sv", "false"));

	// transport protocols
	m_txt->push_back(new KeyValue("tp", m_transportType->c_str()));
	m_txt->push_back(new KeyValue("txtvers", m_txtVersion->c_str()));
    m_txt->push_back(new KeyValue("vn", "65537"));

    // aitunes server version
	m_txt->push_back(new KeyValue("vs", m_serverVersion->c_str()));
	m_txt->push_back(new KeyValue("vv", "1"));


#if 0
    // txt record version
	m_txt->push_back(new KeyValue("txtvers", m_txtVersion->c_str()));
	m_txt->push_back(new KeyValue("cn", m_audioCodecs->c_str()));
	m_txt->push_back(new KeyValue("ch", ssAudioChannels.str().c_str()));
	
    // encryption tyeps;
    //  0: no encryption
    //  1: airport express (RSA+AES)
    //  3: apple tv (FairPlay+AES)
	m_txt->push_back(new KeyValue("ek", "1"));
	m_txt->push_back(new KeyValue("et", m_encryptionTypes->c_str()));

	m_txt->push_back(new KeyValue("sv", "false"));

	// transport protocols
	m_txt->push_back(new KeyValue("tp", m_transportType->c_str()));

	m_txt->push_back(new KeyValue("sm", "false"));

    // 2 channels, 44100 Hz, 16-bit audio

	m_txt->push_back(new KeyValue("ss", m_audioSize->c_str()));
	m_txt->push_back(new KeyValue("sr", m_audioRate->c_str()));

    // no password
	m_txt->push_back(new KeyValue("pw", m_requirePassword ? "true" : "false"));

    m_txt->push_back(new KeyValue("vn", "3"));
    
	m_txt->push_back(new KeyValue("da", "true"));

    // aitunes server version
	m_txt->push_back(new KeyValue("vs", m_serverVersion->c_str()));

	m_txt->push_back(new KeyValue("md", m_metadataTypes->c_str()));

	// device model
	m_txt->push_back(new KeyValue("am", m_deviceModel->c_str()));
#endif

#if 0
	// transport protocols
	m_txt->push_back(new KeyValue("tp", m_transportType->c_str()));

	m_txt->push_back(new KeyValue("sm", "false"));
	m_txt->push_back(new KeyValue("sv", "false"));

	// encryption tyeps;
    //  0: no encryption
    //  1: airport express (RSA+AES)
    //  3: apple tv (FairPlay+AES)
	m_txt->push_back(new KeyValue("ek", "1"));
	m_txt->push_back(new KeyValue("et", m_encryptionTypes->c_str()));

	m_txt->push_back(new KeyValue("cn", m_audioCodecs->c_str()));

    // 2 channels, 44100 Hz, 16-bit audio
	m_txt->push_back(new KeyValue("ch", ssAudioChannels.str().c_str()));
	m_txt->push_back(new KeyValue("ss", m_audioSize->c_str()));
	m_txt->push_back(new KeyValue("sr", m_audioRate->c_str()));

    // no password
	m_txt->push_back(new KeyValue("pw", m_requirePassword ? "true" : "false"));

    m_txt->push_back(new KeyValue("vn", "3"));
    
    // txt record version
	m_txt->push_back(new KeyValue("txtvers", m_txtVersion->c_str()));
	
	m_txt->push_back(new KeyValue("da", "true"));

	// new, for ios7
	m_txt->push_back(new KeyValue("ft", "0x100029FF"));

	m_txt->push_back(new KeyValue("md", m_metadataTypes->c_str()));

    // aitunes server version
	m_txt->push_back(new KeyValue("vs", m_serverVersion->c_str()));

	// new, for ios7
	m_txt->push_back(new KeyValue("rhd", "4.1.3"));
	m_txt->push_back(new KeyValue("vv", "1"));
	m_txt->push_back(new KeyValue("vn", "65537"));

	// device model
    // 2013-2-28，jxfengzi@gmail.com，貌似没有"am=AppleTV3,1"字段才能单独让iPhone出现喇叭
	m_txt->push_back(new KeyValue("am", m_deviceModel->c_str()));

	m_txt->push_back(new KeyValue("sf", "0x4"));
#endif

#if 0
#if 1
	// transport protocols
    m_txt->insert(std::make_pair<std::string, std::string>("tp", std::string(*m_transportType)));
	
	m_txt->insert(std::make_pair<std::string, std::string>("sm", "false"));
    m_txt->insert(std::make_pair<std::string, std::string>("sv", "false"));

	// encryption tyeps;
    //  0: no encryption
    //  1: airport express (RSA+AES)
    //  3: apple tv (FairPlay+AES)
    m_txt->insert(std::make_pair<std::string, std::string>("ek", "1"));
	m_txt->insert(std::make_pair<std::string, std::string>("et", std::string(*m_encryptionTypes)));

    m_txt->insert(std::make_pair<std::string, std::string>("cn", std::string(*m_audioCodecs)));

    // 2 channels, 44100 Hz, 16-bit audio
    m_txt->insert(std::make_pair<std::string, std::string>("ch", std::string(ssAudioChannels.str())));
    m_txt->insert(std::make_pair<std::string, std::string>("ss", std::string(*m_audioSize)));
	m_txt->insert(std::make_pair<std::string, std::string>("sr", std::string(*m_audioRate)));

    // no password
    m_txt->insert(std::make_pair<std::string, std::string>("pw", m_requirePassword ? "true" : "false"));

	m_txt->insert(std::make_pair<std::string, std::string>("vn", "3"));

	// txt record version
    m_txt->insert(std::make_pair<std::string, std::string>("txtvers", std::string(*m_txtVersion)));

    m_txt->insert(std::make_pair<std::string, std::string>("da", "true"));

	// new, for ios7
    m_txt->insert(std::make_pair<std::string, std::string>("ft", "0x100029FF"));

    m_txt->insert(std::make_pair<std::string, std::string>("md", std::string(*m_metadataTypes)));

    // aitunes server version
    m_txt->insert(std::make_pair<std::string, std::string>("vs", std::string(*m_serverVersion)));

	// new, for ios7
    m_txt->insert(std::make_pair<std::string, std::string>("rhd", "4.1.3"));
	m_txt->insert(std::make_pair<std::string, std::string>("vv", "1"));
	m_txt->insert(std::make_pair<std::string, std::string>("vn", "65537"));

	// device model
    // 2013-2-28，jxfengzi@gmail.com，貌似没有"am=AppleTV3,1"字段才能单独让iPhone出现喇叭
    m_txt->insert(std::make_pair<std::string, std::string>("am", *m_deviceModel));

    m_txt->insert(std::make_pair<std::string, std::string>("sf", "0x4"));
   
#else
    // 2013-2-28，jxfengzi@gmail.com，貌似只有这样才能单独让iPhone出现喇叭
    m_txt->insert(std::make_pair<std::string, std::string>("tp", "UDP"));
    m_txt->insert(std::make_pair<std::string, std::string>("sm", "false"));
    m_txt->insert(std::make_pair<std::string, std::string>("sv", "false"));
    m_txt->insert(std::make_pair<std::string, std::string>("ek", "1"));
    m_txt->insert(std::make_pair<std::string, std::string>("et", "0,1"));
    m_txt->insert(std::make_pair<std::string, std::string>("cn", "0,1"));
    m_txt->insert(std::make_pair<std::string, std::string>("ch", "2"));
    m_txt->insert(std::make_pair<std::string, std::string>("ss", "16"));
    m_txt->insert(std::make_pair<std::string, std::string>("sr", "44100"));
    m_txt->insert(std::make_pair<std::string, std::string>("pw", "false"));
    m_txt->insert(std::make_pair<std::string, std::string>("vn", "3"));
    m_txt->insert(std::make_pair<std::string, std::string>("txtvers", "1"));
#endif
#endif

    return *m_txt;
}

int AirTunesServiceInfo::hexStrToInt(const char *str)   
{
    std::vector<char> myvector;
    std::vector<char>::iterator it;

    size_t i = 0;
    while (str[i] != 0)
    {
        myvector.push_back(str[i]);
        i++;
    }

    std::reverse(myvector.begin(),myvector.end());       // 9 8 7 6 5 4 3 2 1

    int intValue = 0;
    int value = 0;
    int n = 0;
    for (it = myvector.begin(); it!=myvector.end(); ++it, ++n)
    {
        char c = *it;
        if (c >= '0' && c <= '9')
        {
            c -= '0';
        }
        else if (c >= 'a' && c<= 'f')
        {
            c = c - 'a' + 10;
        }
        else if (c >= 'A' && c<= 'F')
        {
            c = c - 'A' + 10;
        }
        else
        {
            return 0;
        }

        int value = c;
        for (int i = 0; i < n; ++i)
            value = value * 16;

        intValue += value;
    }

    return intValue;
}
