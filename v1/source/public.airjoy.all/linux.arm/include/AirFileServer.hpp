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
         * �����ļ����������첽������
         * @return 
         *         true - �ɹ�
         *         false - ʧ��
         */
        bool start(void);
        
        /**
         * ֹͣ�ļ�������
         * @return 
         *         true - �ɹ�
         *         false - ʧ��
         */
        bool stop(void);

        /**
         * ��ȡ�ļ��������˿�
         * @return �˿ں�
         */
        int port(void);
        
        /**
         * ��ѯ�ļ��������Ƿ�����
         * @return 
         *         true - ��������
         *         false - û������
         */
        bool isRunning(void);

        /**
         * ���ĳһ������Ŀ¼Ϊ����Ŀ¼
         * @sharedFolderUtf8          [in] ����Ŀ¼���ƣ�UTF8���룩
         * @localFolderUtf8           [in] ����Ŀ¼��UTF8���룩
         */
        void addSharedFolder(const std::string &sharedFolderUtf8, const std::string &localFolderUtf8);
        
        /**
         * ȡ��ĳһ������Ŀ¼
         * @sharedFolderUtf8          [in] ����Ŀ¼���ƣ�UTF8���룩
         */
        void cancelSharedFolder(const std::string &sharedFolderUtf8);

        /**
         * ���ĳһ�������ļ�Ϊ�����ļ�
         * @sharedFileUtf8          [in] �����ļ����ƣ�UTF8���룩
         * @localFileUtf8           [in] �����ļ���UTF8���룩
         * @return �����ļ���url��ַ��������ip��ַ�������Ϊ���ַ��������ʾʧ�ܡ�
         */
        const std::string & addSharedFile(const std::string &sharedFileUtf8, const std::string &localFileUtf8);

        /**
         * ȡ��ĳһ�������ļ�
         * @sharedFileUtf8          [in] �����ļ����ƣ�UTF8���룩
         */
        void cancelSharedFile(const std::string &sharedFileUtf8);

        /**
         * ��ȡ���������ļ�����Ŀ
         * @return  �ļ�����Ŀ
         */
        int numberOfRootFolder(void);

        /**
         * ��ȡ�����������ļ���
         * @pathUtf8          [in] �����ļ���·��
         * @list              [out] �ļ��б�
         * @return 
         *         true - �ɹ�
         *         false - ʧ��
         */
        bool getSharedFolder(const std::string &pathUtf8, FileInfoList *list);

        /**
         * ��ȡ�����ļ�����Ϣ
         * @sharedFileUtf8          [in] �����ļ�����
         * @fileInfo                [out] �ļ���Ϣ
         * @return 
         *         true - �ɹ�
         *         false - ʧ��
         */
        bool getSharedFileInfo(const std::string &sharedFileUtf8, FileInfo *fileInfo); 
    };

}

#endif // __AIR_FILE_SERVER_H__