
package com.example.testaiplayserver;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Menu;
import android.view.View;

import com.airjoy.airplay.server.AirPlayServer;
import com.airjoy.airplay.server.AirPlayServerListener;
import com.airjoy.airplay.server.AirPlayServer.EventAction;
import com.airjoy.airplay.server.AirPlayServer.EventCategory;
import com.airjoy.airtunes.server.AirTunesServer;
import com.airjoy.airtunes.server.AirTunesServerListener;
import com.airjoy.bonjour.serviceinfo.AirPlayServiceInfo;
import com.airjoy.bonjour.serviceinfo.AirTunesServiceInfo;
import com.airjoy.bonjour.Bonjour;
import com.airjoy.bonjour.BonjourListener;
import com.example.airplay.R;

import java.util.Map;

public class MainActivity extends Activity implements BonjourListener,
        AirPlayServerListener,
        AirTunesServerListener {

    private static final String TAG = MainActivity.class.getSimpleName();

    private boolean mStart = false;
    private String mClientIp = ""; // 客户端ip
    private boolean mIsPlaying = false; // 播放与否
    private EventCategory mPlayingMediaType = EventCategory.EventUnknown; // 正在播放的媒体类型
    private String mSlideshowsClientIp = ""; // 幻灯片客户端ip
    private float mPlayPosition = 0.0f;
    private Bonjour mBonjour = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mBonjour = Bonjour.getInstance();
        mBonjour.setContent(this);
        mBonjour.setListener(this);
        mBonjour.start();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    public void onStart(View button) {
        Log.d(TAG, "Button start");

        if (!mStart) {
            mStart = true;

            String name = "HisenseTV";
            //byte[] deviceId = NetWork.getMacAddress();
            // 68:DF:DD:64:02:D8
            //byte[] deviceId = {0x68, (byte) 0xdf, (byte) 0xdd, 0x64, 0x02, (byte) 0xd8};

            // 14:F6:5A:B9:93:FC
            byte[] deviceId = {0x14, (byte) 0xF6, (byte) 0x5A, (byte)0xB9, (byte)0x93, (byte) 0xFC};

            int airplayPort = 7000;
            int airtunesPort = 5000;

            AirPlayServer.getInstance().start(deviceId, airplayPort, this);
            AirTunesServer.getInstance().start(deviceId, airtunesPort, this);

            Log.d(TAG, "airplay port: " + AirPlayServer.getInstance().getPort());
            Log.d(TAG, "airtunes port: " + AirTunesServer.getInstance().getPort());

            AirPlayServiceInfo info1 = new AirPlayServiceInfo(deviceId,
                    name, 
                    AirPlayServer.getInstance().getPort());
            mBonjour.publishService(info1);

            AirTunesServiceInfo info2 = new AirTunesServiceInfo(deviceId, 
                    name, 
                    AirTunesServer.getInstance().getPort());
            mBonjour.publishService(info2);
        }
    }

    public void onStop(View button) {
        Log.d(TAG, "Button stop");

        if (mStart) {
            mStart = false;

            AirPlayServer.getInstance().stop();
            AirTunesServer.getInstance().stop();
        }
    }

    @Override
    public void didSubscribeEvent(String clientIp) {
        Log.d(TAG, "didSubscribeEvent: " + clientIp);
    }

    @Override
    public void didUnsubscribeEvent(String clientIp) {
        Log.d(TAG, "didUnsubscribeEvent: " + clientIp);
    }

    @Override
    public void willPutPhoto(String photoId, String clientIp) {

        // 0. 如果客户端IP和原来的不一样，认为这是一个新的iPhone发送了图片
        // 需要发布事件，通知上一个iPhone，照片或视频已经停止。
        if (mClientIp.equalsIgnoreCase(clientIp)) {
            AirPlayServer.getInstance().publishEvent(mPlayingMediaType,
                    EventAction.EventActionStopped, mClientIp);
            AirPlayServer.getInstance().closeConnection(mClientIp);
            mClientIp = clientIp;
        }

        // 1. 保存信息, publishEvent() 需要用到
        mPlayingMediaType = EventCategory.EventPhoto;
    }

    @Override
    public void didPutPhoto(String photoId, byte[] data, int len, String clientIp) {
        Log.d(TAG, String.format("didPutPhoto: photo id = %s, len = %d, client ip = %s", photoId,
                len, clientIp));

        mIsPlaying = true;
    }

    @Override
    public void willPutPhotoCacheOnly(String photoId, String clientIp) {
        // 这里的逻辑与willPutPhoto一样，因为willPutPhoto和willPutPhotoCacheOnly调用顺序不确定

        // 0. 如果客户端IP和原来的不一样，认为这是一个新的iPhone发送了图片
        // 需要发布事件，通知上一个iPhone，照片或视频已经停止。
        if (mClientIp.equalsIgnoreCase(clientIp)) {
            AirPlayServer.getInstance().publishEvent(mPlayingMediaType,
                    EventAction.EventActionStopped, mClientIp);
            AirPlayServer.getInstance().closeConnection(mClientIp);
            mClientIp = clientIp;
        }

        // 1. 保存信息, publishEvent() 需要用到
        mPlayingMediaType = EventCategory.EventPhoto;
    }

    @Override
    public void didPutPhotoCacheOnly(String photoId, byte[] data, int len, String clientIp) {
        Log.d(TAG, String.format("didPutPhotoCacheOnly: photo id = %s, len = %d, client ip = %s",
                photoId, len, clientIp));
    }

    @Override
    public void didDisplayCachedPhoto(String photoId, String clientIp) {
        Log.d(TAG, String.format("didDisplayCachedPhoto: photo id = %s, client ip = %s", photoId,
                clientIp));

        // 1. 保存信息，publishEvent() 需要用到
        mClientIp = clientIp;
        mIsPlaying = true;
        mPlayingMediaType = EventCategory.EventPhoto;
    }

    @Override
    public void didStartPlayVideo(String contentLocation, double startPosition, String clientIp) {
        Log.d(TAG, String.format("didStartPlayVideo: url: %s, pos: %f, %s", contentLocation,
                startPosition, clientIp));

        // 0. 如果客户端IP和原来的不一样，认为这是一个新的iPhone发送了视频
        // 需要发布事件，通知上一个iPhone，照片或视频已经停止。
        if (mClientIp.equalsIgnoreCase(clientIp)) {
            AirPlayServer.getInstance().publishEvent(mPlayingMediaType,
                    EventAction.EventActionStopped, mClientIp);
            AirPlayServer.getInstance().closeConnection(mClientIp);
            mClientIp = clientIp;
        }
        else
        {
            if (mIsPlaying)
                AirPlayServer.getInstance().publishEvent(mPlayingMediaType,
                        EventAction.EventActionStopped, mClientIp);
        }

        // 1. 保存信息, publishEvent() 需要用到
        mPlayingMediaType = EventCategory.EventVideo;

        // 2. 发布事件，告诉iPhone视频已经开始播放
        AirPlayServer.getInstance().publishEvent(mPlayingMediaType, EventAction.EventActionPlaying,
                mClientIp);

        mIsPlaying = true;

        float videoLength = 285.0f; // 假设视频长度
        mPlayPosition = (float) startPosition * videoLength; // 计算当前时间点
    }

    @Override
    public void didStartPlayMusic(String contentLocation, double startPosition, String clientIp) {
        Log.d(TAG, String.format("didStartPlayMusic: url: %s, pos: %f, %s", contentLocation,
                startPosition, clientIp));

        // 0. 如果客户端IP和原来的不一样，认为这是一个新的iPhone发送了视频
        // 需要发布事件，通知上一个iPhone，照片或视频已经停止。
        if (mClientIp.equalsIgnoreCase(clientIp)) {
            AirPlayServer.getInstance().publishEvent(mPlayingMediaType,
                    EventAction.EventActionStopped, mClientIp);
            AirPlayServer.getInstance().closeConnection(mClientIp);
            mClientIp = clientIp;
        }
        else
        {
            if (mIsPlaying)
                AirPlayServer.getInstance().publishEvent(mPlayingMediaType,
                        EventAction.EventActionStopped, mClientIp);
        }

        // 1. 保存信息, publishEvent() 需要用到
        mPlayingMediaType = EventCategory.EventVideo;

        // 2. 发布事件，告诉iPhone视频已经开始播放
        AirPlayServer.getInstance().publishEvent(mPlayingMediaType, EventAction.EventActionPlaying,
                mClientIp);

        mIsPlaying = true;

        float videoLength = 285.0f; // 假设视频长度
        mPlayPosition = (float) startPosition * videoLength; // 计算当前时间点
    }

    @Override
    public void didSetPlaybackRate(float rate, String clientIp) {
        Log.d(TAG, String.format("didSetPlaybackRate: %f, %s", rate, clientIp));

        if (rate == 1.0)
            mIsPlaying = true;
        else
            mIsPlaying = false;
    }

    @Override
    public void didStop(String clientIp) {
        Log.d(TAG, String.format("didStop: %s", clientIp));

        mIsPlaying = false;
    }

    @Override
    public void setCurrentPlaybackProgress(float playPosition, String clientIp) {
        Log.d(TAG, String.format("setCurrentPlaybackProgress: %f, %s", playPosition, clientIp));

        mPlayPosition = playPosition;
    }

    @Override
    public void getCurrentPlaybackProgress(PlaybackTime t, String clientIp) {
        Log.d(TAG, String.format("getCurrentPlaybackProgress: %s", clientIp));

        t.playPosition = mPlayPosition;
        t.duration = 285.0f;
    }

    @Override
    public void getPlaybackInfo(PlaybackInfo info, String clientIp) {
        Log.d(TAG, String.format("getPlaybackInfo: %s", clientIp));

        /*
         * typedef struct AIRJOY_API _PlaybackInfo { PlaybackTime playbackTime;
         * // 视频总长度 & 正在播放的时间点 float rate; // 0.0 - 暂停, 1.0 - 正常速率播放 bool
         * readyToPlay; // true - 已经准备播放, false - 还未准备播放，（缺省值：true） bool
         * playbackBufferEmpty; // true - 播放缓冲空，false - 播放缓冲非空，（缺省值：true） bool
         * playbackBufferFull; // true - 播放缓冲已满，false - 播放缓冲未满，（缺省值：false） bool
         * playbackLikelyToKeepUp; // true（缺省值：true） TimeRanges
         * loadedTimeRanges; // 视频加载长度 & 视频开始播放的时间点 TimeRanges
         * seekableTimeRanges; // iPhone/iPad端可以滑动视频的时间范围（视频长度 & 起始点） }
         * PlaybackInfo;
         */

        mPlayPosition += 1.0;

        info.playbackTime.duration = 285.0f; // 视频长度
        info.playbackTime.playPosition = mPlayPosition;

        if (mIsPlaying)
            info.rate = 1.0f;
        else
            info.rate = 0.0f;

        info.readyToPlay = true;

        info.playbackBufferEmpty = true;
        info.playbackBufferFull = false;
        info.playbackLikelyToKeepUp = true;

        info.loadedTimeRanges.duration = 285.0f;
        info.loadedTimeRanges.startPosition = 0.0f;

        info.seekableTimeRanges.duration = 285.0f;
        info.seekableTimeRanges.startPosition = 0.0f;
    }

    @Override
    public void didSetVolume(float value, String clientIp) {
        Log.d(TAG, String.format("didSetVolume: %s", clientIp));
    }

    @Override
    public void didStartSlideshows(int slideDuration, String clientIp) {
        Log.d(TAG, String.format("didStartSlideshows: %d, %s", slideDuration, clientIp));
    }

    @Override
    public void didStopSlideshows(String clientIp) {
        Log.d(TAG, String.format("didStopSlideshows: %s", clientIp));
    }

    @Override
    public void didGetSlideshowsPicture(int index, byte[] data, int len, String clientIp) {
        Log.d(TAG, String.format("didGetSlideshowsPicture: %s, index:%d, len:%d", clientIp, index,
                len));
    }

    @Override
    public void didGetSlideshowsPictureFailed(String clientIp) {
        Log.d(TAG, String.format("didGetSlideshowsPictureFailed: %s", clientIp));
    }

    @Override
    public void onStarted() {
    }

    @Override
    public void onStartFailed() {
    }

    @Override
    public void onStopped() {
    }

    @Override
    public void onServiceFound(String name, String type, String ip, int port,
            Map<String, String> properties) {
    }
    

    @Override
    public void onServiceLost(String name, String type, String ip) {
    }

}
