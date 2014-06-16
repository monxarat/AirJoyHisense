
#include "AJServer.h"

AJState::AJState()
{
	m_id = "";
	m_url = "";
	m_name = "";
	m_ip = "";
	m_action = "none";
	m_mediaType = "none";
}

AJState::~AJState()
{
}

void AJState::publishState()
{
//	g_AJServer->getAirJoyServer()->publishMediaEvent(m_mediaType, m_action, m_id, m_url, m_name, m_ip);
}


void AJState::setMediaTypePhoto()
{
	m_mediaType = "photo";
}

void AJState::setMediaTypeAudio()
{
	m_mediaType = "audio";
}

void AJState::setMediaTypeVideo()
{
	m_mediaType = "video";
}


void AJState::initState(const std::string &id,
			   const std::string &url,
			   const std::string &name)
{
	m_id = id;
	m_url = url;
	m_name = name;
}



#if 0
void AJState::checkIP(const std::string &fromIp)
{
	if(m_ip == fromIp) 
		return;
	m_action = "stop";
	m_ip = fromIp;
	publishState();
}
#endif

void AJState::setStatePlay(const std::string &fromIp)
{
//	checkIP(fromIp);
	m_action = "play";
	publishState();
}

void AJState::setStatePause(const std::string &fromIp)
{
//	checkIP(fromIp);
	m_action = "pause";
	publishState();
}

void AJState::setStateStop(const std::string &fromIp)
{
//	checkIP(fromIp);
	m_action = "stop";
	publishState();
}

