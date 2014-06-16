/*
 * Copyright (C), 2011-2212, AirJoy
 *
 * File name:  	AJServer.cpp
 * Author:     	SangWencheng
 * Version:    	V1.0
 * Date: 		2012-11-01
 *
 * Description:    
 * 		AJState.cpp 保存服务端的实时状态信息.
 *
 * History:        
 * 	1. 	Date:
 * 		Author:
 * 		Modification:
 * 	2. ...
 */

#ifndef __AJSTATE_H__
#define __AJSTATE_H__

#include <stdio.h>
#include <stdlib.h>

class AJState 
{
	public:
		AJState();
		virtual ~AJState();

		void setMediaTypePhoto();
		void setMediaTypeAudio();
		void setMediaTypeVideo();

		void initState(const std::string &id,
					   const std::string &url,
					   const std::string &name);

		void setStatePlay(const std::string &fromIp);
		void setStatePause(const std::string &fromIp);
		void setStateStop(const std::string &fromIp);
		void publishState();

	private:
		std::string m_mediaType;
		std::string m_action; 
		std::string m_id;
		std::string m_url;
		std::string m_name;
		std::string m_ip;
		//AirJoyServer *m_AirJoyServer;
};

#endif
