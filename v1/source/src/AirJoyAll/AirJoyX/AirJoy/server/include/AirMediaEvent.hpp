/*
* AirMediaEvent.hpp
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

#ifndef __AIR_MEDIA_EVENT__
#define __AIR_MEDIA_EVENT__

#include "AirJoyTypeDef.hpp"

namespace airjoy
{

    class AirMediaEvent
    {
    private:
        bool            m_isValid;
        std::string     m_mediaType;
        std::string     m_action; 
        std::string     m_id;
        std::string     m_url;
        std::string     m_name;
        std::string     m_fromIp;
        std::string     m_emptyString;
        std::string     m_publishString;

    public:
        AirMediaEvent();
        virtual ~AirMediaEvent();

        bool isValid(void);
        void set(const std::string &mediaType, 
                 const std::string &action, 
                 const std::string &id,
                 const std::string &url,
                 const std::string &name,
                 const std::string &fromIp);

        const std::string & toPublishString(void);
    };

}


#endif // __AIR_MEDIA_EVENT__
