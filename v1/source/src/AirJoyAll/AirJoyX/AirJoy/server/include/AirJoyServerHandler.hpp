/*
* AirJoyServerHandler.hpp
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

#ifndef __AIRJOY_SERVER_HANDLER_H__
#define __AIRJOY_SERVER_HANDLER_H__

#include "TcpServerMessage.hpp"
#include "AirJoyTypeDef.hpp"
#include "UmpServerHandler.hpp"
#include "UmpMessage.hpp"
#include "HttpMessage.hpp"
#include "HttpMessageType.hpp"
#include "AirMediaEvent.hpp"
#include <map>

namespace airjoy
{
    
    class AirMessageHandler;

    class AirJoyServerHandler : public UmpServerHandler
    {
    public:
        typedef TcpServerMessage::TcpSessionPointer                 TcpSessionPointer;
        typedef boost::shared_ptr<AirMessageHandler>                AirMessageHandlerPointer;
        typedef std::map<std::string, AirMessageHandlerPointer>     AirHandlerMap;
        typedef std::map<int, std::string>                          BufferMap;
        typedef boost::mutex                                        MutexType;
        typedef MutexType::scoped_lock                              LockType;

    private:
        HttpMessagePointer          m_pHttpMessage;

        // �����б�
        BufferMap                   m_buffers;
        MutexType                   m_buffersMutex;

        // ��Ϣ�������б�
        AirHandlerMap               m_handlers;
        MutexType                   m_handlersMutex;

        // AirMedia�������б�
        typedef std::map<TcpSessionPointer, std::string>    SubscriberMap;
        SubscriberMap                                       m_subscribersForAirMedia;
        MutexType                                           m_subscribersForAirMediaMutex;

        // ��һ���¼�
        AirMediaEvent                       m_lastEvent;
        MutexType                           m_lastEventMutex;

    public:
        AirJoyServerHandler();
        virtual ~AirJoyServerHandler();

        // UmpServerHandler
        virtual bool didReceiveMessage(TcpServerMessagePointer request);
        virtual void didClose(int connectedId);

        // add & remove message handler
        void addAirMessageHandler(const std::string &type, AirMessageHandlerPointer handler);
        void removeAirMessageHandler(const std::string &type);

        /**
         * ����ý���¼�
         *  @mediaType  [in] �¼����ͣ�����Ϊ: [photo, audio, video, init]
         *  @action     [in] �¼�����, ����Ϊ��[play, pause, stop, init]
         *  @id         [in] ͼƬID����Ƶ��ID����Ƶ��ID
         *  @url        [in] ͼƬ��ַ����Ƶ��ַ����Ƶ��ַ
         *  @name       [in] ͼƬ���ơ���Ƶ���ơ���Ƶ����
         *  @fromIp     [in] ý����Դ��ַ
         */
        bool publishMediaEvent(const std::string &mediaType, 
                               const std::string &action, 
                               const std::string &id,
                               const std::string &url,
                               const std::string &name,
                               const std::string &fromIp);

    private:
        void addBuffer(TcpServerMessagePointer request);
        bool handleBufferBySession(TcpSessionPointer session);
        
    private:
        bool handleHttpMessage(TcpSessionPointer session, HttpMessagePointer pHttpMessage);
        bool notSupportHttpRequest(TcpSessionPointer session, HttpMessagePointer pHttpRequest);

        bool handleAirJoyRequest(TcpSessionPointer session, 
                                 const std::string &sessionId, 
                                 const std::string &topic, 
                                 const std::string &action, 
                                 const std::string &param);

        bool notSupportAirJoyRequest(TcpSessionPointer session, 
                                     const std::string &sessionId, 
                                     const std::string &topic, 
                                     const std::string &action, 
                                     const std::string &param);


        bool handlePubsubRequest(TcpSessionPointer session, 
                                 const std::string &sessionId, 
                                 const std::string &action, 
                                 const std::string &param);
        
        bool handleAirmediaRequest(TcpSessionPointer session, 
                                   const std::string &sessionId, 
                                   const std::string &action, 
                                   const std::string &param);

        bool notSupportAirmediaRequest(TcpSessionPointer session, 
                                       const std::string &sessionId, 
                                       const std::string &action, 
                                       const std::string &param);

    private:
        bool keepaliveSessionIfNecessory(TcpSessionPointer session);
        void sendMessage(TcpSessionPointer session, const std::string &message);

        void sendHttpResponse(TcpSessionPointer session, int httpCode, const std::string &httpStatus);

        void sendAirJoyResponse(TcpSessionPointer session,
                                int httpCode,
                                const std::string &httpStatus,
                                const std::string &sessionId,
                                const std::string &topic, 
                                const std::string &action, 
                                const std::string &result);

        void sendAirJoyResponseOk(TcpSessionPointer session,
                                  const std::string &sessionId,
                                  const std::string &topic, 
                                  const std::string &action, 
                                  const std::string &result);
        
        void sendAirJoyResponseErrorAction(TcpSessionPointer session,
                                           const std::string &sessionId,
                                           const std::string &topic, 
                                           const std::string &action, 
                                           const std::string &result);

        void sendAirJoyResponseErrorTopic(TcpSessionPointer session,
                                          const std::string &sessionId,
                                          const std::string &topic, 
                                          const std::string &action, 
                                          const std::string &result);
    };

}

#endif // __AIRJOY_SERVER_HANDLER_H__