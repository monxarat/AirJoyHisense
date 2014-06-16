/*
* AirPlayServerHandler.cpp
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

#include "AirPlayServerHandler.hpp"
#include "AirPlayServerListener.hpp"
#include "TcpSession.hpp"
#include "Plist.hpp"
#include "HttpTextParam.hpp"
#include "AirPlayEvent.hpp"
#include <ctime>
#include <vector>

#include "AirJoyLog.hpp"

using namespace airjoy;


AirPlayServerHandler::AirPlayServerHandler(AirPlayServer *server, AirPlayServerListener *listener)
    : m_listener(listener)
{
}

AirPlayServerHandler::~AirPlayServerHandler()
{
    LockType lock1(m_eventSubscribersMutex);
    EventSubscriberMap::iterator iter1 = m_eventSubscribers.begin();
    for (; iter1 != m_eventSubscribers.end(); ++iter1)
        iter1->first->close();   
    m_eventSubscribers.clear();

    LockType lock2(m_pushSessionsMutex);
    PushSessionMap::iterator iter2 = m_pushSessions.begin();
    for (; iter2 != m_pushSessions.end(); ++iter2)
        iter2->first->close();   
    m_pushSessions.clear();

    LockType lock3(m_slideshowsMutex);
    SlideshowsMap::iterator iter3 = m_slideshows.begin();
    for (; iter3 != m_slideshows.end(); ++iter3)
        iter3->first->close();   
    m_slideshows.clear();
}

void AirPlayServerHandler::didClose(int connectedId)
{
    //std::cout << "didClose: " << connectedId << std::endl;

    if (! this->removeEventSubscriber(connectedId))
        if (! this->removePushSession(connectedId))
            this->removeSlideshowsSession(connectedId);

    // remove buffer for this connection
    LockType bufferLock(m_buffersMutex);
    BufferMap::iterator p = m_buffers.find(connectedId);
    if (p != m_buffers.end())
        m_buffers.erase(p);
}

void AirPlayServerHandler::closeConnection(const std::string &clientIp)
{
    //std::cout << "closeConnection:" << clientIp << std::endl;

    if (clientIp.empty())
        return;

    // 断开订阅连接
    this->closeEventSessionByClientIp(clientIp);

    // 断开推送连接
    this->closePushSessionByClientIp(clientIp);
}

void AirPlayServerHandler::publishEvent(EventCategory type, EventAction action, const std::string &clientIp)
{
    //std::cout << "publishEvent:" << std::endl;

    if (type == AirPlayServer::EventUnknown)
        return;

    LockType subscriberslock(m_eventSubscribersMutex);
    EventSubscriberMap::iterator iter = m_eventSubscribers.begin();
    for (; iter != m_eventSubscribers.end(); ++iter)
	{
        if (clientIp == iter->first->getPeerIp())
        {
            AirPlayEvent e(type, action, iter->second);
            sendMessage(iter->first, e.toString());

            //std::cout << "Publish Event To -> " << clientIp << std::endl << e.toString() << std::endl;
            //LOGD(e.toString().c_str());
#if 0
            //std::cout << "Publish Event To -> " << clientIp << std::endl << e.toString() << std::endl;
            
            // 如果是stop事件
            if (action == AirPlayServer::EventActionStopped)
            {
                // 断开订阅连接
                iter->first->close();

                // 断开推送连接
                this->closePushSession(iter->second);
            }
#endif
        }
	}

    return;
}
        
bool AirPlayServerHandler::addEventSubscriber(TcpSessionPointer p, const std::string &appleSessionId)
{
    LockType lock(m_eventSubscribersMutex);
    EventSubscriberMap::iterator iter = m_eventSubscribers.find(p);
    if (iter == m_eventSubscribers.end())
    {
        //std::cout << "addEventSubscriber: " << p->getPeerIp() << " connectedId: " << p->connectedId() << " appleSessionId: "  << appleSessionId  << std::endl;

        // 通知上层，客户端请求订阅事件
        m_listener->didSubscribeEvent(p->getPeerIp());
        m_eventSubscribers.insert(std::make_pair(p, appleSessionId));
        return true;
    }

    return false;
}

bool AirPlayServerHandler::removeEventSubscriber(TcpSessionPointer p)
{
    LockType lock(m_eventSubscribersMutex);
    EventSubscriberMap::iterator iter = m_eventSubscribers.find(p);
    if (iter == m_eventSubscribers.end())
    {
        //std::cout << "removeEventSubscriber: " << p->getPeerIp() << " : " << p->connectedId()  << std::endl;
        
        // 通知上层，客户端取消订阅
        m_listener->didUnsubscribeEvent(p->getPeerIp());
        m_eventSubscribers.erase(iter);
        return true;
    }

    return false;
}

bool AirPlayServerHandler::removeEventSubscriber(int connectedId)
{
    LockType lock(m_eventSubscribersMutex);
    EventSubscriberMap::iterator iter = m_eventSubscribers.begin();
    for (; iter != m_eventSubscribers.end(); ++iter)
    {
        if (iter->first->connectedId() == connectedId)
        {   
            //std::cout << "removeEventSubscriber: " << iter->first->getPeerIp() << " : " << connectedId << std::endl;

            // 通知上层，客户端取消订阅
            m_listener->didUnsubscribeEvent(iter->first->getPeerIp());
            m_eventSubscribers.erase(iter);
            return true;
        }
    }

    return false;
}

bool AirPlayServerHandler::closeEventSessionByClientIp(const std::string &clientIp)
{
    //std::cout << "closeEventSessionByClientIp:" << std::endl;

    LockType subscriberslock(m_eventSubscribersMutex);
    EventSubscriberMap::iterator iter = m_eventSubscribers.begin();
    for (; iter != m_eventSubscribers.end(); ++iter)
	{
        if (clientIp == iter->first->getPeerIp())
        {
            //std::cout << "closeEventSessionByClientIp: " << clientIp << std::endl;

            iter->first->close();
            return true;
        }
    }

    return false;
}

bool AirPlayServerHandler::addPushSession(TcpSessionPointer p, const std::string &appleSessionId)
{
    LockType lock(m_pushSessionsMutex);
    PushSessionMap::iterator iter = m_pushSessions.find(p);
    if (iter == m_pushSessions.end())
    {
        //std::cout << "addPushSession: " << appleSessionId << " ip: " << p->getPeerIp() << " appleSessionId: " << appleSessionId << std::endl;

        m_pushSessions.insert(std::make_pair(p, appleSessionId));
        return true;
    }

    return false;
}

bool AirPlayServerHandler::removePushSession(TcpSessionPointer p)
{
    LockType lock(m_pushSessionsMutex);
    PushSessionMap::iterator iter = m_pushSessions.find(p);
    if (iter == m_pushSessions.end())
    {
        //std::cout << "removePushSession: " << p->getPeerIp() << " : " << p->connectedId() << std::endl;

        m_pushSessions.erase(iter);
        return true;
    }

    return false;
}

bool AirPlayServerHandler::removePushSession(const std::string &applsSessionId)
{
    LockType lock(m_pushSessionsMutex);
    PushSessionMap::iterator iter = m_pushSessions.begin();
    for (; iter != m_pushSessions.end(); ++iter)
    {
        if (iter->second == applsSessionId)
        {
            //std::cout << "removePushSession: " << iter->first->getPeerIp() << " : " << iter->first->connectedId() << std::endl;

            m_pushSessions.erase(iter);
            return true;
        }
    }

    return false;
}

bool AirPlayServerHandler::removePushSession(int connectedId)
{
    LockType lock(m_pushSessionsMutex);
    PushSessionMap::iterator iter = m_pushSessions.begin();
    for (; iter != m_pushSessions.end(); ++iter)
    {
        if (iter->first->connectedId() == connectedId)
        {   
            //std::cout << "removePushSession: " << iter->first->getPeerIp() << " : " << iter->first->connectedId() << std::endl;

            m_pushSessions.erase(iter);
            return true;
        }
    }

    return false;
}

bool AirPlayServerHandler::closePushSession(const std::string &applsSessionId)
{
    LockType lock(m_pushSessionsMutex);
    PushSessionMap::iterator iter = m_pushSessions.begin();
    for (; iter != m_pushSessions.end(); ++iter)
    {
        if (iter->second == applsSessionId)
        {
            //std::cout << "stopPushSession: " << iter->first->connectedId() << " : " << iter->first->connectedId() << std::endl;

            iter->first->close();
            return true;
        }
    }

    return false;
}

bool AirPlayServerHandler::closePushSessionByClientIp(const std::string &clientIp)
{
    //std::cout << "closePushSessionByClientIp:" << std::endl;

    LockType lock(m_pushSessionsMutex);
    PushSessionMap::iterator iter = m_pushSessions.begin();
    for (; iter != m_pushSessions.end(); ++iter)
    {
        if (iter->first->getPeerIp() == clientIp)
        {
            //std::cout << "closePushSessionByClientIp: " << clientIp << " : " << iter->first->connectedId() << std::endl;
            
            iter->first->close();
            return true;
        }
    }

    return false;
}

void AirPlayServerHandler::addBuffer(TcpServerMessagePointer request)
{
    //std::cout << "--------- add buffer -----------: " << request->message().length() << std::endl;
    //std::cout << request->message() << std::endl;

    LockType lock(m_buffersMutex);
    BufferMap::iterator p = m_buffers.find(request->session()->connectedId());
    if (p == m_buffers.end())
    {
        m_buffers.insert(std::make_pair(request->session()->connectedId(), request->message()));
        return;
    }
    
    p->second.append(request->message());
}

void AirPlayServerHandler::handleBufferBySession(TcpSessionPointer session)
{
    LockType lock(m_buffersMutex);
    BufferMap::iterator p = m_buffers.find(session->connectedId());
    if (p == m_buffers.end())
        return;
        
    std::string &buffer = p->second;

    while (1)
    {
        //std::cout << "handle buffer: " << buffer.size() << std::endl;

        // We need a new HttpMessage ...
        HttpMessagePointer pHttpMessage = boost::make_shared<HttpMessage>();

        HttpParserStatus status = pHttpMessage->loadBytes(buffer.c_str(), buffer.size());
        if (HttpParserDone == status)
        {
        #if 0
            std::cout << "----------------- didReceiveRequest: -------------" << std::endl;
            pHttpMessage->print();
        #endif

            if (pHttpMessage->consumeLength() > buffer.size())
            {
                //std::cout << "consumeLength > buffer.size() " << std::endl;
                break;
            }

            //std::cout << "buffer consume: " << pHttpMessage->consumeLength() << std::endl;

            bool result = handleHttpMessage(session, pHttpMessage);

            buffer.erase(0, pHttpMessage->consumeLength());
        }
        else if (status == HttpParserIncomplete)
        {
            //std::cout << "HttpParserIncomplete" << std::endl;

            // will PutPhoto or will PutPhotoCacheOnly
            HttpMessage httpMessage;
            if (httpMessage.loadBytesWithoutContent(buffer.c_str(), buffer.size()))
            {
                if (httpMessage.httpType() == HttpRequest)
                {
                    willAction(httpMessage, session->getPeerIp());
                }
            }

            break;
        }
        else if (status == HttpParserHeaderIncomplete)
        {
            //std::cout << "HttpParserHeaderIncomplete " << std::endl;
            break;
        }
        else
        {
            //std::cout << "HttpParserError " << std::endl;

            // 非HTTP消息
            buffer.clear();
            break;
        }

        if (buffer.size() == 0)
            break;
    }
    
    //std::cout << "--------- handle buffer end ! -----------" << std::endl;

    return;
}

bool AirPlayServerHandler::didReceiveMessage(TcpServerMessagePointer request)
{
    this->addBuffer(request);

    this->handleBufferBySession(request->session());

    return true;
}

bool AirPlayServerHandler::handleHttpMessage(TcpSessionPointer session, HttpMessagePointer pHttpMessage)
{
    AirPlayStatus status = AIRPLAY_STATUS_NOT_IMPLEMENTED;
    std::string responseContent;

    if (pHttpMessage->httpType() == HttpRequest)
    {
        if (pHttpMessage->httpMethod() == "GET")
        {
            status = didGet(session, pHttpMessage, responseContent);
        }
        else if (pHttpMessage->httpMethod() == "POST")
        {
            status = didPost(session, pHttpMessage, responseContent);
        }
        else if (pHttpMessage->httpMethod() == "PUT")
        {
            status = didPut(session, pHttpMessage, responseContent);
        }
        else
        {
            status = AIRPLAY_STATUS_NOT_IMPLEMENTED;
        }
    }
    else
    {
        this->recvHttpResponse(session, pHttpMessage);

        status = AIRPLAY_STATUS_NO_RESPONSE_NEEDED;
    }

    switch (status)
    {
    case AIRPLAY_STATUS_UNKNOWN:
        break;

    case AIRPLAY_STATUS_OK:
        sendResponseOk(session, responseContent);
        break;

    case AIRPLAY_STATUS_SWITCHING_PROTOCOLS:
        sendResponseSwitchingProtocols(session);
        break;

    case AIRPLAY_STATUS_NEED_AUTH:
        sendResponseUnauthorized(session);
        break;

    case AIRPLAY_STATUS_NOT_FOUND:
        sendResponseNotFound(session);
        break;

    case AIRPLAY_STATUS_METHOD_NOT_ALLOWED:
        sendResponseMethodNotAllowed(session);
        break;

    case AIRPLAY_STATUS_NOT_IMPLEMENTED:
        sendResponseNotImplemented(session);
        break;

    case AIRPLAY_STATUS_NO_RESPONSE_NEEDED:
        break;

    default:
        break;
    }

    return true;
}

void AirPlayServerHandler::willAction(HttpMessage &httpMessage, const std::string &clientIp)
{
    if (httpMessage.httpMethod() == "PUT")
    {
        if (httpMessage.uri() == "/photo")
        {
            // photoId
            std::string photoId = httpMessage.getValueByName("X-Apple-AssetKey");
            
            if (m_lastPhotoId == photoId)
                return;

            m_lastPhotoId = photoId;

            // action(optoinal)
            std::string action = httpMessage.getValueByName("X-Apple-AssetAction");

            if (m_listener)
            {
                if (action.empty())
                    m_listener->willPutPhoto(photoId, clientIp);
                else if (action == "cacheOnly")
                    m_listener->willPutPhotoCacheOnly(photoId, clientIp);
            }
        }
    }
}

const std::string & AirPlayServerHandler::getNowTime(void)
{
    time_t t = std::time(0);
    char tmp[64];
    
    // Wed, 27 June 2012 16:47:41 GMT
    std::strftime(tmp, sizeof(tmp), "%a, %d %b %Y %X GMT", localtime(&t) );
    
    m_time = tmp;

    return m_time;
}

void AirPlayServerHandler::sendMessage(TcpSessionPointer session, const std::string & message)
{
    TcpServerMessagePointer response = boost::make_shared<TcpServerMessage>(session);
    response->message() = message;

    //std::cout << "---------------------- startWrite ----------------------" << std::endl;
    //std::cout << response->message() << std::endl;

    session->startWrite(response);
}

void AirPlayServerHandler::sendResponseOk(TcpSessionPointer session, std::string &responseContent)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(200);
    httpResponse.sethttpResponseStatus("OK");

    httpResponse.addHeaders("Date", getNowTime());
    
    if (responseContent.empty())
    {
        httpResponse.addHeaders("Content-Length", "0");
    }
    else
    {
        std::stringstream ssSize;
        ssSize << responseContent.length();

        httpResponse.addHeaders("Content-Type", "text/x-apple-plist+xml");
        httpResponse.addHeaders("Content-Length", ssSize.str());

        httpResponse.setContent(responseContent);
    }

    sendMessage(session, httpResponse.toString());
}

void AirPlayServerHandler::sendResponseUnauthorized(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(401);
    httpResponse.sethttpResponseStatus("Unauthorized");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirPlayServerHandler::sendResponseNotFound(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(404);
    httpResponse.sethttpResponseStatus("Not Found");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirPlayServerHandler::sendResponseNotImplemented(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(501);
    httpResponse.sethttpResponseStatus("Not Implemented");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirPlayServerHandler::sendResponseMethodNotAllowed(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(501);
    httpResponse.sethttpResponseStatus("Method Not Allowed");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirPlayServerHandler::sendResponseSwitchingProtocols(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(101);
    httpResponse.sethttpResponseStatus("Switching Protocols");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Upgrade", "PTTH/1.0");
    httpResponse.addHeaders("Connection", "Upgrade");

    sendMessage(session, httpResponse.toString());
}

AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didGet(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    if (pHttpMessage->uri() == "/server-info")
        return didGetServerInfo(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri() == "/slideshow-features")
        return didGetSlideshowFeatures(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri() == "/scrub")
        return didGetScrub(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri() == "/playback-info")
        return didGetPlaybackInfo(session, pHttpMessage, responseContent);

    return AIRPLAY_STATUS_NOT_IMPLEMENTED;
}

/*
client → server
GET /server-info HTTP/1.1h

X-Apple-Device-ID: 0xdc2b61a0ce79
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Thu, 23 Feb 2012 17:33:41 GMT
Content-Type: text/x-apple-plist+xml
Content-Length: 427

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
 <dict>
  <key>deviceid</key>
  <string>58:55:CA:1A:E2:88</string>
  <key>features</key>
  <integer>14839</integer>
  <key>model</key>
  <string>AppleTV2,1</string>
  <key>protovers</key>
  <string>1.0</string>
  <key>srcvers</key>
  <string>120.2</string>
 </dict>
</plist>
*/

AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didGetServerInfo(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{    
    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\r\n";
    ss << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n";
    ss << "<plist version=\"1.0\">" << "\r\n";
    ss << "<dict>" << "\r\n";
    ss << "<key>deviceid</key>" << "\r\n";
    ss << "<string>" << m_serviceInfo->deviceId() << "</string>" << "\r\n";
    ss << "<key>features</key>" << "\r\n";
    ss << "<integer>"<< m_serviceInfo->features() << "</integer>" << "\r\n";
    ss << "<key>model</key>" << "\r\n";
    ss << "<string>" << m_serviceInfo->deviceModel() << "</string>" << "\r\n";
    ss << "<key>protovers</key>" << "\r\n";
    ss << "<string>" << m_serviceInfo->protovers() << "</string>" << "\r\n";
    ss << "<key>srcvers</key>" << "\r\n";
    ss << "<string>" << m_serviceInfo->version() << "</string>" << "\r\n";
    ss << "</dict>" << "\r\n";
    ss << "</plist>" << "\r\n";

    responseContent = ss.str();
    return AIRPLAY_STATUS_OK;
}

/*
client → server
GET /slideshow-features HTTP/1.1
Accept-Language: English
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: cdda804c-33ae-4a0b-a5f2-f0e532fd5abd
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Thu, 23 Feb 2012 17:33:41 GMT
Content-Type: text/x-apple-plist+xml
Content-Length: 6411

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
 <dict>
  <key>themes</key>
  <array>
   <dict>
    <key>key</key>
    <string>Reflections</string>
    <key>name</key>
    <string>Reflections</string>
   </dict>
   ...
  </array>
 </dict>
</plist>
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didGetSlideshowFeatures(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    //std::cout << "AirPlayServerHandler::didGetSlideshowFeatures" << std::endl;

    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\r\n";
    ss << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" << "\r\n";
    ss << "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" <<  "\r\n";
    ss << "<plist version=\"1.0\">" << "\r\n";
    ss << " <dict>" << "\r\n";
    ss <<"   <key>themes</key>" << "\r\n";
    ss << "  <array>" << "\r\n";
    ss << "   <dict>" << "\r\n";
    ss << "    <key>key</key>" << "\r\n";
    ss << "    <string>Reflections</string>" << "\r\n";
    ss << "    <key>name</key>" << "\r\n";
    ss << "    <string>Reflections</string>" << "\r\n";
    ss << "   </dict>" << "\r\n";
    ss << "  </array>" << "\r\n";
    ss <<"  </dict>" << "\r\n";
    ss <<" </plist>" << "\r\n";
    
    responseContent = ss.str();
    return AIRPLAY_STATUS_OK;
}

/*
client → server
GET /scrub HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 0
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Mon, 08 Mar 2012 18:08:31 GMT
Content-Type: text/parameters
Content-Length: 40

duration: 83.124794
position: 14.467000
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didGetScrub(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    //std::cout << "AirPlayServerHandler::didGetScrub" << std::endl;

    PlaybackTime time;
    time.playPosition = 0.0;
    time.duration = 0.0;

    if (m_listener)
        m_listener->getCurrentPlaybackProgress(time, session->getPeerIp());

    std::stringstream ss;
    ss <<"duration: " << time.duration << "\r\n";
    ss <<"position: " << time.playPosition << "\r\n";
        
    responseContent = ss.str();
    return AIRPLAY_STATUS_OK;
}

/*
client → server
GET /playback-info HTTP/1.1
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 24b3fd94-1b6d-42b1-89a3-47108bfbac89
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Fri, 16 Mar 2012 15:31:42 GMT
Content-Type: text/x-apple-plist+xml
Content-Length: 801
X-Transmit-Date: 2012-03-16T15:31:42.607066Z

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
  <dict>
    <key>duration</key> 
    <real>1801</real>         // 视频长度（单位：s）
    <key>loadedTimeRanges</key>
    <array>
      <dict>
        <key>duration</key> 
        <real>51.541130402</real>   // 已经加载的长度（单位：s）
        <key>start</key> 
        <real>18.118717650000001</real>   // 开始播放点（单位：s）
      </dict>
    </array>
  <key>playbackBufferEmpty</key> <true/>     // 播放缓冲：空
  <key>playbackBufferFull</key> <false/>     // 播放缓冲：未满
  <key>playbackLikelyToKeepUp</key> <true/>  // true
  <key>position</key>                       
  <real>18.043869775000001</real>       // 播放时间点
  <key>rate</key> 
  <real>1</real>                        // 正在播放
  <key>readyToPlay</key> <true/>        // 已经准备播放
  <key>seekableTimeRanges</key>         // iPad可以滑动的范围
  <array>
   <dict>
    <key>duration</key>                 // 视频长度
    <real>1801</real>
    <key>start</key>                    // 起始点
    <real>0.0</real>
   </dict>
  </array>
 </dict>
</plist>
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didGetPlaybackInfo(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    //std::cout << "AirPlayServerHandler::didGetPlaybackInfo" << std::endl;

    PlaybackInfo info;
    info.playbackTime.playPosition = 0.0;
    info.playbackTime.duration = 0.0;
    info.rate = 1.0;
    info.readyToPlay = true;                            // ?
    info.playbackBufferEmpty = true;
    info.playbackBufferFull = false;
    info.playbackLikelyToKeepUp = true;                 // ?
    info.loadedTimeRanges.startPosition = 0.0;
    info.loadedTimeRanges.duration = 0.0;
    info.seekableTimeRanges.startPosition = 0.0;
    info.seekableTimeRanges.duration = 0.0;

    if (m_listener)
        m_listener->getPlaybackInfo(info, session->getPeerIp());

    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\r\n";
    ss << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" <<  "\r\n";
    ss << "<plist version=\"1.0\">" << "\r\n";
    ss << "<dict>" << "\r\n";
    ss << "<key>duration</key>" << "\r\n";
    ss << "<real>" << info.playbackTime.duration << "</real>" << "\r\n";
    ss << "<key>loadedTimeRanges</key>" << "\r\n";
    ss << "<array>" << "\r\n";
    ss << "\t\t<dict>" << "\r\n";
    ss << "\t\t\t<key>duration</key>" << "\r\n";
    ss << "\t\t\t<real>" << info.loadedTimeRanges.duration<< "</real>" << "\r\n";
    ss << "\t\t\t<key>start</key>" << "\r\n";
    ss << "\t\t\t<real>" << info.loadedTimeRanges.startPosition << "</real>" << "\r\n";
    ss << "\t\t</dict>" << "\r\n";
    ss << "</array>" << "\r\n";
    ss << "<key>playbackBufferEmpty</key>" << "\r\n";
    ss << "<" << (info.playbackBufferEmpty ? "true" : "false") <<  "/>" << "\r\n";
    ss << "<key>playbackBufferFull</key>" << "\r\n";
    ss << "<" << (info.playbackBufferFull ? "true" : "false") <<  "/>" << "\r\n";
    ss << "<key>playbackLikelyToKeepUp</key>" << "\r\n";
    ss << "<" << (info.playbackLikelyToKeepUp ? "true" : "false") <<  "/>" << "\r\n";
    ss << "<key>position</key>" << "\r\n"; 
    ss << "<real>" << info.playbackTime.playPosition << "</real>" << "\r\n";
    ss << "<key>rate</key>" << "\r\n";
    ss << "<real>" << info.rate << "</real>" << "\r\n";
    ss << "<key>readyToPlay</key>" << "\r\n";
    ss << "<" << (info.readyToPlay ? "true" : "false") <<  "/>" << "\r\n";
    ss << "<key>seekableTimeRanges</key>" << "\r\n";
    ss << "<array>" << "\r\n";
    ss << "\t\t<dict>" << "\r\n";
    ss << "\t\t\t<key>duration</key>" << "\r\n";
    ss << "\t\t\t<real>" << info.seekableTimeRanges.duration << "</real>" << "\r\n";
    ss << "\t\t\t<key>start</key>" << "\r\n";
    ss << "\t\t\t<real>" << info.seekableTimeRanges.startPosition << "</real>" << "\r\n";
    ss << "\t\t</dict>" << "\r\n";
    ss << "</array>" << "\r\n";
    ss << "</dict>" << "\r\n";
    ss << "</plist>" << "\r\n";

    responseContent = ss.str();
    return AIRPLAY_STATUS_OK;
}

AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPost(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    if (pHttpMessage->uri() == "/reverse")
        return didPostReverse(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri() == "/stop")
        return didPostStop(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri() == "/play")
        return didPostPlay(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri().find("/scrub") != std::string::npos)
        return didPostScrub(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri().find("/rate") != std::string::npos)
        return didPostRate(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri().find("/getProperty") != std::string::npos)
        return didPostGetProperty(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri().find("/volume") != std::string::npos)
        return didPostVolume(session, pHttpMessage, responseContent);

    return AIRPLAY_STATUS_NOT_IMPLEMENTED;
}

/*
client → server
POST /reverse
Upgrade: PTTH/1.0
Connection: Upgrade
X-Apple-Purpose: event
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c
-----------------------------------------------------------------
server → client
HTTP/1.1 101 Switching Protocols
Date: Thu, 23 Feb 2012 17:33:41 GMT
Upgrade: PTTH/1.0
Connection: Upgrade
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPostReverse(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    std::string appleSessionId("00000000-0000-0000-0000-000000000000");
    if (! pHttpMessage->getValueByName("X-Apple-Session-ID").empty())
        appleSessionId = pHttpMessage->getValueByName("X-Apple-Session-ID");

    std::string purpose;
    if (! pHttpMessage->getValueByName("x-apple-purpose").empty())
        purpose = pHttpMessage->getValueByName("x-apple-purpose");

    if (purpose == "slideshow")
    {
        this->addSlideshowsSession(session, appleSessionId);
    }
    else
    {
        // purpose == "event"

        this->addEventSubscriber(session, appleSessionId);
    }

    return AIRPLAY_STATUS_SWITCHING_PROTOCOLS;
}

/*
client → server
POST /play HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 163
Content-Type: text/parameters

Content-Location: http://192.168.1.18:3689/airplay.mp4?database-spec='dmap.persistentid:0x63b5e5c0c201542e'&item-spec='dmap.itemid:0x21d'
Start-Position: 0.174051
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Mon, 08 Mar 2012 18:08:25 GMT
Content-Length: 0
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPostPlay(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    //std::cout << "didPostPlay peer IP: " << request->session()->getPeerIp() << std::endl;

    // X-Apple-Session-ID
    std::string appleSessionId("00000000-0000-0000-0000-000000000000");
    if (! pHttpMessage->getValueByName("X-Apple-Session-ID").empty())
        appleSessionId = pHttpMessage->getValueByName("X-Apple-Session-ID");

    this->addPushSession(session, appleSessionId);
        
    std::string contentType = pHttpMessage->getValueByName("Content-Type");
    if (contentType == "text/parameters" || contentType.empty())
    {

        try
        {
            HttpTextParam textParam;
            textParam.loadBytes(pHttpMessage->content().c_str(), pHttpMessage->content().length());

            std::string contentLocation = textParam.getValueByKey("Content-Location");
            std::string startPosition = textParam.getValueByKey("Start-Position");
            float fStartPosition = boost::lexical_cast<float>(startPosition);

            // mp3
            std::string mp3(".mp3");
            std::string mediaType;
            if (contentLocation.length() > mp3.length())
                mediaType = contentLocation.substr(contentLocation.length() - mp3.length());

            if (m_listener)
            {
                if (mediaType == mp3)
                    m_listener->didStartPlayMusic(contentLocation, fStartPosition, session->getPeerIp());
                else
                    m_listener->didStartPlayVideo(contentLocation, fStartPosition, session->getPeerIp());
            }
        }
        catch (std::exception &e)
        {
            std::cout << "read textParam error : " << e.what() << std::endl;
        }

    }
    else // "application/x-apple-binary-plist"
    {
        try
        {
            typedef std::map<std::string, boost::any> DictType;
            DictType dict; 
            DictType::iterator iter;

            Plist::readPlist(pHttpMessage->content().c_str(), pHttpMessage->content().length(), dict);

            std::string contentLocation;
            double startPosition = 0.0;

            iter = dict.find("Content-Location");
            if (iter != dict.end())
            {
                contentLocation = boost::any_cast<const std::string&>(iter->second);
            }
            else
            {
                //  for iOS 6.1

                double rate = 0.0;
                iter = dict.find("rate");
                if (iter != dict.end())
                    rate = boost::any_cast<const double&>(iter->second);

                std::string path;
                iter = dict.find("path");
                if (iter != dict.end())
                    path = boost::any_cast<const std::string&>(iter->second);

                std::string host;
                iter = dict.find("host");
                if (iter != dict.end())
                    host = boost::any_cast<const std::string&>(iter->second);

                contentLocation.append("http://").append(host).append(path);
            }

            iter = dict.find("Start-Position");
            if (iter != dict.end())
                startPosition = boost::any_cast<const double&>(iter->second);

            // mp3
            std::string mp3(".mp3");
            std::string mediaType;
            if (contentLocation.length() > mp3.length())
                mediaType = contentLocation.substr(contentLocation.length() - mp3.length());

            if (m_listener)
            {
                if (mediaType == mp3)
                    m_listener->didStartPlayMusic(contentLocation, startPosition, session->getPeerIp());
                else
                    m_listener->didStartPlayVideo(contentLocation, startPosition, session->getPeerIp());
            }
        }
        catch (std::exception &e)
        {
            std::cout << "read Plist error : " << e.what() << std::endl;
        }
    }

    return AIRPLAY_STATUS_OK;
}

/*
client → server
POST /scrub?position=20.097000 HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 0
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Mon, 08 Mar 2012 18:08:42 GMT
Content-Length: 0
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPostScrub(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    //std::cout << "AirPlayServerHandler::didPostScrub" << std::endl;

    float playPositoin = 0.0f;

    try
    {
        playPositoin =  boost::lexical_cast<float>(pHttpMessage->getValueInUri("position="));
    }
    catch (std::exception &e)
    {
        std::cout << "didPostScrub: " << e.what() << std::endl;
    }

    if (m_listener)
        m_listener->setCurrentPlaybackProgress(playPositoin, session->getPeerIp());

    return AIRPLAY_STATUS_OK;
}

/*
client → server
POST /rate?value=0.000000 HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 0
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Mon, 08 Mar 2012 18:08:37 GMT
Content-Length: 0
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPostRate(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    // get rate value
    float rate = 0;

    try
    {
        rate =  boost::lexical_cast<float>(pHttpMessage->getValueInUri("value="));
    }
    catch (std::exception &e)
    {
        std::cout << "didPostRate: " << e.what() << std::endl;
    }

    if (m_listener)
        m_listener->didSetPlaybackRate(rate, session->getPeerIp());
    
    return AIRPLAY_STATUS_OK;
}

/*
-----------------------------------------------------------------
Stop a photo or slideshow session
-----------------------------------------------------------------
client → server
POST /stop HTTP/1.1
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Thu, 23 Feb 2012 17:33:55 GMT
Content-Length: 0
-----------------------------------------------------------------
Stop playback
-----------------------------------------------------------------
client → server
POST /stop HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 0
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Mon, 08 Mar 2012 18:09:06 GMT
Content-Length: 0
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPostStop(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    //std::cout << "didPostStop senderIp: " << request->session()->getPeerIp() << std::endl;

    if (m_listener)
        m_listener->didStop(session->getPeerIp());

    return AIRPLAY_STATUS_OK;
}

AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPut(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    if (pHttpMessage->uri() == "/photo")
        return didPutPhoto(session, pHttpMessage, responseContent);

    if (pHttpMessage->uri().find("/setProperty") != std::string::npos)
        return didPutSetProperty(session, pHttpMessage, responseContent);
    
    if (pHttpMessage->uri().find("/slideshows") != std::string::npos)
        return didPutSlideshows(session, pHttpMessage, responseContent);

    return AIRPLAY_STATUS_NOT_IMPLEMENTED;
}

/*
client → server
POST /getProperty?playbackAccessLog HTTP/1.1
Content-Type: application/x-apple-binary-plist
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 24b3fd94-1b6d-42b1-89a3-47108bfbac89
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Fri, 16 Mar 2012 15:31:42 GMT
Content-Type: application/x-apple-binary-plist
Content-Length: 530

<BINARY PLIST DATA>

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
 <dict>
  <key>errorCode</key>
  <integer>0</integer>
  <key>value</key>
  <array>
   <dict>
    <key>bytes</key> <integer>1818336</integer>
    <key>c-duration-downloaded</key> <real>70</real>
    <key>c-duration-watched</key> <real>18.154102027416229</real>
    <key>c-frames-dropped</key> <integer>0</integer>
    <key>c-observed-bitrate</key> <real>14598047.302367469</real>
    <key>c-overdue</key> <integer>0</integer>
    <key>c-stalls</key> <integer>0</integer>
    <key>c-start-time</key> <real>0.0</real>
    <key>c-startup-time</key> <real>0.27732497453689575</real>
    <key>cs-guid</key> <string>B475F105-78FD-4200-96BC-148BAB6DAC11</string>
    <key>date</key> <date>2012-03-16T15:31:24Z</date>
    <key>s-ip</key> <string>213.152.6.89</string>
    <key>s-ip-changes</key> <integer>0</integer>
    <key>sc-count</key> <integer>7</integer>
    <key>uri</key> <string>http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8</string>
   </dict>
  </array>
 </dict>
</plist>
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPostGetProperty(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    //std::cout << "AirPlayServerHandler::didPostGetProperty" << std::endl;
    //pHttpMessage->print();

#if 0
    m_xAppleSessionId = pHttpMessage->getValueByName("X-Apple-Session-ID");

    if (pHttpMessage->uri().find("playbackAccessLog") != std::string::npos)
    {
        m_xAppleSessionId = pHttpMessage->getValueByName("X-Apple-Session-ID");

        std::stringstream ssSize;
        std::stringstream ss;
        ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\r\n";
        ss << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" << "\r\n";
        ss << "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" <<  "\r\n";
        ss << "<plist version=\"1.0\">" << "\r\n";
        ss << " <dict>" << "\r\n";
        ss <<"   <key>errorCode</key>" << "\r\n";
        ss << "  <integer>" << 0 << "</integer>" << "\r\n";
        ss << "  <key>value</key>" << "\r\n";
        ss << "  <array>" << "\r\n";
        ss << "  <dict>" << "\r\n";
        ss << "  <key>bytes</key> <integer>1818336</integer>" << "\r\n";
        ss << "  <key>c-duration-downloaded</key> <real>70</real>" << "\r\n";
        ss << "  <key>c-duration-watched</key> <real>18.154102027416229</real>" << "\r\n";
        ss << "  <key>c-frames-dropped</key> <integer>0</integer>" << "\r\n";
        ss << "  <key>c-observed-bitrate</key> <real>14598047.302367469</real>" << "\r\n";
        ss << "  <key>c-overdue</key> <integer>0</integer>" << "\r\n";
        ss << "  <key>c-stalls</key> <integer>0</integer>" << "\r\n";
        ss << "  <key>c-start-time</key> <real>0.0</real>" << "\r\n";
        ss << "  <key>c-startup-time</key> <real>0.27732497453689575</real>" << "\r\n";
        ss << "  <key>cs-guid</key> <string>B475F105-78FD-4200-96BC-148BAB6DAC11</string>" << "\r\n";
        ss << "  <key>date</key> <date>2012-03-16T15:31:24Z</date>" << "\r\n";
        ss << "  <key>s-ip</key> <string>213.152.6.89</string>" << "\r\n";
        ss << "  <key>s-ip-changes</key> <integer>0</integer>" << "\r\n";
        ss << "  <key>sc-count</key> <integer>7</integer>" << "\r\n";
        ss << "  <key>uri</key> <string>http://devimages.apple.com/iphone/samples/bipbop/gear1/prog_index.m3u8</string>" << "\r\n";
        ss << "  </dict>" << "\r\n";
        ss << "  </array>" << "\r\n";
        ss <<"  </dict>" << "\r\n";
        ss <<" </plist>" << "\r\n";

        responseContent = ss.str();
        return AIRPLAY_STATUS_OK;
    }
#endif

    return AIRPLAY_STATUS_NOT_FOUND;
}

/*
client → server
POST /volume?volume=0.224712 HTTP/1.1
content-length: 0
user-agent: iTunes/10.6.3 (Macintosh; Intel Mac OS X 10.7.4) AppleWebKit/534.56.5
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Thu, 23 Feb 2012 17:33:42 GMT
Content-Length: 0
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPostVolume(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    // get rate value
    float value = 0;

    try
    {
        value =  boost::lexical_cast<float>(pHttpMessage->getValueInUri("volume="));
    }
    catch (std::exception &e)
    {
        std::cout << "didPostVolume: " << e.what() << std::endl;
    }

    if (m_listener)
        m_listener->didSetVolume(value, session->getPeerIp());
    
    return AIRPLAY_STATUS_OK;
}

/*
-----------------------------------------------------------------
X-Apple-Transition:
    None
    SlideLeft
    SlideRight
    Dissolve
-----------------------------------------------------------------
client → server
PUT /photo HTTP/1.1
X-Apple-AssetKey: F92F9B91-954E-4D63-BB9A-EEC771ADE6E8
X-Apple-Transition: Dissolve
Content-Length: 462848
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c

<JPEG DATA>
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Thu, 23 Feb 2012 17:33:42 GMT
Content-Length: 0
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPutPhoto(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    std::size_t lengthOfPhoto = pHttpMessage->contentLength();

    // photoId = X-Apple-AssetKey
    std::string photoId("00000000-0000-0000-0000-000000000000");
    if (! pHttpMessage->getValueByName("X-Apple-AssetKey").empty())
        photoId = pHttpMessage->getValueByName("X-Apple-AssetKey");
    
    // action(optoinal)
    std::string action = pHttpMessage->getValueByName("X-Apple-AssetAction");
    
    // X-Apple-Session-ID
    std::string appleSessionId("00000000-0000-0000-0000-000000000000");
    if (! pHttpMessage->getValueByName("X-Apple-Session-ID").empty())
        appleSessionId = pHttpMessage->getValueByName("X-Apple-Session-ID");

    this->addPushSession(session, appleSessionId);

    if (m_listener)
    {
        if (action.empty())
        {
            m_listener->didPutPhoto(photoId, pHttpMessage->content().c_str(), pHttpMessage->content().length(), session->getPeerIp());
        }
        else if (action == "cacheOnly")
        {
            m_listener->didPutPhotoCacheOnly(photoId, pHttpMessage->content().c_str(), pHttpMessage->content().length(), session->getPeerIp());
        }
        else if (action == "displayCached")
        {
            m_listener->didDisplayCachedPhoto(photoId, session->getPeerIp());
        }
    }

    return AIRPLAY_STATUS_OK;
}

/*
client → server
PUT /setProperty?forwardEndTime HTTP/1.1
Content-Type: application/x-apple-binary-plist
Content-Length: 96
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 24b3fd94-1b6d-42b1-89a3-47108bfbac89

<BINARY PLIST DATA>

<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
 <dict>
  <key>value</key>
  <dict>
   <key>epoch</key> <integer>0</integer>
   <key>flags</key> <integer>0</integer>
   <key>timescale</key> <integer>0</integer>
   <key>value</key> <integer>0</integer>
  </dict>
 </dict>
</plist>
-----------------------------------------------------------------
server → client
HTTP/1.1 200 OK
Date: Fri, 16 Mar 2012 15:23:11 GMT
Content-Type: application/x-apple-binary-plist
Content-Length: 58

<BINARY PLIST DATA>

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
 <dict>
  <key>errorCode</key>
  <integer>0</integer>
 </dict>
</plist>
*/
AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPutSetProperty(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    //std::cout << "AirPlayServerHandler::didPutSetProperty" << std::endl;
    //pHttpMessage->print();

#if 0
    enum _SetPropertyType
    {
        Unknown             = 0,
        ForwardEndTime      = 1,
        ReverseEndTime      = 2,
    } timeType;

    if (pHttpMessage->uri().find("forwardEndTime") != std::string::npos)
        timeType = ForwardEndTime;
    else if (pHttpMessage->uri().find("reverseEndTime") != std::string::npos)
        timeType = ReverseEndTime;
    else
        timeType = Unknown;

    std::string contentType = pHttpMessage->getValueByName("Content-Type");
    if (contentType == "text/parameters")
    {
    }
    else // "application/x-apple-binary-plist"
    {
        try
        {
            typedef std::map<std::string, boost::any> DictType;
            DictType dict;
            DictType valueDict;
            DictType::iterator iter;

            Plist::readPlist(pHttpMessage->content().c_str(), pHttpMessage->content().length(), dict);
            
            // In iOS 5 this command is accompanied by a dictionary 
            //{
            //  value =     {
            //    epoch = 0;
            //    flags = 0;
            //    timescale = 0;
            //    value = 0;
            //};}
            int64_t epochValue = 0;
            int64_t flagsValue = 0;
            int64_t timescaleValue = 0;
            int64_t valueValue = 0;

            iter = dict.find("value");
            if (iter != dict.end())
            {
                valueDict = boost::any_cast<DictType>(iter->second);

                iter = valueDict.find("epoch");
                if (iter != valueDict.end())
                    epochValue = boost::any_cast<const int64_t>(iter->second);

                iter = valueDict.find("flags");
                if (iter != valueDict.end())
                    flagsValue = boost::any_cast<const int64_t>(iter->second);

                iter = valueDict.find("timescale");
                if (iter != valueDict.end())
                    timescaleValue = boost::any_cast<const int64_t>(iter->second);

                iter = valueDict.find("value");
                if (iter != valueDict.end())
                    valueValue = boost::any_cast<const int64_t>(iter->second);
            }

            //std::cout << " epochValue: " << epochValue << std::endl;
            //std::cout << " flagsValue: " << flagsValue << std::endl;
            //std::cout << " timescaleValue: " << timescaleValue << std::endl;
            //std::cout << " valueValue: " << valueValue << std::endl;
        }
        catch (std::exception &e)
        {
            std::cout << "read Plist error : " << e.what() << std::endl;
        }
    }
        
    m_xAppleSessionId = pHttpMessage->getValueByName("X-Apple-Session-ID");

    std::stringstream ssSize;
    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\r\n";
    ss << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"" << "\r\n";
    ss << "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">" <<  "\r\n";
    ss << "<plist version=\"1.0\">" << "\r\n";
    ss << " <dict>" << "\r\n";
    ss <<"   <key>errorCode</key>" << "\r\n";
    ss << "  <integer>" << 0 << "</integer>" << "\r\n";
    ss <<"  </dict>" << "\r\n";
    ss <<" </plist>" << "\r\n";

    responseContent = ss.str();
    return AIRPLAY_STATUS_OK;
#endif

    // xmbc like this!
    return AIRPLAY_STATUS_NOT_FOUND;
}

AirPlayServerHandler::AirPlayStatus AirPlayServerHandler::didPutSlideshows(TcpSessionPointer session, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    // X-Apple-Session-ID
    std::string appleSessionId("00000000-0000-0000-0000-000000000000");
    if (! pHttpMessage->getValueByName("X-Apple-Session-ID").empty())
        appleSessionId = pHttpMessage->getValueByName("X-Apple-Session-ID");

    std::string contentType = pHttpMessage->getValueByName("Content-Type");
    if (contentType == "text/x-apple-plist+xml")
    {
        int64_t slideDuration = 3;        // 缺省为3s
        std::string theme;
        std::string state;

        try
        {
            typedef std::map<std::string, boost::any> DictType;
            DictType dict; 

            //std::cout << "------------------" << std::endl;
            //std::cout << pHttpMessage->content() << std::endl;
            //std::cout << "------------------" << std::endl;

            Plist::readPlist(pHttpMessage->content().c_str(), pHttpMessage->content().length(), dict);

            DictType::iterator pDict;
            pDict = dict.find("settings");
            if (pDict != dict.end())
            {
                DictType    settingsDict;

                settingsDict = boost::any_cast<DictType>(pDict->second);

                DictType::iterator pSettings;
                pSettings = settingsDict.find("slideDuration");
                if (pSettings != settingsDict.end())
                    slideDuration = boost::any_cast<const int64_t>(pSettings->second);

                pSettings = settingsDict.find("theme");
                if (pSettings != settingsDict.end())
                    theme = boost::any_cast<const std::string&>(pSettings->second);
            }

            pDict = dict.find("state");
            if (pDict != dict.end())
                state = boost::any_cast<const std::string&>(pDict->second);
        }
        catch (std::exception &e)
        {
            std::cout << "read Plist error : " << e.what() << std::endl;
        }

        //std::cout << "slideDuration: " << slideDuration << std::endl;          
        //std::cout << "theme: " << theme << std::endl;
        //std::cout << "state: " << state << std::endl;

        if (state == "playing")
        {
            m_listener->didStartSlideshows(slideDuration, session->getPeerIp());
        }
        else if (state == "stopped")
        {
            m_listener->didStopSlideshows(session->getPeerIp());
        }
    }
    else
    {
    }

    return AIRPLAY_STATUS_OK;
}

void AirPlayServerHandler::getSlideshowsPicture(const std::string &clientIp)
{
    //LOGD("AirPlayServerHandler::getSlideshowsPicture");

    LockType lock(m_slideshowsMutex);
    SlideshowsMap::iterator iter = m_slideshows.begin();
    for (; iter != m_slideshows.end(); ++iter)
    {
        if (iter->first->getPeerIp() == clientIp)
        {
            HttpMessage httpRequest;
            httpRequest.setHttpType(HttpRequest);
            httpRequest.setHttpVersionMajor(1);
            httpRequest.setHttpVersionMinor(1);
            httpRequest.setHttpMethod("GET");
            httpRequest.setUri("/slideshows/1/assets/1");

            httpRequest.addHeaders("Content-Length", "0");
            httpRequest.addHeaders("Accept", "application/x-apple-binary-plist");
            httpRequest.addHeaders("X-Apple-Session-ID", iter->second);

            //std::string info("AirPlayServerHandler::get slideshows photo");
            //info.append(": ");
            //info.append(clientIp);
            //LOGD(info.c_str());
            //LOGD(httpRequest.toString().c_str());

            this->sendMessage(iter->first, httpRequest.toString());
            return;
        }
    }
}

bool AirPlayServerHandler::addSlideshowsSession(TcpSessionPointer p, const std::string &appleSessionId)
{
    LockType lock(m_slideshowsMutex);
    SlideshowsMap::iterator iter = m_slideshows.find(p);
    if (iter == m_slideshows.end())
    {
        m_slideshows.insert(std::make_pair(p, appleSessionId));
        return true;
    }

    return false;
}

bool AirPlayServerHandler::removeSlideshowsSession(TcpSessionPointer p)
{
    LockType lock(m_slideshowsMutex);
    SlideshowsMap::iterator iter = m_slideshows.find(p);
    if (iter == m_slideshows.end())
    {
        m_slideshows.erase(iter);
        return true;
    }

    return false;
}

bool AirPlayServerHandler::removeSlideshowsSession(const std::string &applsSessionId)
{
    LockType lock(m_slideshowsMutex);
    SlideshowsMap::iterator iter = m_slideshows.begin();
    for (; iter != m_slideshows.end(); ++iter)
    {
        if (iter->second == applsSessionId)
        {
            m_slideshows.erase(iter);
            return true;
        }
    }

    return false;
}

bool AirPlayServerHandler::removeSlideshowsSession(int connectedId)
{
    LockType lock(m_slideshowsMutex);
    SlideshowsMap::iterator iter = m_slideshows.begin();
    for (; iter != m_slideshows.end(); ++iter)
    {
        if (iter->first->connectedId() == connectedId)
        {   
            m_slideshows.erase(iter);
            return true;
        }
    }

    return false;
}

bool AirPlayServerHandler::isSlideshowsSession(TcpSessionPointer p)
{
    LockType lock(m_slideshowsMutex);
    SlideshowsMap::iterator iter = m_slideshows.find(p);
    if (iter != m_slideshows.end())
        return true;

    return false;
}

void AirPlayServerHandler::recvHttpResponse(TcpSessionPointer session, HttpMessagePointer pHttpMessage)
{
    if (this->isSlideshowsSession(session))
    {
        //LOGD("AirPlayServerHandler::receive slideshows photo");

        if (pHttpMessage->httpResponseCode() == 204)
        {
            //LOGD("AirPlayServerHandler::receive slideshows photo failed");

            // 获取图片失败
            m_listener->didGetSlideshowsPictureFailed(session->getPeerIp());
        }
        
        std::string contentType = pHttpMessage->getValueByName("Content-Type");
        if (contentType == "application/x-apple-binary-plist")
        {
#if 1
            // C++
            static int n = 0;
            std::stringstream ss;
            ss << n++;
            std::string path;
            path.append("f:/plist/");
            path.append(ss.str());
            path.append(".plist");

            std::ofstream plistFile;
            plistFile.open(path.c_str(), std::ios_base::out | std::ios_base::binary);
            for (std::size_t i = 0; i < pHttpMessage->content().length(); ++i)
                plistFile.put(pHttpMessage->content().c_str()[i]);
            plistFile.close();
#endif

            try
            {
                std::vector<char> image;
                int64_t index = 0;
                std::string key;

                typedef std::map<std::string, boost::any> DictType;
                DictType dict; 

                Plist::readPlist(pHttpMessage->content().c_str(), pHttpMessage->content().length(), dict);

                DictType::iterator pDict;
                pDict = dict.find("data");
                if (pDict != dict.end())
                {
                    image = boost::any_cast<const std::vector<char>& >(pDict->second);
                }

                pDict = dict.find("info");
                if (pDict != dict.end())
                {
                    DictType    infoDict;

                    infoDict = boost::any_cast<DictType>(pDict->second);

                    DictType::iterator pInfo;
                    pInfo = infoDict.find("id");
                    if (pInfo != infoDict.end())
                        index = boost::any_cast<const int64_t>(pInfo->second);

                    pInfo = infoDict.find("key");
                    if (pInfo != infoDict.end())
                        key = boost::any_cast<const std::string &>(pInfo->second);
                }

                //LOGD("AirPlayServerHandler::receive slideshows photo ok");
                m_listener->didGetSlideshowsPicture(index, image.data(), image.size(), session->getPeerIp());
            }
            catch (std::exception &e)
            {
                //LOGD("AirPlayServerHandler::recvHttpResponse : read Plist error");
                std::cout << "read Plist error : " << e.what() << std::endl;
            }
        }
    }
}
