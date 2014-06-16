#include "MyTcpClient.hpp"
#include <iostream>

using namespace airjoy;

MyTcpClient gClient;


class MyCommand
{
private:
    bool               m_loop;

public:
    MyCommand() { }

    ~MyCommand() { }
    
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

private:
    void parseCommand(const std::string &cmd)
    {

        if (cmd == "help" || cmd == "h")
        {
            std::cout << "---------- help ----------" << std::endl;
            std::cout << "help  -- help infomation" << std::endl;
            std::cout << "exit  -- exit" << std::endl;
            std::cout << "stop  -- stop" << std::endl;
            std::cout << "start -- start" << std::endl;
			
			std::cout << "t     -- test timeout" << std::endl;
			std::cout << "s     -- send rquest" << std::endl;

			std::cout << "cons  -- connect to server" << std::endl;
			std::cout << "disc  -- disconnect" << std::endl;

            return;
        }
		
		if (cmd == "t")
		{
			gClient.testTimeout(1);
		}

		if (cmd == "s")
		{
			gClient.startRequest("192.168.100.118", 11570, "hello", 1);
		}

		if (cmd == "cons")
		{
			gClient.connect("192.168.100.118", 11570, 2);
		}
		
		if (cmd == "disc")
		{
			gClient.disconnect();
		}

        if (cmd == "stop")
        {
            gClient.stop();
            return;
        }

        if (cmd == "start")
        {
            gClient.start();
            return;
        }

        if (cmd == "exit" || cmd == "x")
        {
            gClient.stop();
            m_loop = false;
            return;
        }
    }
};


int main()
{
    gClient.start();

    MyCommand cmd;
    cmd.run();

    std::cout << "byebye! " << std::endl;
    

    return 0;
}
