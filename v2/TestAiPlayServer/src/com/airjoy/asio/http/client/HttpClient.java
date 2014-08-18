
package com.airjoy.asio.http.client;

import android.util.Log;

import com.airjoy.util.HttpMessage;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SocketChannel;

public class HttpClient {

    private static final String TAG = HttpClient.class.getSimpleName();
    private static final int SLEEP_INTERVAL = 100; // milliseconds;
    private static final int REQUEST_BUFFER = 1024 * 50;
    private static final int RESPONSE_BUFFER = 1024 * 50;

    private SocketChannel mChannel = null;
    private String mIp = null;
    private int mPort = 0;

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

    public HttpMessage sendRequest(HttpMessage request) throws HttpClientException {
        if (mChannel == null)
            throw new HttpClientException("Channel is NULL");

        if (!mChannel.isConnected())
            throw new HttpClientException("Channel is not connected");

        try {
            // send
            ByteBuffer bufferSend = ByteBuffer.wrap(request.toBytes());
            bufferSend.clear();
            int writeSize = mChannel.write(bufferSend);
            if (writeSize <= 0) {
                throw new HttpClientException("Channel write failed");
            }

            // receive
            HttpMessage response = new HttpMessage();
            ByteBuffer buffer = ByteBuffer.allocateDirect(RESPONSE_BUFFER);
            buffer.clear();
            int bufferSize = 0;
            while (true) {
                int readSize = mChannel.read(buffer);
                if (readSize < 0) {
                    throw new HttpClientException("Channel read failed: -1");
                }
                else if (readSize == 0) {
                    throw new HttpClientException("Channel read failed: 0");
                }

                bufferSize += readSize;

                ByteBuffer bufferRead = buffer.duplicate();
                bufferRead.flip();

                byte[] data = new byte[bufferSize];
                bufferRead.get(data, 0, bufferSize);

                if (response.loadBytes(data, data.length) == HttpMessage.LoadStatus.Done) {
                    break;
                } else {
                    response = new HttpMessage();
                }
            }

            return response;
        } catch (IOException e) {
            e.printStackTrace();
            throw new HttpClientException("sendRequest failed");
        }
    }

    public HttpMessage recv() throws HttpClientException {
        if (mChannel == null)
            return null;

        if (!mChannel.isConnected())
            return null;

        try {

            HttpMessage request = new HttpMessage();
            while (true) {
                ByteBuffer bufferRead = ByteBuffer.allocateDirect(REQUEST_BUFFER);
                int readSize = mChannel.read(bufferRead);

                byte[] data = new byte[readSize];
                bufferRead.get(data, 0, readSize);

                if (request.loadBytes(data, data.length) == HttpMessage.LoadStatus.Done) {
                    break;
                }
            }

            return request;
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public int sendResponse(HttpMessage response) throws HttpClientException {
        if (mChannel == null)
            return 0;

        if (!mChannel.isConnected())
            return 0;

        try {
            ByteBuffer bufferSend = ByteBuffer.wrap(response.toBytes());
            bufferSend.clear();
            int writeSize = mChannel.write(bufferSend);
            return writeSize;
        } catch (IOException e) {
            e.printStackTrace();
            return 0;
        }
    }
}
