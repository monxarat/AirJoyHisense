/*
 * AirJoySS.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 *
 */

#include "AirJoySS.hpp"
#include "AirJoyCore.hpp"
#include "AnySS.hpp"

using namespace airjoy;

AirJoySS::AirJoySS(AirJoyCore * core)
    : m_core(core)
{
}

AirJoySS::~AirJoySS()
{
}

//------------------------------------------------------------------------------------
// file service
//------------------------------------------------------------------------------------
void AirJoySS::addSharedFolder(const std::string &sharedFolderUtf8, const std::string &localFolderUtf8)
{
    m_core->ss()->addSharedFolder(sharedFolderUtf8, localFolderUtf8);
}

void AirJoySS::cancelSharedFolder(const std::string &sharedFolderUtf8)
{
    m_core->ss()->cancelSharedFolder(sharedFolderUtf8);
}

bool AirJoySS::start(void)
{
    return m_core->ss()->start();
}

bool AirJoySS::stop(void)
{
    return m_core->ss()->stop();
}