/*
 * AirPlayClientVideo.cpp
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

#include "AirPlayClientVideo.hpp"
#include "AirPlayClient.hpp"
#include "HttpMessage.hpp"
#include <sstream>

using namespace anyplay;

AirPlayClientVideo::AirPlayClientVideo(AirPlayClient *sender)
    : m_sender(sender)
{
}

AirPlayClientVideo::~AirPlayClientVideo()
{
}

/*
client ¡ú server
-----------------------------------------------------------------
POST /play HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 163
Content-Type: text/parameters

Content-Location: http://192.168.1.18:3689/airplay.mp4?database-spec='dmap.persistentid:0x63b5e5c0c201542e'&item-spec='dmap.itemid:0x21d'
Start-Position: 0.174051
*/
bool AirPlayClientVideo::playVideo(AnyPlayURL &url, const std::string &videoId, float startPosition)
{
    if (url.protocolType() == File)
    {
        return false;
    }

    std::stringstream ssStartPosition;
    ssStartPosition << startPosition;

    std::string content;
    content.append("Content-Location: ");
    content.append(url.url());
    content.append("Start-Position: ");
    content.append(ssStartPosition.str());

    std::stringstream ssContentLength;
    ssContentLength << content.length();

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("POST");
    httpRequest.setUri("/play");

    httpRequest.addHeaders("User-Agent", "iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5");
    httpRequest.addHeaders("Content-Length", ssContentLength.str());
    httpRequest.addHeaders("Content-Type", "text/parameters");

    httpRequest.setContent(content);

    m_sender->setRequestType(AirPlayClient::RequestTypePostPlay);

    return m_sender->send(httpRequest.toString());
}

/*
client ¡ú server
-----------------------------------------------------------------
POST /rate?value=0.000000 HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 0
*/
bool AirPlayClientVideo::setPlayVideoRate(float rate)
{
    std::stringstream ssRate;
    ssRate << "/rate?value=" << rate;

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("POST");
    httpRequest.setUri(ssRate.str());

    httpRequest.addHeaders("User-Agent", "iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5");
    httpRequest.addHeaders("Content-Length", "0");
    
    m_sender->setRequestType(AirPlayClient::RequestTypePostRate);

    return m_sender->send(httpRequest.toString());
}
/*
client ¡ú server
-----------------------------------------------------------------
POST /stop HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 0
*/
bool AirPlayClientVideo::stopPlayVideo(void)
{
    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("POST");
    httpRequest.setUri("/stop");

    httpRequest.addHeaders("User-Agent", "iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5");
    httpRequest.addHeaders("Content-Length", "0");
    
    m_sender->setRequestType(AirPlayClient::RequestTypePostStop);

    return m_sender->send(httpRequest.toString());
}

/*
client ¡ú server
-----------------------------------------------------------------
POST /scrub?position=20.097000 HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 0
*/
bool AirPlayClientVideo::setPlayVideoProgress(float playPosition)
{
    std::stringstream ssPlayPosition;
    ssPlayPosition << "/scrub?position=" << playPosition;

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("POST");
    httpRequest.setUri(ssPlayPosition.str());

    httpRequest.addHeaders("User-Agent", "iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5");
    httpRequest.addHeaders("Content-Length", "0");
    
    m_sender->setRequestType(AirPlayClient::RequestTypePostScrub);

    return m_sender->send(httpRequest.toString());
}

/*
client ¡ú server
-----------------------------------------------------------------
GET /scrub HTTP/1.1
User-Agent: iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5
Content-Length: 0
*/
bool AirPlayClientVideo::getPlayVideoProgress(void)
{
    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("GET");
    httpRequest.setUri("/scrub");

    httpRequest.addHeaders("User-Agent", "iTunes/10.6 (Macintosh; Intel Mac OS X 10.7.3) AppleWebKit/535.18.5");
    httpRequest.addHeaders("Content-Length", "0");
    
    m_sender->setRequestType(AirPlayClient::RequestTypeGetScrub);

    return m_sender->send(httpRequest.toString());
}

/*
client ¡ú server
-----------------------------------------------------------------
GET /playback-info HTTP/1.1
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 24b3fd94-1b6d-42b1-89a3-47108bfbac89
*/
bool AirPlayClientVideo::getPlayVideoInfo(void)
{
    std::string xAppleSessionId("1bd6ceeb-fffd-456c-a09c-996053a7a08c");

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("GET");
    httpRequest.setUri("/playback-info");

    httpRequest.addHeaders("Content-Length", "0");
    httpRequest.addHeaders("User-Agent", "MediaControl/1.0");
    httpRequest.addHeaders("X-Apple-Session-ID", xAppleSessionId);
    
    m_sender->setRequestType(AirPlayClient::RequestTypeGetPlaybackInfo);

    return m_sender->send(httpRequest.toString());
}

/*
client ¡ú server
-----------------------------------------------------------------
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
*/
bool AirPlayClientVideo::getProperty(void)
{
    return false;
}

/*
client ¡ú server
-----------------------------------------------------------------
POST /getProperty?playbackAccessLog HTTP/1.1
Content-Type: application/x-apple-binary-plist
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 24b3fd94-1b6d-42b1-89a3-47108bfbac89
*/
bool AirPlayClientVideo::setProperty(void)
{
    return false;
}