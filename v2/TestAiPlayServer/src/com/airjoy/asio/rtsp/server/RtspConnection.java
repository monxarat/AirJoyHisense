
package com.airjoy.asio.rtsp.server;

import java.net.InetSocketAddress;
import com.airjoy.util.RtspMessage;

import java.io.IOException;
import java.net.SocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class RtspConnection {

    private static final int BUFFER_LEN = 1024 * 10;
    private SocketChannel mChannel = null;
    private Receiver mReceiver = null;
    private RtspConnectionListener mListener = null;
    private String mRemoteIp = null;
    private int mRemotePort = 0;
    private int mId = 0;

    public RtspConnection(SocketChannel channel, RtspConnectionListener listener) {
        mChannel = channel;
        mListener = listener;
        mReceiver = new Receiver(channel);

        mRemoteIp = mChannel.socket().getInetAddress().getHostAddress();
        mRemotePort = mChannel.socket().getPort();
    }

    public void close() {
        mReceiver.stop();
        mReceiver = null;
    }

    public void setId(int id) {
        mId = id;
    }

    public int getId() {
        return mId;
    }

    public String getPeerIp() {
        return mRemoteIp;
    }

    public int getPeerPort() {
        return mRemotePort;
    }
    
    public byte[] getLocalIpBytes() {
        SocketAddress localAddress = mChannel.socket().getLocalSocketAddress();
        byte[] ip = ((InetSocketAddress) localAddress).getAddress().getAddress();
        return ip;
    }
    
    public int send(RtspMessage message) {
        ByteBuffer bufferSend = ByteBuffer.wrap(message.toBytes());
        bufferSend.clear();

        int ret = 0;
        try {
            ret = mChannel.write(bufferSend);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return ret;
    }

    private class Receiver implements Runnable {
        private Thread mThread = null;
        private SocketChannel mChannel = null;

        public Receiver(SocketChannel channel) {
            mChannel = channel;
            mThread = new Thread(this);
            mThread.start();
        }

        public void stop() {
            try {
                mChannel.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            RtspMessage message = new RtspMessage();

            try {

                ByteBuffer buffer = ByteBuffer.allocateDirect(BUFFER_LEN);
                int bufferSize = 0;

                while (true) {
                    int readSize = mChannel.read(buffer);
                    if (readSize <= 0) {
                        break;
                    }

                    bufferSize += readSize;

                    ByteBuffer bufferRead = buffer.duplicate();
                    bufferRead.flip();

                    byte[] data = new byte[bufferSize];
                    bufferRead.get(data, 0, bufferSize);

                    if (message.loadBytes(data, data.length) == RtspMessage.LoadStatus.Done) {
                        mListener.didReceive(RtspConnection.this, message);
                    }

                    message = new RtspMessage();
                }
            } catch (IOException e) {
            }

            mListener.didClose(RtspConnection.this);
        }
    }
}
