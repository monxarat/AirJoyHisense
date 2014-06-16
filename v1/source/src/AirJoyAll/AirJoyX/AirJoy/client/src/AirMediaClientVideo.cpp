/*
 * AirMediaClientVideo.cpp
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

#include "AirMediaClientVideo.hpp"
#include "AirMediaClient.hpp"
#include "AirJoyDefault.hpp"
#include "HttpTextParam.hpp"
#include <sstream>

using namespace airjoy;


AirMediaClientVideo::AirMediaClientVideo(AirMediaClient *sender)
    : m_sender(sender)
{
}

AirMediaClientVideo::~AirMediaClientVideo()
{
}

AirJoySessionId AirMediaClientVideo::playVideo(const std::string &videoUrl,
                                               const std::string &videoId,
                                               const std::string &videoNameUtf8,
                                               float startPosition)
{
    std::stringstream ssStartPosition;
    ssStartPosition << startPosition;

    HttpTextParam param;
    param.setValueByKey("id", videoId);
    param.setValueByKey("url", videoUrl);
    param.setValueByKey("name", videoNameUtf8);
    param.setValueByKey("position", ssStartPosition.str());

    return m_sender->send(AIRMEDIA_VIDEO_PUT, param.toString());
}

AirJoySessionId AirMediaClientVideo::setPlayVideoRate(const std::string &videoId,
                                                      float rate)
{
    std::stringstream ssRate;
    ssRate << rate;

    HttpTextParam param;
    param.setValueByKey("id", videoId);
    param.setValueByKey("rate", ssRate.str());

    return m_sender->send(AIRMEDIA_VIDEO_RATE, param.toString());
}

AirJoySessionId AirMediaClientVideo::stopPlayVideo(const std::string &videoId)
{
    HttpTextParam param;
    param.setValueByKey("id", videoId);

    return m_sender->send(AIRMEDIA_VIDEO_STOP, param.toString());
}

AirJoySessionId AirMediaClientVideo::setPlayVideoProgress(const std::string &videoId,
                                                          float playPosition)
{
    std::stringstream ssPlayPosition;
    ssPlayPosition << playPosition;

    HttpTextParam param;
    param.setValueByKey("id", videoId);
    param.setValueByKey("position", ssPlayPosition.str());

    return m_sender->send(AIRMEDIA_VIDEO_SETPROGRESS, param.toString());
}

AirJoySessionId AirMediaClientVideo::getPlayVideoProgress(const std::string &videoId)
{
    HttpTextParam param;
    param.setValueByKey("id", videoId);

    return m_sender->send(AIRMEDIA_VIDEO_GETPROGRESS, param.toString());
}

AirJoySessionId AirMediaClientVideo::getPlayVideoInfo(void)
{
    HttpTextParam param;

    return m_sender->send(AIRMEDIA_VIDEO_GETPLAYBACKINFO, param.toString());
}