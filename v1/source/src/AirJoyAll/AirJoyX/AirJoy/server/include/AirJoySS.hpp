/*
* AirJoySS.hpp
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

#ifndef __ANY_PLAY_STREAM_SERVER_H__
#define __ANY_PLAY_STREAM_SERVER_H__

#include <string>

#include "AirJoyApi.hpp"

namespace airjoy
{

    class AirJoyCore;

    class AIRJOY_API AirJoySS
    {
    public:
        AirJoySS(AirJoyCore * core);
        virtual ~AirJoySS();

        //------------------------------------------------------------------------------------
        // file service
        //------------------------------------------------------------------------------------
        void addSharedFolder(const std::string &sharedFolderUtf8, const std::string &localFolderUtf8);
        void cancelSharedFolder(const std::string &sharedFolderUtf8);
        bool start(void);
        bool stop(void);

    private:
        AirJoyCore * m_core;    
    };
}

#endif // __ANY_PLAY_STREAM_SERVER_H__

