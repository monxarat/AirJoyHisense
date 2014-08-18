package com.airjoy.asio.udp.async;

public interface UdpListener {

    void didRecvBytes(Udp udp, byte[] data, String fromIp, int fromPort);

    void didSendBytes(Udp udp, String toIp, int toPort, int size);
}