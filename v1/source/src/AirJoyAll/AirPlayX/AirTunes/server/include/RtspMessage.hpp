/*
* RtspMessage.hpp
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

#ifndef __RTSP_MESSAGE_H__
#define __RTSP_MESSAGE_H__

#include <string>
#include <map>

namespace airjoy 
{


    typedef enum _RtspType
    {
        RtspUnknown     = 0,
        RtspRequest     = 1,
        RtspResponse    = 2
    } RtspType;

    typedef enum _RtspParserStatus {
        RtspParserDone        = 0,
        RtspParserError       = 1,
        RtspParserIncomplete  = 2
    } RtspParserStatus;

    class RtspMessage
    {
    public:
        RtspMessage();
        virtual ~RtspMessage();

        // load rtsp data with content
        RtspParserStatus loadBytes(const char* bytes, std::size_t len);
        
        // load rtsp data without content
        bool loadBytesWithoutContent(const char *bytes, std::size_t length);

        const std::string & toString(void);

        /// The rtsp type, e.g. "RTSP-REQUEST" or "RTSP-RESPONSE"
        RtspType rtspType(void) { return m_rtspType; }
        void setRtspType(RtspType type) { m_rtspType = type; }

        /// The request method, e.g. "SETUP", "OPTIONS", "PAUSE".
        const std::string & rtspMethod(void) { return m_rtspMethod; }
        void setRtspMethod(const std::string & method) { m_rtspMethod = method; }

        /// The rtsp response status code , e.g. "200", "400"
        int statusCode(void) { return m_statusCode; }
        void setStatusCode(int code) { m_statusCode = code; }
        
        void setStatus(int code, const std::string & method) 
        { 
            m_statusCode = code;
            m_rtspMethod = method; 
        }

        /// The rtsp response status string, e.g. "OK", "Not Found"
        const std::string & statusString(void) { return m_statusString; }
        void setStatusString(const std::string & statusString) { m_statusString= statusString; }

        /// The requested URI, such as a path to a file.
        const std::string & uri(void) { return m_uri; }
        void setUri(const std::string & uri) { m_uri = uri; }

        /// Major version number, usually 1.
        int majorVersion(void) { return m_majorVersion; }
        void setMajorVersion(int v) { m_majorVersion = v; }

        /// Minor version number, usually 0 or 1.
        int minorVersion(void) { return m_minorVersion; }
        void setMinorVersion(int v) { m_minorVersion = v; }

        void setVersion(int major, int minor)
        {
            m_majorVersion = major;
            m_minorVersion = minor;
        }

        /// The optional content sent with the request.
        const std::string & content(void) { return m_content; }
        void setContent(const std::string &c) { m_content = c; }

        std::size_t contentLength(void) { return m_contentLength; }

        void addHeaders(const std::string &name, const std::string &value);
        const std::string & getHeadValueByName(const std::string &name);

        // value in URI
        const std::string & getValueInUri(const std::string &key);

        void print(void);

    private:
        // return length of status line
        std::size_t loadStatusLine(const char *bytes);

        // return length of status line
        std::size_t loadRequestLine(const char *bytes);
        
        // return length of heads
        std::size_t loadHeads(const char *bytes);

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
        /// for loadBytes()
        RtspParserStatus    m_parserStatus;
        std::size_t         m_contentLengthLoaded;

        /// The rtsp type, e.g. "RTSP-REQUEST" or "RTSP-RESPONSE"
        RtspType m_rtspType;

        /// RTSP response status code , e.g. "200", "400"
        int m_statusCode;

        /// The rtsp response status string, e.g. "OK", "Not Found"
        std::string m_statusString;

        /// The request method, e.g. "SETUP", "PAUSE".
        std::string m_rtspMethod;

        /// The requested URI, such as a path to a file.
        std::string m_uri;

        /// Major version number, usually 1.
        int m_majorVersion;

        /// Minor version number, usually 0 or 1.
        int m_minorVersion;

        /// The headers included with the request.
        typedef std::map<std::string, std::string> HeaderType;
        HeaderType    m_headers;

        /// The optional content sent with the request.
        std::size_t m_contentLength;
        std::string m_content;

        /// for toString()
        std::string m_buffer;

        /// for getValueWithName()
        std::string m_emptyValue;

        /// for getUriValue();
        std::string m_value;

    private:
        static std::string ContentLengthName;
    };


} // airjoy 

#endif // __RTSP_MESSAGE_H__
