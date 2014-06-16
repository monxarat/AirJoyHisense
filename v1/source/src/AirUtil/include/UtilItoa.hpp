/*
* coding.tom@gmail.com 
*
* UtilItoa.h
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __UTIL_ITOA_H__
#define __UTIL_ITOA_H__

#include <stdlib.h>

namespace airjoy
{

#if defined(_WIN32) || defined(_WIN64)    
#else
    typedef int errno_t;
    errno_t _itoa_s (int value, char *buffer, size_t sizeInCharacters, int radix);
#endif // _WIN32

}

#endif // __UTIL_ITOA_H__
