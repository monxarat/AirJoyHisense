
package com.airjoy.airplay.server;

import android.util.Log;

import com.airjoy.asio.http.server.HttpConnection;
import com.airjoy.asio.http.server.HttpServer;
import com.airjoy.asio.http.server.HttpServerListener;
import com.airjoy.util.HttpMessage;
import com.airjoy.util.KeyValueText;
import com.airjoy.util.plist.NSData;
import com.airjoy.util.plist.NSDictionary;
import com.airjoy.util.plist.NSObject;
import com.airjoy.util.plist.PropertyListParser;

import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.HashMap;
import java.util.Locale;
import java.util.Map;

public class AirPlayServer implements HttpServerListener {

    private static final AirPlayServer mSingleInstance = new AirPlayServer();
    private static final String TAG = AirPlayServer.class.getSimpleName();

    private static final String XmlDeclare = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\"\r\n \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n";
    private AirPlayServerListener mListener = null;
    private HttpServer mServer = null;
    private boolean mStart = false;

    private String mDeviceId = null;
    private int mFeatures = 0x100029ff;
    private String mDeviceModel = "AppleTV3,2";
    private String mProtovers = "1.0";
    private String mVersion = "150.33";

    public enum EventCategory {
        EventUnknown, EventPhoto, EventVideo, EventSlideshow,
    };

    public enum EventAction {
        EventActionNone, EventActionPlaying, EventActionPaused, EventActionLoading, EventActionStopped,
    };

    private AirPlayServer() {
    }

    public static AirPlayServer getInstance() {
        return mSingleInstance;
    }

    public void start(byte deviceId[], int port, AirPlayServerListener listener) {
        if (deviceId.length != 6)
            return;

        if (mStart)
            return;

        mListener = listener;

        mDeviceId = String.format("%02X:%02X:%02X:%02X:%02X:%02X",
                deviceId[0],
                deviceId[1],
                deviceId[2],
                deviceId[3],
                deviceId[4],
                deviceId[5]);

        Log.d(TAG, String.format("start, deviceId: %s", mDeviceId));
        
        mStart = true;
        mServer = new HttpServer(this, port);
        mServer.start();
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

    public void publishEvent(EventCategory type, EventAction action, String clientIp) {
        if (type == EventCategory.EventUnknown)
            return;

        synchronized (mEventSubscribers) {
            for (HttpConnection conn : mEventSubscribers.keySet()) {
                if (conn.getPeerIp() == clientIp) {
                    String sessionId = mEventSubscribers.get(conn);
                    HttpMessage req = createEvent(type, action, sessionId);
                    conn.send(req);
                }
            }
        }
    }

    public void closeConnection(String clientIp) {
        if (clientIp == null)
            return;

        this.closeEventSessionByClientIp(clientIp);

        this.closePushSessionByClientIp(clientIp);
    }

    public void getSlideshowsPicture(String clientIp) {

        synchronized (mSlideshows) {
            for (HttpConnection conn : mSlideshows.keySet()) {
                if (conn.getPeerIp().equalsIgnoreCase(clientIp)) {
                    String sessionId = mSlideshows.get(conn);

                    HttpMessage req = new HttpMessage();
                    req.setType(HttpMessage.Type.Request);
                    req.setVersion(1, 1);
                    req.setMethod("GET");
                    req.setUri("/slideshows/1/assets/1");

                    req.addHeaders("Content-Length", "0");
                    req.addHeaders("Accept", "application/x-apple-binary-plist");
                    req.addHeaders("X-Apple-Session-ID", sessionId);

                    conn.send(req);
                }
            }
        }
    }

    private String mTime = null;

    // (HttpConnection, AppleSessionId)
    private Map<HttpConnection, String> mEventSubscribers = new HashMap<HttpConnection, String>();

    // (HttpConnection, AppleSessionId)
    private Map<HttpConnection, String> mPushSessions = new HashMap<HttpConnection, String>();

    // (HttpConnection, AppleSessionId)
    private Map<HttpConnection, String> mSlideshows = new HashMap<HttpConnection, String>();

    @Override
    public void didStarted(HttpServer server) {
        Log.d(TAG, "didStarted");
    }

    @Override
    public void didStopped(HttpServer server) {
        Log.d(TAG, "didStopped");
    }

    @Override
    public void didConnected(HttpServer server, HttpConnection conn) {
        Log.d(TAG, String.format("didConnected: %d", conn.getId()));
    }

    @Override
    public void didDisconnected(HttpServer server, HttpConnection conn) {
        if (!this.removeEventSubscriber(conn.getId())) {
            if (!this.removePushSession(conn.getId())) {
                this.removeSlideshowsSession(conn.getId());
            }
        }
    }

    @Override
    public void didReceive(HttpServer server, HttpConnection conn,
            HttpMessage msg) {

        if (msg.getType() == HttpMessage.Type.Request) {
            if (msg.getMethod().equalsIgnoreCase("GET")) {
                didGet(conn, msg);
            } else if (msg.getMethod().equalsIgnoreCase("POST")) {
                didPost(conn, msg);
            } else if (msg.getMethod().equalsIgnoreCase("PUT")) {
                didPut(conn, msg);
            } else {
                sendNotImplemented(conn);
            }
        } else if (msg.getType() == HttpMessage.Type.Response) {
            onRecvResponse(conn, msg);
        }
    }

    private boolean addEventSubscriber(HttpConnection conn,
            String appleSessionId) {
        synchronized (mEventSubscribers) {
            String sessionId = mEventSubscribers.get(conn);
            if (sessionId == null) {
                mListener.didSubscribeEvent(conn.getPeerIp());
                mEventSubscribers.put(conn, appleSessionId);
                return true;
            }
        }

        return false;
    }

    private boolean removeEventSubscriber(HttpConnection conn) {
        synchronized (mEventSubscribers) {
            String sessionId = mEventSubscribers.get(conn);
            if (sessionId != null) {
                mListener.didUnsubscribeEvent(conn.getPeerIp());
                mEventSubscribers.remove(conn);
                return true;
            }
        }

        return false;
    }

    private boolean removeEventSubscriber(int connectedId) {
        synchronized (mEventSubscribers) {
            for (HttpConnection conn : mEventSubscribers.keySet()) {
                if (conn.getId() == connectedId) {
                    mEventSubscribers.remove(conn);
                    return true;
                }
            }
        }

        return false;
    }

    private boolean closeEventSessionByClientIp(String clientIp) {
        synchronized (mEventSubscribers) {
            for (HttpConnection conn : mEventSubscribers.keySet()) {
                if (conn.getPeerIp().equalsIgnoreCase(clientIp)) {
                    mEventSubscribers.remove(conn);
                    return true;
                }
            }
        }

        return false;
    }

    private boolean addPushSession(HttpConnection conn, String appleSessionId) {
        synchronized (mPushSessions) {
            String sessionId = mPushSessions.get(conn);
            if (sessionId == null) {
                mPushSessions.put(conn, appleSessionId);
                return true;
            }
        }

        return false;
    }

    private boolean removePushSession(HttpConnection conn) {
        synchronized (mPushSessions) {
            String sessionId = mPushSessions.get(conn);
            if (sessionId != null) {
                mPushSessions.remove(conn);
                return true;
            }
        }

        return false;
    }

    private boolean removePushSession(String applsSessionId) {
        synchronized (mPushSessions) {
            for (HttpConnection conn : mPushSessions.keySet()) {
                String sessionId = mPushSessions.get(conn);
                if (sessionId.equalsIgnoreCase(applsSessionId)) {
                    mPushSessions.remove(conn);
                    return true;
                }
            }
        }

        return false;
    }

    private boolean removePushSession(int connectedId) {
        synchronized (mPushSessions) {
            for (HttpConnection conn : mPushSessions.keySet()) {
                if (conn.getId() == connectedId) {
                    mPushSessions.remove(conn);
                    return true;
                }
            }
        }

        return false;
    }

    private boolean closePushSession(String applsSessionId) {
        synchronized (mPushSessions) {
            for (HttpConnection conn : mPushSessions.keySet()) {
                String sessionId = mPushSessions.get(conn);
                if (sessionId.equalsIgnoreCase(applsSessionId)) {
                    conn.close();
                    return true;
                }
            }
        }

        return false;
    }

    private boolean closePushSessionByClientIp(String clientIp) {
        synchronized (mPushSessions) {
            for (HttpConnection conn : mPushSessions.keySet()) {
                if (conn.getPeerIp().equals(clientIp)) {
                    conn.close();
                    return true;
                }
            }
        }

        return false;
    }

    private boolean addSlideshowsSession(HttpConnection conn,
            String appleSessionId) {
        synchronized (mSlideshows) {
            String sessionId = mSlideshows.get(conn);
            if (sessionId == null) {
                mSlideshows.put(conn, appleSessionId);
                return true;
            }
        }

        return false;
    }

    private boolean removeSlideshowsSession(HttpConnection conn) {
        synchronized (mSlideshows) {
            String sessionId = mSlideshows.get(conn);
            if (sessionId != null) {
                mSlideshows.remove(conn);
                return true;
            }
        }

        return false;
    }

    private boolean removeSlideshowsSession(String applsSessionId) {
        synchronized (mSlideshows) {
            for (HttpConnection conn : mSlideshows.keySet()) {
                String sessionId = mSlideshows.get(conn);
                if (sessionId.equalsIgnoreCase(applsSessionId)) {
                    mSlideshows.remove(conn);
                    return true;
                }
            }
        }

        return false;
    }

    private boolean removeSlideshowsSession(int connectedId) {
        synchronized (mSlideshows) {
            for (HttpConnection conn : mSlideshows.keySet()) {
                if (conn.getId() == connectedId) {
                    mSlideshows.remove(conn);
                    return true;
                }
            }
        }

        return false;
    }

    private boolean isSlideshowsSession(HttpConnection conn) {
        synchronized (mSlideshows) {
            String sessionId = mSlideshows.get(conn);
            if (sessionId != null)
                return true;
        }

        return false;
    }

    private void willAction(HttpMessage msg, String clientIp) {
        return;
    }

    private String getNowTime() {
        // Wed, 27 June 2012 16:47:41 GMT
        SimpleDateFormat fmt = new SimpleDateFormat("EEE, w MMM yyyy hh:mm:ss", Locale.US);

        Date now = new Date();
        String d = String.format("%s GMT", fmt.format(now));

        return d;
    }

    private void sendResponseOk(HttpConnection conn) {
        HttpMessage resp = new HttpMessage();
        resp.setType(HttpMessage.Type.Response);
        resp.setVersion(1, 1);
        resp.setResponse(200, "OK");
        resp.addHeaders("Date", getNowTime());
        resp.addHeaders("Content-Length", 0);
        conn.send(resp);
    }

    private void sendResponseOk(HttpConnection conn, String contentType,
            byte[] content) {
        HttpMessage resp = new HttpMessage();
        resp.setType(HttpMessage.Type.Response);
        resp.setVersion(1, 1);
        resp.setResponse(200, "OK");
        resp.addHeaders("Date", getNowTime());
        resp.addHeaders("Content-Type", contentType);
        resp.addHeaders("Content-Length", content.length);
        resp.setContent(content);
        
        conn.send(resp);
    }

    private void sendResponseSwitchingProtocols(HttpConnection conn) {
        HttpMessage resp = new HttpMessage();
        resp.setType(HttpMessage.Type.Response);
        resp.setVersion(1, 1);
        resp.setResponse(101, "Switching Protocols");
        resp.addHeaders("Date", getNowTime());
        resp.addHeaders("Upgrade", "PTTH/1.0");
        resp.addHeaders("Connection", "Upgrade");
        conn.send(resp);
    }

    private void sendResponseUnauthorized(HttpConnection conn) {
        HttpMessage resp = new HttpMessage();
        resp.setType(HttpMessage.Type.Response);
        resp.setVersion(1, 1);
        resp.setResponse(401, "Unauthorized");
        resp.addHeaders("Date", getNowTime());
        resp.addHeaders("Content-Length", 0);
        conn.send(resp);
    }

    private void sendNotFound(HttpConnection conn) {
        HttpMessage resp = new HttpMessage();
        resp.setType(HttpMessage.Type.Response);
        resp.setVersion(1, 1);
        resp.setResponse(404, "Not Found");
        resp.addHeaders("Date", getNowTime());
        resp.addHeaders("Content-Length", 0);
        conn.send(resp);
    }

    private void sendNotImplemented(HttpConnection conn) {
        HttpMessage resp = new HttpMessage();
        resp.setType(HttpMessage.Type.Response);
        resp.setVersion(1, 1);
        resp.setResponse(501, "Not Implemented");
        resp.addHeaders("Date", getNowTime());
        resp.addHeaders("Content-Length", 0);
        conn.send(resp);
    }

    private void sendResponseMethodNotAllowed(HttpConnection conn) {
        HttpMessage resp = new HttpMessage();
        resp.setType(HttpMessage.Type.Response);
        resp.setVersion(1, 1);
        resp.setResponse(501, "Method Not Allowed");
        resp.addHeaders("Date", getNowTime());
        resp.addHeaders("Content-Length", 0);
        conn.send(resp);
    }

    private boolean didGet(HttpConnection conn, HttpMessage msg) {
        if (msg.getUri().equalsIgnoreCase("/server-info"))
            return didGetServerInfo(conn, msg);

        if (msg.getUri().equalsIgnoreCase("/slideshow-features"))
            return didGetSlideshowFeatures(conn, msg);

        if (msg.getUri().equalsIgnoreCase("/scrub"))
            return didGetScrub(conn, msg);

        if (msg.getUri().equalsIgnoreCase("/playback-info"))
            return didGetPlaybackInfo(conn, msg);

        sendNotImplemented(conn);
        return true;
    }

    private boolean didGetServerInfo(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didGetServerInfo: %d", conn.getId()));
        
        String content = XmlDeclare 
                + "<plist version=\"1.0\">\r\n" 
                + "  <dict>\r\n"
                + "    <key>deviceid</key>\r\n" 
                + "    <string>" + mDeviceId + "</string>\r\n" 
                + "    <key>features</key>\r\n" 
                + "    <integer>" + mFeatures + "</integer>\r\n" 
                + "    <key>model</key>\r\n"
                + "    <string>" + mDeviceModel + "</string>\r\n"
                + "    <key>protovers</key>\r\n" 
                + "    <string>" + mProtovers + "</string>\r\n" 
                + "    <key>srcvers</key>\r\n" 
                + "    <string>" + mVersion + "</string>\r\n" 
                + "  </dict>\r\n" 
                + "</plist>\r\n"
                + "\r\n"; 

//        String content = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
//        		+ "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\r\n"
//        		+ "<plist version=\"1.0\">\r\n"
//        		+ "<dict>\r\n"
//        		+ "\t<key>deviceid</key>\r\n"
//        		+ "\t<string>" + mDeviceId + "</string>\r\n"
//        		+ "\t<key>features</key>\r\n"
//        		+ "\t<integer>" + mFeatures + "</integer>\r\n"
//        		+ "\t<key>model</key>\r\n"
//        		+ "\t<string>" + mDeviceModel + "</string>\r\n"
//        		+ "\t<key>protovers</key>\r\n"
//        		+ "\t<string>" + mProtovers + "</string>\r\n"
//        		+ "\t<key>srcvers</key>\r\n"
//        		+ "\t<string>" + mVersion + "</string>\r\n"
//        		+ "</dict>\r\n"
//        		+ "</plist>\r\n";

        String resp = "HTTP/1.1 200 OK\r\n"
                + "Date: " + getNowTime() + "\r\n"
                + "Content-Type: text/x-apple-plist+xml\r\n"
                + "Content-Length: " + content.length() + "\r\n"
                + "\r\n"
                + content;
        
        conn.send(resp);
        
        return true;
    }

    private boolean didGetSlideshowFeatures(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didGetSlideshowFeatures: %d", conn.getId()));
        
        String xml = XmlDeclare + "<plist version=\"1.0\">\r\n" + "<dict>\r\n"
                + "<key>themes</key>\r\n" + "<array>\r\n" + "<dict>\r\n"
                + "<key>key</key>\r\n" + "<string>Reflections</string>\r\n"
                + "<key>name</key>\r\n" + "<string>Reflections</string>\r\n"
                + "</dict>\r\n" + "</array>\r\n" + "</dict>\r\n"
                + "</plist>\r\n";

        sendResponseOk(conn, "text/x-apple-plist+xml", xml.getBytes());

        return true;
    }

    private boolean didGetScrub(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didGetScrub: %d", conn.getId()));

        AirPlayServerListener.PlaybackTime t = new AirPlayServerListener.PlaybackTime();
        t.playPosition = 0.0f;
        t.duration = 0.0f;

        if (mListener != null)
            mListener.getCurrentPlaybackProgress(t, conn.getPeerIp());

        String content = String.format("duration: %f\r\nposition: %f\r\n",
                t.duration, t.playPosition);
        sendResponseOk(conn, "text/parameters", content.getBytes());

        return true;
    }

    private boolean didGetPlaybackInfo(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didGetPlaybackInfo: %d", conn.getId()));

        AirPlayServerListener.PlaybackInfo info = new AirPlayServerListener.PlaybackInfo();
        info.playbackTime.playPosition = 0.0f;
        info.playbackTime.duration = 0.0f;
        info.rate = 1.0f;
        info.readyToPlay = true;
        info.playbackBufferEmpty = true;
        info.playbackBufferFull = false;
        info.playbackLikelyToKeepUp = true;
        info.loadedTimeRanges.startPosition = 0.0f;
        info.loadedTimeRanges.duration = 0.0f;
        info.seekableTimeRanges.startPosition = 0.0f;
        info.seekableTimeRanges.duration = 0.0f;

        if (mListener != null)
            mListener.getPlaybackInfo(info, conn.getPeerIp());

        String xml = XmlDeclare + "<plist version=\"1.0\">\r\n" + "<dict>\r\n"
                + "<key>duration</key>\r\n" + "<real>"
                + info.playbackTime.duration + "</real>\r\n"
                + "<key>loadedTimeRanges</key>\r\n" + "<array>\r\n"
                + "<dict>\r\n" + "<key>duration</key>\r\n" + "<real>"
                + info.loadedTimeRanges.duration + "</real>\r\n"
                + "<key>start</key>\r\n" + "<real>"
                + info.loadedTimeRanges.startPosition + "</real>\r\n"
                + "</dict>\r\n" + "</array>\r\n"
                + "<key>playbackBufferEmpty</key>\r\n" + "<"
                + (info.playbackBufferEmpty ? "true" : "false") + "/>\r\n"
                + "<key>playbackBufferFull</key>\r\n" + "<"
                + (info.playbackBufferFull ? "true" : "false") + "/>\r\n"
                + "<key>playbackLikelyToKeepUp</key>\r\n" + "<"
                + (info.playbackLikelyToKeepUp ? "true" : "false") + "/>\r\n"
                + "<key>position</key>\r\n" + "<real>"
                + info.playbackTime.playPosition + "</real>\r\n"
                + "<key>rate</key>\r\n" + "<real>" + info.rate + "</real>\r\n"
                + "<key>readyToPlay</key>\r\n" + "<"
                + (info.readyToPlay ? "true" : "false") + "/>\r\n"
                + "<key>seekableTimeRanges</key>\r\n " + "<array>\r\n"
                + "<dict>\r\n" + "<key>duration</key>\r\n" + "<real>"
                + info.seekableTimeRanges.duration + "</real>\r\n"
                + "<key>start</key>\r\n" + "<real>"
                + info.seekableTimeRanges.startPosition + "</real>\r\n"
                + "</dict>\r\n" + "</array>\r\n" + "</dict>\r\n"
                + "</plist>\r\n";

        sendResponseOk(conn, "text/x-apple-plist+xml", xml.getBytes());

        return true;
    }

    private boolean didPost(HttpConnection conn, HttpMessage msg) {
        if (msg.getUri().equalsIgnoreCase("/reverse"))
            return didPostReverse(conn, msg);

        if (msg.getUri().equalsIgnoreCase("/stop"))
            return didPostStop(conn, msg);

        if (msg.getUri().equalsIgnoreCase("/play"))
            return didPostPlay(conn, msg);

        if (msg.getUri().startsWith("/scrub"))
            return didPostScrub(conn, msg);

        if (msg.getUri().startsWith("/rate"))
            return didPostRate(conn, msg);

        if (msg.getUri().startsWith("/getProperty"))
            return didPostGetProperty(conn, msg);

        if (msg.getUri().startsWith("/volume"))
            return didPostVolume(conn, msg);

        sendNotImplemented(conn);

        return true;
    }

    private boolean didPostReverse(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPostReverse: %d", conn.getId()));

        String appleSessionId = msg.getValueByName("X-Apple-Session-ID");
        if (appleSessionId == null)
            appleSessionId = "00000000-0000-0000-0000-000000000000";

        String purpose = msg.getValueByName("x-apple-purpose");
        if (purpose == null) {
            sendNotImplemented(conn);

            return true;
        }

        if (purpose.equalsIgnoreCase("slideshow")) {
            addSlideshowsSession(conn, appleSessionId);
        } else if (purpose.equalsIgnoreCase("event")) {
            addEventSubscriber(conn, appleSessionId);
        }

        this.sendResponseSwitchingProtocols(conn);

        return true;
    }

    private boolean didPostStop(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPostStop: %d", conn.getId()));

        if (mListener != null)
            mListener.didStop(conn.getPeerIp());

        sendResponseOk(conn);

        return true;
    }

    private boolean didPostPlay(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPostPlay: %d", conn.getId()));

        String appleSessionId = msg.getValueByName("X-Apple-Session-ID");
        if (appleSessionId == null)
            appleSessionId = "00000000-0000-0000-0000-000000000000";

        this.addPushSession(conn, appleSessionId);

        String contentType = msg.getValueByName("Content-Type");
        if (contentType == null) {
            sendNotImplemented(conn);
            return true;
        }

        if (contentType.equalsIgnoreCase("text/parameters"))
        {
            KeyValueText txt = new KeyValueText();
            txt.loadBytes(msg.getContent());

            String location = txt.getValue("Content-Location");
            float position = 0.0f;

            String pos = txt.getValue("Start-Position");
            if (pos != null) {
                try {
                    position = Float.valueOf(pos);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }

            if (mListener != null) {
                if (location.endsWith("mp3"))
                    mListener.didStartPlayMusic(location, position, conn.getPeerIp());
                else
                    mListener.didStartPlayVideo(location, position, conn.getPeerIp());
            }
        }
        else if (contentType.equalsIgnoreCase("application/x-apple-binary-plist")) {
            String location = null;
            double rate = 0.0f;
            float position = 0.0f;

            NSObject root = null;
            try {
                root = PropertyListParser.parse(msg.getContent());
            } catch (Exception e) {
                e.printStackTrace();

                sendResponseOk(conn);
                return true;
            }

            if (root != null) {
                NSDictionary dict = (NSDictionary) root;

                NSObject objLocation = dict.objectForKey("Content-Location");
                if (objLocation != null) {
                    location = objLocation.toString();
                }
                else {
                    // for ios6.1
                    NSObject objRate = dict.objectForKey("rate");
                    if (objRate != null) {
                        try {
                            rate = Double.valueOf(objRate.toString());
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }

                    String path = null;
                    NSObject objPath = dict.objectForKey("path");
                    if (objPath != null) {
                        path = objPath.toString();
                    }

                    String host = null;
                    NSObject objHost = dict.objectForKey("host");
                    if (objHost != null) {
                        host = objHost.toString();
                    }

                    location = String.format("http://%s%s", host, path);
                }

                NSObject objPosition = dict.objectForKey("Start-Position");
                if (objPosition != null) {
                    try {
                        position = Float.valueOf(objPosition.toString());
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }

                if (mListener != null) {
                    if (location.endsWith("mp3"))
                        mListener.didStartPlayMusic(location, position, conn.getPeerIp());
                    else
                        mListener.didStartPlayVideo(location, position, conn.getPeerIp());
                }
            }
        }

        sendResponseOk(conn);

        return true;
    }

    private boolean didPostScrub(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPostScrub: %d", conn.getId()));

        float playPositoin = 0.0f;
        String value = msg.getValueInUri("position=");

        try {
            playPositoin = Float.valueOf(value);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (mListener != null)
            mListener
                    .setCurrentPlaybackProgress(playPositoin, conn.getPeerIp());

        sendResponseOk(conn);

        return true;
    }

    private boolean didPostRate(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPostRate: %d", conn.getId()));

        float rate = 0;
        String value = msg.getValueInUri("value=");

        try {
            rate = Float.valueOf(value);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (mListener != null)
            mListener.didSetPlaybackRate(rate, conn.getPeerIp());

        sendResponseOk(conn);

        return true;
    }

    private boolean didPostGetProperty(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPostGetProperty: %d", conn.getId()));

        this.sendNotFound(conn);

        return true;
    }

    private boolean didPostVolume(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPostVolume: %d", conn.getId()));

        float valume = 0;
        String value = msg.getValueInUri("volume=");

        try {
            valume = Float.valueOf(value);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (mListener != null)
            mListener.didSetVolume(valume, conn.getPeerIp());

        sendResponseOk(conn);

        return true;
    }

    private boolean didPut(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPut: %d", conn.getId()));

        if (msg.getUri().equalsIgnoreCase("/photo"))
            return didPutPhoto(conn, msg);

        if (msg.getUri().startsWith("/setProperty"))
            return didPutSetProperty(conn, msg);

        if (msg.getUri().startsWith("/slideshows"))
            return didPutSlideshows(conn, msg);

        sendNotImplemented(conn);
        return true;
    }

    private boolean didPutPhoto(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPutPhoto: %d", conn.getId()));

        String photoId = msg.getValueByName("X-Apple-AssetKey");
        if (photoId == null)
            photoId = "00000000-0000-0000-0000-000000000000";

        String action = msg.getValueByName("X-Apple-AssetAction");

        String appleSessionId = msg.getValueByName("X-Apple-Session-ID");
        if (appleSessionId == null)
            appleSessionId = "00000000-0000-0000-0000-000000000000";

        this.addPushSession(conn, appleSessionId);

        if (mListener != null) {
            if (action == null) {
                mListener.willPutPhoto(photoId, conn.getPeerIp());

                mListener.didPutPhoto(photoId, msg.getContent(),
                        msg.getContent().length, conn.getPeerIp());
            } else if (action == "cacheOnly") {
                mListener.willPutPhotoCacheOnly(photoId, conn.getPeerIp());

                mListener.didPutPhotoCacheOnly(photoId, msg.getContent(),
                        msg.getContent().length, conn.getPeerIp());
            } else if (action == "displayCached") {
                mListener.didDisplayCachedPhoto(photoId, conn.getPeerIp());
            }
        }

        sendResponseOk(conn);

        return true;
    }

    private boolean didPutSetProperty(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPutSetProperty: %d", conn.getId()));

        sendNotFound(conn);

        return true;
    }

    private boolean didPutSlideshows(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("didPutSlideshows: %d", conn.getId()));

        String appleSessionId = msg.getValueByName("X-Apple-Session-ID");
        if (appleSessionId == null)
            appleSessionId = "00000000-0000-0000-0000-000000000000";

        String contentType = msg.getValueByName("Content-Type");
        if (contentType.equalsIgnoreCase("text/x-apple-plist+xml")) {
            int duration = 3;
            String theme = "Classic";
            String state = "playing";

            NSObject root = null;
            try {
                root = PropertyListParser.parse(msg.getContent());
            } catch (Exception e) {
                e.printStackTrace();

                sendResponseOk(conn);
                return true;
            }

            if (root != null) {
                NSDictionary dict = (NSDictionary) root;

                NSObject objSettings = dict.objectForKey("settings");
                if (objSettings != null) {
                    NSDictionary settings = (NSDictionary) objSettings;
                    NSObject objSlideDuration = settings.get("slideDuration");
                    if (objSlideDuration != null) {
                        try {
                            duration = Integer.valueOf(objSlideDuration.toString());
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }

                    NSObject objTheme = settings.get("theme");
                    if (objTheme != null) {
                        theme = objTheme.toString();
                    }
                }

                NSObject objState = dict.objectForKey("state");
                if (objState != null)
                    state = objState.toString();

            } else {
                sendResponseOk(conn);
                return true;
            }

            if (state.equalsIgnoreCase("playing")) {
                if (mListener != null)
                    mListener.didStartSlideshows(duration, conn.getPeerIp());
            }
            else if (state.equalsIgnoreCase("stopped")) {
                if (mListener != null)
                    mListener.didStopSlideshows(conn.getPeerIp());
            }
        }

        sendResponseOk(conn);

        return true;
    }

    private boolean onRecvResponse(HttpConnection conn, HttpMessage msg) {
        Log.d(TAG, String.format("onRecvResponse: %d", conn.getId()));

        if (this.isSlideshowsSession(conn)) {
            if (msg.getStatusCode() == 204) {
                if (mListener != null) {
                    mListener.didGetSlideshowsPictureFailed(conn.getPeerIp());
                    return true;
                }
            }

            String contentType = msg.getValueByName("Content-Type");
            if (contentType == "application/x-apple-binary-plist") {
                byte data[] = null;
                int id = 0;
                String key = null;

                NSObject root = null;
                try {
                    root = PropertyListParser.parse(msg.getContent());
                } catch (Exception e) {
                    e.printStackTrace();
                    return true;
                }

                if (root != null) {
                    NSDictionary dict = (NSDictionary) root;
                    NSObject objData = dict.objectForKey("data");
                    if (objData != null) {
                        NSData d = (NSData) objData;
                        data = d.bytes();
                    }

                    NSObject objInfo = dict.objectForKey("info");
                    if (objInfo != null) {
                        NSDictionary d = (NSDictionary) objInfo;
                        NSObject objId = d.objectForKey("id");
                        if (objId != null) {
                            try {
                                id = Integer.valueOf(objId.toString());
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }

                        NSObject objKey = d.objectForKey("key");
                        if (objKey != null) {
                            key = objKey.toString();
                        }
                    }
                } else {
                    sendResponseOk(conn);
                    return true;
                }

                if (mListener != null)
                    mListener.didGetSlideshowsPicture(id, data, data.length, conn.getPeerIp());
            }
        }

        return true;
    }

    private HttpMessage createEvent(EventCategory type, EventAction action, String sessionId) {
        String stringCategory = null;
        switch (type) {
            case EventPhoto:
                stringCategory = "photo";
                break;

            case EventSlideshow:
                stringCategory = "slideshow";
                break;

            case EventVideo:
                stringCategory = "video";
                break;

            default:
                break;
        }

        if (stringCategory == null)
            return null;

        String stringState = null;
        switch (action) {
            case EventActionLoading:
                stringState = "loading";
                break;

            case EventActionPaused:
                stringState = "paused";
                break;

            case EventActionPlaying:
                stringState = "playing";
                break;

            case EventActionStopped:
                stringState = "stopped";
                break;

            default:
                break;
        }

        if (stringState == null)
            return null;

        String xml = XmlDeclare +
                "<plist version=\"1.0\">\r\n" +
                "<dict>\r\n" +
                "<key>category</key>\r\n" +
                "<string>" + stringCategory + "</string>\r\n" +
                "<key>state</key>\r\n" +
                "<string>" + stringState + "</string>\r\n" +
                "</dict>\r\n" +
                "</plist>\r\n";

        HttpMessage resp = new HttpMessage();
        resp.setType(HttpMessage.Type.Request);
        resp.setVersion(1, 1);
        resp.setMethod("POST");
        resp.setUri("/event");
        resp.addHeaders("Content-Type", "text/x-apple-plist+xml");
        resp.addHeaders("Content-Length", xml.getBytes().length);
        resp.addHeaders("X-Apple-Session-ID", sessionId);
        resp.setContent(xml.getBytes());

        return resp;
    }
}
