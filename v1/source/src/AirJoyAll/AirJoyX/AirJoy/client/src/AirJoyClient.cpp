/*
 * AirJoyClient.cpp
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

#include "AirJoyClient.hpp"
#include "AirJoyClientCore.hpp"
#include "UmpClient.hpp"
#include "UtilBase64.hpp"
#include "UmpMessage.hpp"
#include "UmpClientHandler.hpp"

using namespace airjoy;

AirJoyClient::AirJoyClient()
    : m_isStart(false)
{
    m_core = new AirJoyClientCore;
}

AirJoyClient::~AirJoyClient()
{
    delete m_core;
}

void AirJoyClient::start(void)
{
    if (m_isStart)
        return;
    
    m_isStart = true;

    m_core->start();
}

void AirJoyClient::stop(void)
{
    if (! m_isStart)
        return;
    
    m_isStart = false;

    m_core->stop();
}

AirJoyClientCore * AirJoyClient::core(void)
{
    return m_core;
}