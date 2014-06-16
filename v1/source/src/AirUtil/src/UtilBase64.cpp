/*
* coding.tom@gmail.com 
*
* utilbase64.cpp
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/


#include "UtilBase64.hpp"

#include <iostream>

#include <openssl/bio.h>
#include <openssl/ssl.h>


namespace airjoy
{
namespace base64
{

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";  

static inline bool is_base64(unsigned char c)  
{  
    return (isalnum(c) || (c == '+') || (c == '/'));  
}

std::string encode(const std::string & string_to_encode)
{
    return encode(string_to_encode.c_str(), (unsigned int)(string_to_encode.size()));
}

/******************************************************************* 
函 数 名 称:  base64_encode 
功 能 描 述：  Base64编码 
参 数 说 明：  
bytes_to_encode:    输入的BYTE流[in] 
in_len:             字节长度[in] 

返回值 说明：   编码后std::string 
作       者:    huangjf 
更 新 日 期：  2009.12.8 
*******************************************************************/  
std::string encode(const char* bytes_to_encode, unsigned int in_len)  
{  
    std::string ret;  
    int i = 0;  
    int j = 0;  
    unsigned char char_array_3[3];  
    unsigned char char_array_4[4];  

    while (in_len--)  
    {  
        char_array_3[i++] = *(bytes_to_encode++);  
        if (i == 3) {  
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;  
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);  
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);  
            char_array_4[3] = char_array_3[2] & 0x3f;  

            for (i = 0; (i <4) ; i++)  
                ret += base64_chars[char_array_4[i]];  
            i = 0;  
        }  
    }  

    if (i)  
    {  
        for (j = i; j < 3; j++)  
            char_array_3[j] = '\0';  

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;  
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);  
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);  
        char_array_4[3] = char_array_3[2] & 0x3f;  

        for (j = 0; (j < i + 1); j++)  
            ret += base64_chars[char_array_4[j]];  

        while ((i++ < 3))  
            ret += '=';  

    }  

    return ret;  
}  

/******************************************************************* 
函 数 名 称:  base64_decode 
功 能 描 述：  Base64解码 
参 数 说 明：  encoded_string: 输入的BYTE流[in] 

返回值 说明：   解码后std::string 
作       者:    huangjf 
更 新 日 期：  2009.12.8 
*******************************************************************/  
std::string decode(const std::string & encoded_string)  
{  
    int in_len = static_cast<int>(encoded_string.size());  
    int i = 0;  
    int j = 0;  
    int in_ = 0;  
    unsigned char char_array_4[4], char_array_3[3];  
    std::string ret;  

    while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_]))  
    {  
        char_array_4[i++] = encoded_string[in_];  
        in_++;  
        if (i ==4) {  
            for (i = 0; i <4; i++)  
                char_array_4[i] = (unsigned char)base64_chars.find(char_array_4[i]);  

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);  
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);  
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];  

            for (i = 0; (i < 3); i++)  
                ret += char_array_3[i];  
            i = 0;  
        }  
    }  

    if (i) {  
        for (j = i; j <4; j++)  
            char_array_4[j] = 0;  

        for (j = 0; j <4; j++)  
            char_array_4[j] = (unsigned char)base64_chars.find(char_array_4[j]);  

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);  
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);  
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];  

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];  
    }  

    return ret;  
}


}   // namespace base64
}   // namespace airjoy