
#include <iostream>

#include "Nsd.hpp"
#include "NsdDiscoveryHandler.hpp"
#include "NsdPublishHandler.hpp"
#include "NsdServiceInfo.hpp"

#ifndef _WIN32
#include <unistd.h>
#endif

using namespace airjoy;

class AirPlayNsdDiscoveryHandler : public NsdDiscoveryHandler
{
public:
    AirPlayNsdDiscoveryHandler()
    {
    }

    virtual ~AirPlayNsdDiscoveryHandler()
    {
    }

    virtual void willStart(Nsd * nsd)
    {
        std::cout << "willStart: " << std::endl;
    }

    virtual void didStart(Nsd * nsd)
    {
        std::cout << "didStart: " << std::endl;
    }

    virtual void didNotStart(Nsd * nsd)
    {
        std::cout << "didNotStart: " << std::endl;
    }

    virtual void didAddService(NsdServiceInfoPointer s)
    {
        std::cout << "didAddService: " << s->getName() << " type: " << s->getType() << " domain: " << s->getDomain() << " port: " << s->getPort() << std::endl;
    }

    virtual void didRemoveService(NsdServiceInfoPointer s)
    {
        std::cout << "didRemoveService: " << s->getName() << std::endl;
    }

    virtual void didResolvService(NsdServiceInfoPointer s)
    {
        std::cout << "didResolvService: " << s->getFullResolveName() << " hoseName: " << s->getHostName() << " port:" << s->getPort() << std::endl;
        //std::cout << "deviceId: " << s->getTXTRecordValueWithKey("deviceId") << std::endl;
        //std::cout << "deviceType: " << s->getTXTRecordValueWithKey("deviceType") << std::endl;

        std::cout << "deviceid: " << s->getTXTRecordValueWithKey("deviceid") << std::endl;
        std::cout << "features: " << s->getTXTRecordValueWithKey("features") << std::endl;
        std::cout << "model: " << s->getTXTRecordValueWithKey("model") << std::endl;

    }

    virtual void didGetIPAddressForService(NsdServiceInfoPointer s)
    {
        std::cout << "didGetIPAddressForService: " << s->getName() << " ip: " << s->getIPWithString() << std::endl;
    }
};


class AirPlayNsdPublishHandler : public NsdPublishHandler
{
public:
    AirPlayNsdPublishHandler()
    {
    }

    virtual ~AirPlayNsdPublishHandler()
    {
    }

    virtual void didNotPublish(NsdServiceInfoPointer serviceInfo)
    {
        std::cout << "didNotPublish: " << std::endl;
    }

    virtual void willPublish(NsdServiceInfoPointer serviceInfo)
    {
        std::cout << "willPublish: " << std::endl;
    }

    virtual void didPublish(NsdServiceInfoPointer serviceInfo)
    {
        std::cout << "didPublish: " << std::endl;
    }

};

Nsd gNsd;
bool gLoop = true;

void  parseCommand(const std::string &cmd)
{
    if (cmd == "help" || cmd == "h")
    {
        std::cout << "---------- help ----------" << std::endl;
        std::cout << "help(h) -- help infomation" << std::endl;
        std::cout << "exit(x) -- exit" << std::endl;
        std::cout << "req(r)  -- send request" << std::endl;
        return;
    }

    if (cmd == "exit" || cmd == "x")
    {
        gNsd.stopServiceDiscovery();
        gLoop = false;
        return;
    }
}

void command()
{
    std::string buf;

    while (gLoop)
    {
        std::cout << "> ";
        buf.clear();
        std::cin >> buf;
        parseCommand(buf);
    }
}

/*
name: Apple TV
type: _airplay._tcp
port: 7000
txt:
deviceid=58:55:CA:1A:E2:88
features=0x39f7
model=AppleTV2,1
srcvers=130.14

0	Video	                    video supported
1	Photo	                    photo supported
2	VideoFairPlay	            video protected with FairPlay DRM
3	VideoVolumeControl	        volume control supported for videos
4	VideoHTTPLiveStreams	    http live streaming supported
5	Slideshow	                slideshow supported
7	Screen	                    mirroring supported
8	ScreenRotate	            screen rotation supported
9	Audio	                    audio supported
11	AudioRedundant	            audio packet redundancy supported
12	FPSAPv2pt5_AES_GCM	        FairPlay secure auth supported
13	PhotoCaching	            photo preloading supported
*/

#define AirPlay_Video                       (1 << 0)
#define AirPlay_Photo                       (1 << 1)
#define AirPlay_VideoFairPlay               (1 << 2)
#define AirPlay_VideoVolumeControl          (1 << 3)
#define AirPlay_VideoHTTPLiveStreams        (1 << 4)
#define AirPlay_Slideshow                   (1 << 5) 
#define AirPlay_Screen                      (1 << 7)
#define AirPlay_ScreenRotate                (1 << 8)
#define AirPlay_Audio                       (1 << 9)
#define AirPlay_AudioRedundant              (1 << 11)
#define AirPlay_FPSAPv2pt5_AES_GCM          (1 << 12)
#define AirPlay_PhotoCaching                (1 << 13)

#include <sstream>

int main()
{
#if 1
    std::string type("_airplay._tcp");
    std::string domain("local.");
#else
    std::string type("_http._tcp");
    std::string domain("local.");
#endif

    Nsd::NsdDiscoveryHandlerPointer myDiscoveryHandler = boost::make_shared<AirPlayNsdDiscoveryHandler>();
    gNsd.discoveryService(type, domain, myDiscoveryHandler);

    Nsd::NsdServiceInfoPointer s = boost::make_shared<NsdServiceInfo>("W500", type, domain);
    s->setPort(7000);

    uint32_t features = AirPlay_Video
        | AirPlay_Photo
        | AirPlay_VideoVolumeControl
        | AirPlay_VideoHTTPLiveStreams
        | AirPlay_Slideshow
        | AirPlay_PhotoCaching;

    std::stringstream ssFeatures;
    ssFeatures << "0x" << std::hex << features;
    //std::cout << "features: " << ssFeatures.str() << std::endl;
    //std::cout << "f:" << features  << std::endl;

    s->setTXTRecordValueWithKey("deviceid", "00:27:13:B6:0B:71");
    s->setTXTRecordValueWithKey("features", ssFeatures.str().c_str());
    s->setTXTRecordValueWithKey("model", "AppleTV2,1");
    s->setTXTRecordValueWithKey("srcvers", "101.28");
    //s->setTXTRecordValueWithKey("srcvers", "130.14");

    Nsd::NsdPublishHandlerPointer myPublishHandler = boost::make_shared<AirPlayNsdPublishHandler>();
    gNsd.publishService(s, myPublishHandler);

    command();

    return 0;
}
