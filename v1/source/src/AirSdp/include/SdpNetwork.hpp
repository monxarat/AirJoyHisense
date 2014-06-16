/*
*
* SdpNetwork.hpp
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


#ifndef __SDP_NETWORK__
#define __SDP_NETWORK__

#include <vector>
#include <string>

namespace airjoy
{

    class SdpNetwork
    {
    public:
        SdpNetwork();
        virtual ~SdpNetwork();

        bool getBroadcastList(std::vector<std::string> &list);

    private:
        bool getSubnetWithIp(char *subnet, const char *ip);
   };

}

#endif // __SDP_NETWORK__
