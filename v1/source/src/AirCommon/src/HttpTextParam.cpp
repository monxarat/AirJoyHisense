/*
* HttpTextParam.cpp
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

#include "HttpTextParam.hpp"
#include <cctype>
#include <sstream>
#include <algorithm>

using namespace airjoy;

HttpTextParam::HttpTextParam()
{
}

HttpTextParam::~HttpTextParam()
{
}

void HttpTextParam::loadBytes(const char *bytes, std::size_t len)
{
    const char *p = bytes;

    m_keyValues.clear();

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

            return;
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
                return;

            if (*p++ != ' ') 
                return;
        }

        // Header value.
#if 0
        while (is_char(*p) && !is_ctl(*p) && *p != '\r')
        {
            value.push_back(*p);
            p++;
        }
#else
        while (1)
        {
            if (*p == '\0')
                break;

            if (*p == '\r')
            {
                p++;
                break;
            }

            if (*p == '\n')
                break;

            value.push_back(*p);
            p++;
        }
#endif

        std::transform(name.begin(), name.end(), name.begin(), ::tolower);
        m_keyValues.insert(std::make_pair(name, value));

        if (*p++ == '\0')
            return;

#if 0
        // CRLF.
        if (*p++ != '\r') 
            return;

        if (*p++ != '\n') 
            return;
#endif
    }

    return;
}

const std::string & HttpTextParam::toString(void)
{
    m_buffer.clear();

    std::stringstream ss;

    KeyValueType::iterator iter;
    for (iter = m_keyValues.begin(); iter != m_keyValues.end(); ++iter)
        ss << iter->first << ": " << iter->second << "\r\n";

    m_buffer.append(ss.str());

    return m_buffer;
}

const std::string & HttpTextParam::getValueByKey(const std::string &name)
{
    std::string n(name);
    std::transform(n.begin(), n.end(), n.begin(), ::tolower);
 
    KeyValueType::iterator iter = m_keyValues.find(n);
    if (iter != m_keyValues.end())
        return iter->second;

    return m_emptyValue;
}

void HttpTextParam::setValueByKey(const std::string &key, const std::string &value)
{
    m_keyValues.insert(std::make_pair(key, value));
}

bool HttpTextParam::is_char(int c)
{
    return c >= 0 && c <= 127;
}

bool HttpTextParam::is_ctl(int c)
{
    return (c >= 0 && c <= 31) || (c == 127);
}

bool HttpTextParam::is_tspecial(int c)
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

bool HttpTextParam::is_digit(int c)
{
    return c >= '0' && c <= '9';
}

