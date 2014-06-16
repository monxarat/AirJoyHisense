/*
* HttpMessageType.hpp
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

#ifndef __HTTP_MESSAGE_TYPE_H__
#define __HTTP_MESSAGE_TYPE_H__

#include "HttpMessage.hpp"
#include "TheBoost.hpp"

namespace airjoy
{

    typedef enum _HttpMessageType
    {
        HttpMessageUnknown              = 0,          // δ֪��Ϣ

        HttpRequestNormal               = 1000,      // Http��ͨ����
        HttpRequestAirJoyComplete       = 1001,      // AirJoy����:������Ϣ
        HttpRequestAirJoyInComplete     = 1002,      // AirJoy����:��������Ϣ

        HttpResponseAirJoyComplete      = 2001,      // AirJoyӦ��:������Ϣ
        HttpResponseAirJoyInComplete    = 2002,      // AirJoyӦ��:��������Ϣ
    } HttpMessageType;

    typedef boost::shared_ptr<HttpMessage> HttpMessagePointer;
    
    HttpMessageType GetHttpMessageType(HttpMessagePointer pHttpMessage);
}

#endif // __HTTP_MESSAGE_TYPE_H__
