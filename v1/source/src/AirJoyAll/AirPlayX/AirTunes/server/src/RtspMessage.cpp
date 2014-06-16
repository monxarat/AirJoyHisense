/*
* RtspMessage.cpp
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

#include "RtspMessage.hpp"

#include <cctype>
#include <sstream>
#include <algorithm>

#include "TheBoost.hpp"

namespace airjoy 
{

//---------------------------------------------------------------------------
// static
//---------------------------------------------------------------------------
std::string RtspMessage::ContentLengthName = "Content-Length";

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
RtspMessage::RtspMessage()
    : m_parserStatus(RtspParserError),
      m_rtspType(RtspUnknown),
      m_statusCode(0),
      m_majorVersion(0),
      m_minorVersion(0),
      m_contentLength(0),
      m_contentLengthLoaded(0)
{
}

RtspMessage::~RtspMessage()
{
}

RtspParserStatus RtspMessage::loadBytes(const char *bytes, std::size_t length)
{
    if (m_parserStatus == RtspParserIncomplete)
    {
        m_contentLengthLoaded += loadContent(bytes, length);
        m_parserStatus = (m_contentLengthLoaded < m_contentLength) ? RtspParserIncomplete : RtspParserDone;
        return m_parserStatus;
    }

    m_rtspType = RtspUnknown;
    m_statusCode = 0;
    m_statusString.clear();
    m_rtspMethod.clear();
    m_uri.clear();
    m_majorVersion = 0;
    m_minorVersion = 0;
    m_headers.clear();
    m_contentLength = 0;
    m_content.clear();

    std::size_t requestLinelength = loadStatusLine(bytes);
    if (requestLinelength == 0)
    {
        requestLinelength = loadRequestLine(bytes);
        if (requestLinelength == 0)
            return RtspParserError;
    }

    int headLength = loadHeads(bytes + requestLinelength);
    if (headLength == 0)
    {
        return RtspParserError;
    }

#if 0
    try
    {
		// 在vc2012下会有First-chance exception，不知何故！
		m_contentLength = boost::lexical_cast<std::size_t>(getHeadValueByName(ContentLengthName).c_str());
		
		//int length = boost::lexical_cast<int>(getHeadValueByName(ContentLengthName).c_str());
		//std::string l(this->getHeadValueByName("Content-Length"));
		//int length = boost::lexical_cast<int>(l);
    }
    catch (boost::bad_lexical_cast&)
    {
        m_contentLength = 0;
    }
#else
	m_contentLength = atoi(getHeadValueByName(ContentLengthName).c_str());
#endif

    if (m_contentLength == 0)
        return RtspParserDone;

    if (requestLinelength + headLength + 1 < length)
    {
        const char *pContentStart = bytes + requestLinelength + headLength + 1;
        std::size_t contentLength = length - (requestLinelength + headLength + 1);
        m_contentLengthLoaded = loadContent(pContentStart, contentLength);

        if (m_contentLengthLoaded >= m_contentLength)
            return RtspParserDone;

        m_parserStatus = RtspParserIncomplete;
        return m_parserStatus;
    }

    return RtspParserDone;
}

bool RtspMessage::loadBytesWithoutContent(const char *bytes, std::size_t length)
{
    m_rtspType = RtspUnknown;
    m_statusCode = 0;
    m_statusString.clear();
    m_rtspMethod.clear();
    m_uri.clear();
    m_majorVersion = 0;
    m_minorVersion = 0;
    m_headers.clear();
    m_contentLength = 0;
    m_content.clear();

    std::size_t requestLinelength = loadStatusLine(bytes);
    if (requestLinelength == 0)
    {
        requestLinelength = loadRequestLine(bytes);
        if (requestLinelength == 0)
            return false;
    }

    int headLength = loadHeads(bytes + requestLinelength);
    if (headLength == 0)
        return false;

#if 0
    try
    {
		// 在vc2012下会有First-chance exception，不知何故！
		m_contentLength = boost::lexical_cast<std::size_t>(getHeadValueByName(ContentLengthName).c_str());
		
		//int length = boost::lexical_cast<int>(getHeadValueByName(ContentLengthName).c_str());
		//std::string l(this->getHeadValueByName("Content-Length"));
		//int length = boost::lexical_cast<int>(l);
    }
    catch (boost::bad_lexical_cast&)
    {
        m_contentLength = 0;
    }
#else
	m_contentLength = atoi(getHeadValueByName(ContentLengthName).c_str());
#endif

    return true;
}

const std::string & RtspMessage::toString(void)
{
    m_buffer.clear();

    if (m_rtspType == RtspUnknown)
        return m_buffer;

    std::stringstream ss;

    if (m_rtspType == RtspRequest)
        ss << m_rtspMethod << " "<< m_uri << " " << "RTSP/" << m_majorVersion << "." << m_minorVersion << "\r\n";
    else
        ss << "RTSP/" << m_majorVersion << "." << m_minorVersion << " " << m_statusCode << " "<< m_statusString << "\r\n";

    HeaderType::iterator iter;
    for (iter = m_headers.begin(); iter != m_headers.end(); ++iter)
        ss << iter->first << ": " << iter->second << "\r\n";

    ss << "\r\n";

    if  (! m_content.empty())
        ss << m_content;

    m_buffer.append(ss.str());

    return m_buffer;
}

void RtspMessage::addHeaders(const std::string &name, const std::string &value)
{
    m_headers.insert(std::make_pair(name, value));
}
        
const std::string & RtspMessage::getHeadValueByName(const std::string &name)
{
    std::string n(name);
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
 
    HeaderType::iterator iter = m_headers.find(n);
    if (iter != m_headers.end())
        return iter->second;

    return m_emptyValue;
}

const std::string & RtspMessage::getValueInUri(const std::string &key)
{
    m_value = m_uri;
    std::string::size_type index = m_value.find(key);
    if (index != std::string::npos)
        m_value.erase(0, index + key.length());

    return m_value;
}

void RtspMessage::print(void)
{
    if (this->rtspType() == RtspRequest)
        std::cout << this->rtspMethod() << " "<< this->uri() << " " << "RTSP/" << this->majorVersion() << "." << this->minorVersion() << "\r\n";
    else
        std::cout << "RTSP/" << this->majorVersion() << "." << this->minorVersion() << " " << this->statusCode() << " "<< this->statusString() << "\r\n";

    HeaderType::iterator iter;
    for (iter = m_headers.begin(); iter != m_headers.end(); ++iter)
        std::cout << iter->first << ": " << iter->second << "\r\n";

    if (this->uri() == "/fp-setup")
        return;

    if (this->rtspMethod() == "SET_PARAMETER")
        return;

    if (! this->content().empty())
        //std::cout << std::endl << this->content() << std::endl;
        std::cout << "--: have content" << std::endl;
}

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
std::size_t RtspMessage::loadStatusLine(const char *bytes)
{
    const char *p = bytes;

    // RTSP protocol identifier.
    if (*p++ != 'R')
        return 0;

    if (*p++ != 'T') 
        return 0;
    
    if (*p++ != 'S')
        return false;
    
    if (*p++ != 'P') 
        return 0;

    // Slash.
    if (*p++ != '/') 
        return 0;

    // Major version number.
    if (! is_digit(*p)) 
        return 0;

    m_majorVersion = 0;
    while (is_digit(*p))
    {
        m_majorVersion = m_majorVersion * 10 + *p - '0';
        p++;
    }

    // Dot.
    if (*p++ != '.') 
        return 0;

    // Minor version number.
    if (! is_digit(*p)) 
        return 0;

    m_minorVersion = 0;
    while (is_digit(*p))
    {
        m_minorVersion = m_minorVersion * 10 + *p - '0';
        p++;
    }

    // Space.
    if (*p++ != ' ')
        return 0;

    // reason
    m_statusString.clear();
    while (! is_ctl(*p))
    {
        m_statusString.push_back(*p);
        p++;
    }

    // CRLF.
    if (*p++ != '\r') 
        return 0;

    if (*p++ != '\n') 
        return 0;

    m_rtspType = RtspResponse;

    // length of status line
    return (p - bytes);
}

std::size_t RtspMessage::loadRequestLine(const char *bytes)
{
    const char *p = bytes;

    // Request method.
    m_rtspMethod.clear();
    while (is_char(*p) && !is_ctl(*p) && !is_tspecial(*p) && *p != ' ')
    {
        m_rtspMethod.push_back(*p);
        p++;
    }

    if (m_rtspMethod.empty())
        return 0;

    // Space.
    if (*p++ != ' ') 
        return 0;
  
    // URI.
    m_uri.clear();
    while (!is_ctl(*p) && *p != ' ')
    {
      m_uri.push_back(*p);
      p++;
    }

    if (m_uri.empty()) 
        return 0;
        
    // Space.
    if (*p++ != ' ')
        return 0;

    // RTSP protocol identifier.
    if (*p++ != 'R')
        return 0;

    if (*p++ != 'T') 
        return 0;
    
    if (*p++ != 'S')
        return 0;
    
    if (*p++ != 'P') 
        return 0;

    // Slash.
    if (*p++ != '/') 
        return 0;

    // Major version number.
    if (! is_digit(*p)) 
        return 0;

    m_majorVersion = 0;
    while (is_digit(*p))
    {
        m_majorVersion = m_majorVersion * 10 + *p - '0';
        p++;
    }

    // Dot.
    if (*p++ != '.') 
        return 0;

    // Minor version number.
    if (! is_digit(*p)) 
        return 0;

    m_minorVersion = 0;
    while (is_digit(*p))
    {
        m_minorVersion = m_minorVersion * 10 + *p - '0';
        p++;
    }

    // CRLF.
    if (*p++ != '\r') 
        return 0;

    if (*p++ != '\n') 
        return 0;

    m_rtspType = RtspRequest;

    // length of status line
    return (p - bytes);
}

std::size_t RtspMessage::loadHeads(const char *bytes)
{
    const char *p = bytes;

    m_headers.clear();

    // Headers.
    while ((is_char(*p) && !is_ctl(*p) && !is_tspecial(*p) && *p != '\r')
        || (*p == ' ' || *p == '\t'))
    {
        std::string name;
        std::string value;

        if (*p == ' ' || *p == '\t')
        {
            // Leading whitespace. Must be continuation of previous header's value.
            //if (m_headers.empty()) 
            //    return false;

            return 0;
        }
        else
        {
            // Start the next header.

            // Header name.
            while (is_char(*p) && !is_ctl(*p) && !is_tspecial(*p) && *p != ':')
            {
                name.push_back(*p);
                p++;
            }

            // Colon and space separates the header name from the header value.
            if (*p++ != ':')
                return 0;

            if (*p++ != ' ') 
                return 0;
        }

        // Header value.
        
#if 0
        while (is_char(*p) && !is_ctl(*p) && *p != '\r')
        {
            value.push_back(*p);
            p++;
        }
#else
        while ( *p != '\r')
        {
            value.push_back(*p);
            p++;
        }
#endif

        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        m_headers.insert(std::make_pair(name, value));

        // CRLF.
        if (*p++ != '\r') 
            return 0;

        if (*p++ != '\n') 
            return 0;
    }

    // CRLF.
    if (*p++ != '\r') 
        return 0;

    if (*p != '\n')
        return 0;

    // length of heads
    return (p - bytes);
}

std::size_t RtspMessage::loadContent(const char *bytes, std::size_t length)
{
    for (std::size_t i = 0; i < length; ++i)
        m_content.push_back(bytes[i]);

    return length;
}

bool RtspMessage::is_char(int c)
{
    return c >= 0 && c <= 127;
}

bool RtspMessage::is_ctl(int c)
{
    return (c >= 0 && c <= 31) || (c == 127);
}

bool RtspMessage::is_tspecial(int c)
{
    switch (c)
    {
    case '(': case ')': case '<': case '>': case '@':
    case ',': case ';': case ':': case '\\': case '"':
    case '/': case '[': case ']': case '?': case '=':
    case '{': case '}': case ' ': case '\t':
        return true;
    default:
        return false;
    }
}

bool RtspMessage::is_digit(int c)
{
    return c >= '0' && c <= '9';
}

bool RtspMessage::tolowerCompare(char a, char b)
{
    return std::tolower(a) == std::tolower(b);
}

bool RtspMessage::headersEqual(const std::string& a, const std::string& b)
{
    if (a.length() != b.length())
        return false;

    return std::equal(a.begin(), a.end(), b.begin(), &RtspMessage::tolowerCompare);
}


} // airjoy
