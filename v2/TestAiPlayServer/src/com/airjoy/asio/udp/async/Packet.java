package com.airjoy.asio.udp.async;

public class Packet {

    enum Type {
        Unknown,
        Exit,
        Normal,
    };

    public Type type = Type.Unknown;
    public byte[] data = null;
    public String remoteIp = null;
    public int remotePort = 0;
}
