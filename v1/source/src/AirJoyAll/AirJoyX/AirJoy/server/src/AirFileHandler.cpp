/*
* AirFileHandler.cpp
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

#include "AirFileHandler.hpp"
#include "AirFileListener.hpp"
#include "AirJoyDefault.hpp"

using namespace airjoy;

AirFileHandler::AirFileHandler(AirFileListener *listener)
    : m_listener(listener)
{
}

AirFileHandler::~AirFileHandler()
{
}

bool AirFileHandler::doAction(const std::string &fromIp,
                              const std::string &xmlns, 
                              const std::string &action, 
                              const std::string &param, 
                              std::string &result)
{
    if (action == AIRFILE_GETFILELIST)
        return doActionGetFileList(fromIp, param, result);

    if (action == AIRFILE_GETFILECOUNT)
        return doActionGetFileCount(fromIp, param, result);

    if (action == AIRFILE_PUTFILE)
        return doActionPutFile(fromIp, param, result);

    return false;
}

bool AirFileHandler::doActionGetFileList(const std::string &fromIp, 
                                         const std::string &param, 
                                         std::string &result)
{
    return true;
}

bool AirFileHandler::doActionGetFileCount(const std::string &fromIp,
                                          const std::string &param,
                                          std::string &result)
{
    return true;
}

bool AirFileHandler::doActionPutFile(const std::string &fromIp,
                                     const std::string &param,
                                     std::string &result)
{
    return true;
}