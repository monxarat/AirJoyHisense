/*
* AirQueryHandler.hpp
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

#ifndef __AIR_QUERY_HANDLER_H__
#define __AIR_QUERY_HANDLER_H__


#include "AirJoyServiceInfo.hpp"
#include "AirMessageHandler.hpp"

namespace airjoy
{

    class AirQueryHandler : public AirMessageHandler
    {
    public:
        AirQueryHandler(const AirJoyServiceInfo &info);
        virtual ~AirQueryHandler();

        // AirMessageHandler
        virtual bool doAction(const std::string &fromIp,
                              const std::string &xmlns, 
                              const std::string &action, 
                              const std::string &param, 
                              std::string &result);

    };

}


#endif // __AIR_QUERY_HANDLER_H__
