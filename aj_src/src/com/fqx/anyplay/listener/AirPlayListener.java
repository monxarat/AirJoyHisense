
package com.fqx.anyplay.listener;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import cn.airjoy.android.service.LocalAudioPlayer;
import cn.airjoy.android.service.LocalPlayer;
import cn.airjoy.android.service.ShowImgView;

import com.airjoy.airplay.server.AirPlayServer;
import com.airjoy.airplay.server.AirPlayServerListener;
import com.airjoy.airplay.server.AirPlayServer.EventAction;
import com.airjoy.airplay.server.AirPlayServer.EventCategory;
import com.example.testaiplayserver.MainActivity;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.images.AirImgCache;
import com.fqx.anyplay.service.VideoInfo;



/**
* <p>描述: 本文件比较重要，实现了AirPlay所有的操作行为的接口回调，包含图片音乐视频的推送操作处理。</p>
* 
* @author sangwencheng
* @version 1.0
*/
public class AirPlayListener implements AirPlayServerListener {

	private String mClientIp = ""; // 客户端ip
	private boolean mIsPlaying = false; // 播放与否
	private EventCategory mPlayingMediaType = EventCategory.EventUnknown; // 正在播放的媒体类型
	private String mSlideshowsClientIp = ""; // 幻灯片客户端ip
	private float mPlayPosition = 0.0f;

	private Context mContext;
	private LocalInfo mLocalInfo;
	private boolean sg_image_show = false;
	private int mChannel;
	private VideoInfo mVideoInfo;
	  
	  
	
	public static Class<?> mVideoClass;
	  

	
	private static final String TAG = MainActivity.class.getSimpleName();
	
	
	public AirPlayListener(Context context) {
		// TODO Auto-generated constructor stub
		mContext = context;
		mLocalInfo = LocalInfo.getInstance(mContext);
	    this.mChannel = APPEnum.AirChannel.AirPlay.GetValue();
	    this.mVideoInfo = VideoInfo.getInstance();
	}

	private void trySendReady() {
		if(mLocalInfo.g_CustomUI != APPEnum.MDCustomUI.hisense.GetValue()) {
			return;
		}
		Intent localIntent = new Intent();
		localIntent.setAction(AnyPlayUtils.ACTION_HX_EXIT);
		mContext.sendBroadcast(localIntent);
	}
	
	private void gotoImgView(int cmd, String imgID) {
	    Intent localIntent = new Intent(this.mContext, ShowImgView.class);
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("IMGChannel", this.mChannel);
	    localBundle.putInt("IMGCMD", cmd);
	    localBundle.putString("IMGID", imgID);
	    localIntent.putExtras(localBundle);
	    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	    this.mContext.startActivity(localIntent);
	    isShow(cmd);
	}

	private void gotoImgView(int cmd, String imgID, byte[] imgBytes) {
	    Intent localIntent = new Intent(this.mContext, ShowImgView.class);
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("IMGChannel", this.mChannel);
	    localBundle.putInt("IMGCMD", cmd);
	    localBundle.putString("IMGID", imgID);
	    AirImgCache.sg_img_data = imgBytes;
	    localIntent.putExtras(localBundle);
	    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	    this.mContext.startActivity(localIntent);
	    isShow(cmd);
	}

	public boolean isShow(int value) {
		if((value == APPEnum.AirImgCmd.didStopPhotoOrSlideshow.GetValue()) ||
				(value == APPEnum.AirImgCmd.didSlidePictureStop.GetValue())) {
			sg_image_show = false;
		}else{
			sg_image_show = true;
		}
		
		return sg_image_show;
	}
	
	private void start_video(String urlString, int postion) {
	    if (urlString.indexOf(".mp3") >= 0) { 
	    	mVideoClass = LocalAudioPlayer.class;
	    }else if ((urlString.indexOf(".m3u8") >= 0) || (urlString.indexOf("/m3u8") >= 0) ) {
	    	mVideoClass = LocalPlayer.class;
	    }else {
	    	mVideoClass = LocalPlayer.class;
	    }
	    AnyPlayUtils.LOG_ERR("start_video", " mVideoClass=" + mVideoClass);
	    Intent localIntent = new Intent(this.mContext, mVideoClass);
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didStartPlayVideo.GetValue());
	    localBundle.putString("UriString", urlString);
	    localBundle.putInt("StartPositon", postion);
	    localBundle.putInt("AirChannel", APPEnum.AirChannel.AirPlay.GetValue());
	    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	    localIntent.putExtras(localBundle);
	    this.mContext.startActivity(localIntent);
	}
	
	private void tryExitVideo() {
	    if (!LocalInfo.APVideoisRuning) {
	      AnyPlayUtils.LOG_ERR("didStopPlayback", " ERR: APVideoisRuning = false");
	      return;
	    }
	    if (mVideoClass == null) {
	        AnyPlayUtils.LOG_ERR("didStopPlayback", " ERR: Stop Class");
	        return;
	    }
	    Intent localIntent = new Intent(this.mContext, mVideoClass);
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didStopPlayback.GetValue());
	    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	    localIntent.putExtras(localBundle);
	    this.mContext.startActivity(localIntent);
	}
	
	    
	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didSubscribeEvent(java.lang.String)
	 */
	@Override
	public void didSubscribeEvent(String clientIp) {
		// TODO Auto-generated method stub
		Log.d(TAG, "didSubscribeEvent: " + clientIp);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didUnsubscribeEvent(java.lang.String)
	 */
	@Override
	public void didUnsubscribeEvent(String clientIp) {
		// TODO Auto-generated method stub
		Log.d(TAG, "didUnsubscribeEvent: " + clientIp);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#willPutPhoto(java.lang.String, java.lang.String)
	 */
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
	    gotoImgView(APPEnum.AirImgCmd.willPutPhoto.GetValue(), photoId);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didPutPhoto(java.lang.String, byte[], int, java.lang.String)
	 */
	@Override
	public void didPutPhoto(String photoId, byte[] data, int len,
			String clientIp) {
		// TODO Auto-generated method stub
		 Log.d(TAG, String.format("didPutPhoto: photo id = %s, len = %d, client ip = %s", photoId, len, clientIp));
		LocalInfo.gAirImgCache.add(photoId, data);
	    gotoImgView(APPEnum.AirImgCmd.didPutPhote.GetValue(), photoId, data);
        mIsPlaying = true;
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#willPutPhotoCacheOnly(java.lang.String, java.lang.String)
	 */
	@Override
	public void willPutPhotoCacheOnly(String photoId, String clientIp) {
		// TODO Auto-generated method stub
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

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didPutPhotoCacheOnly(java.lang.String, byte[], int, java.lang.String)
	 */
	@Override
	public void didPutPhotoCacheOnly(String photoId, byte[] data, int len,
			String clientIp) {
		// TODO Auto-generated method stub
		Log.d(TAG, String.format("didPutPhoto: photo id = %s, len = %d, client ip = %s", photoId, len, clientIp));
		LocalInfo.gAirImgCache.add(photoId, data);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didDisplayCachedPhoto(java.lang.String, java.lang.String)
	 */
	@Override
	public void didDisplayCachedPhoto(String photoId, String clientIp) {
		// TODO Auto-generated method stub
        // 1. 保存信息，publishEvent() 需要用到
        mClientIp = clientIp;
        mIsPlaying = true;
        mPlayingMediaType = EventCategory.EventPhoto;
	    gotoImgView(APPEnum.AirImgCmd.didDisplayCachedPhoto.GetValue(), photoId);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didStartPlayVideo(java.lang.String, double, java.lang.String)
	 */
	@Override
	public void didStartPlayVideo(String contentLocation, double startPosition,
			String clientIp) {
		// TODO Auto-generated method stub
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
        
		trySendReady();
	    this.mVideoInfo.setCur((int) startPosition);
	    start_video(contentLocation, (int) startPosition);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didStartPlayMusic(java.lang.String, double, java.lang.String)
	 */
	@Override
	public void didStartPlayMusic(String contentLocation, double startPosition,
			String clientIp) {
		// TODO Auto-generated method stub
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
        
		trySendReady();
	    this.mVideoInfo.setCur((int) startPosition);
	    start_video(contentLocation, (int) startPosition);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didSetPlaybackRate(float, java.lang.String)
	 */
	@Override
	public void didSetPlaybackRate(float rate, String clientIp) {
		// TODO Auto-generated method stub
        if (rate == 1.0)
            mIsPlaying = true;
        else
            mIsPlaying = false;

	    Intent localIntent = new Intent();
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didSetPlaybackRate.GetValue());
	    localBundle.putLong("Rate", (long) rate);
	    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	    localIntent.putExtras(localBundle);
	    localIntent.setAction(AnyPlayUtils.ACTION_PLAYER_CMD);
	    this.mContext.sendBroadcast(localIntent);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didStop(java.lang.String)
	 */
	@Override
	public void didStop(String clientIp) {
		// TODO Auto-generated method stub
		mIsPlaying = false;
		AnyPlayUtils.is_anyplay = false;
		if (mVideoClass == null) { 
			AnyPlayUtils.LOG_ERR("didStopPlayback", " ERR: Stop Class");
		    return;
		}
		tryExitVideo();
		if(sg_image_show) {
		    gotoImgView(APPEnum.AirImgCmd.didStopPhotoOrSlideshow.GetValue(), "");
		}
		mVideoClass = null;
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#setCurrentPlaybackProgress(float, java.lang.String)
	 */
	@Override
	public void setCurrentPlaybackProgress(float playPosition, String clientIp) {
		// TODO Auto-generated method stub
		mPlayPosition = playPosition;
		
		if (mVideoClass == null) { 
			return;
		}
		this.mVideoInfo.setCur((int)playPosition);
		Intent localIntent = new Intent(this.mContext, mVideoClass);
		Bundle localBundle = new Bundle();
		localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.setCurrentPlaybackProgress.GetValue());
		localBundle.putLong("PlayPosition", (long) playPosition);
		localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		localIntent.putExtras(localBundle);
		this.mContext.startActivity(localIntent);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#getCurrentPlaybackProgress(com.airjoy.airplay.server.AirPlayServerListener.PlaybackTime, java.lang.String)
	 */
	@Override
	public void getCurrentPlaybackProgress(PlaybackTime time, String clientIp) {
		// TODO Auto-generated method stub
		time.playPosition = mVideoInfo.getPosition();
		time.duration = mVideoInfo.getDuration();
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#getPlaybackInfo(com.airjoy.airplay.server.AirPlayServerListener.PlaybackInfo, java.lang.String)
	 */
	@Override
	public void getPlaybackInfo(PlaybackInfo info, String clientIp) {
		// TODO Auto-generated method stub
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

        info.playbackTime.duration = mVideoInfo.getDuration(); // 视频长度
        info.playbackTime.playPosition = mVideoInfo.getPosition();

        if (mIsPlaying)
            info.rate = 1.0f;
        else
            info.rate = 0.0f;

        info.readyToPlay = true;

        info.playbackBufferEmpty = true;
        info.playbackBufferFull = false;
        info.playbackLikelyToKeepUp = true;

        info.loadedTimeRanges.duration = mVideoInfo.getDuration();
        info.loadedTimeRanges.startPosition = mVideoInfo.getPosition();

        info.seekableTimeRanges.duration = mVideoInfo.getDuration();
        info.seekableTimeRanges.startPosition = mVideoInfo.getPosition();
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didSetVolume(float, java.lang.String)
	 */
	@Override
	public void didSetVolume(float value, String clientIp) {
		// TODO Auto-generated method stub
		if (mVideoClass == null) {
	    	return;
	    }
	    Intent localIntent = new Intent(this.mContext, mVideoClass);
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didSetVolume.GetValue());
	    localBundle.putFloat("Volume", value);
	    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	    localIntent.putExtras(localBundle);
	    this.mContext.startActivity(localIntent);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didStartSlideshows(int, java.lang.String)
	 */
	@Override
	public void didStartSlideshows(int slideDuration, String clientIp) {
		// TODO Auto-generated method stub
		 gotoImgView(APPEnum.AirImgCmd.didSlidePictureStart.GetValue(), " ");
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didStopSlideshows(java.lang.String)
	 */
	@Override
	public void didStopSlideshows(String clientIp) {
		// TODO Auto-generated method stub
		AnyPlayUtils.LOG_DEBUG("didStopSlideshow", "doing...");
		gotoImgView(APPEnum.AirImgCmd.didStopPhotoOrSlideshow.GetValue(), "");
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didGetSlideshowsPicture(int, byte[], int, java.lang.String)
	 */
	@Override
	public void didGetSlideshowsPicture(int index, byte[] data, int len,
			String clientIp) {
		// TODO Auto-generated method stub
		gotoImgView(APPEnum.AirImgCmd.didGetSlideshowsPicture.GetValue(), String.valueOf(index), data);
	}

	/* (non-Javadoc)
	 * @see com.airjoy.airplay.server.AirPlayServerListener#didGetSlideshowsPictureFailed(java.lang.String)
	 */
	@Override
	public void didGetSlideshowsPictureFailed(String clientIp) {
		// TODO Auto-generated method stub
		gotoImgView(APPEnum.AirImgCmd.didSlidePictureOver.GetValue(), " ");
	}

}
