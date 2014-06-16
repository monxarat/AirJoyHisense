/*
 * AirPlayClient.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "AirPlayClient.hpp"
#include "AnyPlayCore.hpp"
#include "UmpClient.hpp"
#include "AirPlayClientHandler.hpp"
#include "HttpMessage.hpp"

using namespace anyplay;

AirPlayClient::AirPlayClient(AnyPlayCore * core)
    : m_core(core)
    , m_photo(NULL)
    , m_video(NULL)
    , m_requestType(AirPlayClient::RequestTypeUnknown)
    , m_isStart(false)
    , m_isConnected(false)
{
    m_photo = new AirPlayClientPhoto(this);
    m_video = new AirPlayClientVideo(this);
}

AirPlayClient::~AirPlayClient()
{
    delete m_video;
    delete m_photo;
}
        
AirPlayClient::AirPlayClientRequestType AirPlayClient::requestType(void)
{
    return m_requestType;
}

void AirPlayClient::setRequestType(AirPlayClientRequestType t)
{
    m_requestType = t;
}

void AirPlayClient::resetRequestType(void)
{
    m_requestType = AirPlayClient::RequestTypeUnknown;
}

void AirPlayClient::start(AirPlayClientListener *listener)
{
    if (m_isStart)
        return;
    
    m_isStart = true;

    typedef boost::shared_ptr<AirPlayClientHandler>         AirPlayClientHandlerPointer;
    AirPlayClientHandlerPointer clientHandler = boost::make_shared<AirPlayClientHandler>(this, listener);
    m_core->airplayClient()->setHandler(clientHandler);
    m_core->airplayClient()->start();
}

void AirPlayClient::stop(void)
{
    if (! m_isStart)
        return;
    
    m_isStart = false;

    m_core->airplayClient()->stop();
}

AirPlayClientPhoto * AirPlayClient::photo(void)
{
    return m_photo;
}

AirPlayClientVideo * AirPlayClient::video(void)
{
    return m_video;
}

void AirPlayClient::connectToServer(const std::string &ip, uint16_t port)
{
    if (m_isConnected)
        return;

    m_core->airplayClient()->connect(ip, port, 1);
    
    m_isConnected = true;
}

void AirPlayClient::disconnect(void)
{
    if (! m_isConnected)
        return;

    m_core->airplayClient()->disconnect();

    m_isConnected = false;
}

bool AirPlayClient::send(const std::string &message)
{
    if (! m_core->airplayClient()->isConnected())
        return false;

    TcpClientMessagePointer pMessage = boost::make_shared<TcpClientMessage>();
    pMessage->setType(TcpClientMessage::NormalMessage);
    pMessage->message() = message;

    m_core->airplayClient()->send(pMessage, 1);

    return true;
}


/*
client → server
-----------------------------------------------------------------
GET /server-info HTTP/1.1
X-Apple-Device-ID: 0xdc2b61a0ce79
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c
*/
bool AirPlayClient::getServerInfo(void)
{
    m_requestType = AirPlayClient::RequestTypeGetServerInfo;

    std::string xAppleDeviceId("0xdc2b61a0ce79");
    std::string xAppleSessionId("1bd6ceeb-fffd-456c-a09c-996053a7a08c");

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("GET");
    httpRequest.setUri("/server-info");

    httpRequest.addHeaders("X-Apple-Device-ID", xAppleDeviceId);
    httpRequest.addHeaders("Content-Length", "0");
    httpRequest.addHeaders("User-Agent", "MediaControl/1.0");
    httpRequest.addHeaders("X-Apple-Session-ID", xAppleSessionId);

    return this->send(httpRequest.toString());
}

/*
client → server
-----------------------------------------------------------------
GET /slideshow-features HTTP/1.1
Accept-Language: English
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: cdda804c-33ae-4a0b-a5f2-f0e532fd5abd
*/
bool AirPlayClient::getSlideshowFeatures(void)
{
    m_requestType = AirPlayClient::RequestTypeGetSlideshowFeatures;

    std::string xAppleSessionId("1bd6ceeb-fffd-456c-a09c-996053a7a08c");

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("GET");
    httpRequest.setUri("/slideshow-features");

    httpRequest.addHeaders("Accept-Language", "English");
    httpRequest.addHeaders("Content-Length", "0");
    httpRequest.addHeaders("User-Agent", "MediaControl/1.0");
    httpRequest.addHeaders("X-Apple-Session-ID", xAppleSessionId);

    return this->send(httpRequest.toString());
}

/*
在单独的TCP请求中发送反向HTTP请求
-----------------------------------------------------------------
client → server
-----------------------------------------------------------------
POST /reverse
Upgrade: PTTH/1.0
Connection: Upgrade
X-Apple-Purpose: event
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c
*/
bool AirPlayClient::postReverse(void)
{
    return false;

#if 0
    std::string xAppleSessionId("1bd6ceeb-fffd-456c-a09c-996053a7a08c");

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("POST");
    httpRequest.setUri("/reverse");

    httpRequest.addHeaders("Upgrade", "PTTH/1.0");
    httpRequest.addHeaders("Connection", "Upgrade");
    httpRequest.addHeaders("X-Apple-Purpose", "event");
    httpRequest.addHeaders("Content-Length", "0");
    httpRequest.addHeaders("User-Agent", "MediaControl/1.0");
    httpRequest.addHeaders("X-Apple-Session-ID", xAppleSessionId);

    return this->send(httpRequest.toString());
#endif
}