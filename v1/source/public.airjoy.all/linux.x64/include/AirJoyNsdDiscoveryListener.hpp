/*
* AirJoyNsdDiscoveryListener.hpp
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

#ifndef __AIRJOY_NSD_DISCOVERY_LISTENER_H__
#define __AIRJOY_NSD_DISCOVERY_LISTENER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"
#include "AirJoyServiceInfo.hpp"

namespace airjoy
{

    class AIRJOY_API AirJoyNsdDiscoveryListener
    {
    public:
        AirJoyNsdDiscoveryListener() {}
        virtual ~AirJoyNsdDiscoveryListener() {}

        /**
         * ĳ�������Ѿ�����
         *  @serviceInfo    [in] ������Ϣ
         */
         virtual void onServiceLost(AirJoyServiceInfo &serviceInfo) = 0;

        /**
         * ĳ�������Ѿ�����
         *  @serviceInfo    [in] ������Ϣ
         */
        virtual void onServiceFound(AirJoyServiceInfo &serviceInfo) = 0;
    };

}



#endif // __AIRJOY_NSD_DISCOVERY_LISTENER_H__
