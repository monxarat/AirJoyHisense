
package com.airjoy.asio.tcp.client;

public class TcpPacket {

    enum Type {
        Unknown,
        Exit,
        Normal,
    };

    public Type type = Type.Unknown;
    public byte[] data = null;
}
