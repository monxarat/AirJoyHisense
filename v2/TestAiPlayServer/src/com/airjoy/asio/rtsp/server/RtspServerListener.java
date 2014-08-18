
package com.airjoy.asio.rtsp.server;

import com.airjoy.util.RtspMessage;

public interface RtspServerListener {

    void didStarted(RtspServer server);

    void didStopped(RtspServer server);

    void didConnected(RtspServer server, RtspConnection conn);

    void didDisconnected(RtspServer server, RtspConnection conn);

    void didReceive(RtspServer server, RtspConnection conn, RtspMessage message);
}
