
package com.airjoy.asio.http.server;

import android.util.Log;

import com.airjoy.util.HttpMessage;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class HttpConnection {

    private static final String TAG = HttpConnection.class.getSimpleName();

    private static final int BUFFER_LEN = 1024 * 50;
    private SocketChannel mChannel = null;
    private Receiver mReceiver = null;
    private HttpConnectionListener mListener = null;
    private String mRemoteIp = null;
    private int mRemotePort = 0;
    private int mId = 0;

    public HttpConnection(SocketChannel channel, HttpConnectionListener listener) {
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

    public int send(String msg) {
        ByteBuffer bufferSend = ByteBuffer.wrap(msg.getBytes());
        bufferSend.clear();

        // Log.d("send", msg);

        int ret = 0;
        try {
            ret = mChannel.write(bufferSend);
        } catch (IOException e) {
            e.printStackTrace();
        }

        return ret;
    }

    public int send(HttpMessage message) {
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
            try {
                HttpMessage message = new HttpMessage();

                while (true) {
                    ByteBuffer buffer = ByteBuffer.allocateDirect(BUFFER_LEN);

                    int readSize = mChannel.read(buffer);
                    if (readSize <= 0) {
                        break;
                    }

                    ByteBuffer bufferRead = buffer.duplicate();
                    bufferRead.flip();

                    byte[] data = new byte[readSize];
                    bufferRead.get(data, 0, readSize);

                    if (message.loadBytes(data, data.length) == HttpMessage.LoadStatus.Done) {
                        mListener.didReceive(HttpConnection.this, message);
                        message = new HttpMessage();
                    }
                }
            } catch (IOException e) {
            }

            mListener.didClose(HttpConnection.this);
        }
    }
}
