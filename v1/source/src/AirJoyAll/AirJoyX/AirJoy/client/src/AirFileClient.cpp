/*
 * AirFileClient.cpp
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

#include "AirFileClient.hpp"
#include "AirFileClientHandler.hpp"
#include "AirJoyClientCore.hpp"
#include "UmpClient.hpp"
#include "UtilBase64.hpp"
#include "HttpMessage.hpp"
#include "HttpTextParam.hpp"
#include "AirJoyDefault.hpp"

using namespace airjoy;

#define     DEFAULT_TIME_OUT    6

AirFileClient::AirFileClient()
	: m_sessionId(1000000)
    , m_photo(NULL)
    , m_video(NULL)
    , m_audio(NULL)
    , m_volume(NULL)
{
    m_photo = new AirFileClientPhoto(this);
    m_video = new AirFileClientVideo(this);
    m_audio = new AirFileClientAudio(this);
    m_volume = new AirFileClientVolume(this);
}

AirFileClient::~AirFileClient()
{
    delete m_volume;
    delete m_audio;
    delete m_video;
    delete m_photo;
}

void AirFileClient::setListener(AirFileClientListener *listener)
{
    typedef boost::shared_ptr<AirFileClientHandler>         AirFileClientHandlerPointer;
    AirFileClientHandlerPointer queryHandler = boost::make_shared<AirFileClientHandler>(this, listener);
    this->core()->client()->setHandler(queryHandler);
}

bool AirFileClient::subscribeEvent(const std::string &ip, uint16_t port)
{
    if (this->core()->client()->isConnected())
    {
        this->core()->client()->disconnect();
    }

    HttpTextParam text;
    text.setValueByKey("Pubsub-Topic", AIRJOY_TOPIC_AIRMEDIA);
    std::string paramBase64 = base64::encode(text.toString());

    std::stringstream ssSize;
    ssSize << paramBase64.length();

    HttpMessage httpMessage;
    httpMessage.setHttpType(HttpRequest);
    httpMessage.setHttpVersionMajor(1);
    httpMessage.setHttpVersionMinor(1);
    httpMessage.setHttpMethod("POST");
    httpMessage.setUri("/airjoy");
    httpMessage.addHeaders("User-Agent", "AirJoyClient/1.0");
    httpMessage.addHeaders("Query-Session-ID", "00000000-0000-0000-0000-000000000000");
    httpMessage.addHeaders("Query-Topic", AIRJOY_TOPIC_PUBSUB);
    httpMessage.addHeaders("Query-Action", PUBSUB_SUBSCRIBE);

    httpMessage.addHeaders("Content-Length", ssSize.str());
    httpMessage.addHeaders("Content-Type", "text");
    httpMessage.setContent(paramBase64);

    return this->core()->client()->send(ip, port, httpMessage.toString(), DEFAULT_TIME_OUT);
}

bool AirFileClient::unsubscribeEvent(void)
{
    if (! this->core()->client()->isConnected())
        return false;

    HttpTextParam text;
    text.setValueByKey("Pubsub-Topic", AIRJOY_TOPIC_AIRMEDIA);
    std::string paramBase64 = base64::encode(text.toString());

    std::stringstream ssSize;
    ssSize << paramBase64.length();
    
    HttpMessage httpMessage;
    httpMessage.setHttpType(HttpRequest);
    httpMessage.setHttpVersionMajor(1);
    httpMessage.setHttpVersionMinor(1);
    httpMessage.setHttpMethod("POST");
    httpMessage.setUri("/airjoy");
    httpMessage.addHeaders("User-Agent", "AirJoyClient/1.0");
    httpMessage.addHeaders("Query-Session-ID", "00000000-0000-0000-0000-000000000000");
    httpMessage.addHeaders("Query-Topic", AIRJOY_TOPIC_PUBSUB);
    httpMessage.addHeaders("Query-Action", PUBSUB_UNSUBSCRIBE);

    httpMessage.addHeaders("Content-Length", ssSize.str());
    httpMessage.addHeaders("Content-Type", "text");
    httpMessage.setContent(paramBase64);

    return this->core()->client()->send(httpMessage.toString(), DEFAULT_TIME_OUT);
}

AirJoySessionId AirFileClient::send(const std::string &appAction,
                                     const std::string &appParam)
{
    std::stringstream ssSessionId;
    ssSessionId << m_sessionId;
    
    HttpMessage httpMessage;
    httpMessage.setHttpType(HttpRequest);
    httpMessage.setHttpVersionMajor(1);
    httpMessage.setHttpVersionMinor(1);
    httpMessage.setHttpMethod("POST");
    httpMessage.setUri("/airjoy");
    httpMessage.addHeaders("User-Agent", "AirJoyClient/1.0");
    httpMessage.addHeaders("Query-Session-ID", ssSessionId.str());
    httpMessage.addHeaders("Query-Topic", AIRJOY_TOPIC_AIRMEDIA);
    httpMessage.addHeaders("Query-Action", appAction);
    httpMessage.addHeaders("Content-Type", "text");    

    if (appParam.empty())
    {
        httpMessage.addHeaders("Content-Length", "0");
    }
    else
    {
        std::string paramBase64 = base64::encode(appParam);
    
        std::stringstream ssSize;
        ssSize << paramBase64.length();   

        httpMessage.addHeaders("Content-Length", ssSize.str());
        httpMessage.setContent(paramBase64);
    }

    if (! this->core()->client()->isConnected())
        return 0;

    this->core()->client()->send(httpMessage.toString(), DEFAULT_TIME_OUT);

	return m_sessionId++;
}

AirFileClientPhoto * AirFileClient::photo(void)
{
    return m_photo;
}

AirFileClientVideo * AirFileClient::video(void)
{
    return m_video;
}

AirFileClientAudio * AirFileClient::audio(void)
{
    return m_audio;
}

AirFileClientVolume * AirFileClient::volume(void)
{
    return m_volume;
}
