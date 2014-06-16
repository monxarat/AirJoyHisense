/*
* AirPlayEvent.hpp
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
*
*/

#ifndef __AIRPLAY_EVENT_H__
#define __AIRPLAY_EVENT_H__

#include "AirPlayServer.hpp"

#include <string>

namespace airjoy
{

    class AirPlayEvent
    {
    public:
        typedef AirPlayServer::EventCategory    EventCategory;
        typedef AirPlayServer::EventAction      EventAction;

    private:
        EventCategory               m_type;
        EventAction                 m_action;
        std::string                 m_sessionId;
        std::string                 m_string;

    public:
        AirPlayEvent(EventCategory type, EventAction action, const std::string &sessionId);
        virtual ~AirPlayEvent();

        const std::string & toString(void);
    };

}

#endif // __AIRPLAY_EVENT_H__