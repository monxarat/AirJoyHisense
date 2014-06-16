/*
* UmpClientHandler.hpp
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

#ifndef __UMP_CLIENT_HANDLER_H__
#define __UMP_CLIENT_HANDLER_H__

#include "TheBoost.hpp"

namespace airjoy
{

    class TcpClientMessage;
    typedef boost::shared_ptr<TcpClientMessage>   TcpClientMessagePointer;

    class UmpClientHandler
    {
    public:
        UmpClientHandler() {}
        virtual ~UmpClientHandler() {}

        virtual void didReceiveMessage(TcpClientMessagePointer message) {}
    };

}


#endif // __UMP_CLIENT_HANDLER_H__
