package com.airjoy.asio.udp.async;

public interface UdpSenderListener {

    void didSendBytes(UdpSender sender, String toIp, int toPort, int size);
}
