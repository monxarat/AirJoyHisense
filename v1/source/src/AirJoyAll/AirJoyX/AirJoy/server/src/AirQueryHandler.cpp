/*
* AirQueryHandler.cpp
*
*
* coding.tom@gmail.com
* 2012-5-23
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#include "AirQueryHandler.hpp"

using namespace airjoy;

AirQueryHandler::AirQueryHandler(const AirJoyServiceInfo &info)
{
}

AirQueryHandler::~AirQueryHandler()
{
}

bool AirQueryHandler::doAction(const std::string &fromIp,
                               const std::string &xmlns, 
                               const std::string &action, 
                               const std::string &param, 
                               std::string &result)
{
    if (action == AIRQUERY_GETFEATURES)
    {
        result.append("<?xml version=\"1.0\"?><result>");
        result.append("<code>").append("200").append("</code>");
        result.append("<features>").append("0x03").append("</features>");
        result.append("</result>");

        return true;
    }

    return false;
}
