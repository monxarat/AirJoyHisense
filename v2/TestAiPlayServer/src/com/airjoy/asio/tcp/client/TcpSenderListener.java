
package com.airjoy.asio.tcp.client;

public interface TcpSenderListener {

    void didSendBytes(TcpSender sender, int sendBytes);
}
