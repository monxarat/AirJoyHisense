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

    // ��ʱ
    if (p->messageType() == TcpClientMessage::Timeout)
    {
        m_listener->didTimeout();
        return;
    }

    // ����ʧ��
    if (p->messageType() == TcpClientMessage::ConnectFailed)
    {
        m_listener->didConnectFailed();
        return;
    }
    
    //  �������
    if (p->messageType() == TcpClientMessage::ErrorNet)
    {
        m_listener->didErrorNet();
        return;
    }

	//std::cout << "DacpClientHandler::didReceiveMessage() " << std::endl << p->message() << std::endl;

	// ��ȡ�ı���Ϣ
    if (p->messageType() == TcpClientMessage::NormalMessage)
    {
    }
}

