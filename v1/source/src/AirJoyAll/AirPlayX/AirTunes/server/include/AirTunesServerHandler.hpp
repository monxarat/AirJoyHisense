/*
* AirTunesServerHandler.hpp
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

#ifndef __AIRTUNES_SERVER_HANDLER_H__
#define __AIRTUNES_SERVER_HANDLER_H__

#include "TcpServerMessage.hpp"
#include "AirJoyTypeDef.hpp"
#include "UmpServerHandler.hpp"
#include "UmpMessage.hpp"
#include "RtspMessage.hpp"
#include "AirTunesServiceInfo.hpp"
#include "AirCrypto.hpp"

#include <map>

namespace airjoy
{

    class AirTunesServer;
    class AirTunesServerListener;
    class AirPort;
    class AudioPlayer;
    class AudioBuffer;

    class AirTunesServerHandler : public UmpServerHandler
    {
    public:
        typedef struct _MediaValue
        {
            std::string value[3];     ;
        } MediaValue;
        typedef boost::shared_ptr<MediaValue>               MediaValuePointer;
        typedef std::map<std::string, MediaValuePointer>    MediaValueMap;

    public:
        typedef TcpServerMessage::TcpSessionPointer         TcpSessionPointer;
        typedef boost::shared_ptr<AirTunesServiceInfo>      AirTunesServiceInfoPointer;
        typedef boost::shared_ptr<RtspMessage>              RtspMessagePointer;
        typedef std::map<std::string, std::string>          KeyValueMap;
        typedef boost::mutex                                MutexType;
        typedef MutexType::scoped_lock                      LockType;
        typedef boost::shared_ptr<AirCrypto>                AirCryptoPointer;

        typedef enum _AirTunesStatus
        {
            AIRTUNES_STATUS_UNKNOWN              = 0,
            AIRTUNES_STATUS_OK                   = 200,
            AIRTUNES_STATUS_NOT_FOUND            = 404,
            AIRTUNES_STATUS_METHOD_NOT_ALLOWED   = 405,
            AIRTUNES_STATUS_NOT_IMPLEMENTED      = 501,
            AIRTUNES_STATUS_NO_RESPONSE_NEEDED   = 1000,
        } AirTunesStatus;
        
        typedef enum _StreamType
        {
            StreamTypeAirTunes           = 0,
            StreamTypeAirPlayMirroring   = 1,
        } StreamType;

    private:
        AirTunesServer              *m_server;
        AirTunesServerListener      *m_listener;
        AirTunesServiceInfoPointer   m_serviceInfo;

        // <TcpSession, IP>
        typedef std::map<TcpSessionPointer, std::string>   ConnectionMap;
        ConnectionMap                m_connections;
        MutexType                    m_connectionMutex;
        std::string                  m_currentPlayIp;

        // 一个会话，对应一个RTSP消息解析器
        typedef std::map<int, RtspMessagePointer> RtspParserMap;
        RtspParserMap                m_rtspParsers;
        MutexType                    m_rtspParsersMutex;

        AirPort                     *m_airport;
        AudioBuffer                 *m_audioBuffer;
        AudioPlayer                 *m_audioPlayer;

        AirCryptoPointer             m_pAirCrypto;

        int                          m_fmtp[12];
        unsigned char               *m_aeskey;
        int                          m_aeskeyLen;
        unsigned char               *m_aesiv;
        int                          m_aesivLen;

        std::string                  m_dacpServerIp;
        std::string                  m_dacpServerHostName;
        std::string                  m_dacpServerActiveRemote;

        // 流类型，AirTunes音频 或者是 AirPlay镜像音视频
        StreamType                   m_streamType;

        // 媒体格式
        std::string                  m_mediaFormat;

    public:
        AirTunesServerHandler(AirTunesServer *server, AirTunesServerListener *listener);
        virtual ~AirTunesServerHandler();

        void setServiceInfo(AirTunesServiceInfoPointer info) { m_serviceInfo = info; }

        void setDacpServerIp(const std::string &ip);
        void setDacpServerHostName(const std::string &hostName);
        void setDacpServerActiveRemote(const std::string &activeRemote);

        const std::string & dacpServerIp(void);
        const std::string & dacpServerHostName(void);
        const std::string & dacpServerActiveRemote(void);

        void disconnect(const std::string &ip);
        void stopAudioPlayer(const std::string &ip);

        // UmpServerHandler
        virtual bool didReceiveMessage(TcpServerMessagePointer request);
        virtual void didClose(int connectedId);

    private:
        bool insertRtspParser(int connectedId);
        bool removeRtspParser(int connectedId);
        RtspMessagePointer getRtspParser(int connectedId);
        AirCryptoPointer getAirCrypto(void);
       
        void createAirPort(const std::string &clientIp, uint16_t controlPort);
        void releaseAirPortIfNecessary(void);
        
        bool isCurrentConnection(TcpSessionPointer session);
        void insertConnection(TcpSessionPointer session);
        void closeConnection(const std::string &ip);
        void removeConnection(int connectId);

    private:
        void sendMessage(TcpSessionPointer session, const std::string & message);
        void sendMessage(TcpSessionPointer session, 
                         int statusCode, 
                         const std::string &statusString,
                         const std::string &server,
                         const std::string &seq);
            
        void getKeyValue(const std::string &text, KeyValueMap &map);
        void getMediaValue(const std::string &text, MediaValueMap &map);

    private:
        AirTunesStatus doPost(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doFpSetup(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doOptions(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doAnnounce(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doSetup(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doRecord(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doFlush(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doTeardown(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doGetParameter(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doSetParameter(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);

#if 0
        AirTunesStatus doDescribe(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doPause(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doPlay(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
        AirTunesStatus doRedirect(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage, std::string &responseContent);
#endif

    private:
        void doSetText(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage);
        void doSetImage(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage);
        void doSetMetadata(TcpServerMessagePointer request, RtspMessagePointer pRtspMessage);
    };

}


#endif // __AIRTUNES_SERVER_HANDLER_H__
