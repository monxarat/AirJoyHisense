/*
* UmpHandler.hpp
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

#ifndef __UMP_HANDLER_H__
#define __UMP_HANDLER_H__

#include "TheBoost.hpp"

namespace airjoy
{

    class UdpMessage;
    typedef boost::shared_ptr<UdpMessage>   UdpMessagePointer;

    class UmpHandler
    {
    public:
        UmpHandler() {}
        virtual ~UmpHandler() {}

        // notify app, the udp server has been started;
        virtual void didStart() {}

        // return true if this message can be handled, otherwise return false.
        virtual bool didReceiveMessage(UdpMessagePointer message) { return false; }

        // notify app, the tcp session has been closed;
        virtual void didClose() {}
    };

}

#endif // __UMP_HANDLER_H__
