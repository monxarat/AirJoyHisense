/*
 *
 * SdpNetwork.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-3-21
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "SdpNetwork.hpp"

#include <iostream>


#if defined(_WIN32) || defined(_WIN64)
#include <ws2tcpip.h>
#include <windows.h>
#include <Iphlpapi.h>   // 用于枚举所有网卡IP
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>   // for ::close()
#endif // _WIN32

#include <stdlib.h>
#include <string.h>

using namespace airjoy;

SdpNetwork::SdpNetwork ()
{
}

SdpNetwork::~SdpNetwork ()
{
}

#if defined(_WIN32) || defined(_WIN64)

bool SdpNetwork::getBroadcastList(std::vector<std::string> &list)
{
    IP_ADAPTER_INFO   *   ipAdaptersInfo;

    ULONG infolen = sizeof(IP_ADAPTER_INFO);
    ipAdaptersInfo = (IP_ADAPTER_INFO *) malloc(infolen);

    if (GetAdaptersInfo(ipAdaptersInfo, &infolen) == ERROR_BUFFER_OVERFLOW)
    {
        free(ipAdaptersInfo);
        ipAdaptersInfo = (IP_ADAPTER_INFO *) malloc(infolen);
    }

    if (GetAdaptersInfo(ipAdaptersInfo, &infolen))
    {
        free(ipAdaptersInfo);
        return false;
    }

    IP_ADAPTER_INFO * pNextAdapter = NULL;
    for (pNextAdapter = ipAdaptersInfo; pNextAdapter; pNextAdapter = pNextAdapter->Next)
    {
        IP_ADDR_STRING *pNextIp = NULL;
        for (pNextIp = &(pNextAdapter->IpAddressList); pNextIp; pNextIp = pNextIp->Next) 
        {
            unsigned long ip = inet_addr(pNextIp->IpAddress.String);
            if (ip == 0)
                break;

            char subnet[4 * 4];
            memset(subnet, 0, 4 * 4);
            getSubnetWithIp(subnet, pNextIp->IpAddress.String);

            //StringPointer broadcastIP = boost::make_shared<std::string>(subnet);
            //m_list.push_back(broadcastIP);

            list.push_back(subnet);
        }
    }

    free(ipAdaptersInfo);

    return true;
}

#else

bool SdpNetwork::getBroadcastList(std::vector<std::string> &list)
{
    int socketNo = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socketNo < 0)
        return false;

    int ret = 0;
    struct ifreq   buf[32];
    struct ifconf  ifc;

    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = (caddr_t)buf;

    ret = ::ioctl(socketNo, SIOCGIFCONF, (char *) &ifc);
    if (ret != 0)
    {
        ::close(socketNo);
        return false;
    }

#ifdef __MAC_OSX__
    struct ifreq *ifr = NULL;
    char *p = NULL;
    for (p = ifc.ifc_buf; p < ifc.ifc_buf + ifc.ifc_len; )
    {
        ifr = (struct ifreq *)p;

        //std::cout << "--- name: " << ifr->ifr_name << std::endl;

        // p point next ifreq
        int len = 0;

    #ifdef __MAC_OSX__
        if (ifr->ifr_addr.sa_len > sizeof(struct sockaddr))
            len = ifr->ifr_addr.sa_len;
        else
            len = sizeof(struct sockaddr);
    #else
        len = sizeof(struct sockaddr);
    #endif

        p += sizeof(ifr->ifr_name) + len;

        // get flag
        ret = ::ioctl(socketNo, SIOCGIFFLAGS, ifr);
        if (ret != 0)
        {
            //std::cout << "get if flags failed" << std::endl;
            continue;
        }

        if (ifr->ifr_flags & IFF_LOOPBACK)
        {
            //std::cout << "loopback " << std::endl;
            continue;
        }

        // get ip address
        ret = ::ioctl(socketNo, SIOCGIFADDR, ifr);
        if (ret != 0)
        {
            //std::cout << "get ip address failed" << std::endl;
            continue;
        }

        struct sockaddr_in *addr = (struct sockaddr_in *)&(ifr->ifr_addr);
        //std::cout <<"ip: " << inet_ntoa(addr->sin_addr) << std::endl;

        // get broadcast address
        ret = ::ioctl(socketNo, SIOCGIFBRDADDR, ifr);
        if (ret != 0)
            continue;

        addr = (struct sockaddr_in *)&(ifr->ifr_addr);
        //std::cout << "broadcast: " << inet_ntoa(addr->sin_addr) << std::endl;

        list.push_back(inet_ntoa(addr->sin_addr));
    }
#else
    int len = sizeof(struct ifreq);
    int count = ifc.ifc_len / len;
    for (int i = 0; i < count; ++i)
    {
        struct ifreq *ifr = &ifc.ifc_req[i];

        //std::cout << "--- name: " << ifr->ifr_name << std::endl;

        // get flag
        ret = ::ioctl(socketNo, SIOCGIFFLAGS, ifr);
        if (ret != 0)
        {
            //std::cout << "get if flags failed" << std::endl;
            continue;
        }

        if (ifr->ifr_flags & IFF_LOOPBACK)
        {
            //std::cout << "loopback " << std::endl;
            continue;
        }

        // get ip address
        ret = ::ioctl(socketNo, SIOCGIFADDR, ifr);
        if (ret != 0)
        {
            //std::cout << "get ip address failed" << std::endl;
            continue;
        }

        struct sockaddr_in *addr = (struct sockaddr_in *)&(ifr->ifr_addr);
        //std::cout <<"ip: " << inet_ntoa(addr->sin_addr) << std::endl;

        // get broadcast address
        ret = ::ioctl(socketNo, SIOCGIFBRDADDR, ifr);
        if (ret != 0)
            continue;

        addr = (struct sockaddr_in *)&(ifr->ifr_addr);
        //std::cout << "broadcast: " << inet_ntoa(addr->sin_addr) << std::endl;

        list.push_back(inet_ntoa(addr->sin_addr));
    }
#endif // __MAC_OSX__

    ::close(socketNo);

    return true;
}

#endif // _WIN32

bool SdpNetwork::getSubnetWithIp(char *subnet, const char *ip)
{
    int dot = 0;
    int i = 0;

    for (i = 0; ip[i]; ++i)
    {
        subnet[i] = ip[i];
        
        if (subnet[i] == '.')
            dot ++;
        
        if (dot == 3)
        {
            subnet[i + 1] = '2';
            subnet[i + 2] = '5';
            subnet[i + 3] = '5';
            return true;
        }
    }

    return false;
}
