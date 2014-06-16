/*
* AirJoyServerHandler.cpp
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

#include "AirJoyServerHandler.hpp"
#include "TcpSession.hpp"
#include "AirJoyDefault.hpp"
#include "UtilBase64.hpp"
#include "UtilUtf8.hpp"
#include "AirMessageHandler.hpp"
#include "HttpTextParam.hpp"

using namespace airjoy;


AirJoyServerHandler::AirJoyServerHandler()
{
}

AirJoyServerHandler::~AirJoyServerHandler()
{
}

bool AirJoyServerHandler::didReceiveMessage(TcpServerMessagePointer request)
{
    //std::cout << "didReceiveRequest: " << request->message() << std::endl;

    this->addBuffer(request);

    return this->handleBufferBySession(request->session());
}

void AirJoyServerHandler::didClose(int connectedId)
{
    LockType lock(m_subscribersForAirMediaMutex);
    SubscriberMap::iterator iter = m_subscribersForAirMedia.begin();
    for (; iter != m_subscribersForAirMedia.end(); ++iter)
    {
        if (iter->first->connectedId() == connectedId)
        {
            m_subscribersForAirMedia.erase(iter);
            break;
        }
    }

    // remove buffer for this connection
    LockType bufferLock(m_buffersMutex);
    BufferMap::iterator p = m_buffers.find(connectedId);
    if (p != m_buffers.end())
        m_buffers.erase(p);
}

void AirJoyServerHandler::addAirMessageHandler(const std::string &type, AirMessageHandlerPointer handler)
{
    LockType lock(m_handlersMutex);
    m_handlers.insert(std::make_pair(type, handler));
}

void AirJoyServerHandler::removeAirMessageHandler(const std::string &type)
{
    LockType lock(m_handlersMutex);
    m_handlers.erase(type);
}

bool AirJoyServerHandler::publishMediaEvent(const std::string &mediaType, 
                                            const std::string &action, 
                                            const std::string &id,
                                            const std::string &url,
                                            const std::string &name,
                                            const std::string &fromIp)
{
    LockType eventlock(m_lastEventMutex);
    m_lastEvent.set(mediaType, action, id, url, name, fromIp);
    
    LockType subscriberslock(m_subscribersForAirMediaMutex);
    SubscriberMap::iterator iter = m_subscribersForAirMedia.begin();
    for (; iter != m_subscribersForAirMedia.end(); ++iter)
	{
        sendMessage(iter->first, m_lastEvent.toPublishString());
	}

    return true;
}
        
void AirJoyServerHandler::addBuffer(TcpServerMessagePointer request)
{
    //std::cout << "--------- add buffer -----------"<< std::endl;
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

bool AirJoyServerHandler::handleBufferBySession(TcpSessionPointer session)
{
    bool result = true;

    LockType lock(m_buffersMutex);
    BufferMap::iterator p = m_buffers.find(session->connectedId());
    if (p == m_buffers.end())
        return keepaliveSessionIfNecessory(session);
        
    std::string &buffer = p->second;

    while (1)
    {
        //std::cout << "handle buffer: " << buffer.size() << std::endl;
                    
        // We need a new HttpMessage ...
        m_pHttpMessage = boost::make_shared<HttpMessage>();

        HttpParserStatus status = m_pHttpMessage->loadBytes(buffer.c_str(), buffer.size());
        if (HttpParserDone == status)
        {
            if (m_pHttpMessage->consumeLength() > buffer.size())
            {
                //std::cout << "consumeLength > buffer.size() " << std::endl;
                break;
            }

            //std::cout << "buffer consume: " << m_pHttpMessage->consumeLength() << std::endl;

            HttpMessageType type = GetHttpMessageType(m_pHttpMessage);
            if (type == HttpRequestAirJoyInComplete)
            {
                if (buffer.size() <= m_pHttpMessage->consumeLength())
                {
                    result = true;
                    break;
                }
            }

            result = handleHttpMessage(session, m_pHttpMessage);

            buffer.erase(0, m_pHttpMessage->consumeLength());
        }
        else if (status == HttpParserIncomplete)
        {
            //std::cout << "HttpParserIncomplete" << std::endl;
            result = true;
            break;
        }
        else if (status == HttpParserHeaderIncomplete)
        {
            //std::cout << "HttpParserHeaderIncomplete " << std::endl;
            result = true;
            break;
        }
        else
        {
            //std::cout << "HttpParserError " << std::endl;

            // 非HTTP消息
            buffer.clear();
            sendHttpResponse(session, 400, "Error");
            result = keepaliveSessionIfNecessory(session);
            break;
        }

        if (buffer.size() == 0)
            break;
    }
    
    return result;
}

bool AirJoyServerHandler::handleHttpMessage(TcpSessionPointer session, HttpMessagePointer pHttpMessage)
{
    if (pHttpMessage->httpType() == HttpRequest)
    {
        if (pHttpMessage->httpMethod() == "POST" && pHttpMessage->uri() == "/airjoy")
        {
            std::string sessionId = pHttpMessage->getValueByName("Query-Session-ID");
            std::string topic     = pHttpMessage->getValueByName("Query-Topic");
            std::string action    = pHttpMessage->getValueByName("Query-Action");
            std::string param     = base64::decode(pHttpMessage->content());

            return handleAirJoyRequest(session, sessionId, topic, action, param);
        } 

        return notSupportHttpRequest(session, pHttpMessage);
    }
    else
    {
        // nop
    }

    return true;
}

bool AirJoyServerHandler::notSupportHttpRequest(TcpSessionPointer session, HttpMessagePointer pHttpRequest)
{
    sendHttpResponse(session, 400, "Error");

    return keepaliveSessionIfNecessory(session);
}

bool AirJoyServerHandler::handleAirJoyRequest(TcpSessionPointer session, 
                                              const std::string &sessionId, 
                                              const std::string &topic, 
                                              const std::string &action, 
                                              const std::string &param)
{
    if (topic == AIRJOY_TOPIC_PUBSUB)
    {
        return handlePubsubRequest(session, sessionId, action, param);
    }

    if (topic == AIRJOY_TOPIC_AIRMEDIA)
    {
        return handleAirmediaRequest(session, sessionId, action, param);
    }

    return notSupportAirJoyRequest(session, sessionId, topic, action, param);
}

bool AirJoyServerHandler::notSupportAirJoyRequest(TcpSessionPointer session, 
                                                  const std::string &sessionId, 
                                                  const std::string &topic, 
                                                  const std::string &action, 
                                                  const std::string &param)
{
    sendAirJoyResponseErrorTopic(session, sessionId, topic, action, param);

    return keepaliveSessionIfNecessory(session);
}

bool AirJoyServerHandler::handlePubsubRequest(TcpSessionPointer session, 
                                              const std::string &sessionId, 
                                              const std::string &action, 
                                              const std::string &param)
{
    HttpTextParam text;
    text.loadBytes(param.c_str(), param.length());
    
    // 订阅
    if (action == PUBSUB_SUBSCRIBE)
    {
        // 订阅成功
        sendAirJoyResponseOk(session, sessionId, AIRJOY_TOPIC_PUBSUB, action, param);

        // Topic: AirMedia
        if (text.getValueByKey("Pubsub-Topic") == AIRJOY_TOPIC_AIRMEDIA)
        {
            LockType lock(m_subscribersForAirMediaMutex);
            SubscriberMap::iterator iter = m_subscribersForAirMedia.find(session);
            if (iter == m_subscribersForAirMedia.end())
                m_subscribersForAirMedia.insert(std::make_pair(session, session->getPeerIp()));

            // notify app
            AirHandlerMap::iterator iter2 = m_handlers.find(AIRJOY_TOPIC_AIRMEDIA);
            if (iter2 != m_handlers.end())
                iter2->second->didSubscribeEvent(session->getPeerIp(), AIRJOY_TOPIC_AIRMEDIA);

#ifdef _WIN32
            Sleep(10);
#else
            usleep(1000 * 10);
#endif // _WIN32

            // publish event if necessary
            LockType lock2(m_lastEventMutex);
            if (m_lastEvent.isValid())
                sendMessage(session, m_lastEvent.toPublishString());
        }

		return true;
    }

    // 取消订阅
    else if (action == PUBSUB_UNSUBSCRIBE)
    {
        sendAirJoyResponseOk(session, sessionId, AIRJOY_TOPIC_PUBSUB, action, param);

        // Topic: AirMedia
        if (text.getValueByKey("Pubsub-Topic") == AIRJOY_TOPIC_AIRMEDIA)
        {
            LockType lock(m_subscribersForAirMediaMutex);
            SubscriberMap::iterator iter = m_subscribersForAirMedia.find(session);
            if (iter != m_subscribersForAirMedia.end())
                m_subscribersForAirMedia.erase(iter);

            // notify app
            AirHandlerMap::iterator iter2 = m_handlers.find(AIRJOY_TOPIC_AIRMEDIA);
            if (iter2 != m_handlers.end())
                iter2->second->didUnsubscribeEvent(session->getPeerIp(), AIRJOY_TOPIC_AIRMEDIA);
        }
	
		return false;
	}
    else
    {
        sendAirJoyResponseErrorAction(session, sessionId, AIRJOY_TOPIC_PUBSUB, action, param);

		return keepaliveSessionIfNecessory(session);
    }

    return true;
}

bool AirJoyServerHandler::handleAirmediaRequest(TcpSessionPointer session, 
                                              const std::string &sessionId, 
                                              const std::string &action, 
                                              const std::string &param)
{
    LockType lock(m_handlersMutex);

    AirHandlerMap::iterator iter = m_handlers.find(AIRJOY_TOPIC_AIRMEDIA);
    if (iter == m_handlers.end())
    {
        sendAirJoyResponseErrorTopic(session, sessionId, AIRJOY_TOPIC_AIRMEDIA, action, param);
        return keepaliveSessionIfNecessory(session);
    }

    std::string appResult;
    bool done = iter->second->doAction(session->getPeerIp(),
                                       AIRJOY_TOPIC_AIRMEDIA,
                                       action,
                                       param,
                                       appResult);

    sendAirJoyResponseOk(session, sessionId, AIRJOY_TOPIC_AIRMEDIA, action, appResult);

    return keepaliveSessionIfNecessory(session);
}

bool AirJoyServerHandler::keepaliveSessionIfNecessory(TcpSessionPointer session)
{
    // 请求和订阅在一个连接
    LockType lock(m_subscribersForAirMediaMutex);

    SubscriberMap::iterator iter = m_subscribersForAirMedia.find(session);
    if (iter == m_subscribersForAirMedia.end())
        return false;

    return true;
}

void AirJoyServerHandler::sendMessage(TcpSessionPointer session, const std::string & message)
{
    TcpServerMessagePointer response = boost::make_shared<TcpServerMessage>(session);
    response->message() = message;

    session->startWrite(response);
}

void AirJoyServerHandler::sendHttpResponse(TcpSessionPointer session, int httpCode, const std::string &httpStatus)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(httpCode);
    httpResponse.sethttpResponseStatus(httpStatus);
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirJoyServerHandler::sendAirJoyResponse(TcpSessionPointer session,
                                             int httpCode,
                                             const std::string &httpStatus,
                                             const std::string &sessionId,
                                             const std::string &topic, 
                                             const std::string &action, 
                                             const std::string &result)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(httpCode);
    httpResponse.sethttpResponseStatus(httpStatus);

    httpResponse.addHeaders("User-Agent", "AirJoyServer/1.0");
    httpResponse.addHeaders("Query-Session-ID", sessionId);
    httpResponse.addHeaders("Query-Topic", topic);
    httpResponse.addHeaders("Query-Action", action);
    httpResponse.addHeaders("Content-Type", "text");    

    if (result.empty())
    {
        httpResponse.addHeaders("Content-Length", "0");
    }
    else
    {
        std::string resultBase64 = base64::encode(result);
    
        std::stringstream ssSize;
        ssSize << resultBase64.length();   

        httpResponse.addHeaders("Content-Length", ssSize.str());
        httpResponse.setContent(resultBase64);
    }

    sendMessage(session, httpResponse.toString());
}

void AirJoyServerHandler::sendAirJoyResponseOk(TcpSessionPointer session,
                                               const std::string &sessionId,
                                               const std::string &topic, 
                                               const std::string &action, 
                                               const std::string &result)
{
    sendAirJoyResponse(session, 200, "OK", sessionId, topic, action, result);
}

void AirJoyServerHandler::sendAirJoyResponseErrorAction(TcpSessionPointer session,
                                                        const std::string &sessionId,
                                                        const std::string &topic, 
                                                        const std::string &action, 
                                                        const std::string &result)
{
    sendAirJoyResponse(session, 404, "Not Support Action", sessionId, topic, action, result);
}

void AirJoyServerHandler::sendAirJoyResponseErrorTopic(TcpSessionPointer session,
                                                       const std::string &sessionId,
                                                       const std::string &topic, 
                                                       const std::string &action, 
                                                       const std::string &result)
{
    sendAirJoyResponse(session, 405, "Not Support Topic", sessionId, topic, action, result);
}
