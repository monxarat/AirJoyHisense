/*
 * DacpClient.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "DacpClient.hpp"
#include "DacpClientCore.hpp"
#include "UmpClient.hpp"
#include "DacpClientHandler.hpp"

using namespace airjoy;

#define     DEFAULT_TIME_OUT    6

DacpClient::DacpClient()
    : m_isStart(false)
{
    m_core = new DacpClientCore;
}

DacpClient::~DacpClient()
{
    delete m_core;
}

void DacpClient::start(void)
{
    //std::cout << "DacpClient::start()" << std::endl;

    if (m_isStart)
        return;
    
    m_isStart = true;

    m_core->start();
}

void DacpClient::stop(void)
{
    //std::cout << "DacpClient::stop()" << std::endl;

    if (! m_isStart)
        return;

    m_isStart = false;

    m_core->stop();
}

void DacpClient::setListener(DacpClientListener *listener)
{
    typedef boost::shared_ptr<DacpClientHandler>         DacpClientHandlerPointer;
    DacpClientHandlerPointer dacpClientHandler = boost::make_shared<DacpClientHandler>(this, listener);
    m_core->client()->setHandler(dacpClientHandler);
}

void DacpClient::addServer(const std::string &ip, uint16_t port)
{
    m_dacpServer.insert(std::make_pair(ip, port));
}

void DacpClient::removeServer(const std::string &ip)
{
    DacpServerMap::iterator pos = m_dacpServer.find(ip);
    if (pos != m_dacpServer.end())
        m_dacpServer.erase(pos);
}

bool DacpClient::send(const std::string &ip, const std::string &message)
{
    if (! m_isStart)
        return false;

    DacpServerMap::iterator pos = m_dacpServer.find(ip);
    if (pos == m_dacpServer.end())
        return false;
    
    //std::cout << "DacpClient::send() " << ip << std::endl;
    //std::cout << message << std::endl;

    if (m_core->client()->isConnected())
    {
        if (m_core->client()->serverIp() == ip)
        {
            return m_core->client()->send(message, DEFAULT_TIME_OUT);
        }
        else
        {
            m_core->client()->disconnect();
            return m_core->client()->send(pos->first, pos->second, message, DEFAULT_TIME_OUT);
        }
    }
    else
    {
        return m_core->client()->send(pos->first, pos->second, message, DEFAULT_TIME_OUT);
    }
}


/*
----------------------------------------------------------------------------------------------
Server（49186） -> Client（58285）建立TCP连接，然后发送HTTP GET请求
----------------------------------------------------------------------------------------------
下一首歌
-----------------------------------------------
GET /ctrl-int/1/nextitem HTTP/1.1
Host: iPod-Touch5.local.
Active-Remote: 1647774228
-----------------------------------------------
HTTP/1.1 200 OK
Date: Fri, 22 Feb 2013 01:14:22 GMT
Content-Length: 0
-----------------------------------------------

暂停播放
-----------------------------------------------
GET /ctrl-int/1/pause HTTP/1.1
Host: iPod-Touch5.local.
Active-Remote: 1647774228
-----------------------------------------------
HTTP/1.1 200 OK
Date: Fri, 22 Feb 2013 01:14:22 GMT
Content-Length: 0
-----------------------------------------------

----------------------------------------------------------------------------------------------
Server（49187） -> Client（58285）建立TCP连接，然后发送HTTP GET请求
----------------------------------------------------------------------------------------------

恢复播放
-----------------------------------------------
GET /ctrl-int/1/playpause HTTP/1.1
Host: iPod-Touch5.local.
Active-Remote: 1647774228
-----------------------------------------------
HTTP/1.1 200 OK
Date: Fri, 22 Feb 2013 01:14:27 GMT
Content-Length: 0
-----------------------------------------------
*/