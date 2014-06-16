#include "Ump.hpp"
#include "UmpHandler.hpp"

using namespace airjoy;

std::string remoteIp = "10.0.1.6";
int remotePort = 9999;

class MyUmpHandler : public UmpHandler
{
public:
    MyUmpHandler() 
    {}

    virtual ~MyUmpHandler() 
    {}

    // notify app, the udp server has been started;
    virtual void didStart() 
    {
        std::cout << "didStart" << std::endl;
    }

    // return true if this message can be handled, otherwise return false.
    virtual bool didReceiveMessage(UdpMessagePointer message) 
    { 
        std::cout << "didReceiveMessage: " << message->remoteIp() << " : " << message->remotePort() << std::endl;
        std::cout << message->message()  << std::endl;
        return true;
    }

    // notify app, the tcp session has been closed;
    virtual void didClose() 
    {
        std::cout << "didClose" << std::endl;
    }
};

class TestUmp
{
    public:
        typedef boost::shared_ptr<Ump>                          UmpPointer;
        typedef boost::shared_ptr<boost::thread>                ThreadPointer;
        typedef Ump::UmpHandlerPointer                          HandlerPointer;
        typedef boost::asio::deadline_timer						DeadlineTimer;

    private:
        bool                            m_isStart;
        boost::asio::io_service         m_ios;
        ThreadPointer                   m_pThread;
        UmpPointer                      m_pUmp;
        HandlerPointer                  m_pHandler;
        DeadlineTimer					m_timer;

    public:
        TestUmp()
            : m_isStart(false)
            , m_timer(m_ios)
        {
            m_pUmp = boost::make_shared<Ump>(boost::ref(m_ios), 9999);

            m_pHandler = boost::make_shared<MyUmpHandler>();
            m_pUmp->setHandler(m_pHandler);

#if 0
            //
            // 异步调用，需要启动定时器，用来保证m_ios线程一直运行
            //

            m_timer.expires_from_now(boost::posix_time::seconds(1));
            m_timer.async_wait(boost::bind(&TestUmp::didTimeout,
                this,
                boost::asio::placeholders::error));
#endif
        }

        virtual ~TestUmp()
        {
            stop();
        }

        bool start(void)
        {
            if (m_isStart)
                return false;

            if (m_pThread.use_count() > 0)
                return false;

            try
            {
                m_pUmp->start();
                m_pThread = boost::make_shared<boost::thread>(boost::bind(&boost::asio::io_service::run, &m_ios));

                std::cout << "port: " << m_pUmp->port() << std::endl;
            }
            catch (std::exception &e)
            {
                //std::cout << "AirPlayServerCore::start: " << e.what() << std::endl;
                return false;
            }

            m_isStart = true;

            return true;
        }

        bool stop(void)
        {
            if (! m_isStart)
                return false;

            if (m_pThread.use_count() == 0)
                return false;

            m_pUmp->stop();

            m_ios.stop();

            m_pThread->join();
            m_pThread.reset();

            m_ios.reset();

            m_isStart = false;

            return true;
        }

        bool run(void)
        {
            if (! this->start())
                return false;

            m_pThread->join();
            m_pThread.reset();
            m_ios.reset();

            return true;
        }

        UmpPointer ump(void)
        { 
            return m_pUmp; 
        }

        bool sendTo(const std::string &remoteIp, int remotePort, const std::string &message)
        {
            if (! m_isStart)
                return false;

            m_pUmp->send(remoteIp, remotePort, message);

            return true;
        }

        void didTimeout(const boost::system::error_code& error)
        {
            std::cout << "didTimeout" << std::endl;

            if (error)
            {
                std::cout << "didTimeout error : " << error << std::endl;

                return;	
            }

            //
            // 异步调用，需要启动定时器，用来保证m_ios线程一直运行
            //

            m_timer.expires_from_now(boost::posix_time::seconds(1));
            m_timer.async_wait(boost::bind(&TestUmp::didTimeout, 
                this, 
                boost::asio::placeholders::error));
        }
};
    
TestUmp g_ump;


class MyCommand
{
public:
    MyCommand() 
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
            std::cout << "s     -- send message to" << std::endl;
            return;
        }

        if (cmd == "stop")
        {
            g_ump.stop();
            return;
        }

        if (cmd == "start")
        {
            g_ump.start();
            return;
        }

        if (cmd == "exit" || cmd == "x")
        {
            g_ump.stop();

            m_loop = false;
            return;
        }

        if (cmd == "s")
        {
            g_ump.sendTo(remoteIp, remotePort, "hello");
            return;
        }
    }

private:
    bool               m_loop;
};

int main()
{
    g_ump.start();

    MyCommand cmd;
    cmd.run();

    return 0;
}
