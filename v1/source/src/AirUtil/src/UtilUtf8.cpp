/*
 * coding.tom@gmail.com
 * 
 * UtilUtf8.cpp
 *
 * 2011-5-14
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "UtilUtf8.hpp"

#if defined(_WIN32) || defined(_WIN64)
#   include <windows.h>
#endif // _WIN32

namespace airjoy
{

#if defined(_WIN32) || defined(_WIN64)
namespace utf8
{

bool MultiByteToUTF8(char* pUtf8, const char* pMultiByte, int pUtf8_len) 
{  
    // convert an MBCS string to widechar   
    int nLen = MultiByteToWideChar(CP_ACP, 0, pMultiByte, -1, NULL, 0); 

    WCHAR* lpszW = NULL;  
    lpszW = new WCHAR[nLen];  
    if (lpszW == NULL)
        return false;

    int nRtn = MultiByteToWideChar(CP_ACP, 0, pMultiByte, -1, lpszW, nLen); 

    if(nRtn != nLen)  
    {  
        delete[] lpszW;  
        return false;  
    }

    // convert an widechar string to utf8  
    int utf8Len = WideCharToMultiByte(CP_UTF8, 0, lpszW, -1, NULL, 0, NULL, NULL);  
    if (utf8Len <= 0
            || pUtf8_len < utf8Len)  
    { 
        delete[] lpszW;  
        return false;  
    }

    nRtn = WideCharToMultiByte(CP_UTF8, 0, lpszW, nLen, pUtf8, utf8Len, NULL, NULL);  
    delete[] lpszW; 

    return true;  
} 

// UTF8编码转为多字节编码  
bool UTF8ToMultiByte(char* pMultiByte, const char* pUtf8, int pMultiByte_len)  
{
    int nLen = MultiByteToWideChar( CP_UTF8, 0, pUtf8, -1, NULL, NULL );  // 得到UTF8编码的字符串长度，是2
    LPWSTR lpwsz = new WCHAR[nLen];
    MultiByteToWideChar( CP_UTF8, 0, pUtf8, -1, lpwsz, nLen );            // 转换的结果是UCS2格式的价值两个字
    int nLen1 = WideCharToMultiByte( CP_ACP, 0, lpwsz, nLen, NULL, NULL, NULL, NULL );
    if (pMultiByte_len < nLen1)
    {
        return false;
    }

    WideCharToMultiByte( CP_ACP, 0, lpwsz, nLen, pMultiByte, nLen1, NULL, NULL ); //转换完毕
    delete [] lpwsz;

    return true;  
}

} // namespace utf8
#endif

} // namespace airjoy