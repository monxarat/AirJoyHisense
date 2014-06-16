/*
* AirPlayEvent.cpp
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

#include "AirPlayEvent.hpp"
#include "HttpMessage.hpp"
#include <sstream>

using namespace airjoy;

AirPlayEvent::AirPlayEvent(EventCategory type, EventAction action, const std::string &sessionId)
    : m_type(type)
    , m_action(action)
    , m_sessionId(sessionId)
{
}

AirPlayEvent::~AirPlayEvent()
{
}

const std::string & AirPlayEvent::toString(void)
{
    std::string stringCategory;

    switch (m_type)
    {
    case AirPlayServer::EventUnknown:
        break;

    case AirPlayServer::EventPhoto:
        stringCategory = "photo";
        break;

    case AirPlayServer::EventVideo:
        stringCategory = "video";
        break;

    case AirPlayServer::EventSlideshow:
        stringCategory = "slideshow";
        break;

    default:
        break;
    }

    std::string stringState;

    switch (m_action)
    {
    case AirPlayServer::EventActionPlaying:
        stringState = "playing";
        break;

    case AirPlayServer::EventActionLoading:
        stringState = "loading";
        break;

    case AirPlayServer::EventActionPaused:
        stringState = "paused";
        break;

    case AirPlayServer::EventActionStopped:
        stringState = "stopped";
        break;

    default:
        break;
    }
    
    std::stringstream ss;
    ss <<  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\r\n";
    ss << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n";
    ss << "<plist version=\"1.0\">" << "\r\n";
    ss << "<dict>" << "\r\n";
    ss << "<key>category</key>" << "\r\n";
    ss << "<string>" << stringCategory << "</string>" << "\r\n";
    ss << "<key>state</key>" << "\r\n";
    ss << "<string>" << stringState << "</string>" << "\r\n";
    ss << "</dict>" << "\r\n";
    ss << "</plist>" << "\r\n";
    
    std::string eventInfo(ss.str());
    std::stringstream ssSize;
    ssSize << eventInfo.length();

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("POST");
    httpRequest.setUri("/event");

    httpRequest.addHeaders("Content-Type", "text/x-apple-plist+xml");
    httpRequest.addHeaders("Content-Length", ssSize.str());
    httpRequest.addHeaders("X-Apple-Session-ID", m_sessionId);

    httpRequest.setContent(eventInfo);

    m_string = httpRequest.toString();

    return m_string;
}