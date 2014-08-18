
package com.airjoy.asio.udp.async;

import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class UdpReceiver implements Runnable {

    private static final String TAG = UdpReceiver.class.getSimpleName();
    private static final int UDP_BUF_MAX_LEN = 1024 * 10;

    private DatagramSocket mSocket = null;
    private Thread mThread = null;
    private UdpReceiverListener mListener = null;
    private Receiver mReceiver = null;

    public UdpReceiver(DatagramSocket socket, UdpReceiverListener listener) {
        mSocket = socket;
        mListener = listener;

        mReceiver = new Receiver();

        mThread = new Thread(this);
        mThread.start();
    }

    public void close() {
        // NOOP
    }

    @Override
    public void run() {
        Log.v(TAG, "run");

        byte[] buffer = new byte[UDP_BUF_MAX_LEN];
        while (true) {
            DatagramPacket packet = new DatagramPacket(buffer, buffer.length);

            try {
                mSocket.receive(packet);
            } catch (IOException e) {
                break;
            }

            byte[] data = new byte[packet.getLength()];
            System.arraycopy(packet.getData(), 0, data, 0, packet.getLength());

            mReceiver.push(data, packet.getAddress().getHostAddress(), packet.getPort());
        }

        Log.v(TAG, "stop");

        mReceiver.close();
    }

    public class Receiver implements Runnable {

        private Thread mThread = null;
        private BlockingQueue<Packet> mQueue = null;

        public Receiver() {
            mQueue = new ArrayBlockingQueue<Packet>(128);
            mThread = new Thread(this);
            mThread.start();
        }

        public void close() {
            Packet packet = new Packet();
            packet.type = Packet.Type.Exit;

            try {
                mQueue.clear();
                mQueue.put(packet);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        public void push(byte[] data, String fromIp, int fromPort) {
            Packet packet = new Packet();
            packet.type = Packet.Type.Normal;
            packet.data = data.clone();
            packet.remoteIp = fromIp;
            packet.remotePort = fromPort;

            try {
                mQueue.put(packet);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void run() {
            while (true) {
                Packet packet;
                try {
                    packet = mQueue.take();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                    break;
                }

                if (packet.type == Packet.Type.Exit)
                    break;

                mListener.didRecvBytes(UdpReceiver.this, packet.data, packet.remoteIp,
                        packet.remotePort);
            }

            mQueue.clear();
        }
    }
}
