/*
* AirPlayNsdPublishHandler.cpp
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


#include "AirPlayNsdPublishHandler.hpp"
#include "AirPlayNsdPublishListener.hpp"
#include "Nsd.hpp"
#include "AirPlayServiceInfo.hpp"
#include "NsdServiceInfo.hpp"

using namespace airjoy;

AirPlayNsdPublishHandler::AirPlayNsdPublishHandler(AirPlayNsdPublishListener * listener)
    : m_listener(listener)
{
}

AirPlayNsdPublishHandler::~AirPlayNsdPublishHandler() 
{
}

void AirPlayNsdPublishHandler::didNotPublish(NsdServiceInfoPointer serviceInfo)
{
    if (m_listener)
        m_listener->onPublishFailed(serviceInfo->getType(), serviceInfo->getName(), 0);
}

void AirPlayNsdPublishHandler::willPublish(NsdServiceInfoPointer serviceInfo)
{
}

void AirPlayNsdPublishHandler::didPublish(NsdServiceInfoPointer serviceInfo)
{
    if (m_listener)
        m_listener->onServicePublished(serviceInfo->getType(), serviceInfo->getName());
}