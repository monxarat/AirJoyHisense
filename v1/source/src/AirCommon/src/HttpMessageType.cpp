/*
* HttpMessageType.cpp
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

#include "HttpMessageType.hpp"

namespace airjoy
{

HttpMessageType GetHttpMessageType(HttpMessagePointer pHttpMessage)
{
    if (pHttpMessage->httpType() == HttpRequest)
    {
        if (pHttpMessage->uri() != "/airjoy")
            return HttpRequestNormal;

        if (pHttpMessage->getValueByName("User-Agent").empty())
            return HttpRequestAirJoyInComplete;

        if (pHttpMessage->getValueByName("Content-Length").empty())
            return HttpRequestAirJoyInComplete;

        if (pHttpMessage->getValueByName("Content-Type").empty())
            return HttpRequestAirJoyInComplete;

        if (pHttpMessage->getValueByName("Query-Session-ID").empty())
            return HttpRequestAirJoyInComplete;

        if (pHttpMessage->getValueByName("Query-Topic").empty())
            return HttpRequestAirJoyInComplete;

        if (pHttpMessage->getValueByName("Query-Action").empty())
            return HttpRequestAirJoyInComplete;

        return HttpRequestAirJoyComplete;
    }

    else if (pHttpMessage->httpType() == HttpResponse)
    {
        if (pHttpMessage->getValueByName("Content-Length").empty())
            return HttpResponseAirJoyInComplete;

        if (pHttpMessage->getValueByName("Content-Type").empty())
            return HttpResponseAirJoyInComplete;

        if (pHttpMessage->getValueByName("Query-Session-ID").empty())
            return HttpResponseAirJoyInComplete;

        if (pHttpMessage->getValueByName("Query-Topic").empty())
            return HttpResponseAirJoyInComplete;

        if (pHttpMessage->getValueByName("Query-Action").empty())
            return HttpResponseAirJoyInComplete;

        return HttpResponseAirJoyComplete;
    }

    return HttpMessageUnknown;
}

} // namespace airjoy