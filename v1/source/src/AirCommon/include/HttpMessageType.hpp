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
        HttpMessageUnknown              = 0,          // 未知消息

        HttpRequestNormal               = 1000,      // Http普通请求
        HttpRequestAirJoyComplete       = 1001,      // AirJoy请求:完整消息
        HttpRequestAirJoyInComplete     = 1002,      // AirJoy请求:非完整消息

        HttpResponseAirJoyComplete      = 2001,      // AirJoy应答:完整消息
        HttpResponseAirJoyInComplete    = 2002,      // AirJoy应答:非完整消息
    } HttpMessageType;

    typedef boost::shared_ptr<HttpMessage> HttpMessagePointer;
    
    HttpMessageType GetHttpMessageType(HttpMessagePointer pHttpMessage);
}

#endif // __HTTP_MESSAGE_TYPE_H__
