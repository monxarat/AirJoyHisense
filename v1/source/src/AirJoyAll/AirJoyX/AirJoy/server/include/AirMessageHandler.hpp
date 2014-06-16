/*
* AirMessageHandler.hpp
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

#ifndef __AIR_MESSAGE_HANDLER_H__
#define __AIR_MESSAGE_HANDLER_H__

#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class AirMessageHandler
    {
    public:
        AirMessageHandler() {}
        virtual ~AirMessageHandler() {}

        virtual bool doAction(const std::string &fromIp,
                              const std::string &topic, 
                              const std::string &action, 
                              const std::string &param, 
                              std::string &result)
        {
            return false;
        }

        virtual void didSubscribeEvent(const std::string &fromIp, const std::string &topic)
        {
        }
        
        virtual void didUnsubscribeEvent(const std::string &fromIp, const std::string &topic)
        {
        }

    };

}

#endif // __AIR_MESSAGE_HANDLER_H__

