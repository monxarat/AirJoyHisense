/*
* coding.tom@gmail.com 
*
* UtilBase64.h
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __UTIL_BASE64_H__
#define __UTIL_BASE64_H__

#include <string>  

namespace airjoy
{

    namespace base64
    {
        std::string encode(const char * bytes_to_encode, unsigned int in_len);
        std::string decode(const std::string & encoded_string);
        std::string encode(const std::string & string_to_encode);
    }

}

#endif // __UTIL_BASE64_H__
