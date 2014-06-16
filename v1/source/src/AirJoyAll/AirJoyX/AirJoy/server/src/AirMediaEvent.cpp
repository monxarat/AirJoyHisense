/*
* AirMediaEvent.cpp
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

#include "AirMediaEvent.hpp"
#include "HttpMessage.hpp"
#include "HttpTextParam.hpp"
#include "AirJoyDefault.hpp"
#include "UtilBase64.hpp"
#include <sstream>

using namespace airjoy;

AirMediaEvent::AirMediaEvent()
    : m_isValid(false)
{
}

AirMediaEvent::~AirMediaEvent()
{
}

bool AirMediaEvent::isValid()
{
    return m_isValid;
}

void AirMediaEvent::set(const std::string &mediaType, 
                        const std::string &action, 
                        const std::string &id,
                        const std::string &url,
                        const std::string &name,
                        const std::string &fromIp)
{
    m_mediaType = mediaType;
    m_action    = action;
    m_id        = id;
    m_url       = url;
    m_name      = name;
    m_fromIp    = fromIp;

    m_isValid   = true;
}

const std::string & AirMediaEvent::toPublishString(void)
{
    if (! m_isValid)
        return m_emptyString;

    HttpTextParam text;
    text.setValueByKey("Pubsub-Topic", AIRJOY_TOPIC_AIRMEDIA);
    text.setValueByKey("Type", m_mediaType);
    text.setValueByKey("Action", m_action);
    text.setValueByKey("Id", m_id);
    text.setValueByKey("Url", m_url);
    text.setValueByKey("Name", m_name);
    text.setValueByKey("From", m_fromIp);
    std::string eventBase64 = base64::encode(text.toString());

    std::stringstream ssSize;
    ssSize << eventBase64.length();

    HttpMessage httpMessage;
    httpMessage.setHttpType(HttpRequest);
    httpMessage.setHttpVersionMajor(1);
    httpMessage.setHttpVersionMinor(1);
    httpMessage.setHttpMethod("POST");
    httpMessage.setUri("/airjoy");
    httpMessage.addHeaders("User-Agent", "AirJoyServer/1.0");
    httpMessage.addHeaders("Query-Session-ID", "00000000-0000-0000-0000-000000000000");
    httpMessage.addHeaders("Query-Topic", AIRJOY_TOPIC_PUBSUB);
    httpMessage.addHeaders("Query-Action", PUBSUB_PUBLISH);

    httpMessage.addHeaders("Content-Length", ssSize.str());
    httpMessage.addHeaders("Content-Type", "text");
    httpMessage.setContent(eventBase64);

	m_publishString = httpMessage.toString();

    return m_publishString;
}
