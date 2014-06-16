/* 
* AirHttpServer.hpp
*
* coding.tom@gmail.com
* 2011-5-14
*
* 2011-12-29, 文件服务器还可以选用HttpMediaServer（对中文的支持需要测试）：
*             https://github.com/cpearce/HttpMediaServer
*
*/


#ifndef __AIR_HTTP_SERVER_H__
#define __AIR_HTTP_SERVER_H__

#include <string>
#include <map>
#include "AirJoyTypeDef.hpp"
#include "TheBoost.hpp"

struct mg_context;

namespace airjoy
{

    class AirHttpServer 
    {
    private:
        typedef enum _Status
        {
            StatusRun  = 0,
            StatusStop = 1,
        } Status;

        typedef std::map<std::string, std::string> SharedFolderMap;
        typedef std::map<std::string, std::string> SharedFileMap;

        SharedFolderMap     m_folders;
        SharedFileMap       m_files;

        struct mg_context * m_mgCtx;
        Status              m_status;
        int                 m_port;
        int                 m_workThreads;

        std::string         m_url;

    public:
        AirHttpServer(int port = 8080);
        ~AirHttpServer();

        bool start(void);
        bool stop(void);

        void setPort(int port = 8080);
        int getPort(void);
        void setWorkThreads(int n = 5);
        int getWorkThreads(void);
        bool isRunning(void);

        // for manager
        std::string & getUrlWithoutIp(void);

        bool addSharedFolder(const std::string &sharedFolder, const std::string &localFolder);
        bool cancelSharedFolder(const std::string &sharedFolder);
        int numberOfRootFolder(void);

        bool addSharedFile(const std::string &sharedFile, const std::string &localFile);
        bool cancelSharedFile(const std::string &sharedFile);

        // 读取共享文件夹列表
        bool getSharedFolder(const std::string &pathUtf8, FileInfoList *list);

        // 读取共享文件的信息
        bool getSharedFileInfo(const std::string &sharedFileUtf8, FileInfo *fileInfo); 

    private:
        void setUrlWithoutIp(void);
        void addHttpRtspInfo(const std::string &pathUtf8, FileInfoList *list);
        bool getLanSharedFolder(const std::string &pathUtf8, FileInfoList *list);
        bool getRootFolderFromPath(const std::string &pathUtf8, std::string &rootFolderUtf8);
        bool getSubFolderFromPath(const std::string &pathUtf8, std::string &subFolderUtf8);
        bool getListOnPath(const std::string &pathUtf8, FileInfoList *list);
        bool getFileNameFromPath(std::string *filename, const std::string &path);
    };

}

#endif // __AIR_HTTP_SERVER_H__
