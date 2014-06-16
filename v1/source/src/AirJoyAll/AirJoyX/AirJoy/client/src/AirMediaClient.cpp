/*
 * AirMediaClient.cpp
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

#include "AirMediaClient.hpp"
#include "AirMediaClientQueryHandler.hpp"
#include "AirJoyClientCore.hpp"
#include "UmpClient.hpp"
#include "UtilBase64.hpp"
#include "HttpMessage.hpp"
#include "HttpTextParam.hpp"
#include "AirJoyDefault.hpp"

using namespace airjoy;

#define     DEFAULT_TIME_OUT    6

AirMediaClient::AirMediaClient()
	: m_sessionId(1000000)
    , m_photo(NULL)
    , m_video(NULL)
    , m_audio(NULL)
    , m_volume(NULL)
{
    m_photo = new AirMediaClientPhoto(this);
    m_video = new AirMediaClientVideo(this);
    m_audio = new AirMediaClientAudio(this);
    m_volume = new AirMediaClientVolume(this);
}

AirMediaClient::~AirMediaClient()
{
    delete m_volume;
    delete m_audio;
    delete m_video;
    delete m_photo;
}

void AirMediaClient::setListener(AirMediaClientQueryListener *listener)
{
    typedef boost::shared_ptr<AirMediaClientQueryHandler>         AirMediaClientQueryHandlerPointer;
    AirMediaClientQueryHandlerPointer queryHandler = boost::make_shared<AirMediaClientQueryHandler>(this, listener);
    this->core()->client()->setHandler(queryHandler);
}

bool AirMediaClient::subscribeEvent(const std::string &ip, uint16_t port)
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

bool AirMediaClient::unsubscribeEvent(void)
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

AirJoySessionId AirMediaClient::send(const std::string &appAction,
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

AirMediaClientPhoto * AirMediaClient::photo(void)
{
    return m_photo;
}

AirMediaClientVideo * AirMediaClient::video(void)
{
    return m_video;
}

AirMediaClientAudio * AirMediaClient::audio(void)
{
    return m_audio;
}

AirMediaClientVolume * AirMediaClient::volume(void)
{
    return m_volume;
}
