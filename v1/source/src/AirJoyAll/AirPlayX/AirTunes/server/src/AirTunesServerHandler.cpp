/*
* AirTunesServerHandler.cpp
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

#include "AirTunesServerHandler.hpp"
#include "AirTunesServerListener.hpp"
#include "AirTunesServer.hpp"
#include "TcpSession.hpp"
#include "AirBase64.hpp"
#include "UtilString.hpp"
#include "HttpTextParam.hpp"
#include "AirPort.hpp"
#include <vector>
#include "AudioPlayer.hpp"
#include "AudioBuffer.hpp"
#include "Dmap.hpp"
#include "boost/xpressive/xpressive_dynamic.hpp"

#ifdef __USE_LIB_AO__
#include <ao/ao.h>
#endif // __USE_LIB_AO__

using namespace airjoy;

// #define AIRTUNES_DEBUG  1

// service version

//#define DEFAULT_SERVICE_VERSION             "104.29"
//#define SERVICE                             "AirTunes/104.29"

#define DEFAULT_SERVICE_VERSION             "150.33"
#define SERVICE                             "AirTunes/150.33"

AirTunesServerHandler::AirTunesServerHandler(AirTunesServer *server, AirTunesServerListener *listener)
    : m_server(server)
    , m_listener(listener)
    , m_aesiv(NULL)
    , m_aeskey(NULL)
    , m_audioBuffer(NULL)
    , m_audioPlayer(NULL)
    , m_airport(NULL)
    , m_streamType(StreamTypeAirTunes)
{

    // 2013-3-1, move ao_initialize() from AudioDriver.hpp
#ifdef __USE_LIB_AO__
    ao_initialize();
#endif // __USE_LIB_AO__
}

AirTunesServerHandler::~AirTunesServerHandler()
{
    if (m_aesiv != NULL)
        free(m_aesiv);

    if (m_aeskey != NULL)
        free(m_aeskey);

    this->releaseAirPortIfNecessary();
}

void AirTunesServerHandler::setDacpServerIp(const std::string &ip)
{
    m_dacpServerIp = ip;
}

void AirTunesServerHandler::setDacpServerHostName(const std::string &hostName)
{
    m_dacpServerHostName = hostName;
}

void AirTunesServerHandler::setDacpServerActiveRemote(const std::string &activeRemote)
{
    m_dacpServerActiveRemote = activeRemote;
}

const std::string & AirTunesServerHandler::dacpServerIp(void)
{
    return m_dacpServerIp;
}

const std::string & AirTunesServerHandler::dacpServerHostName(void)
{
    return m_dacpServerHostName;
}

const std::string & AirTunesServerHandler::dacpServerActiveRemote(void)
{
    return m_dacpServerActiveRemote;
}

void AirTunesServerHandler::didClose(int connectedId)
{
    //std::cout << "AirTunesServerHandler didClose(): " << connectedId << std::endl;
   
    this->releaseAirPortIfNecessary();

    this->removeConnection(connectedId);

    this->removeRtspParser(connectedId);
}

bool AirTunesServerHandler::insertRtspParser(int connectedId)
{
    LockType lock(m_rtspParsersMutex);
    RtspParserMap::iterator iter = m_rtspParsers.find(connectedId);
    if (iter == m_rtspParsers.end())
    {
        m_rtspParsers.insert(std::make_pair(connectedId, boost::make_shared<RtspMessage>()));
        return true;
    }

    return false;
}

bool AirTunesServerHandler::removeRtspParser(int connectedId)
{
    LockType lock(m_rtspParsersMutex);
    RtspParserMap::iterator iter = m_rtspParsers.find(connectedId);
    if (iter != m_rtspParsers.end())
    {
        m_rtspParsers.erase(iter);
        return true;
    }

    return false;
}

AirTunesServerHandler::RtspMessagePointer AirTunesServerHandler::getRtspParser(int connectedId)
{
    LockType lock(m_rtspParsersMutex);
    RtspParserMap::iterator iter = m_rtspParsers.find(connectedId);
    if (iter != m_rtspParsers.end())
        return iter->second;

    RtspMessagePointer pRtspMessage = boost::make_shared<RtspMessage>();
    m_rtspParsers.insert(std::make_pair(connectedId, pRtspMessage));
    return pRtspMessage;
}

bool AirTunesServerHandler::didReceiveMessage(TcpServerMessagePointer request)
{
    //std::cout << "----------------- didReceiveRequest: -------------" << std::endl;
    //std::cout << "connectedId: " << request->session()->connectedId() << std::endl;
    //std::cout << request->message() << std::endl;
    
    RtspMessagePointer pRtspMessage = this->getRtspParser(request->session()->connectedId());
    RtspParserStatus status = pRtspMessage->loadBytes(request->message().c_str(), request->message().length());
    if (status == RtspParserDone)
    {
    #ifdef AIRTUNES_DEBUG
        std::cout << "----------------- didReceiveRequest: -------------: " << request->session()->connectedId() << std::endl;
        pRtspMessage->print();
    #endif

        AirTunesStatus status = AIRTUNES_STATUS_UNKNOWN;
        std::string responseContent;

        if (pRtspMessage->rtspType() == RtspRequest)
        {
            if (pRtspMessage->rtspMethod() == "POST")
            {
                status = doPost(request, pRtspMessage, responseContent);
            }
            if (pRtspMessage->rtspMethod() == "OPTIONS")
            {
                status = doOptions(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "ANNOUNCE")
            {
                status = doAnnounce(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "SETUP")
            {
                status = doSetup(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "RECORD")
            {
                status = doRecord(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "FLUSH")
            {
                status = doFlush(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "TEARDOWN")
            {
                status = doTeardown(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "GET_PARAMETER")
            {
                status = doGetParameter(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "SET_PARAMETER")
            {
                status = doSetParameter(request, pRtspMessage, responseContent);
            }
#if 0
            else if (pRtspMessage->rtspMethod() == "DESCRIBE")
            {
                status = doDescribe(request, pRtspMessage, responseContent);
            }

            else if (pRtspMessage->rtspMethod() == "PAUSE")
            {
                status = doPause(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "PLAY")
            {
                status = doPlay(request, pRtspMessage, responseContent);
            }
            else if (pRtspMessage->rtspMethod() == "REDIRECT")
            {
                status = doRedirect(request, pRtspMessage, responseContent);
            }
#endif
            else
            {
                status = AIRTUNES_STATUS_NOT_IMPLEMENTED;
            }
        }
        else
        {
            status = AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
        }
        
        switch (status)
        {
        case AIRTUNES_STATUS_UNKNOWN:
            break;

        case AIRTUNES_STATUS_OK:
            break;

        case AIRTUNES_STATUS_NOT_FOUND:
            break;

        case AIRTUNES_STATUS_METHOD_NOT_ALLOWED:
            break;

        case AIRTUNES_STATUS_NOT_IMPLEMENTED:
            break;

        case AIRTUNES_STATUS_NO_RESPONSE_NEEDED:
            break;

        default:
            break;
        }

        // We need a new RtspMessage ...
        this->removeRtspParser(request->session()->connectedId());

        return true;
    }

    return true;
}

void AirTunesServerHandler::sendMessage(TcpSessionPointer session, const std::string & message)
{
    TcpServerMessagePointer response = boost::make_shared<TcpServerMessage>(session);
    response->message() = message;

    session->startWrite(response);
}

void AirTunesServerHandler::sendMessage(TcpSessionPointer session, 
                                        int statusCode, 
                                        const std::string &statusString,
                                        const std::string &server,
                                        const std::string &seq)
{
    RtspMessage rtspResponse;
    rtspResponse.setRtspType(RtspResponse);
    rtspResponse.setVersion(1, 0);
    rtspResponse.setStatus(statusCode, statusString);
    rtspResponse.addHeaders("Server", server);
    rtspResponse.addHeaders("CSeq", seq);

#ifdef AIRTUNES_DEBUG
    std::cout << "---------------------- startWrite ----------------------" << std::endl;
    rtspResponse.print();
#endif

    this->sendMessage(session, rtspResponse.toString());
}

AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doPost(TcpServerMessagePointer request, 
                                                                    RtspMessagePointer pRtspMessage, 
                                                                    std::string &responseContent)
{
    //std::cout << "AirTunesServerHandler::doPost" << std::endl;

    if (pRtspMessage->uri() == "/fp-setup")
    {
        this->doFpSetup(request, pRtspMessage, responseContent);
    }
    else if (pRtspMessage->uri() == "/auth-setup")
    {
    }

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doFpSetup(TcpServerMessagePointer request,
                                                                       RtspMessagePointer pRtspMessage, 
                                                                       std::string &responseContent)
{
    // support ios7, 2014.6.8
#if 1
    // 2 1 1 -> 4 : 02 00 02 bb
    uint8_t fply_1[] = {
        0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x02, 0xbb
    };

    // 2 1 2 -> 130 : 02 02 xxx
    uint8_t fply_2[] = {
        0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x82,
        0x02, 0x02, 0x2f, 0x7b, 0x69, 0xe6, 0xb2, 0x7e, 0xbb, 0xf0, 0x68, 0x5f, 0x98, 0x54, 0x7f, 0x37,
        0xce, 0xcf, 0x87, 0x06, 0x99, 0x6e, 0x7e, 0x6b, 0x0f, 0xb2, 0xfa, 0x71, 0x20, 0x53, 0xe3, 0x94,
        0x83, 0xda, 0x22, 0xc7, 0x83, 0xa0, 0x72, 0x40, 0x4d, 0xdd, 0x41, 0xaa, 0x3d, 0x4c, 0x6e, 0x30,
        0x22, 0x55, 0xaa, 0xa2, 0xda, 0x1e, 0xb4, 0x77, 0x83, 0x8c, 0x79, 0xd5, 0x65, 0x17, 0xc3, 0xfa,
        0x01, 0x54, 0x33, 0x9e, 0xe3, 0x82, 0x9f, 0x30, 0xf0, 0xa4, 0x8f, 0x76, 0xdf, 0x77, 0x11, 0x7e,
        0x56, 0x9e, 0xf3, 0x95, 0xe8, 0xe2, 0x13, 0xb3, 0x1e, 0xb6, 0x70, 0xec, 0x5a, 0x8a, 0xf2, 0x6a,
        0xfc, 0xbc, 0x89, 0x31, 0xe6, 0x7e, 0xe8, 0xb9, 0xc5, 0xf2, 0xc7, 0x1d, 0x78, 0xf3, 0xef, 0x8d,
        0x61, 0xf7, 0x3b, 0xcc, 0x17, 0xc3, 0x40, 0x23, 0x52, 0x4a, 0x8b, 0x9c, 0xb1, 0x75, 0x05, 0x66,
        0xe6, 0xb3
    };

    // 2 1 3 -> 152
    // 4 : 02 8f 1a 9c
    // 128 : xxx
    // 20 : 5b ed 04 ed c3 cd 5f e6 a8 28 90 3b 42 58 15 cb 74 7d ee 85

    uint8_t fply_3[] = {
        0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x98, 0x02, 0x8f,
        0x1a, 0x9c, 0x6e, 0x73, 0xd2, 0xfa, 0x62, 0xb2, 0xb2, 0x07, 0x6f, 0x52, 0x5f, 0xe5, 0x72, 0xa5,
        0xac, 0x4d, 0x19, 0xb4, 0x7c, 0xd8, 0x07, 0x1e, 0xdb, 0xbc, 0x98, 0xae, 0x7e, 0x4b, 0xb4, 0xb7,
        0x2a, 0x7b, 0x5e, 0x2b, 0x8a, 0xde, 0x94, 0x4b, 0x1d, 0x59, 0xdf, 0x46, 0x45, 0xa3, 0xeb, 0xe2,
        0x6d, 0xa2, 0x83, 0xf5, 0x06, 0x53, 0x8f, 0x76, 0xe7, 0xd3, 0x68, 0x3c, 0xeb, 0x1f, 0x80, 0x0e,
        0x68, 0x9e, 0x27, 0xfc, 0x47, 0xbe, 0x3d, 0x8f, 0x73, 0xaf, 0xa1, 0x64, 0x39, 0xf7, 0xa8, 0xf7,
        0xc2, 0xc8, 0xb0, 0x20, 0x0c, 0x85, 0xd6, 0xae, 0xb7, 0xb2, 0xd4, 0x25, 0x96, 0x77, 0x91, 0xf8,
        0x83, 0x68, 0x10, 0xa1, 0xa9, 0x15, 0x4a, 0xa3, 0x37, 0x8c, 0xb7, 0xb9, 0x89, 0xbf, 0x86, 0x6e,
        0xfb, 0x95, 0x41, 0xff, 0x03, 0x57, 0x61, 0x05, 0x00, 0x73, 0xcc, 0x06, 0x7e, 0x4f, 0xc7, 0x96,
        0xae, 0xba, 0x5b, 0xed, 0x04, 0xed, 0xc3, 0xcd, 0x5f, 0xe6, 0xa8, 0x28, 0x90, 0x3b, 0x42, 0x58,
        0x15, 0xcb, 0x74, 0x7d, 0xee, 0x85
    };

    // 2 1 4 -> 20 : 5b ed 04 ed c3 cd 5f e6 a8 28 90 3b 42 58 15 cb 74 7d ee 85
    uint8_t fply_4[] = {
        0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x14, 0x5b,
        0xed, 0x04, 0xed, 0xc3, 0xcd, 0x5f, 0xe6, 0xa8, 0x28, 0x90, 0x3b, 0x42, 0x58, 0x15, 0xcb, 0x74,
        0x7d, 0xee, 0x85
    };

    uint8_t fply_header[12];
    size_t len = sizeof(fply_header);
    memcpy(fply_header, pRtspMessage->content().c_str(), len);

    std::string payload(pRtspMessage->content().c_str() + len, pRtspMessage->content().size() - len);

    if (fply_header[6] == 1)
    {
        memcpy(fply_2 + 13, pRtspMessage->content().c_str() + 12 + 2, 1);

        std::string data((const char *)fply_2, sizeof(fply_2));
        
        std::stringstream ssSize;
        ssSize << data.length();

        std::string seq = pRtspMessage->getHeadValueByName("CSeq");

        RtspMessage rtspResponse;
        rtspResponse.setRtspType(RtspResponse);
        rtspResponse.setVersion(1, 0);
        rtspResponse.setStatus(200, "OK");
        rtspResponse.addHeaders("Server", SERVICE);
        rtspResponse.addHeaders("CSeq", seq);
        rtspResponse.addHeaders("Content-Type", "application/octet-stream");
	    rtspResponse.addHeaders("Content-Length", ssSize.str());
        rtspResponse.setContent(data);

    #ifdef AIRTUNES_DEBUG
        std::cout << "---------------------- startWrite ----------------------" << std::endl;
        rtspResponse.print();
    #endif

        this->sendMessage(request->session(), rtspResponse.toString());
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }
    else if (fply_header[6] == 3)
    {
        std::string data((const char *)fply_4, 12);
        data.append(payload.c_str() + payload.size() - 20, 20);

        std::stringstream ssSize;
        ssSize << data.length();

        std::string seq = pRtspMessage->getHeadValueByName("CSeq");

        RtspMessage rtspResponse;
        rtspResponse.setRtspType(RtspResponse);
        rtspResponse.setVersion(1, 0);
        rtspResponse.setStatus(200, "OK");
        rtspResponse.addHeaders("Server", SERVICE);
        rtspResponse.addHeaders("CSeq", seq);
        rtspResponse.addHeaders("Content-Type", "application/octet-stream");
	    rtspResponse.addHeaders("Content-Length", ssSize.str());
        rtspResponse.setContent(data);

    #ifdef AIRTUNES_DEBUG
        std::cout << "---------------------- startWrite ----------------------" << std::endl;
        rtspResponse.print();
    #endif

        this->sendMessage(request->session(), rtspResponse.toString());
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }
#endif

    std::string seq = pRtspMessage->getHeadValueByName("CSeq");
    this->sendMessage(request->session(), 200, "OK", SERVICE, seq);

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doOptions(TcpServerMessagePointer request, 
                                                                       RtspMessagePointer pRtspMessage, 
                                                                       std::string &responseContent)
{

#if 0
    if (! this->isCurrentConnection(request->session()))
    {
        this->disconnect(request->session()->getPeerIp());
        this->insertConnection(request->session());
        m_currentPlayIp = request->session()->getPeerIp();
    }
#endif

    if (m_currentPlayIp != request->session()->getPeerIp())
    {
        this->disconnect(m_currentPlayIp);
        m_currentPlayIp = request->session()->getPeerIp();
        this->insertConnection(request->session());
    }

    std::string seq = pRtspMessage->getHeadValueByName("CSeq");
    std::string challengeBase64 = pRtspMessage->getHeadValueByName("Apple-Challenge");
    std::string dacpId = pRtspMessage->getHeadValueByName("DACP-ID");
    std::string activeRemote = pRtspMessage->getHeadValueByName("Active-Remote");
    std::string responseBase64;
    
    if (! challengeBase64.empty())
    {
        TcpSession::Bytes ipAddress = request->session()->getSelfIPV4Bytes();

        unsigned char ip[16];
        memset(ip, 0, 16);
        size_t ipLength = ipAddress.size();
        for (size_t i = 0; i < ipLength; ++i)
            ip[i] = ipAddress[i];

        // 本地网卡，并且是此次连接的网卡，可以随便写，但必须和发布服务时的值保持一致
        //unsigned char macAddress[] = {0x00, 0x21, 0x6a, 0xb8, 0xa7, 0x34};
        //size_t macAddressLength = sizeof(macAddress);
    
        m_pAirCrypto = boost::make_shared<AirCrypto>();
        const char *response = m_pAirCrypto->getResponseForChallenge((unsigned char *)challengeBase64.c_str(), 
            challengeBase64.length(),
            ip,
            ipLength,
            m_serviceInfo->macAddress(),
            m_serviceInfo->macAddressLen());
        if (response != NULL)
            responseBase64.append(response);
    }

    RtspMessage rtspResponse;
    rtspResponse.setRtspType(RtspResponse);
    rtspResponse.setVersion(1, 0);
    rtspResponse.setStatus(200, "OK");
    rtspResponse.addHeaders("Public", "ANNOUNCE, SETUP, RECORD, PAUSE, FLUSH, TEARDOWN, OPTIONS, GET_PARAMETER, SET_PARAMETER, POST, GET");
    rtspResponse.addHeaders("Server", SERVICE);
    rtspResponse.addHeaders("CSeq", seq);

    if (! responseBase64.empty())
    {
        rtspResponse.addHeaders("Apple-Response", responseBase64);
    }
    
#ifdef AIRTUNES_DEBUG
    std::cout << "---------------------- startWrite ----------------------" << std::endl;
    rtspResponse.print();
#endif

    this->sendMessage(request->session(), rtspResponse.toString());

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

/*
ANNOUNCE rtsp://fe80::217:f2ff:fe0f:e0f6/3413821438 RTSP/1.0
CSeq: 3
Content-Type: application/sdp
Content-Length: 348
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Client-Instance: 56B29BB6CB904862
DACP-ID: 56B29BB6CB904862
Active-Remote: 1986535575

v=0
o=iTunes 3413821438 0 IN IP4 fe80::217:f2ff:fe0f:e0f6
s=iTunes
c=IN IP4 fe80::5a55:caff:fe1a:e187
t=0 0
m=audio 0 RTP/AVP 96
a=rtpmap:96 AppleLossless
a=fmtp:96 352 0 16 40 10 14 2 255 0 0 44100
a=fpaeskey:RlBMWQECAQAAAAA8AAAAAPFOnNe+zWb5/n4L5KZkE2AAAAAQlDx69reTdwHF9LaNmhiRURTAbcL4brYAceAkZ49YirXm62N4
a=aesiv:5b+YZi9Ikb845BmNhaVo+Q
*/

/*
-----------------
AirTunes
-----------------
ANNOUNCE rtsp://10.0.1.12/8811502524553481798 RTSP/1.0
active-remote: 1405175166
content-length: 638
content-type: application/sdp
cseq: 1
dacp-id: 195E483BC1306260
user-agent: AirPlay/160.10
x-apple-client-name: iPod Touch5
x-apple-device-id: 0x8c2daa89aa5a

v=0
o=AirTunes 8811502524553481798 0 IN IP4 10.0.1.78
s=AirTunes
i=iPod Touch5
c=IN IP4 10.0.1.78
t=0 0
m=audio 0 RTP/AVP 96
a=rtpmap:96 AppleLossless
a=fmtp:96 352 0 16 40 10 14 2 255 0 0 44100
a=rsaaeskey:P3M2/eoZ+asUB3LAwBBiHHLcymD1qLAtJkTENQ1CwVSnU0Y6B7Mn0r0Kb0nq1LODVSrsiXk6ukX06FBcaEzfKjSJCrZ6opODuOBGzGZCBCDPjHg3YVG1kAKThzCq+QMmmYiuMYLuM2ouzEBQoWip
cA7mgqrgPRdEWG486/DfRoYa668BROZGlh6Hw9yFXClYO/+iUKgHJGyp71qNxp7WPWXm5ldXAwDL/oLurdM0aF1YKrpjr1vBCc/wpEhJ1K5v28FM7H1TA9cW9hpbcgjANVFnyvA8itkmV+65w2UypU9gzpBJNMU2
9/Xah+4hqOlfNY6f9ROrVSFYUh+SCqcrcw==
a=aesiv:a2Yj8pKeHnrZ5X/uj+DbqQ==
a=min-latency:11025
a=max-latency:88200
*/

/*
-----------------
AirPlay Mirroring
-----------------
ANNOUNCE rtsp://10.0.1.12/3477538596077768058 RTSP/1.0
active-remote: 3266164082
content-length: 696
content-type: application/sdp
cseq: 1
dacp-id: 195E483BC1306260
user-agent: AirPlay/160.10
x-apple-client-name: iPod Touch5
x-apple-device-id: 0x8c2daa89aa5a

v=0
o=AirTunes 3477538596077768058 0 IN IP4 10.0.1.78
s=AirTunes
i=iPod Touch5
c=IN IP4 10.0.1.78
t=0 0
m=audio 0 RTP/AVP 96                    // media=audio
a=rtpmap:96 mpeg4-generic/44100/2
a=fmtp:96 mode=AAC-eld; constantDuration=480
a=rsaaeskey:lNo/ELRBBIKSeN3wmCfoQ30FslLcfqIeT589cTucGVzc8z75VchD7QTdkERKQuqJ0spG/EdpoK9dVPlpK/xgt2M3WRG/bSodPNXNhbdfj7s8MrxpRUBd/guWeolDiuBPAg8xtXZvzA9dZPdQn4tg
2RCaPeIiSH2k/rLC6f1ze25bFfzdLUekkRuy4u7JOqBXO0f/74iVnlCf2+KNOeT9bom9FEFzR2Zk0jVUI0zGhpWyyOvaqDLIRH64N8sD7BilWQO9Nv8nWgFkh1ibZXQdKWVV07fIbUah36cQCvxjU4+FupHr5Jvp
Aza2PnsqAkrWFfFoPuW4FXirm+QEJrBwvg==
a=aesiv:p9pP+7+VuRzW9ut1vyaF3w==
a=min-latency:4410
a=max-latency:4410
m=video 0 RTP/AVP 97                    // media=video
a=rtpmap:97 H264
a=fmtp:97
*/
AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doAnnounce(TcpServerMessagePointer request,
                                                                        RtspMessagePointer pRtspMessage,
                                                                        std::string &responseContent)
{
    //pRtspMessage->print();

    std::string url = pRtspMessage->uri();
    std::string contentType = pRtspMessage->getHeadValueByName("Content-Type");
    std::string clientInstance  = pRtspMessage->getHeadValueByName("Client-Instance");
    std::string dacpId  = pRtspMessage->getHeadValueByName("DACP-ID");
    std::string activeRemote  = pRtspMessage->getHeadValueByName("Active-Remote");
    std::string appleDeviceId = pRtspMessage->getHeadValueByName("X-Apple-Device-ID");
    std::string seq = pRtspMessage->getHeadValueByName("CSeq");

    this->setDacpServerIp(request->session()->getPeerIp());
    this->setDacpServerHostName("iPod-Touch5.local.");
    this->setDacpServerActiveRemote(activeRemote);

    // 取得 m=audio 和 m=video 信息
    MediaValueMap mediaKeyValue;
    this->getMediaValue(pRtspMessage->content(), mediaKeyValue);
    MediaValueMap::iterator pMedia = mediaKeyValue.find("video");
    if (pMedia != mediaKeyValue.end())
    {
        m_streamType = StreamTypeAirPlayMirroring;
    }
    else
    {
        m_streamType = StreamTypeAirTunes;
    }

    // 暂时不处理AirPlay Mirroring音频
    if (m_streamType == StreamTypeAirPlayMirroring)
    {
        this->sendMessage(request->session(), 200, "OK", SERVICE, seq);
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }

    // 取得音频参数信息
    KeyValueMap keyValue;
    this->getKeyValue(pRtspMessage->content(), keyValue);

    // 目前发现三种rtpmap:
    // a=rtpmap:96 AppleLossless
    // a=rtpmap:96 mpeg4-generic/44100/2
    // a=rtpmap:97 H264

    #define MEDIA_APPLE_LOSS_LESS               "AppleLossless"
    #define MEDIA_MPEG4                         "mpeg4-generic"
    #define MEDIA_H264                          "H264"

    KeyValueMap::iterator pos = keyValue.find("rtpmap");
    if (pos != keyValue.end())
    {
        unsigned int id = 0;
        char format[256];
        memset(format, 0, 256);

        sscanf(pos->second.c_str(), "%u %s", &id, format);
        m_mediaFormat.clear();
        m_mediaFormat.append(format);
    }

    // 苹果无损音频
    if (m_mediaFormat.find(MEDIA_APPLE_LOSS_LESS) != std::string::npos)
    {
        pos = keyValue.find("fmtp");
        if (pos != keyValue.end())
        {
            // a=fmtp:96 352 0 16 40 10 14 2 255 0 0 44100
            sscanf(pos->second.c_str(),
                "%u %u %u %u %u %u %u %u %u %u %u %u",
                &m_fmtp[0], &m_fmtp[1], &m_fmtp[2], &m_fmtp[3], &m_fmtp[4],
                &m_fmtp[5], &m_fmtp[6], &m_fmtp[7], &m_fmtp[8], &m_fmtp[9],
                &m_fmtp[10], &m_fmtp[11]);
        }
    }
    // MPEG4
    else if (m_mediaFormat.find(MEDIA_MPEG4) != std::string::npos)
    {
        m_streamType = StreamTypeAirPlayMirroring;
        this->sendMessage(request->session(), 200, "OK", SERVICE, seq);
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }
    // H264
    else if (m_mediaFormat.find(MEDIA_H264) != std::string::npos)
    {
        m_streamType = StreamTypeAirPlayMirroring;
        this->sendMessage(request->session(), 200, "OK", SERVICE, seq);
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }
    else
    {
        m_streamType = StreamTypeAirPlayMirroring;
        this->sendMessage(request->session(), 200, "OK", SERVICE, seq);
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }

    pos = keyValue.find("aesiv");
    if (pos != keyValue.end())
    {
        char encodeAesIV[1024];
        memset(encodeAesIV, 0, 1024);
        //util::getTrimmed(pos->second.c_str(), pos->second.length(), true, true, encodeAesIV);
        strncpy(encodeAesIV, pos->second.c_str(), pos->second.length());

        if (m_aesiv != NULL)
            free(m_aesiv);

        m_aesiv = (unsigned char *)airbase64::decode_base64((unsigned char*)encodeAesIV, strlen(encodeAesIV), &m_aesivLen);
    }
    
    pos = keyValue.find("rsaaeskey");
    if (pos != keyValue.end())
    {
        char encodeAesKey[1024];
        memset(encodeAesKey, 0, 1024);
        //util::getTrimmed(pos->second.c_str(), pos->second.length(), true, true, encodeAesKey);
        strncpy(encodeAesKey, pos->second.c_str(), pos->second.length());

        if (m_aeskey != NULL)
            free(m_aeskey);

        m_aeskey = (unsigned char *)airbase64::decode_base64((unsigned char*)encodeAesKey, strlen(encodeAesKey), &m_aeskeyLen);

        unsigned char decryptAesKey[1024];
        memset(decryptAesKey, 0, 1024);

        // jxfengzi@gmail.com, 2013-2-21
        m_aeskeyLen = RSA_private_decrypt(m_aeskeyLen, m_aeskey, decryptAesKey,  this->getAirCrypto()->privateKey(), RSA_PKCS1_OAEP_PADDING);

        memcpy(m_aeskey, decryptAesKey, m_aeskeyLen);
    }
    else
    {
        pos = keyValue.find("fpaeskey");
        if (pos != keyValue.end())
        {
            char encodeAesKey[1024];
            memset(encodeAesKey, 0, 1024);
            //util::getTrimmed(pos->second.c_str(), pos->second.length(), true, true, encodeAesKey);
            strncpy(encodeAesKey, pos->second.c_str(), pos->second.length());

            if (m_aeskey != NULL)
                free(m_aeskey);

            m_aeskey = (unsigned char *)airbase64::decode_base64((unsigned char*)encodeAesKey, strlen(encodeAesKey), &m_aeskeyLen);

            unsigned char decryptAesKey[1024];
            memset(decryptAesKey, 0, 1024);

            // bug !!!

            // jxfengzi@gmail.com, 2013-2-21
            m_aeskeyLen = RSA_private_decrypt(m_aeskeyLen, m_aeskey, decryptAesKey, this->getAirCrypto()->privateKey(), RSA_PKCS1_OAEP_PADDING);

            // memcpy(m_aeskey, decryptAesKey, m_aeskeyLen);
        }
    }

    this->sendMessage(request->session(), 200, "OK", SERVICE, seq);

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

/*
client → server
SETUP rtsp://fe80::217:f2ff:fe0f:e0f6/3413821438 RTSP/1.0
CSeq: 4
Transport: RTP/AVP/UDP;unicast;interleaved=0-1;mode=record;control_port=6001;timing_port=6002
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Client-Instance: 56B29BB6CB904862
DACP-ID: 56B29BB6CB904862
Active-Remote: 1986535575

server → client
RTSP/1.0 200 OK
Transport: RTP/AVP/UDP;unicast;mode=record;server_port=53561;control_port=63379;timing_port=50607
Session: 1
Audio-Jack-Status: connected
Server: AirTunes/130.14
CSeq: 4
*/

/*
-----------------
AirTunes
-----------------
SETUP rtsp://10.0.1.12/8811502524553481798/audio RTSP/1.0
active-remote: 1405175166
cseq: 2
dacp-id: 195E483BC1306260
transport: RTP/AVP/UDP;unicast;mode=record;timing_port=52419;events;control_port=50420
user-agent: AirPlay/160.10
*/

/*
-----------------
AirPlay Mirroring
-----------------
SETUP rtsp://10.0.1.12/3477538596077768058/audio RTSP/1.0
active-remote: 3266164082
cseq: 2
dacp-id: 195E483BC1306260
transport: RTP/AVP/UDP;unicast;mode=screen;timing_port=61849;events;control_port=54171;redundant=2
user-agent: AirPlay/160.10
sample_size : -842150451
*/

/*
-----------------
AirPlay Mirroring
-----------------
SETUP rtsp://10.0.1.12/3477538596077768058/video RTSP/1.0
active-remote: 3266164082
cseq: 3
dacp-id: 195E483BC1306260
transport: RTP/AVP/TCP;unicast;mode=record
user-agent: AirPlay/160.10
*/
AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doSetup(TcpServerMessagePointer request,
                                                                     RtspMessagePointer pRtspMessage,
                                                                     std::string &responseContent)
{
    // retrieve control_port and timing_port from the Transport header

    int controlPort = 0;
    int timingPort = 0;

    std::string url = pRtspMessage->uri();
    std::string transport = pRtspMessage->getHeadValueByName("Transport");
    std::string clientInstance  = pRtspMessage->getHeadValueByName("Client-Instance");
    std::string dacpId  = pRtspMessage->getHeadValueByName("DACP-ID");
    std::string activeRemote  = pRtspMessage->getHeadValueByName("Active-Remote");
    std::string seq = pRtspMessage->getHeadValueByName("CSeq");
    
    // 暂时不处理AirPlay Mirroring音视频消息
    if (m_streamType == StreamTypeAirPlayMirroring)
    {
        this->sendMessage(request->session(), 200, "OK", SERVICE, seq);
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }

#if 0
    // 暂时不处理AirPlay Mirroring视频
    if (pRtspMessage->uri().find("video") != std::string::npos)
    {
        this->sendMessage(request->session(), 404, "Not Found", SERVICE, seq);
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }
#endif

    if (transport.empty())
    {
        this->sendMessage(request->session(), 404, "Not Found", SERVICE, seq);
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }

    // 将transport中的值用分号分离成数组，取control_port和timing_port的值
    std::vector<std::string> settings;
    boost::split(settings, transport, boost::is_any_of(";"));

    std::vector<std::string>::iterator iter;
    for (iter = settings.begin(); iter != settings.end(); ++iter)
    {
        std::vector<std::string> keyValues;
        boost::split(keyValues, *iter, boost::is_any_of("="));
        if (keyValues[0] == "control_port")
        {
            try
            {
                controlPort = boost::lexical_cast<int>(keyValues[1]);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "AirTunesServerHandler::doSetup error: " << e.what() << std::endl;
            }
        }
        else if (keyValues[0] == "timing_port")
        {
            try
            {
                timingPort = boost::lexical_cast<int>(keyValues[1]);
            }
            catch (boost::bad_lexical_cast &e)
            {
                std::cout << "AirTunesServerHandler::doSetup error: " << e.what() << std::endl;
            }
        }
    }

    if (controlPort == 0 || timingPort == 0) 
    {
        this->sendMessage(request->session(), 404, "Not Found", SERVICE, seq);
        return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
    }

    // 创建数据接收器：启动3个UDP端口接收数据（数据端口、控制端口、同步端口），以及音频缓冲区、音频播放器
    this->createAirPort(request->session()->getPeerIp(), controlPort);

    std::stringstream ssDataPort;
    std::stringstream ssTimingPort;
    std::stringstream ssControlPort;
    ssDataPort << m_airport->dataPort();
    ssTimingPort << m_airport->timingPort();
    ssControlPort << m_airport->controlPort();

    std::string transportResponse("RTP/AVP/UDP;unicast;mode=record;server_port=");
    transportResponse.append(ssDataPort.str()).append(";control_port=").append(ssControlPort.str()).append(";timing_port=").append(ssTimingPort.str());

    RtspMessage rtspResponse;
    rtspResponse.setRtspType(RtspResponse);
    rtspResponse.setVersion(1, 0);
    rtspResponse.setStatus(200, "OK");
    rtspResponse.addHeaders("Transport", transportResponse);  // Transport: RTP/AVP/UDP;unicast;mode=record;server_port=53561;control_port=63379;timing_port=50607
    rtspResponse.addHeaders("Session", "DEADBEEF");
    rtspResponse.addHeaders("Audio-Jack-Status", "connected");
    rtspResponse.addHeaders("Server", SERVICE);
    rtspResponse.addHeaders("CSeq", seq);

#ifdef AIRTUNES_DEBUG
    std::cout << "---------------------- startWrite ----------------------" << std::endl;
    rtspResponse.print();
#endif

    this->sendMessage(request->session(), rtspResponse.toString());

    if (m_listener != NULL)
        m_listener->didStartPlayAudio(request->session()->getPeerIp());

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

/*
client → server
RECORD rtsp://fe80::217:f2ff:fe0f:e0f6/3413821438 RTSP/1.0
CSeq: 5
Session: 1
Range: npt=0-
RTP-Info: seq=20857;rtptime=1146549156
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Client-Instance: 56B29BB6CB904862
DACP-ID: 56B29BB6CB904862
Active-Remote: 1986535575

server → client
RTSP/1.0 200 OK
Audio-Latency: 2205
Server: AirTunes/130.14
CSeq: 5
*/

/*
-----------------
AirTunes
-----------------
RECORD rtsp://10.0.1.12/8811502524553481798 RTSP/1.0
active-remote: 1405175166
cseq: 3
dacp-id: 195E483BC1306260
range: npt=0-
user-agent: AirPlay/160.10
x-apple-durations: b=4; c=145; au=344; an=143; sa=509; ss=0
*/
AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doRecord(TcpServerMessagePointer request,
                                                                      RtspMessagePointer pRtspMessage,
                                                                      std::string &responseContent)
{
    /*
        RTP-Info: seq=20857;rtptime=1146549156
        seq	16-bit	initial RTP sequence number
        rtptime	32-bit	initial RTP timestamp
    */

    std::string url = pRtspMessage->uri();
    std::string session = pRtspMessage->getHeadValueByName("Session");
    std::string range = pRtspMessage->getHeadValueByName("Range");
    std::string rtpInfo = pRtspMessage->getHeadValueByName("RTP-Info");
    std::string clientInstance = pRtspMessage->getHeadValueByName("Client-Instance");
    std::string activeRemote  = pRtspMessage->getHeadValueByName("Active-Remote");
    std::string dacpId = pRtspMessage->getHeadValueByName("DACP-ID");
    std::string seq = pRtspMessage->getHeadValueByName("CSeq");

    RtspMessage rtspResponse;
    rtspResponse.setRtspType(RtspResponse);
    rtspResponse.setVersion(1, 0);
    rtspResponse.setStatus(200, "OK");
    rtspResponse.addHeaders("Audio-Latency", "2205");
    rtspResponse.addHeaders("Server", SERVICE);
    rtspResponse.addHeaders("CSeq", seq);

#ifdef AIRTUNES_DEBUG
    std::cout << "---------------------- startWrite ----------------------" << std::endl;
    rtspResponse.print();
#endif

    this->sendMessage(request->session(), rtspResponse.toString());

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

/*
client → server
FLUSH rtsp://fe80::217:f2ff:fe0f:e0f6/3413821438 RTSP/1.0
CSeq: 31
Session: 1
RTP-Info: seq=25009;rtptime=1148010660
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Client-Instance: 56B29BB6CB904862
DACP-ID: 56B29BB6CB904862
Active-Remote: 1986535575

server → client
RTSP/1.0 200 OK
RTP-Info: rtptime=1147914212
Server: AirTunes/130.14
CSeq: 31
*/
AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doFlush(TcpServerMessagePointer request,
                                                                     RtspMessagePointer pRtspMessage,
                                                                     std::string &responseContent)
{
    std::string url = pRtspMessage->uri();
    std::string session = pRtspMessage->getHeadValueByName("Session");
    std::string rtpInfo = pRtspMessage->getHeadValueByName("RTP-Info");
    std::string clientInstance = pRtspMessage->getHeadValueByName("Client-Instance");
    std::string activeRemote  = pRtspMessage->getHeadValueByName("Active-Remote");
    std::string dacpId = pRtspMessage->getHeadValueByName("DACP-ID");
    std::string seq = pRtspMessage->getHeadValueByName("CSeq");

    RtspMessage rtspResponse;
    rtspResponse.setRtspType(RtspResponse);
    rtspResponse.setVersion(1, 0);
    rtspResponse.setStatus(200, "OK");
    rtspResponse.addHeaders("RTP-Info", "rtptime=1147914212");
    rtspResponse.addHeaders("Server", SERVICE);
    rtspResponse.addHeaders("CSeq", seq);

#ifdef AIRTUNES_DEBUG
    std::cout << "---------------------- startWrite ----------------------" << std::endl;
    rtspResponse.print();
#endif

    this->sendMessage(request->session(), rtspResponse.toString());

    if (m_audioPlayer != NULL)
        m_audioPlayer->flushDecoder();

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

/*
client → server
TEARDOWN rtsp://fe80::217:f2ff:fe0f:e0f6/3413821438 RTSP/1.0
CSeq: 32
Session: 1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Client-Instance: 56B29BB6CB904862
DACP-ID: 56B29BB6CB904862
Active-Remote: 1986535575

server → client
RTSP/1.0 200 OK
Server: AirTunes/130.14
CSeq: 32
*/
AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doTeardown(TcpServerMessagePointer request,
                                                                        RtspMessagePointer pRtspMessage,
                                                                        std::string &responseContent)
{
    std::string seq = pRtspMessage->getHeadValueByName("CSeq");

    this->sendMessage(request->session(), 200, "OK", SERVICE, seq);
    
    if (m_listener != NULL)
        m_listener->didStop(request->session()->getPeerIp());

    m_currentPlayIp.clear();

    this->releaseAirPortIfNecessary();

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

/*
GET_PARAMETER rtsp://10.0.1.76/1461047585732353411/audio RTSP/1.0
CSeq: 4
DACP-ID: 4264892F04A3AE81
Active-Remote: 1923815866
Content-Type: text/parameters
Content-Length: 8
User-Agent: AirPlay/160.10

volume
*/
AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doGetParameter(TcpServerMessagePointer request,
                                                                            RtspMessagePointer pRtspMessage, 
                                                                            std::string &responseContent)
{
    std::string seq = pRtspMessage->getHeadValueByName("CSeq");
    std::string contentType = pRtspMessage->getHeadValueByName("content-type");

    if (contentType == "text/parameters")
    {
        //std::cout << pRtspMessage->content() << std::endl;

        // 这里需要回复音量参数
        if (pRtspMessage->content() == "volume")
        {
            this->sendMessage(request->session(), 200, "OK", SERVICE, seq);
            return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
        }
    }

    this->sendMessage(request->session(), 200, "OK", SERVICE, seq);
    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

/*
SET_PARAMETER rtsp://10.0.1.76/1461047585732353411/audio RTSP/1.0
CSeq: 5
Content-Type: text/parameters
Content-Length: 20
User-Agent: AirPlay/160.10

volume: -21.000000
*/
AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doSetParameter(TcpServerMessagePointer request, 
                                                                            RtspMessagePointer pRtspMessage, 
                                                                            std::string &responseContent)
{
    std::string contentType = pRtspMessage->getHeadValueByName("content-type");

    if (contentType == "text/parameters")
    {
        this->doSetText(request, pRtspMessage);
    }
    else if (contentType.find("image/") != std::string::npos)
    {
        this->doSetImage(request, pRtspMessage);
    }
    else if (contentType == "application/x-dmap-tagged")
    {
        this->doSetMetadata(request, pRtspMessage);
    }

    std::string seq = pRtspMessage->getHeadValueByName("CSeq");
    this->sendMessage(request->session(), 200, "OK", SERVICE, seq);

    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

#if 0
AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doDescribe(TcpServerMessagePointer request, 
                                                                        RtspMessagePointer pRtspMessage, 
                                                                        std::string &responseContent)
{
    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doPause(TcpServerMessagePointer request,
                                                                     RtspMessagePointer pRtspMessage, 
                                                                     std::string &responseContent)
{
    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doPlay(TcpServerMessagePointer request,
                                                                    RtspMessagePointer pRtspMessage,
                                                                    std::string &responseContent)
{
    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}

AirTunesServerHandler::AirTunesStatus AirTunesServerHandler::doRedirect(TcpServerMessagePointer request,
                                                                        RtspMessagePointer pRtspMessage,
                                                                        std::string &responseContent)
{
    return AIRTUNES_STATUS_NO_RESPONSE_NEEDED;
}
#endif

void AirTunesServerHandler::doSetText(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage)
{
    HttpTextParam textParam;
    textParam.loadBytes(pRtspMessage->content().c_str(), pRtspMessage->content().length());
    
    std::string value = textParam.getValueByKey("volume");
    if (! value.empty())
    {
        // volume: -11.123877

        float valume = 0.0;

        try
        {
            valume = boost::lexical_cast<float>(value);
        }
        catch (boost::bad_lexical_cast &e)
        {
            std::cout << "AirTunesServerHandler::doSetText volume error: " << e.what() << std::endl;
        }

        if (m_listener != NULL)
            m_listener->didSetVolume(valume, request->session()->getPeerIp());

        if (m_audioPlayer != NULL)
            m_audioPlayer->volumeDecoder(valume);

        return;
    }

    value = textParam.getValueByKey("progress");
    if (! value.empty())
    {
        // progress: 1146221540/1146549156/1195701740

        std::vector<std::string> timestamps;
        boost::split(timestamps, value, boost::is_any_of("/"));

        if (timestamps.size() != 3)
            return;

        uint64_t start = 0;
        uint64_t curr = 0;
        uint64_t end = 0;

        try
        {
            start = boost::lexical_cast<uint64_t>(timestamps[0]);
            curr = boost::lexical_cast<uint64_t>(timestamps[1]);
            end = boost::lexical_cast<uint64_t>(timestamps[2]);

            uint64_t position = curr - start;
            uint64_t duration = end - start;
            float pos = (float)position / (float)duration;

        #if 0
            // jxfengzi@gmail.com，音频进度，此值只有在本地视频推送时，才发送，意义不大。
            if (m_listener != NULL)
                m_listener->didSetProgress(pos, request->session()->getPeerIp());
        #endif
        }
        catch (boost::bad_lexical_cast &e)
        {
            std::cout << "AirTunesServerHandler::doSetText progress error: " << e.what() << std::endl;
        }

        return;
    }
}

void AirTunesServerHandler::doSetImage(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage)
{
    if (m_listener != NULL)
        m_listener->didSetImage(pRtspMessage->content().c_str(), pRtspMessage->content().length(), request->session()->getPeerIp());
}

void AirTunesServerHandler::doSetMetadata(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage)
{
    Dmap dmap(pRtspMessage->content());

    std::string itemname("dmap.itemname");
    std::string songartist("daap.songartist");
    std::string songalbum("daap.songalbum");

    std::string name = dmap.get(itemname);
    std::string artist = dmap.get(songartist);
    std::string album = dmap.get(songalbum);

    if (m_listener != NULL)
        m_listener->didSetAudioInfo(name, artist, album, request->session()->getPeerIp());
}

void AirTunesServerHandler::getKeyValue(const std::string &text, KeyValueMap &map)
{
    //
    // a=([^:]+):([^\\r^\\n]+)
    // 

    std::string regex("a=([^:]+):([^\\r^\\n]+)");

    using namespace boost::xpressive;
    sregex reg = sregex::compile(regex.c_str(), icase);

    sregex_iterator pos(text.begin(), text.end(), reg);
    sregex_iterator end;

    while (pos != end)
    {
        map.insert(std::make_pair((*pos)[1], (*pos)[2]));
        ++pos;
    }
}

void AirTunesServerHandler::getMediaValue(const std::string &text, MediaValueMap &map)
{
    //    [1]     [2]     [3]     [4]
    // m=([^ ]+) ([^ ]+) ([^ ]+) ([^\r^\n]+)
    // m=audio 0 RTP/AVP 96
    // [0] m=audio 0 RTP/AVP 96 
    // [1] audio
    // [2] 0
    // [3] RTP/AVP
    // [4] 96

    std::string regex("m=([^ ]+) ([^ ]+) ([^ ]+) ([^\r^\n]+)");

    using namespace boost::xpressive;
    sregex reg = sregex::compile(regex.c_str(), icase);

    sregex_iterator pos(text.begin(), text.end(), reg);
    sregex_iterator end;

    while (pos != end)
    {
        MediaValuePointer value = boost::make_shared<MediaValue>();
        value->value[0] = (*pos)[2];
        value->value[1] = (*pos)[3];
        value->value[2] = (*pos)[4];

        map.insert(std::make_pair((*pos)[1], value));
        ++pos;
    }
}

AirTunesServerHandler::AirCryptoPointer AirTunesServerHandler::getAirCrypto(void)
{
    if (m_pAirCrypto.use_count() == 0)
        m_pAirCrypto = boost::make_shared<AirCrypto>();

    return m_pAirCrypto;
}

void AirTunesServerHandler::createAirPort(const std::string &clientIp, uint16_t controlPort)
{
    this->releaseAirPortIfNecessary();

    m_airport = new AirPort;
    m_airport->setClientIp(clientIp);
    m_airport->setClientControlPort(controlPort);

    // 创建音频播放器
    m_audioPlayer = new AudioPlayer(this->m_listener);
    m_audioPlayer->setFmtp(m_fmtp, 12);
    m_audioPlayer->setAesIV(m_aesiv, m_aesivLen);
    m_audioPlayer->setAesKey(m_aeskey, m_aeskeyLen);
    m_audioPlayer->initDecoder();

    // 创建音频缓冲区
    m_audioBuffer = new AudioBuffer(m_airport, m_audioPlayer);

    // 启动音频播放器
    m_audioPlayer->setBuffer(m_audioBuffer);
    m_audioPlayer->start();
    
    // 启动数据接收器
    m_airport->setBuffer(m_audioBuffer);
    m_airport->start();
}

void AirTunesServerHandler::releaseAirPortIfNecessary(void)
{
    // 停止接收数据
    if (m_airport != NULL)
        m_airport->stop();

    // 停止音频播放
    if (m_audioPlayer != NULL)
        m_audioPlayer->stop();

    // 关闭数据接收端口
    if (m_airport != NULL)
    {
        delete m_airport;
        m_airport = NULL;
    }

    // 关闭音频缓冲
    if (m_audioBuffer != NULL)
    {
        delete m_audioBuffer;
        m_audioBuffer = NULL;
    }

    // 关闭音频播放器
    if (m_audioPlayer != NULL)
    {
        delete m_audioPlayer;
        m_audioPlayer = NULL;
    }
}

bool AirTunesServerHandler::isCurrentConnection(TcpSessionPointer session)
{
    LockType lock(m_connectionMutex);
    ConnectionMap::iterator iter = m_connections.find(session);
    if (iter == m_connections.end())
        return false;
    
    return true;
}

void AirTunesServerHandler::insertConnection(TcpSessionPointer session)
{
    //std::cout << "AirTunesServerHandler::insertConnection: "<< session->getPeerIp() << std::endl;

    LockType lock(m_connectionMutex);
    m_connections.insert(std::make_pair(session, session->getPeerIp()));
}

void AirTunesServerHandler::closeConnection(const std::string &ip)
{
    LockType lock(m_connectionMutex);
    ConnectionMap::iterator iter = m_connections.begin();
    for (; iter != m_connections.end(); ++iter)
	{
        if (iter->second == ip)
        {
            //std::cout << "AirTunesServerHandler::closeConnection: "<< ip << std::endl;
            iter->first->close();
            return;
        }
    }
}

void AirTunesServerHandler::removeConnection(int connectId)
{
    LockType lock(m_connectionMutex);
    ConnectionMap::iterator iter = m_connections.begin();
    for (; iter != m_connections.end(); ++iter)
	{
        if (iter->first->connectedId() == connectId)
        {
            //std::cout << "AirTunesServerHandler::removeConnection: "<< iter->second << std::endl;
            m_connections.erase(iter);
            return;
        }
    }
}

void AirTunesServerHandler::disconnect(const std::string &ip)
{
    LockType lock(m_connectionMutex);
    ConnectionMap::iterator iter = m_connections.begin();
    for (; iter != m_connections.end(); ++iter)
	{
        if (iter->second == ip)
        {
            //std::cout << "AirTunesServerHandler::disconnect: "<< ip << std::endl;
            m_server->sendDacpCommand(ip, AirTunesServer::DACP_CMD_STOP);
            iter->first->close();
            this->releaseAirPortIfNecessary();
            return;
        }
    }
}

void AirTunesServerHandler::stopAudioPlayer(const std::string &ip)
{
#if 0
    LockType lock(m_connectionMutex);
    ConnectionMap::iterator iter = m_connections.begin();
    for (; iter != m_connections.end(); ++iter)
	{
        if (iter->second == ip)
        {
            iter->first->close();
            return;
        }
    }
#else
        
    this->releaseAirPortIfNecessary();

    LockType lock(m_connectionMutex);
    ConnectionMap::iterator iter = m_connections.begin();
    for (; iter != m_connections.end(); ++iter)
	{
        if (iter->second == ip)
        {
            iter->first->close();
            return;
        }
    }
#endif

    //this->closeConnection(ip);
}
