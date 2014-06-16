/* 
* AirFileServer.cpp
*
* coding.tom@gmail.com
* 2011-5-14
*
*/

#include "AirFileServer.hpp"
#include "AirHttpServer.hpp"


using namespace airjoy;

AirFileServer::AirFileServer()
{
    m_httpServer = new AirHttpServer(0);
	m_url = new std::string;
}

AirFileServer::~AirFileServer()
{
    delete m_httpServer;
	delete m_url;
}

bool AirFileServer::start(void)
{
    return m_httpServer->start();
}

bool AirFileServer::stop(void)
{
    return m_httpServer->stop();
}

int AirFileServer::port(void)
{
    return m_httpServer->getPort();
}

bool AirFileServer::isRunning(void)
{
    return m_httpServer->isRunning();
}

void AirFileServer::addSharedFolder(const std::string &sharedFolderUtf8, const std::string &localFolderUtf8)
{
    m_httpServer->addSharedFolder(sharedFolderUtf8, localFolderUtf8);
}

void AirFileServer::cancelSharedFolder(const std::string &sharedFolderUtf8)
{
    m_httpServer->cancelSharedFolder(sharedFolderUtf8);
}

const std::string & AirFileServer::addSharedFile(const std::string &sharedFileUtf8, const std::string &localFileUtf8)
{
	m_url->clear();

	if (m_httpServer->addSharedFile(sharedFileUtf8, localFileUtf8))
	{
		m_url->append(m_httpServer->getUrlWithoutIp());
		m_url->append("/");
		m_url->append(sharedFileUtf8);
	}

    return *m_url;
}

void AirFileServer::cancelSharedFile(const std::string &sharedFileUtf8)
{
    m_httpServer->cancelSharedFile(sharedFileUtf8);
}

int AirFileServer::numberOfRootFolder(void)
{
    return m_httpServer->numberOfRootFolder();
}

bool AirFileServer::getSharedFolder(const std::string &pathUtf8, FileInfoList *list)
{
    return m_httpServer->getSharedFolder(pathUtf8, list);
}