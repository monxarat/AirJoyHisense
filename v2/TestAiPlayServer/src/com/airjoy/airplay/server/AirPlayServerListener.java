package com.airjoy.airplay.server;

public interface AirPlayServerListener {

    public class TimeRanges {
        public float startPosition;
        public float duration;
    };

    public class  PlaybackTime {
        public float playPosition;
        public float duration;
    };

    public class PlaybackInfo {
        public PlaybackTime playbackTime = new PlaybackTime();
        public float rate;
        public boolean readyToPlay;
        public boolean playbackBufferEmpty;
        public boolean playbackBufferFull;
        public boolean playbackLikelyToKeepUp;
        public TimeRanges loadedTimeRanges = new TimeRanges();
        public TimeRanges seekableTimeRanges= new TimeRanges();
    };

    void didSubscribeEvent(String clientIp);

    void didUnsubscribeEvent(String clientIp);

    void willPutPhoto(String photoId, String clientIp);

    void didPutPhoto(String photoId, byte data[], int len, String clientIp);

    void willPutPhotoCacheOnly(String photoId, String clientIp);

    void didPutPhotoCacheOnly(String photoId, byte data[], int len, String clientIp);

    void didDisplayCachedPhoto(String photoId, String clientIp);

    void didStartPlayVideo(String contentLocation, double startPosition, String clientIp);

    void didStartPlayMusic(String contentLocation, double startPosition, String clientIp);

    void didSetPlaybackRate(float rate, String clientIp);

    void didStop(String clientIp);

    void setCurrentPlaybackProgress(float playPosition, String clientIp);

    void getCurrentPlaybackProgress(PlaybackTime time, String clientIp);

    void getPlaybackInfo(PlaybackInfo info, String clientIp);

    void didSetVolume(float value, String clientIp);

    void didStartSlideshows(int slideDuration, String clientIp);

    void didStopSlideshows(String clientIp);

    void didGetSlideshowsPicture(int index, byte data[], int len, String clientIp);

    void didGetSlideshowsPictureFailed(String clientIp);

}