
package com.airjoy.airtunes.server;

import com.airjoy.asio.udp.sync.Udp;
import com.airjoy.asio.udp.sync.UdpListener;

public class AirSpeaker {

    private Udp mUdpData = null;
    private Udp mUdpCtrl = null;
    private Udp mUdpTiming = null;

    private static final AirSpeaker mSingleInstance = new AirSpeaker();
    private static final String TAG = AirSpeaker.class.getSimpleName();

    private AirSpeakerListener mListener = null;
    private boolean mStart = false;
    private String mClientIp = null;
    private int mClientCtrlPort = 0;
    private int mClientTimingPort = 0;

    private AirSpeaker() {
        mUdpData = new Udp(new DataListener());
        mUdpCtrl = new Udp(new CtrlListener());
        mUdpTiming = new Udp(new TimingListener());
    }

    public static AirSpeaker getInstance() {
        return mSingleInstance;
    }

    public void start(AirSpeakerListener listener) {
        if (mStart)
            return;

        mListener = listener;
        mStart = true;

        mUdpData.start();
        mUdpCtrl.start();
        mUdpTiming.start();
    }

    public void stop() {
        if (!mStart)
            return;

        mUdpData.stop();
        mUdpCtrl.stop();
        mUdpTiming.stop();

        mStart = false;
    }

    public void set(String clientIp, int controlPort, int timingPort) {
        mClientIp = clientIp;
        mClientCtrlPort = controlPort;
        mClientTimingPort = timingPort;
    }

    public int getDataPort() {
        if (!mStart)
            return 0;

        return mUdpData.getLocalPort();
    }

    public int getCtrlPort() {
        if (!mStart)
            return 0;

        return mUdpCtrl.getLocalPort();
    }

    public int getTimingPort() {
        if (!mStart)
            return 0;

        return mUdpTiming.getLocalPort();
    }

    private class DataListener implements UdpListener {
        @Override
        public void didRecvBytes(Udp udp, byte[] data, int len, String fromIp, int fromPort) {
        }
    }

    private class CtrlListener implements UdpListener {
        @Override
        public void didRecvBytes(Udp udp, byte[] data, int len, String fromIp, int fromPort) {
        }
    }

    private class TimingListener implements UdpListener {
        @Override
        public void didRecvBytes(Udp udp, byte[] data, int len, String fromIp, int fromPort) {
        }
    }
}
