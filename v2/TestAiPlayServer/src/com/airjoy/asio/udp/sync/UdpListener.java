package com.airjoy.asio.udp.sync;

public interface UdpListener {

    void didRecvBytes(Udp udp, byte[] data, int len, String fromIp, int fromPort);
}