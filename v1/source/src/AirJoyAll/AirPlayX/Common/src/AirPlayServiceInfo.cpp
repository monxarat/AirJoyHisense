/*
* AirPlayServiceInfo.cpp
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

#include "AirPlayServiceInfo.hpp"
#include <string.h> 	// for memset
#include <stdio.h> 	// for snprintf

using namespace airjoy;

#define DEFAULT_DEVICE_ID   "11:22:33:AA:BB:CC"


//#define AIRPLAY_SERVICE_VERSION             "150.33"
//#define AIRPLAY_SERVICE_DEVICE_MODEL        "AppleTV3,1"

//#define AIRPLAY_SERVICE_DEVICE_MODEL        "AppleTV2,1"
//#define AIRPLAY_SERVICE_VERSION             "101.28"

//#define AIRPLAY_SERVICE_DEVICE_MODEL        "Xbmc,1"
//#define AIRPLAY_SERVICE_VERSION             "101.28"

#define AIRPLAY_SERVICE_DEVICE_MODEL        "AppleTV3,2"
#define AIRPLAY_SERVICE_VERSION             "150.33"

#define AIRPLAY_SERVICE_TYPE                "_airplay._tcp"
#define AIRPLAY_SERVICE_PORT                AIRPLAY_SERVER_PORT

/*
bit	name	description
0	Video	video supported
1	Photo	photo supported
2	VideoFairPlay	video protected with FairPlay DRM
3	VideoVolumeControl	volume control supported for videos
4	VideoHTTPLiveStreams	http live streaming supported
5	Slideshow	slideshow supported
7	Screen	mirroring supported
8	ScreenRotate	screen rotation supported
9	Audio	audio supported
11	AudioRedundant	audio packet redundancy supported
12	FPSAPv2pt5_AES_GCM	FairPlay secure auth supported
13	PhotoCaching	photo preloading supported
*/

#define AirPlay_Video                       (1 << 0)
#define AirPlay_Photo                       (1 << 1)
#define AirPlay_VideoFairPlay               (1 << 2)
#define AirPlay_VideoVolumeControl          (1 << 3)

#define AirPlay_VideoHTTPLiveStreams        (1 << 4)
#define AirPlay_Slideshow                   (1 << 5) 
#define AirPlay_6                           (1 << 6) 
#define AirPlay_Screen                      (1 << 7)

#define AirPlay_ScreenRotate                (1 << 8)
#define AirPlay_Audio                       (1 << 9)
#define AirPlay_10                          (1 << 10)
#define AirPlay_AudioRedundant              (1 << 11)

#define AirPlay_FPSAPv2pt5_AES_GCM          (1 << 12)
#define AirPlay_PhotoCaching                (1 << 13)
#define AirPlay_14                          (1 << 14)
#define AirPlay_15                          (1 << 15)

#if 0
#if 1
#define AIRPLAY_SERVICE_FEATURES      (AirPlay_Video \
                                      | AirPlay_Photo \
                                      | AirPlay_VideoVolumeControl \
                                      | AirPlay_VideoHTTPLiveStreams \
                                      | AirPlay_PhotoCaching \
                                      | AirPlay_Audio)
#else
#define AIRPLAY_SERVICE_FEATURES      (AirPlay_Video \
                                      | AirPlay_Photo \
                                      | AirPlay_VideoHTTPLiveStreams \
                                      | AirPlay_PhotoCaching)
#endif
#else
#define AIRPLAY_SERVICE_FEATURES    0x100029ff
//#define AIRPLAY_SERVICE_FEATURES    0x77
#endif


AirPlayServiceInfo::AirPlayServiceInfo()
    : m_protovers(new std::string("1.0"))
    , m_type(new std::string(AIRPLAY_SERVICE_TYPE))
    , m_version(new std::string(AIRPLAY_SERVICE_VERSION))
    , m_port(AIRPLAY_SERVICE_PORT)
    , m_features(AIRPLAY_SERVICE_FEATURES)
    , m_macAddressLen(MAC_LEN)
    , m_deviceId(new std::string)
    , m_deviceName(new std::string)
    , m_deviceModel(new std::string(AIRPLAY_SERVICE_DEVICE_MODEL))
    , m_ip(new std::string)
{
}

AirPlayServiceInfo::AirPlayServiceInfo(const AirPlayServiceInfo &info)
    : m_protovers(new std::string(*info.m_protovers))
    , m_type(new std::string(*info.m_type))
    , m_version(new std::string(*info.m_version))
    , m_port(info.m_port)
    , m_features(info.m_features)
    , m_macAddressLen(info.m_macAddressLen)
    , m_deviceId(new std::string(*info.m_deviceId))
    , m_deviceName(new std::string(*info.m_deviceName))
    , m_deviceModel(new std::string(*info.m_deviceModel))
    , m_ip(new std::string(*info.m_ip))
{
}

AirPlayServiceInfo::~AirPlayServiceInfo()
{
	delete m_ip;
	delete m_deviceModel;
	delete m_deviceName;
	delete m_deviceId;
	delete m_version;
	delete m_type;
	delete m_protovers;
}

// protovers
const std::string & AirPlayServiceInfo::protovers(void) 
{ 
    return *m_protovers; 
}

// service type 
const std::string & AirPlayServiceInfo::type(void) 
{
    return *m_type; 
}

// service version
const std::string & AirPlayServiceInfo::version(void) 
{ 
    return *m_version; 
}

// service features
uint32_t AirPlayServiceInfo::features(void) 
{
    return m_features; 
}

// service port 
void AirPlayServiceInfo::setPort(uint16_t port) 
{
    m_port = port; 
}

uint16_t AirPlayServiceInfo::port(void)
{ 
    return m_port; 
}

// device id
void AirPlayServiceInfo::setDeviceId(const std::string &id)
{
#if 1
    *m_deviceId = id; 
#else
    *m_deviceId = DEFAULT_DEVICE_ID;
#endif
}

const std::string & AirPlayServiceInfo::deviceId(void) 
{
    if (m_deviceId->empty())
    {
        char buf[32];
        memset(buf, 0, sizeof(buf));

    #ifdef _WIN32
        _snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x", m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5]);
    #else
        snprintf(buf, sizeof(buf), "%02x:%02x:%02x:%02x:%02x:%02x", m_mac[0], m_mac[1], m_mac[2], m_mac[3], m_mac[4], m_mac[5]);
    #endif
        m_deviceId->append(buf);
    }

    return *m_deviceId; 
}

void AirPlayServiceInfo::setMacAddress(unsigned char mac[6], size_t len)
{
#if 1
    size_t maclen = (len < MAC_LEN) ? len : MAC_LEN;
    m_macAddressLen = maclen;

    for (size_t i = 0; i < maclen; ++i)
        m_mac[i] = mac[i];
#else
        m_mac[0] = 0x11;
        m_mac[1] = 0x22;
        m_mac[2] = 0x33;
        m_mac[3] = 0xaa;
        m_mac[4] = 0xbb;
        m_mac[5] = 0xcc;
#endif
}

const unsigned char * AirPlayServiceInfo::macAddress(void)
{
    return m_mac;
}

size_t AirPlayServiceInfo::macAddressLen(void)
{
    return m_macAddressLen;
}

// device name
void AirPlayServiceInfo::setDeviceName(const std::string &n) 
{
    *m_deviceName = n; 
}

const std::string & AirPlayServiceInfo::deviceName(void) 
{
    return *m_deviceName; 
}

// device model 
void AirPlayServiceInfo::setDeviceModel(const std::string &m)
{ 
    *m_deviceModel = m; 
}

const std::string & AirPlayServiceInfo::deviceModel(void) 
{ 
    return *m_deviceModel; 
}

// device ip
void AirPlayServiceInfo::setIp(const std::string &ip) 
{
    *m_ip = ip; 
}

const std::string & AirPlayServiceInfo::ip(void) 
{
    return *m_ip; 
}
