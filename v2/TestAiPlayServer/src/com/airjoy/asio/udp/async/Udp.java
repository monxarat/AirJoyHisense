
package com.airjoy.asio.udp.async;

import android.util.Log;

import java.io.IOException;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.SocketException;
import java.net.UnknownHostException;

public class Udp implements UdpReceiverListener, UdpSenderListener {

    private static final String TAG = Udp.class.getSimpleName();

    private UdpListener mListener = null;
    private DatagramSocket mSocket = null;
    private InetAddress mGroup = null;
    private int mPort = 0;
    private UdpReceiver mReceiver = null;
    private UdpSender mSender = null;
    private boolean mEnableMulticast = false;

    public Udp(String groupAddress, int port, UdpListener listener) {
        Log.v(TAG, String.format("MulticastUdp: %s:%d", groupAddress, port));

        mListener = listener;
        mPort = port;
        mEnableMulticast = true;

        try {
            mGroup = InetAddress.getByName(groupAddress);
        } catch (UnknownHostException e1) {
            e1.printStackTrace();
        }
    }

    public Udp(UdpListener listener) {
        mListener = listener;
    }

    @Override
    protected void finalize() throws Throwable {
        this.stop();
        super.finalize();
    }

    public void start() {
        if (mSocket != null)
            return;

        Log.v(TAG, "start");

        if (mEnableMulticast) {
            try {
                MulticastSocket socket = new MulticastSocket(mPort);
                socket.joinGroup(mGroup);
                socket.setLoopbackMode(false);
                mSocket = socket;
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }
        }
        else {
            try {
                mSocket = new DatagramSocket();
            } catch (SocketException e) {
                e.printStackTrace();
            }
        }

        mReceiver = new UdpReceiver(mSocket, this);
        mSender = new UdpSender(mSocket, this);
    }

    public void stop() {
        if (mSocket == null)
            return;

        Log.v(TAG, "stop");

        if (mEnableMulticast) {
            try {
                MulticastSocket socket = (MulticastSocket) mSocket;
                socket.leaveGroup(mGroup);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        mSocket.close();
        mSocket = null;

        mSender.close();
        mSender = null;

        mReceiver.close();
        mReceiver = null;
    }

    public boolean send(byte[] data) {
        if (mSocket.isClosed())
            return false;

        if (mSender != null)
            mSender.push(data, mGroup.getHostAddress(), mPort);

        return true;
    }

    public boolean send(byte[] data, String ip, int port) {
        if (mSocket.isClosed())
            return false;

        if (mSender != null)
            mSender.push(data, ip, port);

        return true;
    }

    @Override
    public void didRecvBytes(UdpReceiver receiver, byte[] data, String fromIp, int fromPort) {
        mListener.didRecvBytes(this, data, fromIp, fromPort);
    }

    @Override
    public void didSendBytes(UdpSender sender, String toIp, int toPort, int size) {
        mListener.didSendBytes(this, toIp, toPort, size);
    }
}
