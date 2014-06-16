/*
* AirPortDataHandler.hpp
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

#ifndef __AIR_PORT_DATA_HANDLER_H__
#define __AIR_PORT_DATA_HANDLER_H__


#include "UmpHandler.hpp"

namespace airjoy
{

    class AudioBuffer;

    class AirPortDataHandler : public UmpHandler
    {
    private:
        AudioBuffer         * m_audioBuffer;

    public:
        AirPortDataHandler();

        virtual ~AirPortDataHandler();

        // notify app, the udp server has been started;
        virtual void didStart();

        // return true if this message can be handled, otherwise return false.
        virtual bool didReceiveMessage(UdpMessagePointer message);

        // notify app, the tcp session has been closed;
        virtual void didClose();

        void setBuffer(AudioBuffer * audioBuffer);
    };

}


#endif // __AIR_PORT_DATA_HANDLER_H__
