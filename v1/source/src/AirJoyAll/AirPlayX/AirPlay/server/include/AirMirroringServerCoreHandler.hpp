/*
 * AirMirroringCoreHandler.hpp
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

#ifndef __AIR_MIRRORING_SERVER_CORE_HANDLER_H__
#define __AIR_MIRRORING_SERVER_CORE_HANDLER_H__

#include "TheBoost.hpp"

namespace airjoy
{

    class TcpServerMessage;
    typedef boost::shared_ptr<TcpServerMessage>   TcpServerMessagePointer;

    class AirMirroringCoreHandler
    {
    public:
        UmpServerHandler() {}
        virtual ~UmpServerHandler() {}

        // return true if this message can be handled, otherwise return false.
        virtual bool didReceiveMessage(TcpServerMessagePointer message) { return false; }

        // notify app, the tcp session has been closed;
        virtual void didClose(int connectedId) {}
    };

}

#endif // __AIR_MIRRORING_SERVER_CORE_HANDLER_H__
