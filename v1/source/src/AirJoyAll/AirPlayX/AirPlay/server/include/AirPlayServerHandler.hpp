/*
* AirPlayServerHandler.hpp
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
*
*/

#ifndef __AIRPLAY_SERVER_HANDLER_H__
#define __AIRPLAY_SERVER_HANDLER_H__

#include "TcpServerMessage.hpp"
#include "AirJoyTypeDef.hpp"
#include "UmpServerHandler.hpp"
#include "UmpMessage.hpp"
#include "HttpMessage.hpp"
#include "AirPlayServiceInfo.hpp"
#include "AirPlayServer.hpp"
#include <map>

namespace airjoy
{

    class AirPlayServerListener;

    class AirPlayServerHandler : public UmpServerHandler
    {
    public:
        typedef TcpServerMessage::TcpSessionPointer         TcpSessionPointer;
        typedef boost::shared_ptr<AirPlayServiceInfo>       AirPlayServiceInfoPointer;
        typedef boost::shared_ptr<HttpMessage>              HttpMessagePointer;
        typedef std::map<int, TcpSessionPointer>            TcpSessionMap;
        typedef AirPlayServer::EventCategory                EventCategory;
        typedef AirPlayServer::EventAction                  EventAction;
        typedef boost::mutex                                MutexType;
        typedef MutexType::scoped_lock                      LockType;

        typedef enum _AirPlayStatus
        {
            AIRPLAY_STATUS_UNKNOWN              = 0,
            AIRPLAY_STATUS_OK                   = 200,
            AIRPLAY_STATUS_SWITCHING_PROTOCOLS  = 101,
            AIRPLAY_STATUS_NEED_AUTH            = 401,
            AIRPLAY_STATUS_NOT_FOUND            = 404,
            AIRPLAY_STATUS_METHOD_NOT_ALLOWED   = 405,
            AIRPLAY_STATUS_NOT_IMPLEMENTED      = 501,
            AIRPLAY_STATUS_NO_RESPONSE_NEEDED   = 1000,
        } AirPlayStatus;

    private:
        AirPlayServerListener           *m_listener;
        AirPlayServiceInfoPointer        m_serviceInfo;
        std::string                      m_time;
        std::string                      m_lastPhotoId;

        // 一个会话，对应一个缓冲区
        typedef std::map<int, std::string>                  BufferMap;
        BufferMap                                           m_buffers;
        MutexType                                           m_buffersMutex;

        // 事件订阅者列表(TcpSessionPointer, AppleSessionId)
        typedef std::map<TcpSessionPointer, std::string>    EventSubscriberMap;
        EventSubscriberMap                                  m_eventSubscribers;
        MutexType                                           m_eventSubscribersMutex;

        // 推送会话列表(TcpSessionPointer, AppleSessionId)
        typedef std::map<TcpSessionPointer, std::string>    PushSessionMap;
        PushSessionMap                                      m_pushSessions;
        MutexType                                           m_pushSessionsMutex;
        
        // 幻灯片反向会话会话列表(TcpSessionPointer, AppleSessionId)
        typedef std::map<TcpSessionPointer, std::string>    SlideshowsMap;
        SlideshowsMap                                       m_slideshows;
        MutexType                                           m_slideshowsMutex;

    public:
        AirPlayServerHandler(AirPlayServer *server, AirPlayServerListener *listener);
        virtual ~AirPlayServerHandler();

        void setServiceInfo(AirPlayServiceInfoPointer info) { m_serviceInfo = info; }

        /**
         * 向客户端发布事件
         *  @type           [in] 事件类型
         *  @action         [in] 动作
         *  @clientIp       [in] 客户端ip
         */
        void publishEvent(EventCategory type, EventAction action, const std::string &clientIp);

        /**
         * 关闭与客户端的连接
         *  @clientIp       [in] 客户端ip
         */ 
        void closeConnection(const std::string &clientIp);
                
        /**
         * 向客户端获取一张幻灯片用的图片
         *  @clientIp       [in] 客户端ip
         */
        void getSlideshowsPicture(const std::string &clientIp);

        // UmpServerHandler
        virtual bool didReceiveMessage(TcpServerMessagePointer request);
        virtual void didClose(int connectedId);

    private:
        void addBuffer(TcpServerMessagePointer request);
        void handleBufferBySession(TcpSessionPointer session);
        bool handleHttpMessage(TcpSessionPointer session, HttpMessagePointer pHttpMessage);

    private:
        bool addEventSubscriber(TcpSessionPointer p, const std::string &appleSessionId);
        bool removeEventSubscriber(TcpSessionPointer p);
        bool removeEventSubscriber(int connectedId);
        bool closeEventSessionByClientIp(const std::string &clientIp);

        bool addPushSession(TcpSessionPointer p, const std::string &appleSessionId);
        bool removePushSession(TcpSessionPointer p);
        bool removePushSession(const std::string &applsSessionId);
        bool removePushSession(int connectedId);
        bool closePushSession(const std::string &applsSessionId);
        bool closePushSessionByClientIp(const std::string &clientIp);

        bool addSlideshowsSession(TcpSessionPointer p, const std::string &appleSessionId);
        bool removeSlideshowsSession(TcpSessionPointer p);
        bool removeSlideshowsSession(const std::string &applsSessionId);
        bool removeSlideshowsSession(int connectedId);
        bool isSlideshowsSession(TcpSessionPointer p);

        void willAction(HttpMessage &httpMessage, const std::string &clientIp);

        const std::string & getNowTime(void);
        void sendMessage(TcpSessionPointer session, const std::string & message);
        void sendResponseOk(TcpSessionPointer session, std::string &responseContent);
        void sendResponseSwitchingProtocols(TcpSessionPointer session);
        void sendResponseUnauthorized(TcpSessionPointer session);
        void sendResponseNotFound(TcpSessionPointer session);
        void sendResponseNotImplemented(TcpSessionPointer session);
        void sendResponseMethodNotAllowed(TcpSessionPointer session);

        AirPlayStatus didGet(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didGetServerInfo(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didGetSlideshowFeatures(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didGetScrub(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didGetPlaybackInfo(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);

        AirPlayStatus didPost(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPostReverse(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPostStop(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPostPlay(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPostScrub(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPostRate(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPostGetProperty(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPostVolume(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);

        AirPlayStatus didPut(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPutPhoto(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPutSetProperty(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);
        AirPlayStatus didPutSlideshows(TcpSessionPointer p, HttpMessagePointer pHttpMessage, std::string &responseContent);

        void recvHttpResponse(TcpSessionPointer p, HttpMessagePointer pHttpMessage);
    };

}


#endif // __AIRPLAY_SERVER_HANDLER_H__