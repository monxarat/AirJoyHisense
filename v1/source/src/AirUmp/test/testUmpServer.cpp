#include "TestUmpServer.hpp"
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace airjoy;


class Command
{
private:
    TestUmpServer     *m_s;
    bool               m_loop;

public:
    Command(TestUmpServer *s)
    { 
        m_s = s; 
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

    void  parseCommand(const std::string &cmd)
    {
        if (cmd == "help" || cmd == "h")
        {
            std::cout << "---------- help ----------" << std::endl;
            std::cout << "help  -- help infomation" << std::endl;
            std::cout << "exit  -- exit" << std::endl;
            std::cout << "reset -- restart" << std::endl;
            std::cout << "stop  -- stop" << std::endl;
            std::cout << "start -- start" << std::endl;
            return;
        }

        if (cmd == "x")
        {
            m_s->stop();
            m_loop = false;
            return;
        }

        if (cmd == "reset" || cmd == "r")
        {
            for (int i = 0; i < 1; i++)
            {
                m_s->stop();
                m_s->start();
                std::cout << "reset: " << i << std::endl;
            }

            return;
        }

        if (cmd == "stop")
        {
            m_s->stop();
            return;
        }

        if (cmd == "start")
        {
            m_s->start();
            return;
        }
    }
};

int main()
{
    TestUmpServer s;

    Command cmd(&s);
    cmd.run();

    return 0;
}
