
package com.airjoy.asio.udp.async;

import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.DatagramSocket;
import java.net.UnknownHostException;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class UdpSender implements Runnable {

    private static final String TAG = UdpSender.class.getSimpleName();

    private DatagramSocket mSocket = null;
    private UdpSenderListener mListener = null;
    private Thread mThread = null;
    private BlockingQueue<Packet> mQueue = null;

    public UdpSender(DatagramSocket socket, UdpSenderListener listener) {
        mSocket = socket;
        mListener = listener;
        mQueue = new ArrayBlockingQueue<Packet>(128);
        mThread = new Thread(this);
        mThread.start();
    }

    public void close() {
        Packet packet = new Packet();
        packet.type = Packet.Type.Exit;

        synchronized(this){
            try {
                mQueue.clear();
                mQueue.put(packet);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void push(byte[] data, String toIp, int toPort) {
        Packet packet = new Packet();
        packet.type = Packet.Type.Normal;
        packet.data = data.clone();
        packet.remoteIp = toIp;
        packet.remotePort = toPort;

        synchronized(this){
            try {
                mQueue.put(packet);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void run() {
        Log.v(TAG, "run");

        while (true) {
            // get packet from Q
            Packet packet = null;
            try {
                packet = mQueue.take();
            } catch (InterruptedException e) {
                e.printStackTrace();
                break;
            }

            if (packet.type == Packet.Type.Exit)
                break;

            InetAddress address = null;
            try {
                address = InetAddress.getByName(packet.remoteIp);
            } catch (UnknownHostException e) {
                e.printStackTrace();
            }

            // send packet
            DatagramPacket datagramPacket = new DatagramPacket(packet.data, packet.data.length,
                    address, packet.remotePort);
            try {
                mSocket.send(datagramPacket);
            } catch (IOException e) {
                e.printStackTrace();
            }

            mListener.didSendBytes(this, packet.remoteIp, packet.remotePort, packet.data.length);
        }

        mQueue.clear();
        Log.v(TAG, "stop");
    }
}
