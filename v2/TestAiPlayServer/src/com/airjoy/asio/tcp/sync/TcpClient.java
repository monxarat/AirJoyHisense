
package com.airjoy.asio.tcp.sync;

import android.util.Log;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class TcpClient {

    private static final String TAG = TcpClient.class.getSimpleName();
    private static final int SLEEP_INTERVAL = 100; // milliseconds;
    private static final int REQUEST_BUFFER = 1024 * 4;

    private TcpClientListener mListener = null;
    private SocketChannel mChannel = null;
    private String mIp = null;
    private int mPort = 0;
    private int mTimeout = 0;
    private Thread mThread = null;

    public TcpClient(TcpClientListener listener) {
        mListener = listener;
    }

    public void asyncConnect(String ip, int port, int timeout) {
        if (mChannel != null) {
            this.disconnect();
            mChannel = null;
        }

        try {
            mChannel = SocketChannel.open();
            mChannel.configureBlocking(false);
        } catch (IOException e) {
            e.printStackTrace();
        }

        mIp = ip;
        mPort = port;
        mTimeout = timeout;

        mThread = new Thread(new AsyncConnect());
        mThread.start();
    }

    private class AsyncConnect implements Runnable {

        @Override
        public void run() {
            try {
                mChannel.connect(new InetSocketAddress(mIp, mPort));

                int count = 0;
                while (!mChannel.finishConnect()) {
                    count += SLEEP_INTERVAL;
                    if (count > mTimeout) {
                        Log.i(TAG, String.format("connect %s:%d failed", mIp, mPort));

                        mListener.didConnectedFailed(TcpClient.this);
                        return;
                    }
                    else
                    {
                        Log.i(TAG, String.format("waiting for connection establish (%s:%d)", mIp,
                                mPort));
                    }

                    Thread.sleep(SLEEP_INTERVAL);
                }

                mChannel.configureBlocking(true);

                Log.i(TAG, String.format("connect %s:%d OK", mIp, mPort));

                mListener.didConnected(TcpClient.this);
            } catch (IOException e) {
                e.printStackTrace();
                mListener.didConnectedFailed(TcpClient.this);
            } catch (IllegalArgumentException e) {
                e.printStackTrace();
                mListener.didConnectedFailed(TcpClient.this);
            } catch (InterruptedException e) {
                e.printStackTrace();
                mListener.didConnectedFailed(TcpClient.this);
            }
        }
    }

    public boolean connect(String ip, int port, int millisecond) {
        Log.i(TAG, String.format("connect: %s:%d timeout:%d", ip, port, millisecond));

        try {
            mChannel = SocketChannel.open();
            mChannel.configureBlocking(false);
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        mIp = ip;
        mPort = port;

        try {
            mChannel.connect(new InetSocketAddress(mIp, mPort));

            int count = 0;
            while (!mChannel.finishConnect()) {
                count += SLEEP_INTERVAL;
                if (count > millisecond) {
                    Log.i(TAG, String.format("connect: %s:%d failed", ip, port));
                    mChannel.configureBlocking(true);
                    return false;
                }

                Thread.sleep(SLEEP_INTERVAL);
            }

            mChannel.configureBlocking(true);

        } catch (IOException e) {
            e.printStackTrace();
            return false;
        } catch (InterruptedException e) {
            e.printStackTrace();
            return false;
        } catch (IllegalArgumentException e) {
            e.printStackTrace();
            return false;
        }

        Log.i(TAG, String.format("connect: %s:%d OK", ip, port));

        return true;
    }

    public boolean disconnect() {
        if (mChannel.isConnected())
        {
            try {
                mChannel.close();
            } catch (IOException e) {
                e.printStackTrace();
            }

            mChannel = null;
            return true;
        }

        return false;
    }

    public boolean isConnected() {
        if (mChannel == null)
            return false;

        return mChannel.isConnected();
    }

    public String selfIp() {
        if (mChannel == null)
            return null;

        if (!mChannel.isConnected())
            return null;

        return mChannel.socket().getLocalAddress().getHostAddress();
    }

    public String peerIp() {
        if (mChannel == null)
            return null;

        if (!mChannel.isConnected())
            return null;

        return mIp;
    }

    public int peerPort() {
        if (mChannel == null)
            return 0;

        if (!mChannel.isConnected())
            return 0;

        return mPort;
    }

    public boolean send(byte[] data) {
        if (mChannel == null)
            return false;

        if (!mChannel.isConnected())
            return false;

        try {
            ByteBuffer bufferSend = ByteBuffer.wrap(data);
            bufferSend.clear();
            int writeSize = mChannel.write(bufferSend);
            if (writeSize <= 0) {
                return false;
            }

        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }

        return true;
    }

    public byte[] recv(int timeout) {
        if (mChannel == null)
            return null;

        if (!mChannel.isConnected())
            return null;

        try {
            ByteBuffer buf = ByteBuffer.allocateDirect(REQUEST_BUFFER);
            int size = mChannel.read(buf);

            if (size <= 0)
                return null;

            byte[] data = new byte[size];
            buf.flip();
            buf.get(data, 0, size);

            return data;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }
}
