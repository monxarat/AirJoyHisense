/*
 *
 * AirFileClientHandler.cpp
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

#include "AirFileClientHandler.hpp"
#include "AirFIleClient.hpp"
#include "TcpClientMessage.hpp"
#include "AirJoyDefault.hpp"
#include "AirFileClientListener.hpp"
#include "AirJoyTypeDef.hpp"
#include "UtilBase64.hpp"
#include "ResultAudio.hpp"
#include "ResultPhoto.hpp"
#include "ResultVideo.hpp"
#include "ResultVolume.hpp"

using namespace airjoy;

AirFileClientHandler::AirFileClientHandler(AirFIleClient *client, AirFileClientListener *listener)
    : m_client(client)
	, m_listener(listener)
{
}

AirFileClientHandler::~AirFileClientHandler()
{
}

void AirFileClientHandler::didReceiveMessage(TcpClientMessagePointer p)
{
    if (m_listener == NULL)
        return;

    if (p->messageType() == TcpClientMessage::ConnectOk)
    {
        m_buffer.clear();
        return;
    }
    
    if (p->messageType() == TcpClientMessage::DisConnect)
    {
        m_buffer.clear();
        m_listener->didUnsubscribeEvent(p->serverIp());
        return;
    }

    // 超时
    if (p->messageType() == TcpClientMessage::Timeout)
    {
        m_listener->didTimeout(getSessionId(p->message()));
        return;
    }

    // 连接失败
    if (p->messageType() == TcpClientMessage::ConnectFailed)
    {
        m_listener->didErrorNet(getSessionId(p->message()));
        return;
    }
    
    //  网络错误
    if (p->messageType() == TcpClientMessage::ErrorNet)
    {
        m_listener->handleResult(getSessionId(p->message()), ResultCodeErrorConnection);
        return;
    }

	//std::cout << "didReceiveMessage: " << p->message() << std::endl;

	// 读取文本消息
    if (p->messageType() == TcpClientMessage::NormalMessage)
    {
#if 0
        if (HttpParserDone == m_pHttpMessage->loadBytes(p->message().c_str(), p->message().length()))
        {
            // 收到应答消息
            if (m_pHttpMessage->httpType() == HttpResponse)
            {
                didHandleHttpResponse(p->serverIp(), m_pHttpMessage);
            }

            // 收到请求消息
            else if (m_pHttpMessage->httpType() == HttpRequest)
            {
                didHandleHttpRequest(p->serverIp(), m_pHttpMessage);
            }

            m_pHttpMessage = boost::make_shared<HttpMessage>();
        }
#else
        this->addBuffer(p);
        this->handleBuffer(p->serverIp());
#endif
    }
}

void AirFileClientHandler::addBuffer(TcpClientMessagePointer p)
{
    //std::cout << "--------- add buffer -----------"<< std::endl;
    //std::cout << p->message() << std::endl;

    LockType lock(m_bufferMutex);
    m_buffer.append(p->message());
}

bool AirFileClientHandler::handleBuffer(const std::string &serverIp)
{
    bool result = true;

    LockType lock(m_bufferMutex);

    while (1)
    {
        //std::cout << "handle buffer: " << m_buffer.size() << std::endl;
        
        // We need a new HttpMessage ...
        m_pHttpMessage = boost::make_shared<HttpMessage>();
        
        HttpParserStatus status = m_pHttpMessage->loadBytes(m_buffer.c_str(), m_buffer.size());
        if (HttpParserDone == status)
        {
            //std::cout << "buffer consume: " << m_pHttpMessage->consumeLength() << std::endl;

            HttpMessageType type = GetHttpMessageType(m_pHttpMessage);
            
            if (type == HttpRequestAirJoyInComplete || type == HttpResponseAirJoyInComplete)
            {
                //std::cout << "AirJoyInComplete" << std::endl;
                if (m_buffer.size() <= m_pHttpMessage->consumeLength())
                {
                    // 并且缓冲没有更多的数据，则此消息不处理，等待下次数据到达再处理
                    std::cout << "do not handle" << std::endl;
                    result = true;
                    break;
                }
            }

            // 消费一些字节数
            m_buffer.erase(0, m_pHttpMessage->consumeLength());

            // 收到应答消息
            if (m_pHttpMessage->httpType() == HttpResponse)
            {
                didHandleHttpResponse(serverIp, m_pHttpMessage);
            }

            // 收到请求消息
            else if (m_pHttpMessage->httpType() == HttpRequest)
            {
                didHandleHttpRequest(serverIp, m_pHttpMessage);
            }

        }
        else if (status == HttpParserIncomplete)
        {
            break;
        }
        else if (status == HttpParserHeaderIncomplete)
        {
            break;
        }
        else
        {
            //std::cout << "HttpParser error" << std::endl;

            // 非HTTP消息
            m_buffer.clear();
            break;
        }

        if (m_buffer.size() == 0)
            break;
    }

    return result;
}

int AirFileClientHandler::getSessionId(const std::string &message)
{
    int sessionId = 0;

    HttpMessage httpMessage;
    if (HttpParserDone == httpMessage.loadBytes(message.c_str(), message.length()))
    {
        const std::string & stringValue = httpMessage.getValueByName("Query-Session-ID");

        try
        {
            sessionId = boost::lexical_cast<int>(stringValue);
        }
        catch (boost::bad_lexical_cast &e)
        {
            //std::cout << "error: " << e.what() << std::endl;
        }
    }

    return sessionId;
}

int AirFileClientHandler::getQuerySessionId(const std::string &stringSessionId)
{
    int sessionId = 0;

    try
    {
        sessionId = boost::lexical_cast<int>(stringSessionId);
    }
    catch (boost::bad_lexical_cast &e)
    {
        //std::cout << "error: " << e.what() << std::endl;
    }

    return sessionId;
}

void AirFileClientHandler::didHandleHttpResponse(const std::string &serverIp, HttpMessagePointer pHttpResponse)
{
    int sessionId = getQuerySessionId(pHttpResponse->getValueByName("Query-Session-ID"));

    if (pHttpResponse->httpResponseCode() == 200)
    {
        std::string topic     = pHttpResponse->getValueByName("Query-Topic");
        std::string action    = pHttpResponse->getValueByName("Query-Action");
        std::string result    = base64::decode(pHttpResponse->content());

        if (topic == AIRJOY_TOPIC_AIRMEDIA)
        {
            didHandleQueryResult(sessionId, action, result);
            return;
        }

        if (topic == AIRJOY_TOPIC_PUBSUB)
        {
            didHandlePubsubResult(serverIp, action, result);
        }

        return;
    }

    m_listener->handleResult(sessionId, ResultCodeErrorNotSupport);
}

void AirFileClientHandler::didHandleHttpRequest(const std::string &serverIp, HttpMessagePointer pHttpResquest)
{
    if (pHttpResquest->httpMethod() == "POST" && pHttpResquest->uri() == "/airjoy")
    {
        std::string topic     = pHttpResquest->getValueByName("Query-Topic");
        std::string action    = pHttpResquest->getValueByName("Query-Action");
        std::string param     = base64::decode(pHttpResquest->content());

        HttpTextParam txt;
        txt.loadBytes(param.c_str(), param.size());

        m_listener->recvEvent(txt.getValueByKey("Type"),
                              txt.getValueByKey("Action"),
                              txt.getValueByKey("Id"),
                              txt.getValueByKey("Url"),
                              txt.getValueByKey("Name"),
                              txt.getValueByKey("From"));
    }
}

void AirFileClientHandler::didHandlePubsubResult(const std::string &serverIp, const std::string &action, const std::string &param)
{
    if (action == PUBSUB_SUBSCRIBE)
    {
        m_listener->didSubscribeEvent(serverIp);
        return;
    }

    if (action == PUBSUB_UNSUBSCRIBE)
    {
        // 断开连接时再通知上层

        // nop
        return;
    }
}

void AirFileClientHandler::didHandleQueryResult(int sessionId, const std::string &action, const std::string &param)
{
    // 照片消息
    if (action == AIRMEDIA_PHOTO_PUT)
    {
        ResultPhoto result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_PHOTO_DISPLAY)
    {
        ResultPhoto result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_PHOTO_STOP)
    {
        ResultPhoto result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_PHOTO_ROTATE)
    {
        ResultPhoto result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }
    
    if (action == AIRMEDIA_PHOTO_ZOOM)
    {
        ResultPhoto result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_PHOTO_MOVE)
    {
        ResultPhoto result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    // 视频消息
    if (action == AIRMEDIA_VIDEO_PUT)
    {
        ResultVideo result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_VIDEO_RATE)
    {
        ResultVideo result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_VIDEO_STOP)
    {
        ResultVideo result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_VIDEO_SETPROGRESS)
    {
        ResultVideo result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_VIDEO_GETPROGRESS)
    {
        ResultVideo result;
        if (result.load(param))
            m_listener->handleGetPlayVideoProgressResult(sessionId, result.code(), result.rate(), result.position());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }
    
    if (action == AIRMEDIA_VIDEO_GETPLAYBACKINFO)
    {
        ResultVideo result;
        if (result.load(param))
        {
            AirJoyPlayBackInfo info;

            size_t len = sizeof(info.id);
            memset(info.id, 0, len);
            strncpy(info.id, result.videoId().c_str(), len - 1);

            len = sizeof(info.name);
            memset(info.name, 0, len);
            strncpy(info.name, result.name().c_str(), len - 1);

            len = sizeof(info.url);
            memset(info.url, 0, len);
            strncpy(info.url, result.url().c_str(), len - 1);

            info.position = result.position();
            info.rate = result.rate();
            info.duration = result.duration();

            m_listener->handleGetPlayVideoInfoResult(sessionId, result.code(), info);
        }
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    // 音频消息
    if (action == AIRMEDIA_AUDIO_PUT)
    {
        ResultAudio result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_AUDIO_RATE)
    {
        ResultAudio result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_AUDIO_STOP)
    {
        ResultAudio result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_AUDIO_SETPROGRESS)
    {
        ResultAudio result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_AUDIO_GETPROGRESS)
    {
        ResultAudio result;
        if (result.load(param))
            m_listener->handleGetPlayAudioProgressResult(sessionId, result.code(), result.rate(), result.position());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_AUDIO_GETPLAYBACKINFO)
    {
        ResultAudio result;
        if (result.load(param))
        {
            AirJoyPlayBackInfo info;

            size_t len = sizeof(info.id);
            memset(info.id, 0, len);
            strncpy(info.id, result.audioId().c_str(), len - 1);

            len = sizeof(info.name);
            memset(info.name, 0, len);
            strncpy(info.name, result.name().c_str(), len - 1);

            len = sizeof(info.url);
            memset(info.url, 0, len);
            strncpy(info.url, result.url().c_str(), len - 1);

            info.position = result.position();
            info.rate = result.rate();
            info.duration = result.duration();

            m_listener->handleGetPlayAudioInfoResult(sessionId, result.code(), info);
        }
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    // 音频消息
    if (action == AIRMEDIA_VOLUME_SET)
    {
        ResultVolume result;
        if (result.load(param))
            m_listener->handleResult(sessionId, result.code());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    if (action == AIRMEDIA_VOLUME_GET)
    {
        ResultVolume result;
        if (result.load(param))
            m_listener->handleGetVolumeResult(sessionId, result.code(), result.volume());
        else
            m_listener->handleResult(sessionId, ResultCodeUnknown);

        return;
    }

    m_listener->handleResult(sessionId, action, param);
}
