/*
* HttpTextParam.hpp
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

#ifndef __HTTP_TEXT_PARAM_H__
#define __HTTP_TEXT_PARAM_H__

#include <string>
#include <map>

namespace airjoy
{


    class HttpTextParam
    {
    public:
        HttpTextParam();
        virtual ~HttpTextParam();

        void loadBytes(const char *bytes, std::size_t len);
        const std::string & toString(void);
       
        const std::string & getValueByKey(const std::string &name);
        void setValueByKey(const std::string &key, const std::string &value);

    private: 
        /// Check if a byte is an HTTP character.
        static bool is_char(int c);

        /// Check if a byte is an HTTP control character.
        static bool is_ctl(int c);

        /// Check if a byte is defined as an HTTP tspecial character.
        static bool is_tspecial(int c);

        /// Check if a byte is a digit.
        static bool is_digit(int c);

    private:
        typedef std::map<std::string, std::string> KeyValueType;
        KeyValueType    m_keyValues;

        /// for getValueWithKey();
        std::string m_emptyValue;

        /// for toString()
        std::string m_buffer;
    };


}


#endif // __HTTP_TEXT_PARAM_H__
