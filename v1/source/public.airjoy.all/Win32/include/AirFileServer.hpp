/* 
* AirFileServer.h
*
* coding.tom@gmail.com
* 2011-5-14
*
*/


#ifndef __AIR_FILE_SERVER_H__
#define __AIR_FILE_SERVER_H__

#include "AirJoyTypeDef.hpp"
#include "AirJoyApi.hpp"


namespace airjoy
{

    class AirHttpServer;
   
    class AIRJOY_API AirFileServer
    {
    private:
        AirHttpServer       * m_httpServer;
		std::string		    * m_url;

    public:
        AirFileServer();
        virtual ~AirFileServer();
        
        /**
         * 启动文件服务器（异步启动）
         * @return 
         *         true - 成功
         *         false - 失败
         */
        bool start(void);
        
        /**
         * 停止文件服务器
         * @return 
         *         true - 成功
         *         false - 失败
         */
        bool stop(void);

        /**
         * 读取文件服务器端口
         * @return 端口号
         */
        int port(void);
        
        /**
         * 查询文件服务器是否运行
         * @return 
         *         true - 正在运行
         *         false - 没有运行
         */
        bool isRunning(void);

        /**
         * 添加某一个本地目录为共享目录
         * @sharedFolderUtf8          [in] 共享目录名称（UTF8编码）
         * @localFolderUtf8           [in] 本地目录（UTF8编码）
         */
        void addSharedFolder(const std::string &sharedFolderUtf8, const std::string &localFolderUtf8);
        
        /**
         * 取消某一个共享目录
         * @sharedFolderUtf8          [in] 共享目录名称（UTF8编码）
         */
        void cancelSharedFolder(const std::string &sharedFolderUtf8);

        /**
         * 添加某一个本地文件为共享文件
         * @sharedFileUtf8          [in] 共享文件名称（UTF8编码）
         * @localFileUtf8           [in] 本地文件（UTF8编码）
         * @return 共享文件的url地址（不包括ip地址），如果为空字符串，则表示失败。
         */
        const std::string & addSharedFile(const std::string &sharedFileUtf8, const std::string &localFileUtf8);

        /**
         * 取消某一个共享文件
         * @sharedFileUtf8          [in] 共享文件名称（UTF8编码）
         */
        void cancelSharedFile(const std::string &sharedFileUtf8);

        /**
         * 读取局域网根文件夹数目
         * @return  文件夹数目
         */
        int numberOfRootFolder(void);

        /**
         * 读取局域网共享文件夹
         * @pathUtf8          [in] 共享文件夹路径
         * @list              [out] 文件列表
         * @return 
         *         true - 成功
         *         false - 失败
         */
        bool getSharedFolder(const std::string &pathUtf8, FileInfoList *list);

        /**
         * 读取共享文件的信息
         * @sharedFileUtf8          [in] 共享文件名称
         * @fileInfo                [out] 文件信息
         * @return 
         *         true - 成功
         *         false - 失败
         */
        bool getSharedFileInfo(const std::string &sharedFileUtf8, FileInfo *fileInfo); 
    };

}

#endif // __AIR_FILE_SERVER_H__