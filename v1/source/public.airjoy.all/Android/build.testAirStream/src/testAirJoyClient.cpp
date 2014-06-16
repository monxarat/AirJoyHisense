/*
*
* testAirJoyClient.cpp
*
*
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "AirFileServer.hpp"

using namespace airjoy;

AirFileServer				   gAirFileServer;

class MyCommand
{
private:
    bool               m_loop;
    int                m_eventSessionId;
    bool               m_playing;

public:
    MyCommand() 
        : m_eventSessionId(0)
        , m_playing(false)
    {
    }

    ~MyCommand()
    {
    }
    
    void run()
    {
        std::string buf;
        m_loop = true;

        while (m_loop)
        {
            std::cout << "> ";
            buf.clear();
            std::cin >> buf;
            parseCommand(buf);
        }
    }

    void setEventSessionId(int id) { m_eventSessionId = id; }

private:
    void parseCommand(const std::string &cmd)
    {

        if (cmd == "help" || cmd == "h")
        {
            std::cout << "---------- help ----------" << std::endl;
            std::cout << "help  -- help infomation" << std::endl;
            std::cout << "exit  -- exit" << std::endl;
            return;
        }
	
        //-----------------------------------------------------------------------
		// �����û��Զ�����Ϣ
		//-----------------------------------------------------------------------
        if (cmd == "start")
        {
			// �����ļ�������
			gAirFileServer.start();

            return;
        }

        if (cmd == "exit" || cmd == "x")
        {
			// ֹͣ�ļ�������
			gAirFileServer.stop();

            m_loop = false;
            return;
        }

    }
};

int main(int argc, char *argv[])
{
	// �����ļ�������
	gAirFileServer.start();

    std::string url = gAirFileServer.addSharedFile("111.jpg", "/mnt/sdcard/1.jpg");
    std::cout << "url: " << url << std::endl;

    MyCommand cmd;
    cmd.run();

    std::cout << "byebye! " << std::endl;

    return 0;
}
