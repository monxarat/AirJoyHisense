/*
* AirMirroringServerHandler.cpp
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

#include "AirMirroringServerHandler.hpp"
#include "AirMirroringServerListener.hpp"
#include "TcpSession.hpp"
#include <ctime>
#include <vector>

#include "AirJoyLog.hpp"

using namespace airjoy;


AirMirroringServerHandler::AirMirroringServerHandler(AirMirroringServer *server, AirMirroringServerListener *listener)
    : m_listener(listener)
{
}

AirMirroringServerHandler::~AirMirroringServerHandler()
{
}

void AirMirroringServerHandler::didClose(int connectedId)
{
    std::cout << "AirMirroringServerHandler didClose: " << connectedId << std::endl;

    this->removeHttpParser(connectedId);
}

bool AirMirroringServerHandler::insertHttpParser(int connectedId)
{
    LockType lock(m_httpParsersMutex);
    HttpParserMap::iterator iter = m_httpParsers.find(connectedId);
    if (iter == m_httpParsers.end())
    {
        m_httpParsers.insert(std::make_pair(connectedId, boost::make_shared<HttpMessage>()));
        return true;
    }

    return false;
}

bool AirMirroringServerHandler::removeHttpParser(int connectedId)
{
    LockType lock(m_httpParsersMutex);
    HttpParserMap::iterator iter = m_httpParsers.find(connectedId);
    if (iter != m_httpParsers.end())
    {
        m_httpParsers.erase(iter);
        return true;
    }

    return false;
}

AirMirroringServerHandler::HttpMessagePointer AirMirroringServerHandler::getHttpParser(int connectedId)
{
    LockType lock(m_httpParsersMutex);
    HttpParserMap::iterator iter = m_httpParsers.find(connectedId);
    if (iter != m_httpParsers.end())
        return iter->second;

    HttpMessagePointer pHttpMessage = boost::make_shared<HttpMessage>();
    m_httpParsers.insert(std::make_pair(connectedId, pHttpMessage));
    return pHttpMessage;
}

bool AirMirroringServerHandler::didReceiveMessage(TcpServerMessagePointer request)
{
    //std::cout << "-";
    //std::cout << "----------------- didReceiveRequest: -------------" << std::endl;
    //std::cout << "connectedId: " << request->session()->connectedId() << " peer IP: " << request->session()->getPeerIp() << std::endl;

    HttpMessagePointer pHttpMessage = this->getHttpParser(request->session()->connectedId());
    HttpParserStatus httpStatus = pHttpMessage->loadBytes(request->message().c_str(), request->message().length());
    if (httpStatus == HttpParserDone)
    {
        // debug
        std::cout << "\n----------------------------" << std::endl;
        std::cout << "AirMirroring connectedId: " << request->session()->connectedId() << " peer IP: " << request->session()->getPeerIp() << std::endl;
        pHttpMessage->print();
        
        AirMrroringStatus status = AIR_MIRRORING_STATUS_NO_RESPONSE_NEEDED;
        std::string responseContent;

        if (pHttpMessage->httpType() == HttpRequest)
        {
            if (pHttpMessage->httpMethod() == "GET")
            {
                status = didGet(request, pHttpMessage, responseContent);
            }
            else if (pHttpMessage->httpMethod() == "POST")
            {
                status = didPost(request, pHttpMessage, responseContent);
            }
            else if (pHttpMessage->httpMethod() == "PUT")
            {
                
            }
            else
            {
                
            }
        }
        else
        {
            //this->recvHttpResponse(request, pHttpMessage);

            status = AIR_MIRRORING_STATUS_NO_RESPONSE_NEEDED;
        }

        switch (status)
        {
        case AIR_MIRRORING_STATUS_UNKNOWN:
            break;

        case AIR_MIRRORING_STATUS_OK:
            sendResponseOk(request->session(), responseContent);
            break;

        case AIR_MIRRORING_STATUS_SWITCHING_PROTOCOLS:
            sendResponseSwitchingProtocols(request->session());
            break;

        case AIR_MIRRORING_STATUS_NEED_AUTH:
            sendResponseUnauthorized(request->session());
            break;

        case AIR_MIRRORING_STATUS_NOT_FOUND:
            sendResponseNotFound(request->session());
            break;

        case AIR_MIRRORING_STATUS_METHOD_NOT_ALLOWED:
            sendResponseMethodNotAllowed(request->session());
            break;

        case AIR_MIRRORING_STATUS_NOT_IMPLEMENTED:
            sendResponseNotImplemented(request->session());
            break;

        case AIR_MIRRORING_STATUS_NO_RESPONSE_NEEDED:
            break;

        default:
            break;
        }

         // We need a new HttpMessage ...
         this->removeHttpParser(request->session()->connectedId());
    }

    return true;
}

const std::string & AirMirroringServerHandler::getNowTime(void)
{
    time_t t = std::time(0);
    char tmp[64];
    
    // Wed, 27 June 2012 16:47:41 GMT
    std::strftime(tmp, sizeof(tmp), "%a, %d %b %Y %X GMT", localtime(&t) );
    
    m_time = tmp;

    return m_time;
}

void AirMirroringServerHandler::sendMessage(TcpSessionPointer session, const std::string & message)
{
    TcpServerMessagePointer response = boost::make_shared<TcpServerMessage>(session);
    response->message() = message;

    //std::cout << "---------------------- startWrite ----------------------" << std::endl;
    //std::cout << response->message() << std::endl;

    session->startWrite(response);
}

void AirMirroringServerHandler::sendResponseOk(TcpSessionPointer session, std::string &responseContent)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(200);
    httpResponse.sethttpResponseStatus("OK");

    httpResponse.addHeaders("Date", getNowTime());
    
    if (responseContent.empty())
    {
        httpResponse.addHeaders("Content-Length", "0");
    }
    else
    {
        std::stringstream ssSize;
        ssSize << responseContent.length();

        httpResponse.addHeaders("Content-Type", "text/x-apple-plist+xml");
        httpResponse.addHeaders("Content-Length", ssSize.str());

        httpResponse.setContent(responseContent);
    }

    sendMessage(session, httpResponse.toString());
}

void AirMirroringServerHandler::sendResponseUnauthorized(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(401);
    httpResponse.sethttpResponseStatus("Unauthorized");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirMirroringServerHandler::sendResponseNotFound(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(404);
    httpResponse.sethttpResponseStatus("Not Found");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirMirroringServerHandler::sendResponseNotImplemented(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(501);
    httpResponse.sethttpResponseStatus("Not Implemented");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirMirroringServerHandler::sendResponseMethodNotAllowed(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(501);
    httpResponse.sethttpResponseStatus("Method Not Allowed");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Content-Length", "0");

    sendMessage(session, httpResponse.toString());
}

void AirMirroringServerHandler::sendResponseSwitchingProtocols(TcpSessionPointer session)
{
    HttpMessage httpResponse;
    httpResponse.setHttpType(HttpResponse);
    httpResponse.setHttpVersionMajor(1);
    httpResponse.setHttpVersionMinor(1);
    httpResponse.setHttpResponseCode(101);
    httpResponse.sethttpResponseStatus("Switching Protocols");

    httpResponse.addHeaders("Date", getNowTime());
    httpResponse.addHeaders("Upgrade", "PTTH/1.0");
    httpResponse.addHeaders("Connection", "Upgrade");

    sendMessage(session, httpResponse.toString());
}


AirMirroringServerHandler::AirMrroringStatus AirMirroringServerHandler::didGet(TcpServerMessagePointer request, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    if (pHttpMessage->uri() == "/stream.xml")
        return didGetServerInfo(request, pHttpMessage, responseContent);

    return AIR_MIRRORING_STATUS_NOT_IMPLEMENTED;
}

AirMirroringServerHandler::AirMrroringStatus AirMirroringServerHandler::didPost(TcpServerMessagePointer request, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    if (pHttpMessage->uri() == "/fp-setup")
        return didPostFpSetup(request, pHttpMessage, responseContent);

    return AIR_MIRRORING_STATUS_NOT_IMPLEMENTED;
}

/*
client ¡ú server
GET /stream.xml HTTP/1.1
Content-Length: 0
----------------------------------------------------------
server ¡ú client
HTTP/1.1 200 OK
Date: Mon, 08 Mar 2012 15:30:27 GMT
Content-Type: text/x-apple-plist+xml
Content-Length: 411

<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN"
 "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
 <dict>
  <key>height</key>
  <integer>720</integer>
  <key>overscanned</key>
  <true/>
  <key>refreshRate</key>
  <real>0.016666666666666666</real>
  <key>version</key>
  <string>130.14</string>
  <key>width</key>
  <integer>1280</integer>
 </dict>
</plist>
*/
AirMirroringServerHandler::AirMrroringStatus AirMirroringServerHandler::didGetServerInfo(TcpServerMessagePointer request, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    std::stringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << "\r\n";
    ss << "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n";
    ss << "<plist version=\"1.0\">" << "\r\n";
    ss << "<dict>" << "\r\n";
    ss << "<key>height</key>" << "\r\n";
    ss << "<integer>720</integer>" << "\r\n";
    ss << "<key>overscanned</key>" << "\r\n";
    ss << "<true/>" << "\r\n";
    ss << "<key>refreshRate</key>" << "\r\n";
    ss << "<real>0.016666666666666666</real>" << "\r\n";
    ss << "<key>version</key>" << "\r\n";
    ss << "<string>150.33</string>" << "\r\n";
    ss << "<key>width</key>" << "\r\n";
    ss << "<integer>1280</integer>" << "\r\n";
    ss << "</dict>" << "\r\n";
    ss << "</plist>" << "\r\n";

    responseContent = ss.str();
    return AIR_MIRRORING_STATUS_OK;
}

AirMirroringServerHandler::AirMrroringStatus AirMirroringServerHandler::didPostFpSetup(TcpServerMessagePointer request, HttpMessagePointer pHttpMessage, std::string &responseContent)
{
    // 2 1 1 -> 4 : 02 00 02 bb
    uint8_t fply_1[] = {
        0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x02, 0xbb
    };

    // 2 1 2 -> 130 : 02 02 xxx
    uint8_t fply_2[] = {
        0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x82,
        0x02, 0x02, 0x2f, 0x7b, 0x69, 0xe6, 0xb2, 0x7e, 0xbb, 0xf0, 0x68, 0x5f, 0x98, 0x54, 0x7f, 0x37,
        0xce, 0xcf, 0x87, 0x06, 0x99, 0x6e, 0x7e, 0x6b, 0x0f, 0xb2, 0xfa, 0x71, 0x20, 0x53, 0xe3, 0x94,
        0x83, 0xda, 0x22, 0xc7, 0x83, 0xa0, 0x72, 0x40, 0x4d, 0xdd, 0x41, 0xaa, 0x3d, 0x4c, 0x6e, 0x30,
        0x22, 0x55, 0xaa, 0xa2, 0xda, 0x1e, 0xb4, 0x77, 0x83, 0x8c, 0x79, 0xd5, 0x65, 0x17, 0xc3, 0xfa,
        0x01, 0x54, 0x33, 0x9e, 0xe3, 0x82, 0x9f, 0x30, 0xf0, 0xa4, 0x8f, 0x76, 0xdf, 0x77, 0x11, 0x7e,
        0x56, 0x9e, 0xf3, 0x95, 0xe8, 0xe2, 0x13, 0xb3, 0x1e, 0xb6, 0x70, 0xec, 0x5a, 0x8a, 0xf2, 0x6a,
        0xfc, 0xbc, 0x89, 0x31, 0xe6, 0x7e, 0xe8, 0xb9, 0xc5, 0xf2, 0xc7, 0x1d, 0x78, 0xf3, 0xef, 0x8d,
        0x61, 0xf7, 0x3b, 0xcc, 0x17, 0xc3, 0x40, 0x23, 0x52, 0x4a, 0x8b, 0x9c, 0xb1, 0x75, 0x05, 0x66,
        0xe6, 0xb3
    };

    // 2 1 3 -> 152
    // 4 : 02 8f 1a 9c
    // 128 : xxx
    // 20 : 5b ed 04 ed c3 cd 5f e6 a8 28 90 3b 42 58 15 cb 74 7d ee 85

    uint8_t fply_3[] = {
        0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x98, 0x02, 0x8f,
        0x1a, 0x9c, 0x6e, 0x73, 0xd2, 0xfa, 0x62, 0xb2, 0xb2, 0x07, 0x6f, 0x52, 0x5f, 0xe5, 0x72, 0xa5,
        0xac, 0x4d, 0x19, 0xb4, 0x7c, 0xd8, 0x07, 0x1e, 0xdb, 0xbc, 0x98, 0xae, 0x7e, 0x4b, 0xb4, 0xb7,
        0x2a, 0x7b, 0x5e, 0x2b, 0x8a, 0xde, 0x94, 0x4b, 0x1d, 0x59, 0xdf, 0x46, 0x45, 0xa3, 0xeb, 0xe2,
        0x6d, 0xa2, 0x83, 0xf5, 0x06, 0x53, 0x8f, 0x76, 0xe7, 0xd3, 0x68, 0x3c, 0xeb, 0x1f, 0x80, 0x0e,
        0x68, 0x9e, 0x27, 0xfc, 0x47, 0xbe, 0x3d, 0x8f, 0x73, 0xaf, 0xa1, 0x64, 0x39, 0xf7, 0xa8, 0xf7,
        0xc2, 0xc8, 0xb0, 0x20, 0x0c, 0x85, 0xd6, 0xae, 0xb7, 0xb2, 0xd4, 0x25, 0x96, 0x77, 0x91, 0xf8,
        0x83, 0x68, 0x10, 0xa1, 0xa9, 0x15, 0x4a, 0xa3, 0x37, 0x8c, 0xb7, 0xb9, 0x89, 0xbf, 0x86, 0x6e,
        0xfb, 0x95, 0x41, 0xff, 0x03, 0x57, 0x61, 0x05, 0x00, 0x73, 0xcc, 0x06, 0x7e, 0x4f, 0xc7, 0x96,
        0xae, 0xba, 0x5b, 0xed, 0x04, 0xed, 0xc3, 0xcd, 0x5f, 0xe6, 0xa8, 0x28, 0x90, 0x3b, 0x42, 0x58,
        0x15, 0xcb, 0x74, 0x7d, 0xee, 0x85
    };

    // 2 1 4 -> 20 : 5b ed 04 ed c3 cd 5f e6 a8 28 90 3b 42 58 15 cb 74 7d ee 85
    uint8_t fply_4[] = {
        0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x14, 0x5b,
        0xed, 0x04, 0xed, 0xc3, 0xcd, 0x5f, 0xe6, 0xa8, 0x28, 0x90, 0x3b, 0x42, 0x58, 0x15, 0xcb, 0x74,
        0x7d, 0xee, 0x85
    };

    uint8_t fply_header[12];
    size_t len = sizeof(fply_header);
    memcpy(fply_header, pHttpMessage->content().c_str(), len);

    std::string payload(pHttpMessage->content().c_str() + len, pHttpMessage->content().size() - len);

    if (fply_header[6] == 1)
    {
        memcpy(fply_2 + 13, pHttpMessage->content().c_str() + 12 + 2, 1);

        std::string data((const char *)fply_2, sizeof(fply_2));
        
        std::stringstream ssSize;
        ssSize << data.length();

        HttpMessage httpResponse;
        httpResponse.setHttpType(HttpResponse);
        httpResponse.setHttpVersionMajor(1);
        httpResponse.setHttpVersionMinor(1);
        httpResponse.setHttpResponseCode(200);
        httpResponse.sethttpResponseStatus("OK");
        httpResponse.addHeaders("Content-Type", "application/octet-stream");
	    httpResponse.addHeaders("Content-Length", ssSize.str());
        httpResponse.setContent(data);

        this->sendMessage(request->session(), httpResponse.toString());
        return AIR_MIRRORING_STATUS_NO_RESPONSE_NEEDED;
    }
    else if (fply_header[6] == 3)
    {
        std::string data((const char *)fply_4, 12);
        data.append(payload.c_str() + payload.size() - 20, 20);

        std::stringstream ssSize;
        ssSize << data.length();

        HttpMessage httpResponse;
        httpResponse.setHttpType(HttpResponse);
        httpResponse.setHttpVersionMajor(1);
        httpResponse.setHttpVersionMinor(1);
        httpResponse.setHttpResponseCode(200);
        httpResponse.sethttpResponseStatus("OK");
        httpResponse.addHeaders("Content-Type", "application/octet-stream");
	    httpResponse.addHeaders("Content-Length", ssSize.str());
        httpResponse.setContent(data);

        this->sendMessage(request->session(), httpResponse.toString());
        return AIR_MIRRORING_STATUS_NO_RESPONSE_NEEDED;
    }

    return AIR_MIRRORING_STATUS_OK;
}