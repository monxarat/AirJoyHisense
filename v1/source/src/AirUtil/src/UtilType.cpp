/*
* coding.tom@gmail.com 
*
* UtilType.cpp
*
* 2011-5-21
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/


#include "UtilType.hpp"

#include <iostream>

namespace airjoy
{
namespace type
{

    const std::string FileTypeToString  (FileType type)
    {
        switch (type)
        {
            case FileTypeUnknown:
                return "unknown";

            case FileTypeFolder:
                return "folder";

            case FileTypeFile:
                return "file";

            case FileTypeFilePicture:
                return "file#picture";

            case FileTypeFileMusic:
                return "file#music";

            case FileTypeFileVideo:
                return "file#video";

            case FileTypeFileText:
                return "file#text";
            case FileTypeFileApk:
                return "file#apk";

            case FileTypeFileRing:
                return "file#ring";

            default:
                break;
        }

        return "unknown";
    }

    FileType StringToFileType  (const std::string &type)
    {
        if (type == "folder")
            return FileTypeFolder;

        if (type == "file")
            return FileTypeFile;

        if (type == "file#picture")
            return FileTypeFilePicture;

        if (type == "file#music")
            return FileTypeFileMusic;

        if (type == "file#video")
            return FileTypeFileVideo;

        if (type == "file#text")
            return FileTypeFileText;

        if (type == "file#apk")
            return FileTypeFileApk;

        if (type == "file#ring")
            return FileTypeFileRing;

        return FileTypeUnknown;
    }

}
}
