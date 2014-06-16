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
         * �����Զ����ַ���
         */
        void start(void);

        /**
         * ֹͣ�Զ����ַ���
         */
        void stop(void);

        /**
         * ��������
         * @listener          [in] ������
         * @return
         *              - true  �ɹ�
         *              - false ʧ��
         */
        bool discoveryService(AirJoyNsdDiscoveryListener *listener);
        
        /**
         * ֹͣ����
         * @listener          [in] ������
         * @return
         *              - true  �ɹ�
         *              - false ʧ��
         */        
        bool stopServiceDiscovery(void);

        /**
         * ��������
         *  @serviceInfo    [in] AirPlay�������Ϣ
         *  @listener       [in] ������
         *  @return
         *              - true  �ɹ�
         *              - false ʧ��
         */
        bool publishService(AirJoyServiceInfo &serviceInfo);

    private:
        AirJoyNsdCore  * m_core;
    };

}

#endif // __AIRJOY_NSD_H__