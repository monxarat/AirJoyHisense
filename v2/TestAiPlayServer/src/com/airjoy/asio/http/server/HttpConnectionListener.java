
package com.airjoy.asio.http.server;

import com.airjoy.util.HttpMessage;

public interface HttpConnectionListener {

    void didReceive(HttpConnection conn, HttpMessage message);

    void didClose(HttpConnection conn);
}
