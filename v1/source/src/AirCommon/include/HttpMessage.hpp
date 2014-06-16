/*
* HttpMessage.hpp
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

#ifndef __HTTP_MESSAGE_H__
#define __HTTP_MESSAGE_H__

#include <string>
#include <map>

namespace airjoy 
{


    typedef enum _HttpType
    {
        HttpUnknown     = 0,
        HttpRequest     = 1,
        HttpResponse    = 2
    } HttpType;

    typedef enum _HttpParserStatus {
        HttpParserDone              = 0,
        HttpParserError             = 1,
        HttpParserIncomplete        = 2,
        HttpParserHeaderIncomplete  = 3
    } HttpParserStatus;

    class HttpMessage
    {
    public:
        HttpMessage();
        virtual ~HttpMessage();

        // load http data with content
        HttpParserStatus loadBytes(const char* bytes, std::size_t len);
        
        // return the length of loadBytes()
        std::size_t consumeLength(void);

        // load http data without content
        bool loadBytesWithoutContent(const char *bytes, std::size_t length);

        const std::string & toString(void);

        /// The http type, e.g. "HTTP-REQUEST" or "HTTP-RESPONSE"
        HttpType httpType(void) { return m_httpType; }
        void setHttpType(HttpType type) { m_httpType = type; }

        /// The request method, e.g. "GET", "POST", "PUT".
        const std::string & httpMethod(void) { return m_httpMethod; }
        void setHttpMethod(const std::string & method) { m_httpMethod = method; }

        /// HTTP response code, e.g. 200
        int httpResponseCode(void) { return m_httpResponseCode; }
        void setHttpResponseCode(int code) { m_httpResponseCode = code; }
        
        /// The http response status
        const std::string & httpResponseStatus(void) { return m_httpResponseStatus; }
        void sethttpResponseStatus(const std::string & status) { m_httpResponseStatus = status; }

        /// The requested URI, such as a path to a file.
        const std::string & uri(void) { return m_uri; }
        void setUri(const std::string & uri) { m_uri = uri; }

        /// Major version number, usually 1.
        int httpVersionMajor(void) { return m_httpVersionMajor; }
        void setHttpVersionMajor(int m) { m_httpVersionMajor = m; }

        /// Minor version number, usually 0 or 1.
        int httpVersionMinor(void) { return m_httpVersionMinor; }
        void setHttpVersionMinor(int m) { m_httpVersionMinor = m; }

        /// The optional content sent with the request.
        const std::string & content(void) { return m_content; }
        void setContent(const std::string &c) { m_content = c; }

        std::size_t contentLength(void) { return m_contentLength; }

        /// http headers
        //std::vector<HttpHeader> & headers(void) { return m_headers; }
        /// header value
        //const std::string & getHttpHeaderValue(const std::string &name);
        
        void addHeaders(const std::string &name, const std::string &value);
        const std::string & getValueByName(const std::string &name);

        // value in URI
        const std::string & getValueInUri(const std::string &key);

        void print(void);

    private:
        // return length of status line
        std::size_t loadStatusLine(const char *bytes, std::size_t length);

        // return length of status line
        std::size_t loadRequestLine(const char *bytes, std::size_t length);
        
        // return length of heads
        std::size_t loadHeads(const char *bytes, std::size_t length);

        // return length of content
        std::size_t loadContent(const char *bytes, std::size_t length);

    private:
        /// Check if a byte is an HTTP character.
        static bool is_char(int c);

        /// Check if a byte is an HTTP control character.
        static bool is_ctl(int c);

        /// Check if a byte is defined as an HTTP tspecial character.
        static bool is_tspecial(int c);

        /// Check if a byte is a digit.
        static bool is_digit(int c);

        /// Check if two characters are equal, without regard to case.
        static bool tolowerCompare(char a, char b);

        /// Check whether the two request header names match.
        bool headersEqual(const std::string& a, const std::string& b);

    private:
        // for loadBytes()
        std::size_t         m_consumeLength;

        /// for loadBytes()
        HttpParserStatus    m_parserStatus;
        std::size_t         m_contentLengthLoaded;

        /// The http type, e.g. "HTTP-REQUEST" or "HTTP-RESPONSE"
        HttpType m_httpType;

        /// The http response code, e.g. 200
        int m_httpResponseCode;
        
        /// The http response status
        std::string m_httpResponseStatus;

        /// The request method, e.g. "GET", "POST".
        std::string m_httpMethod;

        /// The requested URI, such as a path to a file.
        std::string m_uri;

        /// Major version number, usually 1.
        int m_httpVersionMajor;

        /// Minor version number, usually 0 or 1.
        int m_httpVersionMinor;

        /// The headers included with the request.
        typedef std::map<std::string, std::string> HeaderType;
        HeaderType    m_headers;

        /// The optional content sent with the request.
        std::size_t m_contentLength;
        std::string m_content;

        /// for toString()
        std::string m_buffer;

        /// for getValueByName()
        std::string m_emptyValue;

        /// for getUriValue();
        std::string m_value;

    private:
        static std::string ContentLengthName;
    };


} // airjoy 

#endif // __HTTP_MESSAGE_H__
