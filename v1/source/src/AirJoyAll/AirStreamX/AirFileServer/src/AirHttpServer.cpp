/* 
* AirHttpServer.cpp
*
* coding.tom@gmail.com
* 2011-5-14
*
*/

#include "AirHttpServer.hpp"
#include "mongoose.hpp"
#include "UtilItoa.hpp"
#include "UtilUtf8.hpp"

#include <sstream>

using namespace airjoy;

AirHttpServer::AirHttpServer(int port)
{
    m_mgCtx       = NULL;
    m_status      = StatusStop;
    m_port        = port;
    m_workThreads = 5;

    setUrlWithoutIp();
}

AirHttpServer::~AirHttpServer()
{
    this->stop();
}

bool AirHttpServer::start(void)
{
    const char *opt_root       = "document_root";
    const char *opt_root_value = ".";
    const char *opt_ports      = "listening_ports";
    const char *opt_threads    = "num_threads";
    char opt_ports_value[8]    = {0};
    char opt_threads_value[8]  = {0};

    const char *options[] = 
    {
        opt_root,    opt_root_value,
        opt_ports,   opt_ports_value,
        opt_threads, opt_threads_value,
        NULL
    };

    _itoa_s(m_port, opt_ports_value, 8, 10);
    _itoa_s(m_workThreads, opt_threads_value, 8, 10);

    m_mgCtx = ::mg_start(NULL, NULL, options);
    if (m_mgCtx == NULL)
        return false;

    SharedFolderMap::iterator iter;
    for (iter = m_folders.begin(); iter != m_folders.end(); ++iter)
        ::mg_add_virtual_folder(m_mgCtx, iter->first, iter->second);

    m_status = StatusRun;
    return true;
}

bool AirHttpServer::stop(void)
{
    if (m_status == StatusStop)
        return false;

	// mongoose有个bug，stop时会阻塞
    ::mg_stop(m_mgCtx);
    m_status = StatusStop;
    m_port = 0;

    return true;
}

bool AirHttpServer::isRunning(void)
{
    return (m_status == StatusRun);
}

void AirHttpServer::setPort(int port)
{
    m_port = port;
    
    setUrlWithoutIp();
}

int AirHttpServer::getPort(void)
{
    if (m_port == 0)
    {
        m_port = ::mg_get_port(m_mgCtx);
        setUrlWithoutIp();
    }

    return m_port;
}

void AirHttpServer::setWorkThreads(int n)
{
    m_workThreads = n;
}

int AirHttpServer::getWorkThreads(void)
{
    return m_workThreads;
}

bool AirHttpServer::addSharedFolder(const std::string &sharedFolder, const std::string &localFolder)
{
    SharedFolderMap::iterator pos;
    pos = m_folders.find(sharedFolder);
    if (pos != m_folders.end())
        m_folders.erase(pos);

    m_folders.insert(std::pair<std::string, std::string>(sharedFolder, localFolder));

    if (m_status == StatusRun)
    {
        int ret = 0;
        ret = ::mg_add_virtual_folder(m_mgCtx, sharedFolder, localFolder);
        if (ret != 0)
            return false;
    }

    return true;
}

bool AirHttpServer::cancelSharedFolder(const std::string &sharedFolder)
{
    m_folders.erase(sharedFolder);

    if (m_status == StatusRun)
        ::mg_remove_virtual_folder(m_mgCtx, sharedFolder);

    return true;
}

int AirHttpServer::numberOfRootFolder(void)
{
    return (int)m_folders.size();
}

bool AirHttpServer::addSharedFile(const std::string &sharedFile, const std::string &localFile)
{
    SharedFileMap::iterator pos;
    pos = m_files.find(sharedFile);
    if (pos != m_files.end())
		m_files.erase(pos);
	
	m_files.insert(std::pair<std::string, std::string>(sharedFile, localFile));

    if (m_status == StatusRun)
    {
        int ret = 0;
        ret = ::mg_add_virtual_file(m_mgCtx, sharedFile, localFile);
        if (ret != 0)
            return false;
    }

    return true;
}

bool AirHttpServer::cancelSharedFile(const std::string &sharedFile)
{
    m_files.erase(sharedFile);

    if (m_status == StatusRun)
        ::mg_remove_virtual_file(m_mgCtx, sharedFile);

    return true;
}

bool AirHttpServer::getSharedFolder(const std::string &pathUtf8, FileInfoList *list)
{
    if (list == NULL)
        return false;

    if (! this->isRunning())
        return false;

    if (! this->getLanSharedFolder(pathUtf8, list))
        return false;

    addHttpRtspInfo(pathUtf8, list);

    return true;
}

void AirHttpServer::setUrlWithoutIp(void)
{
    char strPort[8] = {0};

    _itoa_s(m_port, strPort, 8, 10);
    m_url.clear();
    m_url.append(":");
    m_url.append(strPort);
}

std::string & AirHttpServer::getUrlWithoutIp(void)
{
    if (m_port == 0)
    {
        m_port = ::mg_get_port(m_mgCtx);
        setUrlWithoutIp();
    }

    return m_url;
}

bool AirHttpServer::getLanSharedFolder(const std::string &pathUtf8, FileInfoList *list)
{
    // read root folder
    if (pathUtf8 == "/" || pathUtf8.size() == 0)
    {
        SharedFolderMap::iterator iter;
        for (iter = m_folders.begin(); iter != m_folders.end(); ++iter)
        {
            FileInfo info;
            info.name       = iter->first;
            info.size       = 0;
            info.type       = FileTypeFolder;

            info.strLastModify      = "2012-3-11 17:51:01";
            info.lastModify.year    = 2012;
            info.lastModify.month   = 3;
            info.lastModify.day     = 11;
            info.lastModify.hour    = 17;
            info.lastModify.minute  = 51;
            info.lastModify.second  = 1;

            list->push_back(info);
        }

        return true;
    }

    // folder: "/music" "/video" "/photo" ...
    std::string rootFolderUtf8;
    if (! getRootFolderFromPath(pathUtf8, rootFolderUtf8))
        return false;

    SharedFolderMap::iterator iter = m_folders.find(rootFolderUtf8);
    if (iter != m_folders.end())
    {
        std::string localFolderUtf8 = iter->second;

        std::string subFolderUtf8;
        if (getSubFolderFromPath(pathUtf8, subFolderUtf8))
            localFolderUtf8.append(subFolderUtf8);

        if (! getListOnPath(localFolderUtf8, list))
            return false;

        return true;
    }
    
    // not found
    return false;
}

void AirHttpServer::addHttpRtspInfo(const std::string &pathUtf8, FileInfoList *list)
{
    FileInfo *p = NULL;
    FileInfoList::iterator iter;
    for (iter = list->begin(); iter != list->end(); ++iter)
    {
        p = &(*iter);

        if (p->type == FileTypeFolder)
            continue;

        // 这里的url地址不包含头："http://192.168.0.109"
        // 因为设备可能有多个网卡，此http地址头由接收方添加。
        // 同理，rtsp地址也没有rtsp地址头。
        p->urlHttp.clear();
        p->urlHttp.append(this->getUrlWithoutIp());

        if (pathUtf8.at(0) != '/')
            p->urlHttp.append("/");

        p->urlHttp.append(pathUtf8);
        p->urlHttp.append("/");
        p->urlHttp.append(iter->name);

        //pos->rtsp = "rtsp://hello.mpg" + path;
    }
}

bool AirHttpServer::getRootFolderFromPath(const std::string &pathUtf8, std::string &rootFolderUtf8)
{
    if (pathUtf8 == "/" || pathUtf8.empty())
        return false;

    const char *p = pathUtf8.c_str();
    if (*p == '/')
        p++;

    while (*p && *p != '/')
    {
        rootFolderUtf8.push_back(*p);
        p++;
    }

    return true;
}

bool AirHttpServer::getSubFolderFromPath(const std::string &pathUtf8, std::string &subFolderUtf8)
{
    if (pathUtf8 == "/" || pathUtf8.empty())
        return false;

    const char *p = pathUtf8.c_str();
    if (*p == '/')
        p++;

    // skip root folder
    while (*p && *p != '/')
        p++;

    if (*p == 0)
        return false;

    subFolderUtf8 = p;

    return true;
}

bool AirHttpServer::getListOnPath(const std::string &pathUtf8, FileInfoList *list)
{
    using namespace boost::filesystem;
    using namespace boost::gregorian;
    using namespace boost::posix_time;
    
    std::string thePath = pathUtf8;

#if defined(_WIN32) || defined(_WIN64)
    char multiByte[1024];
    if (utf8::UTF8ToMultiByte(multiByte, pathUtf8.c_str(), 1024 - 1))
        thePath = multiByte;
#endif

    boost::filesystem::path dir(thePath);

    if (status(dir).type() == file_not_found)
        return false;
    
    directory_iterator end;
    for (directory_iterator pos(dir); pos != end; ++pos)
    {
        FileInfo info;
        
        // filename -> utf8
    
    #if defined(_WIN32) || defined(_WIN64)
        char filename_utf8[256];
        if (utf8::MultiByteToUTF8(filename_utf8, pos->filename().c_str(), 256 - 1))
            info.name = filename_utf8;
        else
            info.name = pos->filename();
    #else
        info.name = pos->filename();
    #endif

        if (is_directory(*pos))
        {
            info.type = FileTypeFolder;
            info.size = 0;
        }
        else
        {
            info.type = FileTypeFile;
            info.size = file_size(*pos);
        }

        time_t time = last_write_time(*pos);
        ptime p = from_time_t(time);
        date d = p.date();
        time_duration e = p.time_of_day();
        
        info.lastModify.year    = d.year();
        info.lastModify.month   = d.month();
        info.lastModify.day     = d.day();
        info.lastModify.hour    = e.hours();
        info.lastModify.minute  = e.minutes();
        info.lastModify.second  = e.seconds();

        std::stringstream ss;
        ss << e.hours() << ":" << e.minutes() << ":" << e.seconds();

        info.strLastModify = to_iso_extended_string(d);
        info.strLastModify.append(" ");
        info.strLastModify.append(ss.str());

        if (info.name[0] != '.')
            list->push_back(info);
    }

    return true;
}

bool AirHttpServer::getSharedFileInfo(const std::string &sharedFileUtf8, FileInfo *fileInfo)
{
    using namespace boost::filesystem;
    using namespace boost::gregorian;
    using namespace boost::posix_time;

    // search files & update
    SharedFileMap::iterator iter = m_files.find(sharedFileUtf8);
    if (iter == m_files.end())
        return false;
    
    std::string pathUtf8 = iter->second;
    if (! getFileNameFromPath(&(fileInfo->name), pathUtf8))
        return false;

    std::string path = pathUtf8;

#if defined(_WIN32) || defined(_WIN64)
    char multiByte[1024];
    if (utf8::UTF8ToMultiByte(multiByte, pathUtf8.c_str(), 1024 - 1))
        path = multiByte;
#endif

    if (is_directory(path))
        return false;

    fileInfo->type = FileTypeFile;
    fileInfo->size = file_size(path);

    time_t time = last_write_time(path);
    ptime p = from_time_t(time);
    date d = p.date();
    time_duration e = p.time_of_day();

    fileInfo->lastModify.year    = d.year();
    fileInfo->lastModify.month   = d.month();
    fileInfo->lastModify.day     = d.day();
    fileInfo->lastModify.hour    = e.hours();
    fileInfo->lastModify.minute  = e.minutes();
    fileInfo->lastModify.second  = e.seconds();

    std::stringstream ss;
    ss << e.hours() << ":" << e.minutes() << ":" << e.seconds();

    fileInfo->strLastModify = to_iso_extended_string(d);
    fileInfo->strLastModify.append(" ");
    fileInfo->strLastModify.append(ss.str());

    // http info
    fileInfo->urlHttp = this->getUrlWithoutIp();
    fileInfo->urlHttp.append("/");
    fileInfo->urlHttp.append(sharedFileUtf8);

    return true;
}

bool AirHttpServer::getFileNameFromPath(std::string *filename, const std::string &path)
{
    std::string::size_type pos = 0;

    pos = path.rfind('\\');
    if (pos != std::string::npos)
    {
        filename->clear();
        filename->append(path.substr(pos + 1));
        return true;
    }

    pos = path.rfind('/');
    if (pos != std::string::npos)
    {
        filename->clear();
        filename->append(path.substr(pos + 1));
        return true;
    }

    return false;
}
