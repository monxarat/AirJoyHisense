
package com.airjoy.asio.udp.sync;

import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.SocketException;
import java.net.UnknownHostException;

public class Udp {

    private static final String TAG = Udp.class.getSimpleName();
    private static final int UDP_BUF_MAX_LEN = 1024 * 2;

    private UdpListener mListener = null;
    private DatagramSocket mSocket = null;
    private InetAddress mGroup = null;
    private int mPort = 0;
    private boolean mEnableMulticast = false;
    private Receiver mReceiver = null;

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

        mReceiver = new Receiver(mSocket);
    }

    public void stop() {
        if (mSocket == null)
            return;

        if (mEnableMulticast) {
            try {
                MulticastSocket socket = (MulticastSocket) mSocket;
                socket.leaveGroup(mGroup);
            } catch (SocketException e) {
                e.printStackTrace();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        mSocket.close();
        mReceiver.join();
        mSocket = null;
    }

    public boolean isStarted() {
        if (mSocket == null)
            return false;

        if (mSocket.isClosed())
            return false;

        return true;
    }

    public int getLocalPort() {
        return mSocket.getLocalPort();
    }

    public boolean send(byte[] data) {
        return this.send(data, mGroup, mPort);
    }

    public boolean send(byte[] data, String ip, int port) {
        InetAddress address = null;
        try {
            address = InetAddress.getByName(ip);
        } catch (UnknownHostException e) {
            e.printStackTrace();
            return false;
        }

        return this.send(data, address, port);
    }

    private boolean send(byte[] data, InetAddress address, int port) {
        Sender sender = Sender.create(mSocket, data, address, port);
        return (sender != null) ? true : false;
    }

    private class Receiver implements Runnable {
        private Thread mThread = null;
        DatagramSocket mSocket = null;

        public Receiver(DatagramSocket socket) {
            mSocket = socket;
            mThread = new Thread(this);
            mThread.start();
        }

        public void join() {
            try {
                mThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            byte[] buffer = new byte[UDP_BUF_MAX_LEN];

            while (true) {
                DatagramPacket packet = null;

                try {
                    packet = new DatagramPacket(buffer, buffer.length);
                } catch (Exception e) {
                    break;
                }

                try {
                    mSocket.receive(packet);
                } catch (IOException e) {
                    break;
                }

                mListener.didRecvBytes(Udp.this,
                        packet.getData(),
                        packet.getLength(),
                        packet.getAddress().getHostAddress(),
                        packet.getPort());
            }
        }
    }

    private static class Sender implements Runnable {
        private Thread mThread = null;
        DatagramSocket mSocket = null;
        byte mData[] = null;
        InetAddress mAddress = null;
        int mPort = 0;

        public static Sender create(DatagramSocket socket, byte data[], InetAddress address,
                int port) {
            if (socket == null)
                return null;

            if (socket.isClosed())
                return null;

            if (data == null)
                return null;

            if (address == null)
                return null;

            if (port == 0)
                return null;

            Sender sender = new Sender();

            sender.mSocket = socket;
            sender.mData = data;
            sender.mAddress = address;
            sender.mPort = port;

            sender.mThread = new Thread(sender);
            sender.mThread.start();

            return sender;
        }

        @Override
        public void run() {
            DatagramPacket packet = new DatagramPacket(mData, mData.length, mAddress, mPort);
            try {
                mSocket.send(packet);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
