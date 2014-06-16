/*
* coding.tom@gmail.com 
*
* UtilType.h
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __UTIL_TYPE_H__
#define __UTIL_TYPE_H__

#include "AirJoyTypeDef.hpp"
#include <string>  

namespace airjoy
{

    namespace type 
    {
        const std::string FileTypeToString (FileType type);
        FileType StringToFileType(const std::string &type);

    }

}

#endif // __UTIL_TYPE_H__
