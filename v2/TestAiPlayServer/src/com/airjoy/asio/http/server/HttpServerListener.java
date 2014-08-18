
package com.airjoy.asio.http.server;

import com.airjoy.util.HttpMessage;

public interface HttpServerListener {

    void didStarted(HttpServer server);

    void didStopped(HttpServer server);

    void didConnected(HttpServer server, HttpConnection conn);

    void didDisconnected(HttpServer server, HttpConnection conn);

    void didReceive(HttpServer server, HttpConnection conn, HttpMessage message);
}
