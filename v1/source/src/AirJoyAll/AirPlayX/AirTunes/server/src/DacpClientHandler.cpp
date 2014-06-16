/*
 *
 * DacpClientHandler.cpp
 *
 * coding.tom@gmail.com
 *
 * 2012-6-15
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "DacpClientHandler.hpp"
#include "DacpClient.hpp"
#include "TcpClientMessage.hpp"
#include "AirJoyDefault.hpp"
#include "DacpClientListener.hpp"
#include "AirJoyTypeDef.hpp"

using namespace airjoy;

DacpClientHandler::DacpClientHandler(DacpClient *client, DacpClientListener *listener)
    : m_client(client)
	, m_listener(listener)
{
}

DacpClientHandler::~DacpClientHandler()
{
}

void DacpClientHandler::didReceiveMessage(TcpClientMessagePointer p)
{
    if (m_listener == NULL)
        return;

    if (p->messageType() == TcpClientMessage::ConnectOk)
    {
        m_listener->didConnectOk();
        return;
    }
    
    if (p->messageType() == TcpClientMessage::DisConnect)
    {
        m_listener->didDisConnect();
        return;
    }

    // 超时
    if (p->messageType() == TcpClientMessage::Timeout)
    {
        m_listener->didTimeout();
        return;
    }

    // 连接失败
    if (p->messageType() == TcpClientMessage::ConnectFailed)
    {
        m_listener->didConnectFailed();
        return;
    }
    
    //  网络错误
    if (p->messageType() == TcpClientMessage::ErrorNet)
    {
        m_listener->didErrorNet();
        return;
    }

	//std::cout << "DacpClientHandler::didReceiveMessage() " << std::endl << p->message() << std::endl;

	// 读取文本消息
    if (p->messageType() == TcpClientMessage::NormalMessage)
    {
    }
}

