
package com.airjoy.asio.tcp.client;

public interface TcpReceiverListener {

    void didDisconnect(TcpReceiver receiver);

    void didRecvBytes(TcpReceiver receiver, byte[] data);
}
