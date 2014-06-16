/*
* HttpMessage.cpp
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

#include "HttpMessage.hpp"

#include <cctype>
#include <sstream>
#include <algorithm>

#include "TheBoost.hpp"

namespace airjoy 
{

//---------------------------------------------------------------------------
// static
//---------------------------------------------------------------------------
std::string HttpMessage::ContentLengthName = "Content-Length";

//---------------------------------------------------------------------------
// public
//---------------------------------------------------------------------------
HttpMessage::HttpMessage()
    : m_consumeLength(0)
    , m_parserStatus(HttpParserError)
    , m_httpType(HttpUnknown)
    , m_httpResponseCode(0)
    , m_httpVersionMajor(0)
    , m_httpVersionMinor(0)
    , m_contentLength(0)
    , m_contentLengthLoaded(0)
{
}

HttpMessage::~HttpMessage()
{
}

std::size_t HttpMessage::consumeLength(void)
{
    if (m_parserStatus == HttpParserError)
        return 0;

    if (m_parserStatus == HttpParserIncomplete)
        return m_consumeLength;

    if (m_parserStatus == HttpParserDone)
        return m_consumeLength;

    return 0;
}

HttpParserStatus HttpMessage::loadBytes(const char *bytes, std::size_t length)
{
    if (m_parserStatus == HttpParserIncomplete)
    {
        std::size_t readableLength = length;                                // 可读长度
        std::size_t leftLength = m_contentLength - m_contentLengthLoaded;   // 内容剩余长度
        
        // 实际读的长度
        std::size_t lengthWillBeRead = readableLength;

        size_t loadLength = loadContent(bytes, lengthWillBeRead);
        
        // 消费的字节
        m_consumeLength = loadLength;
        m_consumeLength++;

        m_contentLengthLoaded += loadLength;

        //std::cout << "m_contentLengthLoaded: " << m_contentLengthLoaded << std::endl;

        m_parserStatus = (m_contentLengthLoaded < m_contentLength) ? HttpParserIncomplete : HttpParserDone;
        return m_parserStatus;
    }
    
    m_httpType = HttpUnknown;
    m_httpResponseCode = 0;
    m_httpResponseStatus.clear();
    m_httpMethod.clear();
    m_uri.clear();
    m_httpVersionMajor = 0;
    m_httpVersionMinor = 0;
    m_headers.clear();
    m_contentLength = 0;
    m_content.clear();

    m_consumeLength = 0;

    std::size_t readableLength = length;
    std::size_t requestLinelength = loadStatusLine(bytes, readableLength);
    if (requestLinelength == 0)
    {
        requestLinelength = loadRequestLine(bytes, readableLength);
        if (requestLinelength == 0)
        {
            m_parserStatus = HttpParserError;
            return m_parserStatus;
        }
    }
    
    m_consumeLength += requestLinelength;

    // 读取header失败，认为此HTTP消息header不完整
    readableLength = length - requestLinelength;
    int headLength = loadHeads(bytes + requestLinelength, readableLength);
    if (headLength == 0)
    {
        m_parserStatus = HttpParserHeaderIncomplete;
        return m_parserStatus;
    }

    m_consumeLength += headLength;

#if 0
    try
    {
		// 在vc2012下会有First-chance exception，不知何故！
		m_contentLength = boost::lexical_cast<std::size_t>(getValueByName(ContentLengthName).c_str());
		
		//int length = boost::lexical_cast<int>(getValueByName(ContentLengthName).c_str());
		//std::string l(this->getValueByName("Content-Length"));
		//int length = boost::lexical_cast<int>(l);
    }
    catch (boost::bad_lexical_cast&)
    {
        m_contentLength = 0;
    }
#else
	m_contentLength = atoi(getValueByName(ContentLengthName).c_str());
#endif

    if (m_contentLength == 0)
    {
        m_parserStatus = HttpParserDone;
        return m_parserStatus;
    }
    
#if 0
    if (requestLinelength + headLength + 1 < length)
    {
        const char *pContentStart = bytes + requestLinelength + headLength + 1;

        // 可读长度
        readableLength = length - (requestLinelength + headLength + 1);   
        
        // 实际读的长度 = 可读长度 和 内容长度的最小值
        std::size_t lengthWillBeRead = readableLength < m_contentLength ? readableLength : m_contentLength;

        m_contentLengthLoaded = loadContent(pContentStart, lengthWillBeRead);
                
        //std::cout << "m_contentLengthLoaded: " << m_contentLengthLoaded << std::endl;

        // 消费的字节，需要加上最后的'\0'
        m_consumeLength += m_contentLengthLoaded;
        m_consumeLength ++;

        if (m_contentLengthLoaded >= m_contentLength)
            m_parserStatus = HttpParserDone;
        else
            m_parserStatus = HttpParserIncomplete;
    }
    else
    {
        m_parserStatus = HttpParserDone;
    }
#else

    // 可读长度
    readableLength = length - requestLinelength - headLength;
    if (readableLength > 0)
    {
        const char *pContentStart = bytes + requestLinelength + headLength;
        
        // 实际读的长度 = 可读长度 和 内容长度的最小值
        std::size_t lengthWillBeRead = (readableLength < m_contentLength) ? readableLength : m_contentLength;

        m_contentLengthLoaded = loadContent(pContentStart, lengthWillBeRead);
                
        // 消费的字节
        m_consumeLength += m_contentLengthLoaded;

        if (m_contentLengthLoaded >= m_contentLength)
            m_parserStatus = HttpParserDone;
        else
            m_parserStatus = HttpParserIncomplete;
    }
#endif

    return m_parserStatus;
}

bool HttpMessage::loadBytesWithoutContent(const char *bytes, std::size_t length)
{
    m_httpType = HttpUnknown;
    m_httpResponseCode = 0;
    m_httpResponseStatus.clear();
    m_httpMethod.clear();
    m_uri.clear();
    m_httpVersionMajor = 0;
    m_httpVersionMinor = 0;
    m_headers.clear();
    m_contentLength = 0;
    m_content.clear();

    std::size_t readableLength = length;
    std::size_t requestLinelength = loadStatusLine(bytes, readableLength);
    if (requestLinelength == 0)
    {
        requestLinelength = loadRequestLine(bytes, readableLength);
        if (requestLinelength == 0)
            return false;
    }

    readableLength = length - requestLinelength;
    int headLength = loadHeads(bytes + requestLinelength, readableLength);
    if (headLength == 0)
        return false;

#if 0
    try
    {
		// 在vc2012下会有First-chance exception，不知何故！
		m_contentLength = boost::lexical_cast<std::size_t>(getValueByName(ContentLengthName).c_str());
		
		//int length = boost::lexical_cast<int>(getValueByName(ContentLengthName).c_str());
		//std::string l(this->getValueByName("Content-Length"));
		//int length = boost::lexical_cast<int>(l);
    }
    catch (boost::bad_lexical_cast&)
    {
        m_contentLength = 0;
    }
#else
	m_contentLength = atoi(getValueByName(ContentLengthName).c_str());
#endif

    return true;
}

const std::string & HttpMessage::toString(void)
{
    m_buffer.clear();

    if (m_httpType == HttpUnknown)
        return m_buffer;

    std::stringstream ss;

    if (m_httpType == HttpRequest)
        ss << m_httpMethod << " "<< m_uri << " " << "HTTP/" << m_httpVersionMajor << "." << m_httpVersionMinor << "\r\n";
    else
        ss << "HTTP/" << m_httpVersionMajor << "." << m_httpVersionMinor << " " << m_httpResponseCode << " "<< m_httpResponseStatus << "\r\n";

    HeaderType::iterator iter;
    for (iter = m_headers.begin(); iter != m_headers.end(); ++iter)
        ss << iter->first << ": " << iter->second << "\r\n";

    ss << "\r\n";

    if  (! m_content.empty())
        ss << m_content;

    m_buffer.append(ss.str());

    return m_buffer;
}

void HttpMessage::addHeaders(const std::string &name, const std::string &value)
{
    m_headers.insert(std::make_pair(name, value));
}
        
const std::string & HttpMessage::getValueByName(const std::string &name)
{
    std::string n(name);
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
 
    HeaderType::iterator iter = m_headers.find(n);
    if (iter != m_headers.end())
        return iter->second;

    return m_emptyValue;
}

const std::string & HttpMessage::getValueInUri(const std::string &key)
{
    m_value = m_uri;
    std::string::size_type index = m_value.find(key);
    if (index != std::string::npos)
        m_value.erase(0, index + key.length());

    return m_value;
}

void HttpMessage::print(void)
{
    if (this->httpType() == HttpRequest)
        std::cout << this->httpMethod() << " "<< this->uri() << " " << "HTTP/" << this->httpVersionMajor() << "." << this->httpVersionMinor() << "\r\n";
    else
        std::cout << "HTTP/" << this->httpVersionMajor() << "." << this->httpVersionMinor() << " " << this->httpResponseCode() << " "<< this->httpResponseStatus() << "\r\n";

    HeaderType::iterator iter;
    for (iter = m_headers.begin(); iter != m_headers.end(); ++iter)
        std::cout << iter->first << ": " << iter->second << "\r\n";

    if (! this->content().empty())
        std::cout << "--: have content" << std::endl;
}

//---------------------------------------------------------------------------
// private
//---------------------------------------------------------------------------
std::size_t HttpMessage::loadStatusLine(const char *bytes, size_t length)
{
    const char *p = bytes;

    // HTTP protocol identifier.
    if (*p++ != 'H')
        return 0;

    if (*p++ != 'T') 
        return 0;

    if (*p++ != 'T')
        return false;

    if (*p++ != 'P') 
        return 0;

    // Slash.
    if (*p++ != '/') 
        return 0;

    // Major version number.
    if (! is_digit(*p)) 
        return 0;

    m_httpVersionMajor = 0;
    while (is_digit(*p))
    {
        m_httpVersionMajor = m_httpVersionMajor * 10 + *p - '0';
        p++;
    }

    // Dot.
    if (*p++ != '.') 
        return 0;

    // Minor version number.
    if (! is_digit(*p)) 
        return 0;

    m_httpVersionMinor = 0;
    while (is_digit(*p))
    {
        m_httpVersionMinor = m_httpVersionMinor * 10 + *p - '0';
        p++;
    }

    // Space.
    if (*p++ != ' ')
        return 0;

    // code
    m_httpResponseCode = 0;
    std::string code;
    while (is_digit(*p))
    {
        code.push_back(*p);
        p++;
    }
    m_httpResponseCode = std::atoi(code.c_str());

    // status
    m_httpResponseStatus.clear();
    while (! is_ctl(*p))
    {
        m_httpResponseStatus.push_back(*p);
        p++;
    }

    // CRLF.
    if (*p++ != '\r') 
        return 0;

    if (*p++ != '\n') 
        return 0;

    m_httpType = HttpResponse;

    // length of status line
    return (p - bytes);
}

std::size_t HttpMessage::loadRequestLine(const char *bytes, size_t length)
{
    const char *p = bytes;

    // Request method.
    m_httpMethod.clear();
    while (is_char(*p) && !is_ctl(*p) && !is_tspecial(*p) && *p != ' ')
    {
        m_httpMethod.push_back(*p);
        p++;
    }

    if (m_httpMethod.empty())
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

    // HTTP protocol identifier.
    if (*p++ != 'H')
        return 0;

    if (*p++ != 'T') 
        return 0;
    
    if (*p++ != 'T')
        return 0;
    
    if (*p++ != 'P') 
        return 0;

    // Slash.
    if (*p++ != '/') 
        return 0;

    // Major version number.
    if (! is_digit(*p)) 
        return 0;

    m_httpVersionMajor = 0;
    while (is_digit(*p))
    {
        m_httpVersionMajor = m_httpVersionMajor * 10 + *p - '0';
        p++;
    }

    // Dot.
    if (*p++ != '.') 
        return 0;

    // Minor version number.
    if (! is_digit(*p)) 
        return 0;

    m_httpVersionMinor = 0;
    while (is_digit(*p))
    {
        m_httpVersionMinor = m_httpVersionMinor * 10 + *p - '0';
        p++;
    }

    // CRLF.
    if (*p++ != '\r') 
        return 0;

    if (*p++ != '\n') 
        return 0;

    m_httpType = HttpRequest;

    // length of status line
    return (p - bytes);
}

std::size_t HttpMessage::loadHeads(const char *bytes, size_t length)
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
            while (is_char(*p) && !is_ctl(*p) && !is_tspecial(*p) && *p != ':' && length > 0)
            {
                name.push_back(*p);
                p++;

                length--;
            }

            // Colon and space separates the header name from the header value.
            if (*p++ != ':')
                return 0;
            
            length--;

            if (*p++ != ' ') 
                return 0;
            
            length--;
        }

        // Header value.
        
#if 0
        while (is_char(*p) && !is_ctl(*p) && *p != '\r')
        {
            value.push_back(*p);
            p++;
        }
#else
        while ( *p != '\r' && length > 0)
        {
            value.push_back(*p);
            p++;

            length--;
        }
#endif

        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        m_headers.insert(std::make_pair(name, value));

        // CRLF.
        if (*p++ != '\r') 
            return 0;
        
        length--;

        if (*p++ != '\n') 
            return 0;

        length--;
    }

    // CRLF.
    if (*p++ != '\r') 
        return 0;
    
    length--;

    if (*p != '\n')
        return 0;
    
    length--;
    
    // length of heads
    return (p - bytes + 1);
}

std::size_t HttpMessage::loadContent(const char *bytes, std::size_t length)
{
    for (std::size_t i = 0; i < length; ++i)
        m_content.push_back(bytes[i]);

    return length;
}

bool HttpMessage::is_char(int c)
{
    return c >= 0 && c <= 127;
}

bool HttpMessage::is_ctl(int c)
{
    return (c >= 0 && c <= 31) || (c == 127);
}

bool HttpMessage::is_tspecial(int c)
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

bool HttpMessage::is_digit(int c)
{
    return c >= '0' && c <= '9';
}

bool HttpMessage::tolowerCompare(char a, char b)
{
    return std::tolower(a) == std::tolower(b);
}

bool HttpMessage::headersEqual(const std::string& a, const std::string& b)
{
    if (a.length() != b.length())
        return false;

    return std::equal(a.begin(), a.end(), b.begin(), &HttpMessage::tolowerCompare);
}


} // airjoy
