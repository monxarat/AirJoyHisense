
package com.airjoy.asio.rtsp.server;

import android.util.Log;

import com.airjoy.util.RtspMessage;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

public class RtspServer {

    private static final String TAG = RtspServer.class.getSimpleName();

    private int mListenPort = 0;
    private boolean mIsStarted = false;
    private ListenTask mListenTask = null;
    private MyHttpConnectionListener mConnectionListener = new MyHttpConnectionListener();
    private RtspServerListener mListener = null;

    public RtspServer(RtspServerListener listener) {
        mListener = listener;
    }

    public RtspServer(RtspServerListener listener, int listenPort) {
        mListenPort = listenPort;
        mListener = listener;
    }

    public int getListenPort() {
        return mListenPort;
    }

    public void start() {
        if (mIsStarted)
            return;

        mIsStarted = true;
        mListenTask = new ListenTask(mListenPort);
    }

    public boolean isStarted() {
        return mIsStarted;
    }

    public void stop() {
        if (!mIsStarted)
            return;

        mIsStarted = false;
        mListenTask.stopTask();
        mListenTask = null;
    }

    private class ListenTask implements Runnable {
        private ServerSocketChannel mServerChannel = null;
        private Thread mThread = null;

        public ListenTask(int listenPort) {
            mListenPort = listenPort;
            mThread = new Thread(this);
            mThread.start();
        }

        public void stopTask() {
            
            if (mServerChannel == null)
                return;

            try {
                mServerChannel.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            try {
                mServerChannel = ServerSocketChannel.open();
                InetSocketAddress localAddress = new InetSocketAddress(mListenPort);
                mServerChannel.socket().bind(localAddress);

                mListenPort = mServerChannel.socket().getLocalPort();
                Log.v(TAG, "start listen: " + mListenPort);

                mListener.didStarted(RtspServer.this);

                int id = 10000;
                while (true) {

                    SocketChannel channel = mServerChannel.accept();
                    RtspConnection conn = new RtspConnection(channel, mConnectionListener);
                    conn.setId(id++);
                    mListener.didConnected(RtspServer.this, conn);
                }
            } catch (IOException e) {
            }

            mListener.didStopped(RtspServer.this);
        }
    }

    private class MyHttpConnectionListener implements RtspConnectionListener {

        @Override
        public void didReceive(RtspConnection conn, RtspMessage message) {
            mListener.didReceive(RtspServer.this, conn, message);
        }

        @Override
        public void didClose(RtspConnection conn) {
            mListener.didDisconnected(RtspServer.this, conn);
        }
    }
}
