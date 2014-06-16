#include "AnyAdpServer.hpp"
#include "AdpListener.hpp"
#include <iostream>
#include "UtilUtf8.hpp"
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace anyplay;


class MyAdpListener : public AdpListener
{
public:
    MyAdpListener() {}
    virtual ~MyAdpListener() {}

    virtual void deviceOnline(DeviceInfo &device)
    {
        std::string deviceName = device.name;

#if defined(_WIN32) || defined(_WIN64)
        char multiByte[256];
        if (utf8::UTF8ToMultiByte(multiByte, device.name.c_str(), 256 -1))
            deviceName = multiByte;
#endif

        std::cout << "deviceOnline: " << deviceName << " device id: " << device.deviceId << std::endl;
    }

    virtual void deviceOffline(DeviceInfo &device)
    {
        std::string deviceName = device.name;

#if defined(_WIN32) || defined(_WIN64)
        char multiByte[256];
        if (utf8::UTF8ToMultiByte(multiByte, device.name.c_str(), 256 -1))
            deviceName = multiByte;
#endif

        std::cout << "deviceOffline: " << deviceName << std::endl;
    }

    virtual void deviceNameChanged(DeviceInfo &device)
    {
        std::string deviceName = device.name;

#if defined(_WIN32) || defined(_WIN64)
        char multiByte[256];
        if (utf8::UTF8ToMultiByte(multiByte, device.name.c_str(), 256 -1))
            deviceName = multiByte;
#endif

        std::cout << "deviceNameChanged: " << deviceName << std::endl;
    }

    virtual void didReceiveRequest(AnyPlaySessionId sessionId,
        const std::string &queryXmlns,
        const std::string &appXmlns,
        const std::string &appParam,
        const std::string &fromDeviceId)
    {
        std::cout << "didReceiveRequest" << std::endl;
    }

    virtual void didReceiveResponse(AnyPlaySessionId sessionId,
        const std::string &queryXmlns,
        const std::string &appXmlns,
        const std::string &appResult,
        const std::string &fromDeviceId)
    {
        std::cout << "didReceiveResponse" << std::endl;
    }


    virtual void didSessionTimeout(AnyPlaySessionId sessionId)
    {
        std::cout << "didSessionTimeout" << std::endl;
    }
};



class Command
{
private:
    AnyAdpServer     *m_s;
    //TestServer        *m_s;
    bool               m_loop;

public:
    Command(AnyAdpServer *s)
    //Command(TestServer *s)
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
            std::cout << "rl    -- refresh lan" << std::endl;
            std::cout << "cn    -- change self device nickname" << std::endl;
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

        if (cmd == "rl")
        {
            //m_s->adp()->refresh();

            return;
        }

#if 0
        if (cmd == "cn")
        {
            static int i = 1;

            char nickname[32];
            memset(nickname, 0, 32);
            sprintf(nickname, "w500-%03d", i); 
            m_s->adp()->setDeviceName(nickname);

            i++;
        }
#endif

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
#if 1
    MyAdpListener myAdpListener;

    AnyAdpServer s;
    s.adp()->setListener(&myAdpListener);

    Command cmd(&s);
    cmd.run();
#else

    TestServer s;

    Command cmd(&s);
    cmd.run();
#endif

    return 0;
}
