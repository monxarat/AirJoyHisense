/*
 * testTcpServer.hpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-8
 *
 */

#include "MyTcpServer.hpp"

using namespace airjoy;

int main()
{
    MyTcpServer myTcpServer;

    myTcpServer.start();

    return 0;
}
