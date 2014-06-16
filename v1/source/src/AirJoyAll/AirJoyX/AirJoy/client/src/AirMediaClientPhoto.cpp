/*
 * AirMediaClientPhoto.cpp
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

#include "AirMediaClientPhoto.hpp"
#include "AirMediaClient.hpp"
#include "AirJoyDefault.hpp"
#include "HttpTextParam.hpp"
#include <sstream>

using namespace airjoy;


AirMediaClientPhoto::AirMediaClientPhoto(AirMediaClient *sender)
    : m_sender(sender)
{
}

AirMediaClientPhoto::~AirMediaClientPhoto()
{
}

AirJoySessionId AirMediaClientPhoto::displayPhoto(const std::string &photoUrl,
                                                  const std::string &photoId,
                                                  PhotoActionType action,
                                                  const std::string &photoNameUtf8)
{
    std::string act;

    if (action == PhotoActionDisplay)
        act.append("display");
    else if (action == PhotoActionCache)
        act.append("cache");

    HttpTextParam param;
    param.setValueByKey("id", photoId);
    param.setValueByKey("url", photoUrl);
    param.setValueByKey("name", photoNameUtf8);
    param.setValueByKey("action", act);

    return m_sender->send(AIRMEDIA_PHOTO_PUT, param.toString());
}

AirJoySessionId AirMediaClientPhoto::displayCachedPhoto(const std::string &photoId)
{
    HttpTextParam param;
    param.setValueByKey("id", photoId);

    return m_sender->send(AIRMEDIA_PHOTO_DISPLAY, param.toString());
}

AirJoySessionId AirMediaClientPhoto::stopDisplayPhoto(void)
{
    HttpTextParam param;

    return m_sender->send(AIRMEDIA_PHOTO_STOP, param.toString());
}

AirJoySessionId AirMediaClientPhoto::rotatePhoto(const std::string &photoId,
                                                 PhotoDirectionType direction)
{
    std::stringstream ssDirection;
    ssDirection << direction;

    HttpTextParam param;
    param.setValueByKey("id", photoId);
    param.setValueByKey("direction", ssDirection.str());

    return m_sender->send(AIRMEDIA_PHOTO_ROTATE, param.toString());
}

AirJoySessionId AirMediaClientPhoto::zoomPhoto(const std::string &photoId,
                                               float rate)
{
    std::stringstream ssRate;
    ssRate << rate;

    HttpTextParam param;
    param.setValueByKey("id", photoId);
    param.setValueByKey("rate", ssRate.str());

    return m_sender->send(AIRMEDIA_PHOTO_ZOOM, param.toString());
}

AirJoySessionId AirMediaClientPhoto::movePhoto(const std::string &photoId,
                                               float x,
                                               float y)
{
    std::stringstream ssX;
    ssX << x;

    std::stringstream ssY;
    ssY << y;

    HttpTextParam param;
    param.setValueByKey("id", photoId);
    param.setValueByKey("x", ssX.str());
    param.setValueByKey("y", ssY.str());

    return m_sender->send(AIRMEDIA_PHOTO_MOVE, param.toString());
}