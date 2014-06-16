/*
* AirFileHandler.hpp
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

#ifndef __AIR_FILE_HANDLER_H__
#define __AIR_FILE_HANDLER_H__


#include "AirMessageHandler.hpp"

namespace airjoy
{

    class AirFileListener;

    class AirFileHandler : public AirMessageHandler
    {
    public:
        AirFileHandler(AirFileListener *listener);
        virtual ~AirFileHandler();

        /*
         * 添加共享目录
         *
         */

        /*
         * 删除共享目录
         *
         */

        // AirMessageHandler
        virtual bool doAction(const std::string &fromIp,
                              const std::string &xmlns,  
                              const std::string &action, 
                              const std::string &param, 
                              std::string &result);

    private:
        bool doActionGetFileList(const std::string &fromIp, 
                                 const std::string &param, 
                                 std::string &result);
        
        bool doActionGetFileCount(const std::string &fromIp, 
                                  const std::string &param, 
                                  std::string &result);
                
        bool doActionPutFile(const std::string &fromIp, 
                             const std::string &param, 
                             std::string &result);

    private:
        AirFileListener     *m_listener;
    };

}

#endif // __AIR_FILE_HANDLER_H__

