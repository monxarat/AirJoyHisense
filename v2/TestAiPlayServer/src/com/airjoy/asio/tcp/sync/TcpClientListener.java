
package com.airjoy.asio.tcp.sync;

public interface TcpClientListener {

    void didConnected(TcpClient client);

    void didConnectedFailed(TcpClient client);

    void didDisconnect(TcpClient client);
}