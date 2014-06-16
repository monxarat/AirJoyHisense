/*
* AirJoyTimer.h
*
*
* coding.tom@gmail.com
*
* 2012-2-29
*
* @remark
*      set tabstop=4
*      set shiftwidth=4
*      set expandtab
*/

#ifndef __AIRJOY_TIMER_H__
#define __AIRJOY_TIMER_H__

#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class AirJoyTimer
    {
    public:
        AirJoyTimer() {}
        virtual ~AirJoyTimer() {}

        AirJoySessionId    sessionId(void)                 { return m_sessionId; }
        void            setSessionId(AirJoySessionId id)   { m_sessionId = id; }

        void            setTimeout(uint64_t t)          { m_timeout = t; }
        uint64_t        timeout(void)                   { return m_timeout; }

        void            setNextTime(uint64_t t)      { m_nextTime = t; }
        uint64_t        nextTime(void)               { return m_nextTime; }

    private:
        AirJoySessionId    m_sessionId;
        uint64_t            m_nextTime; // nextTime = now + timeout;
        uint64_t            m_timeout;
    };

};

#endif // __AIRJOY_TIMER_H__
