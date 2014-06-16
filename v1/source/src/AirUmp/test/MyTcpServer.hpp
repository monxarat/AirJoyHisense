/*
 * MyTcpServer.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#ifndef __MY_TCP_SERVER_H__
#define __MY_TCP_SERVER_H__

namespace airjoy
{

    class MyTcpServer
    {
    public:
        MyTcpServer();
        virtual ~MyTcpServer();

        void start(void);
        void stop(void);
    };
}



#endif // __MY_TCP_SERVER_H__
