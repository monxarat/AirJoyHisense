/*
 * AirJoyNsd.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#ifndef __AIRJOY_NSD_H__
#define __AIRJOY_NSD_H__

#include <string>
#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"
#include "AirJoyServiceInfo.hpp"
#include "AirJoyNsdDiscoveryListener.hpp"

namespace airjoy
{

    class AirJoyNsdCore;

    class AIRJOY_API AirJoyNsd
    {
    public:
        AirJoyNsd();
        virtual ~AirJoyNsd();

        /**
         * 启动自动发现服务
         */
        void start(void);

        /**
         * 停止自动发现服务
         */
        void stop(void);

        /**
         * 启动监听
         * @listener          [in] 监听者
         * @return
         *              - true  成功
         *              - false 失败
         */
        bool discoveryService(AirJoyNsdDiscoveryListener *listener);
        
        /**
         * 停止监听
         * @listener          [in] 监听者
         * @return
         *              - true  成功
         *              - false 失败
         */        
        bool stopServiceDiscovery(void);

        /**
         * 发布服务
         *  @serviceInfo    [in] AirPlay服务端信息
         *  @listener       [in] 监听者
         *  @return
         *              - true  成功
         *              - false 失败
         */
        bool publishService(AirJoyServiceInfo &serviceInfo);

    private:
        AirJoyNsdCore  * m_core;
    };

}

#endif // __AIRJOY_NSD_H__