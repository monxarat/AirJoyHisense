
package com.airjoy.asio.http.server;

import android.util.Log;

import com.airjoy.util.HttpMessage;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

public class HttpServer {

    private static final String TAG = HttpServer.class.getSimpleName();

    private int mListenPort = 0;
    private boolean mIsStarted = false;
    private ListenTask mListenTask = null;
    private MyHttpConnectionListener mConnectionListener = new MyHttpConnectionListener();
    private HttpServerListener mListener = null;

    public HttpServer(HttpServerListener listener) {
        mListener = listener;
    }

    public HttpServer(HttpServerListener listener, int listenPort) {
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

                mListener.didStarted(HttpServer.this);

                int id = 10000;
                while (true) {

                    SocketChannel channel = mServerChannel.accept();
                    HttpConnection conn = new HttpConnection(channel, mConnectionListener);
                    conn.setId(id++);
                    mListener.didConnected(HttpServer.this, conn);
                }
            } catch (IOException e) {
            }

            mListener.didStopped(HttpServer.this);
        }
    }

    private class MyHttpConnectionListener implements HttpConnectionListener {

        @Override
        public void didReceive(HttpConnection conn, HttpMessage message) {
            mListener.didReceive(HttpServer.this, conn, message);
        }

        @Override
        public void didClose(HttpConnection conn) {
            mListener.didDisconnected(HttpServer.this, conn);
        }
    }
}
