/*
 * AirPlayClientPhoto.cpp
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

#include "AirPlayClientPhoto.hpp"
#include "AirPlayClient.hpp"
#include "AnyPlayDefault.hpp"
#include "HttpMessage.hpp"
#include "UtilMd5.hpp"
#include "UtilUtf8.hpp"
#include "TheBoost.hpp"
#include <fstream>

using namespace anyplay;


AirPlayClientPhoto::AirPlayClientPhoto(AirPlayClient *sender)
    : m_sender(sender)
{
}

AirPlayClientPhoto::~AirPlayClientPhoto()
{
}


/*
-----------------------------------------------------------------
1. client → server
-----------------------------------------------------------------
PUT /photo HTTP/1.1
X-Apple-AssetKey: F92F9B91-954E-4D63-BB9A-EEC771ADE6E8
Content-Length: 462848
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c

<JPEG DATA>
-----------------------------------------------------------------
2. client → server
-----------------------------------------------------------------
PUT /photo HTTP/1.1
X-Apple-AssetAction: cacheOnly
X-Apple-AssetKey: B0DDE2C0-6FDD-48F8-9E5B-29CE0618DF5B
Content-Length: 462848
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c

<JPEG DATA>
*/
bool AirPlayClientPhoto::displayPhoto(const std::string &photoPath,
                                      const std::string &photoId,
                                      PhotoActionType action)
{
    // Local File Not Exist !
#if defined(_WIN32) || defined(_WIN64)
    char multiByte[1024];
    if (! utf8::UTF8ToMultiByte(multiByte, photoPath.c_str(), 1024 -1))
        return false;

    std::string localFile(multiByte);
#else
    std::string localFile(photoPath);
#endif
    
    if (! boost::filesystem::exists(localFile))
        return false;
    
    // 读取文件长度
    uint64_t photoLength = 0;
    photoLength = boost::filesystem::file_size(localFile);

    std::stringstream ssPhotoLength;
    ssPhotoLength << photoLength;

    std::string xAppleSessionId("1bd6ceeb-fffd-456c-a09c-996053a7a08c");

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("PUT");
    httpRequest.setUri("/photo");

    if (action == PhotoActionCache)
        httpRequest.addHeaders("X-Apple-AssetAction", "cacheOnly");    

    httpRequest.addHeaders("X-Apple-AssetKey", photoId);
    httpRequest.addHeaders("Content-Length", ssPhotoLength.str());
    httpRequest.addHeaders("User-Agent", "MediaControl/1.0");
    httpRequest.addHeaders("X-Apple-Session-ID", xAppleSessionId);

    char buf[1024 * 1024 * 2];
    std::ifstream jpgFile;
    jpgFile.open(localFile.c_str(), std::ios_base::in | std::ios_base::binary);
    jpgFile.read(buf, 1024 * 1024 * 2);
    jpgFile.close();

    httpRequest.setContent(buf);

    m_sender->setRequestType(AirPlayClient::RequestTypePutPhoto);

    return m_sender->send(httpRequest.toString());
}

/*
client → server
-----------------------------------------------------------------
PUT /photo HTTP/1.1
X-Apple-AssetAction: displayCached
X-Apple-AssetKey: B0DDE2C0-6FDD-48F8-9E5B-29CE0618DF5B
X-Apple-Transition: Dissolve
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08
*/
bool AirPlayClientPhoto::displayCachedPhoto(const std::string &photoId)
{
    std::string xAppleAssetAction("displayCached");
    std::string xAppleSessionId("1bd6ceeb-fffd-456c-a09c-996053a7a08c");

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("PUT");
    httpRequest.setUri("/photo");

    httpRequest.addHeaders("X-Apple-AssetAction", xAppleAssetAction);
    httpRequest.addHeaders("X-Apple-AssetKey", photoId);
    httpRequest.addHeaders("X-Apple-Transition", "Dissolve");
    httpRequest.addHeaders("Content-Length", "0");
    httpRequest.addHeaders("User-Agent", "MediaControl/1.0");
    httpRequest.addHeaders("X-Apple-Session-ID", xAppleSessionId);

    m_sender->setRequestType(AirPlayClient::RequestTypePutPhoto);

    return m_sender->send(httpRequest.toString());
}

/*
client → server
-----------------------------------------------------------------
POST /stop HTTP/1.1
Content-Length: 0
User-Agent: MediaControl/1.0
X-Apple-Session-ID: 1bd6ceeb-fffd-456c-a09c-996053a7a08c
*/
bool AirPlayClientPhoto::stopDisplayPhoto(void)
{
    std::string xAppleDeviceId("0xdc2b61a0ce79");
    std::string xAppleSessionId("1bd6ceeb-fffd-456c-a09c-996053a7a08c");

    HttpMessage httpRequest;
    httpRequest.setHttpType(HttpRequest);
    httpRequest.setHttpVersionMajor(1);
    httpRequest.setHttpVersionMinor(1);
    httpRequest.setHttpMethod("POST");
    httpRequest.setUri("/stop");

    httpRequest.addHeaders("Content-Length", "0");
    httpRequest.addHeaders("User-Agent", "MediaControl/1.0");
    httpRequest.addHeaders("X-Apple-Session-ID", xAppleSessionId);

    m_sender->setRequestType(AirPlayClient::RequestTypePostStop);

    return m_sender->send(httpRequest.toString());
}
