
package com.airjoy.airtunes.server;

import android.util.Base64;
import android.util.Log;

import com.airjoy.asio.rtsp.server.RtspConnection;
import com.airjoy.asio.rtsp.server.RtspServer;
import com.airjoy.asio.rtsp.server.RtspServerListener;
import com.airjoy.util.RtspMessage;

import java.io.ByteArrayOutputStream;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class AirTunesServer implements RtspServerListener, AirSpeakerListener {

    private static final AirTunesServer mSingleInstance = new AirTunesServer();
    private static final String TAG = AirTunesServer.class.getSimpleName();

    private static final String SERVER = "AirTunes/150.33";

    private AirTunesServerListener mListener = null;
    private RtspServer mServer = null;
    private byte[] mMacAddress = null;
    private boolean mStart = false;
    private RtspConnection mCurrentConn = null;
    private String mAudioFmt = null;
    private int[] mFmtp = null;
    private byte[] mAesiv = null;
    private byte[] mAeskey = null;

    private enum AudioType {
        AirTunes,
        AirPlayMirroring,
    }

    private AudioType mAudioType = AudioType.AirTunes;

    private AirTunesServer() {
    }

    public static AirTunesServer getInstance() {
        return mSingleInstance;
    }

    public void start(byte deviceId[], int port, AirTunesServerListener listener) {
        if (mStart)
            return;

        mListener = listener;
        
        mMacAddress = deviceId;
        mStart = true;
        mServer = new RtspServer(this, port);
        mServer.start();

        AirSpeaker.getInstance().start(this);
    }

    public void stop() {
        if (!mStart)
            return;

        mServer.stop();
        mStart = false;
    }

    public int getPort() {
        if (!mStart)
            return 0;

        return mServer.getListenPort();
    }

    @Override
    public void didStarted(RtspServer server) {
        Log.d(TAG, "didStarted");
    }

    @Override
    public void didStopped(RtspServer server) {
        Log.d(TAG, "didStopped");
    }

    @Override
    public void didConnected(RtspServer server, RtspConnection conn) {
        Log.d(TAG, "didConnected");
    }

    @Override
    public void didDisconnected(RtspServer server, RtspConnection conn) {
        Log.d(TAG, "didDisconnected");

        if (mCurrentConn != conn) {
            if (mCurrentConn != null) {
                mCurrentConn.close();
                mCurrentConn = null;
            }
        }
    }

    @Override
    public void didReceive(RtspServer server, RtspConnection conn, RtspMessage msg) {
        if (msg.getType() != RtspMessage.Type.Request)
            return;

        if (msg.getMethod().equalsIgnoreCase("POST")) {
            onPost(conn, msg);
        } else if (msg.getMethod().equalsIgnoreCase("OPTIONS")) {
            onOptions(conn, msg);
        } else if (msg.getMethod().equalsIgnoreCase("ANNOUNCE")) {
            onAnnounce(conn, msg);
        } else if (msg.getMethod().equalsIgnoreCase("SETUP")) {
            onSetup(conn, msg);
        } else if (msg.getMethod().equalsIgnoreCase("RECORD")) {
            onRecord(conn, msg);
        } else if (msg.getMethod().equalsIgnoreCase("FLUSH")) {
            onFlush(conn, msg);
        } else if (msg.getMethod().equalsIgnoreCase("TEARDOWN")) {
            onTeardown(conn, msg);
        } else if (msg.getMethod().equalsIgnoreCase("GET_PARAMETER")) {
            onGetParameter(conn, msg);
        } else if (msg.getMethod().equalsIgnoreCase("SET_PARAMETER")) {
            onSetParameter(conn, msg);
        } else {

        }
    }

    private boolean onPost(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onPost: %d", conn.getId()));

        if (msg.getUri().equalsIgnoreCase("/fp-setup")) {
            onPostFpSetup(conn, msg);
        }
        else if (msg.getUri().equalsIgnoreCase("/auth-setup")) {
        }

        return true;
    }

    private boolean onPostFpSetup(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onPostFpSetup: %d", conn.getId()));

        String seq = msg.getValueByName("CSeq");
        byte[] content = msg.getContent();
        
        if (content.length > 6 && content[6] == 1) {
            int [] fply = {0x46, 0x50, 0x4c, 0x59, 0x03, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x82, 0x02};
            
            ByteArrayOutputStream baos = new ByteArrayOutputStream(142);
            for(int val:fply) {
                baos.write(val);
            }
            for(int i = 0; i < 0x80; ++ i) {
                baos.write((int)(Math.random() * Integer.MAX_VALUE));
            }
            
            byte[] data = baos.toByteArray();
            
            RtspMessage resp = new RtspMessage();
            resp.setType(RtspMessage.Type.Response);
            resp.setVersion(1, 0);
            resp.setResponse(200, "OK");
            resp.addHeaders("Server", SERVER);
            resp.addHeaders("CSeq", seq);
            resp.addHeaders("Content-Type", "application/octet-stream");
            resp.addHeaders("Content-Length", "142");
            resp.setContent(data);
            conn.send(resp);
        }
        else {
            int [] fply = {0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x14};
            ByteArrayOutputStream baos = new ByteArrayOutputStream(32);
            for(int val:fply) {
                baos.write(val);
            }
            
            if(content.length > 20){
                baos.write(content, content.length - 20, 20);
            }
            
            byte[] data = baos.toByteArray();

            RtspMessage resp = new RtspMessage();
            resp.setType(RtspMessage.Type.Response);
            resp.setVersion(1, 0);
            resp.setResponse(200, "OK");
            resp.addHeaders("Server", SERVER);
            resp.addHeaders("CSeq", seq);
            resp.addHeaders("Content-Type", "application/octet-stream");
            resp.addHeaders("Content-Length", "32");
            resp.setContent(data);
            conn.send(resp);
        }
        
        return true;
    }
    
    private boolean onPostFpSetup_0(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onPostFpSetup: %d", conn.getId()));

        // support ios7, 2014.6.8

        // 2 1 1 -> 4 : 02 00 02 bb
        int fply_1[] = {
                0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00,
                0x02, 0xbb
        };

        // 2 1 2 -> 130 : 02 02 xxx
        int fply_2[] = {
                0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x82,
                0x02, 0x02, 0x2f, 0x7b, 0x69, 0xe6, 0xb2, 0x7e, 0xbb, 0xf0, 0x68, 0x5f, 0x98, 0x54,
                0x7f, 0x37,
                0xce, 0xcf, 0x87, 0x06, 0x99, 0x6e, 0x7e, 0x6b, 0x0f, 0xb2, 0xfa, 0x71, 0x20, 0x53,
                0xe3, 0x94,
                0x83, 0xda, 0x22, 0xc7, 0x83, 0xa0, 0x72, 0x40, 0x4d, 0xdd, 0x41, 0xaa, 0x3d, 0x4c,
                0x6e, 0x30,
                0x22, 0x55, 0xaa, 0xa2, 0xda, 0x1e, 0xb4, 0x77, 0x83, 0x8c, 0x79, 0xd5, 0x65, 0x17,
                0xc3, 0xfa,
                0x01, 0x54, 0x33, 0x9e, 0xe3, 0x82, 0x9f, 0x30, 0xf0, 0xa4, 0x8f, 0x76, 0xdf, 0x77,
                0x11, 0x7e,
                0x56, 0x9e, 0xf3, 0x95, 0xe8, 0xe2, 0x13, 0xb3, 0x1e, 0xb6, 0x70, 0xec, 0x5a, 0x8a,
                0xf2, 0x6a,
                0xfc, 0xbc, 0x89, 0x31, 0xe6, 0x7e, 0xe8, 0xb9, 0xc5, 0xf2, 0xc7, 0x1d, 0x78, 0xf3,
                0xef, 0x8d,
                0x61, 0xf7, 0x3b, 0xcc, 0x17, 0xc3, 0x40, 0x23, 0x52, 0x4a, 0x8b, 0x9c, 0xb1, 0x75,
                0x05, 0x66,
                0xe6, 0xb3
        };

        // 2 1 3 -> 152
        // 4 : 02 8f 1a 9c
        // 128 : xxx
        // 20 : 5b ed 04 ed c3 cd 5f e6 a8 28 90 3b 42 58 15 cb 74 7d ee 85
        int fply_3[] = {
                0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00, 0x98, 0x02, 0x8f,
                0x1a, 0x9c, 0x6e, 0x73, 0xd2, 0xfa, 0x62, 0xb2, 0xb2, 0x07, 0x6f, 0x52, 0x5f, 0xe5,
                0x72, 0xa5,
                0xac, 0x4d, 0x19, 0xb4, 0x7c, 0xd8, 0x07, 0x1e, 0xdb, 0xbc, 0x98, 0xae, 0x7e, 0x4b,
                0xb4, 0xb7,
                0x2a, 0x7b, 0x5e, 0x2b, 0x8a, 0xde, 0x94, 0x4b, 0x1d, 0x59, 0xdf, 0x46, 0x45, 0xa3,
                0xeb, 0xe2,
                0x6d, 0xa2, 0x83, 0xf5, 0x06, 0x53, 0x8f, 0x76, 0xe7, 0xd3, 0x68, 0x3c, 0xeb, 0x1f,
                0x80, 0x0e,
                0x68, 0x9e, 0x27, 0xfc, 0x47, 0xbe, 0x3d, 0x8f, 0x73, 0xaf, 0xa1, 0x64, 0x39, 0xf7,
                0xa8, 0xf7,
                0xc2, 0xc8, 0xb0, 0x20, 0x0c, 0x85, 0xd6, 0xae, 0xb7, 0xb2, 0xd4, 0x25, 0x96, 0x77,
                0x91, 0xf8,
                0x83, 0x68, 0x10, 0xa1, 0xa9, 0x15, 0x4a, 0xa3, 0x37, 0x8c, 0xb7, 0xb9, 0x89, 0xbf,
                0x86, 0x6e,
                0xfb, 0x95, 0x41, 0xff, 0x03, 0x57, 0x61, 0x05, 0x00, 0x73, 0xcc, 0x06, 0x7e, 0x4f,
                0xc7, 0x96,
                0xae, 0xba, 0x5b, 0xed, 0x04, 0xed, 0xc3, 0xcd, 0x5f, 0xe6, 0xa8, 0x28, 0x90, 0x3b,
                0x42, 0x58,
                0x15, 0xcb, 0x74, 0x7d, 0xee, 0x85
        };

        // 2 1 4 -> 20 : 5b ed 04 ed c3 cd 5f e6 a8 28 90 3b 42 58 15 cb 74 7d
        // ee 85
        int fply_4[] = {
                0x46, 0x50, 0x4c, 0x59, 0x02, 0x01, 0x04, 0x00, 0x00, 0x00, 0x00, 0x14, 0x5b,
                0xed, 0x04, 0xed, 0xc3, 0xcd, 0x5f, 0xe6, 0xa8, 0x28, 0x90, 0x3b, 0x42, 0x58, 0x15,
                0xcb, 0x74,
                0x7d, 0xee, 0x85
        };

        byte fply_header[] = new byte[12];
        int len = fply_header.length;
        System.arraycopy(msg.getContent(), 0, fply_header, 0, len);

        int payload_len = msg.getContent().length - len;
        byte payload[] = new byte[payload_len];
        System.arraycopy(msg.getContent(), len, payload, 0, payload_len);

        String seq = msg.getValueByName("CSeq");

        if (fply_header[6] == 1) {
            byte[] content = msg.getContent();
            
            // System.arraycopy(content, 14, fply_2, 13, 1);
            fply_2[13] = content[14];

            byte data[] = new byte[fply_2.length];

            //System.arraycopy(fply_2, 0, data, 0, fply_2.length);
            for (int i = 0; i < fply_2.length; ++i) {
                data[i] = (byte)fply_2[i];
            }

            RtspMessage resp = new RtspMessage();
            resp.setType(RtspMessage.Type.Response);
            resp.setVersion(1, 0);
            resp.setResponse(200, "OK");
            resp.addHeaders("Server", SERVER);
            resp.addHeaders("CSeq", seq);
            resp.addHeaders("Content-Type", "application/octet-stream");
            resp.addHeaders("Content-Length", data.length);
            resp.setContent(data);
            conn.send(resp);

            return true;
        }
        else if (fply_header[6] == 3) {
            int data_len = 12 + 20;
            byte data[] = new byte[data_len];
            
//            System.arraycopy(fply_4, 0, data, 0, 12);
            for (int i = 0; i < 12; ++i) {
                data[i] = (byte)fply_4[i];
            }
            
//            System.arraycopy(payload, payload.length - 20, data, 12, 20);
            for (int i = 0; i < 20; ++i) {
                data[12 + i] = (byte)payload[payload.length - 20 + i];
            }

            RtspMessage resp = new RtspMessage();
            resp.setType(RtspMessage.Type.Response);
            resp.setVersion(1, 0);
            resp.setResponse(200, "OK");
            resp.addHeaders("Server", SERVER);
            resp.addHeaders("CSeq", seq);
            resp.addHeaders("Content-Type", "application/octet-stream");
            resp.addHeaders("Content-Length", data.length);
            resp.setContent(data);
            conn.send(resp);

            return true;
        }

        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);
        resp.addHeaders("Content-Length", 0);
        conn.send(resp);

        return true;
    }

    private boolean onOptions(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onOptions: %d", conn.getId()));

        if (mCurrentConn != conn) {
            if (mCurrentConn != null)
                mCurrentConn.close();

            mCurrentConn = conn;
        }

        String seq = msg.getValueByName("CSeq");
        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Public",
                "ANNOUNCE, SETUP, RECORD, PAUSE, FLUSH, TEARDOWN, OPTIONS, GET_PARAMETER, SET_PARAMETER, POST, GET");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);

        String challenge = msg.getValueByName("Apple-Challenge");
        if (challenge != null) {
            byte ip[] = conn.getLocalIpBytes();
            String response = AppleKey.getInstance().getResponse(ip, mMacAddress, challenge);
            resp.addHeaders("Apple-Response", response);
        }

        conn.send(resp);

        return true;
    }

    private boolean onAnnounce(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onAnnounce: %d", conn.getId()));

        String seq = msg.getValueByName("CSeq");
        String content = new String(msg.getContent());

        // 暂时不处理AirPlay Mirroring音频
        Pattern p = Pattern.compile("^m=(video) ([^ ]+) ([^ ]+) ([^ ]+)", Pattern.MULTILINE);
        Matcher m = p.matcher(content);
        if (m.find()) {
            mAudioType = AudioType.AirPlayMirroring;

            RtspMessage resp = new RtspMessage();
            resp.setType(RtspMessage.Type.Response);
            resp.setVersion(1, 0);
            resp.setResponse(200, "OK");
            resp.addHeaders("Server", SERVER);
            resp.addHeaders("CSeq", seq);
            conn.send(resp);

            return true;
        }
        else {
            mAudioType = AudioType.AirTunes;
        }

        // 目前发现4种rtpmap:
        // a=rtpmap:96 AppleLossless
        // a=rtpmap:96 mpeg4-generic/44100/2
        // a=rtpmap:97 H264
        // a=rtpmap:96 L/16/44100/2
        p = Pattern.compile("^a=(rtpmap):(\\d+) (.+)", Pattern.MULTILINE);
        m = p.matcher(content);
        if (m.find()) {
            mAudioFmt = m.group(4);
        }

        p = Pattern.compile("^a=([^:]+):(.+)", Pattern.MULTILINE);
        m = p.matcher(content);
        while (m.find()) {
            if (m.group(1).contentEquals("fmtp")) {
                String[] temp = m.group(2).split(" ");
                mFmtp = new int[temp.length];
                for (int i = 0; i < temp.length; i++) {
                    mFmtp[i] = Integer.valueOf(temp[i]);
                }
            }
            else if (m.group(1).contentEquals("rsaaeskey")) {
                mAeskey = AppleKey.getInstance().decryptRSA(
                        Base64.decode(m.group(2), Base64.DEFAULT));
            }
            else if (m.group(1).contentEquals("aesiv")) {
                mAesiv = Base64.decode(m.group(2), Base64.DEFAULT);
            }
        }

        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);
        conn.send(resp);

        return true;
    }

    private boolean onSetup(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onSetup: %d", conn.getId()));

        String seq = msg.getValueByName("CSeq");
        String transport = msg.getValueByName("Transport");

        int controlPort = 0;
        int timingPort = 0;

        // 暂时不处理AirPlay Mirroring音视频消息
        if (mAudioType == AudioType.AirPlayMirroring) {
            RtspMessage resp = new RtspMessage();
            resp.setType(RtspMessage.Type.Response);
            resp.setVersion(1, 0);
            resp.setResponse(200, "OK");
            resp.addHeaders("Server", SERVER);
            resp.addHeaders("CSeq", seq);
            conn.send(resp);

            return true;
        }

        if (transport == null) {
            RtspMessage resp = new RtspMessage();
            resp.setType(RtspMessage.Type.Response);
            resp.setVersion(1, 0);
            resp.setResponse(404, "Not Found");
            resp.addHeaders("Server", SERVER);
            resp.addHeaders("CSeq", seq);
            conn.send(resp);

            return true;
        }

        // Control port
        Pattern p = Pattern.compile(";control_port=(\\d+)");
        Matcher m = p.matcher(transport);
        if (m.find()) {
            controlPort = Integer.valueOf(m.group(1));
        }

        // Timing port
        p = Pattern.compile(";timing_port=(\\d+)");
        m = p.matcher(transport);
        if (m.find()) {
            timingPort = Integer.valueOf(m.group(1));
        }

        if (controlPort == 0 || timingPort == 0) {
            RtspMessage resp = new RtspMessage();
            resp.setType(RtspMessage.Type.Response);
            resp.setVersion(1, 0);
            resp.setResponse(404, "Not Found");
            resp.addHeaders("Server", SERVER);
            resp.addHeaders("CSeq", seq);
            conn.send(resp);

            return true;
        }

        AirSpeaker.getInstance().set(conn.getPeerIp(), controlPort, timingPort);

        String value = String.format(
                "RTP/AVP/UDP;unicast;mode=record;server_port=%d;control_port=%d;timing_port=%d",
                AirSpeaker.getInstance().getDataPort(),
                AirSpeaker.getInstance().getCtrlPort(),
                AirSpeaker.getInstance().getTimingPort());

        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Transport", value);
        resp.addHeaders("Session", "DEADBEEF");
        resp.addHeaders("Audio-Jack-Status", "connected");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);
        conn.send(resp);

        return true;
    }

    private boolean onRecord(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onRecord: %d", conn.getId()));

        /*
            RTP-Info: seq=20857;rtptime=1146549156
            seq 16-bit  initial RTP sequence number
            rtptime 32-bit  initial RTP timestamp
         */
        String seq = msg.getValueByName("CSeq");

        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Audio-Latency", "2205");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);
        conn.send(resp);

        return true;
    }

    private boolean onFlush(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onFlush: %d", conn.getId()));

        String seq = msg.getValueByName("CSeq");

        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);
        conn.send(resp);
        
        return true;
    }

    private boolean onTeardown(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onTeardown: %d", conn.getId()));

        String seq = msg.getValueByName("CSeq");

        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);
        conn.send(resp);
        
        return true;
    }

    private boolean onGetParameter(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onGetParameter: %d", conn.getId()));

        String seq = msg.getValueByName("CSeq");

        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);
        conn.send(resp);
        
        return true;
    }

    private boolean onSetParameter(RtspConnection conn, RtspMessage msg) {
        Log.d(TAG, String.format("onSetParameter: %d", conn.getId()));

        String seq = msg.getValueByName("CSeq");

        RtspMessage resp = new RtspMessage();
        resp.setType(RtspMessage.Type.Response);
        resp.setVersion(1, 0);
        resp.setResponse(200, "OK");
        resp.addHeaders("Server", SERVER);
        resp.addHeaders("CSeq", seq);
        conn.send(resp);
        
        return true;
    }
}