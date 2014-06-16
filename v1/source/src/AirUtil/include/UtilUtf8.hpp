/*
 * coding.tom@gmail.com 
 *
 * UtilUtf8.h
 *
 * 2011-5-21
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#ifndef __UTIL_UTF8_H__
#define __UTIL_UTF8_H__

namespace airjoy
{

#if defined(_WIN32) || defined(_WIN64)    
    namespace utf8
    {
        bool MultiByteToUTF8(char* pUtf8, const char* pMultiByte, int pUtf8_len);
        bool UTF8ToMultiByte(char* pMultiByte, const char* pUtf8, int pMultiByte_len);
    }
#endif // _WIN32
    

}

#endif // __UTIL_UTF8_H__
