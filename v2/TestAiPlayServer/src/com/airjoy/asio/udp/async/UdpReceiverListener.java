
package com.airjoy.asio.udp.async;

public interface UdpReceiverListener {

    void didRecvBytes(UdpReceiver receiver, byte[] data, String fromIp, int fromPort);
}
