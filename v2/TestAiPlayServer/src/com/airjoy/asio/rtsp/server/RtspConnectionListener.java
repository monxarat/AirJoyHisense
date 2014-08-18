
package com.airjoy.asio.rtsp.server;

import com.airjoy.util.RtspMessage;

public interface RtspConnectionListener {

    void didReceive(RtspConnection conn, RtspMessage message);

    void didClose(RtspConnection conn);
}
