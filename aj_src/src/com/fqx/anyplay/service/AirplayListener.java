package com.fqx.anyplay.service;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import cn.airjoy.android.service.APVideo;
import cn.airjoy.android.service.LocalAudioPlayer;
import cn.airjoy.android.service.LocalPlayer;
import cn.airjoy.android.service.ShowImgView;

import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.PublishState;
import com.fqx.anyplay.images.AirImgCache;

public class AirplayListener {
  public static Class<?> mVideoClass;
  private APAirTunesServerListener mApAirTunesServerListener;
  private int mChannel;
  private Context mContext;
  private PlaybackInfo mPlaybackInfo;
  private PlaybackTime mPlaybackTime;
  private PublishState mPublishState;
  private VideoInfo mVideoInfo;
  private LocalInfo mLocalInfo;
  private boolean sg_image_show = false;

  private void trySendReady() {
	  if(mLocalInfo.g_CustomUI != APPEnum.MDCustomUI.hisense.GetValue()) {
		  return;
	  }

	  Intent localIntent = new Intent();
	  localIntent.setAction(AnyPlayUtils.ACTION_HX_EXIT);
	  mContext.sendBroadcast(localIntent);
  }

  public AirplayListener(Context paramContext) {
    this.mContext = paramContext;
    this.mVideoInfo = VideoInfo.getInstance();
    this.mPlaybackTime = new PlaybackTime();
    this.mPlaybackInfo = PlaybackInfo.getInstance();
    mVideoClass = null;
    sg_image_show = false;
    this.mPublishState = PublishState.getInstance();
    this.mApAirTunesServerListener = APAirTunesServerListener.getInstance(paramContext);
    this.mChannel = APPEnum.AirChannel.AirPlay.GetValue();
    mLocalInfo = LocalInfo.getInstance(mContext);
  }

  private void gotoImgView(int paramInt, String paramString) {
    Intent localIntent = new Intent(this.mContext, ShowImgView.class);
    Bundle localBundle = new Bundle();
    localBundle.putInt("IMGChannel", this.mChannel);
    localBundle.putInt("IMGCMD", paramInt);
    localBundle.putString("IMGID", paramString);
    localIntent.putExtras(localBundle);
    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    this.mContext.startActivity(localIntent);
    isShow(paramInt);
  }

  private void gotoImgView(int paramInt, String paramString, byte[] paramArrayOfByte) {
    Intent localIntent = new Intent(this.mContext, ShowImgView.class);
    Bundle localBundle = new Bundle();
    localBundle.putInt("IMGChannel", this.mChannel);
    localBundle.putInt("IMGCMD", paramInt);
    localBundle.putString("IMGID", paramString);
    AirImgCache.sg_img_data = paramArrayOfByte;
    localIntent.putExtras(localBundle);
    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    this.mContext.startActivity(localIntent);
    isShow(paramInt);
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

	private void start_video(String paramString, int paramInt) {
		String urlString = paramString;
	    if (urlString.indexOf(".mp3") >= 0) {
	      mVideoClass = LocalAudioPlayer.class;
	    }else if ((urlString.indexOf(".m3u8") >= 0) || (urlString.indexOf("/m3u8") >= 0) ) {
	    	if(mLocalInfo.g_IsLocalPlayer_m3u8) {
	    		mVideoClass = LocalPlayer.class;
	    	}else{
	    		mVideoClass = APVideo.class;
	    	}
//	        urlString = VerifyURL.tryDo(paramString);
//	        if(urlString == null) {
//	        	mVideoClass = null;
//	        	AnyPlayApi.LOG_ERR("start_video", "URL:" + paramString);
//	        	return;
//	        }
	    }else {
	      mVideoClass = LocalPlayer.class;
	    }
	    AnyPlayUtils.LOG_ERR("start_video", " mVideoClass=" + mVideoClass);
	    Intent localIntent = new Intent(this.mContext, mVideoClass);
	    Bundle localBundle = new Bundle();
	    localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didStartPlayVideo.GetValue());
	    localBundle.putString("UriString", urlString);
	    localBundle.putInt("StartPositon", paramInt);
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
	    if (!this.mPublishState.isMediaVideo()) {
	        AnyPlayUtils.LOG_ERR("didStopPlayback", " MediaVideo = false");
	        return;
	    }
	      Intent localIntent = new Intent(this.mContext, mVideoClass);
	      Bundle localBundle = new Bundle();
	      localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didStopPlayback.GetValue());
	      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	      localIntent.putExtras(localBundle);
	      this.mContext.startActivity(localIntent);
	}
	
	public void didCreateEventSession(int paramInt, String paramString) {
	    LocalInfo.gVideoSessionID = paramInt;
	}
	
	public void didDisplayCachedPhoto(String paramString) {
	    this.mPublishState.UpdateResInfo(paramString, "", "");
	    this.mPublishState.SetMediaPhotoState(APPEnum.EventState.EventStatePlaying.GetValue());
	    gotoImgView(APPEnum.AirImgCmd.didDisplayCachedPhoto.GetValue(), paramString);
	}
	
	public void didGetSlideshowsPicture(int paramInt, byte[] paramArrayOfByte, long paramLong) {
	    AnyPlayUtils.LOG_DEBUG("Slideshows", " didStopSlideshows");
	    if (!this.mPublishState.iSlidesShowMode()) {
	      AnyPlayUtils.LOG_ERR("Slideshows", "iSlidesShowMode=false");
	      return;
	    }
	      this.mPublishState.UpdateResInfo(String.valueOf(paramInt), "", "");
	      this.mPublishState.SetMediaPhotoState(APPEnum.EventState.EventStatePlaying.GetValue());
	      gotoImgView(APPEnum.AirImgCmd.didGetSlideshowsPicture.GetValue(), String.valueOf(paramInt), paramArrayOfByte);
	}
	
	public void didGetSlideshowsPictureFailed() {
	    gotoImgView(APPEnum.AirImgCmd.didSlidePictureOver.GetValue(), " ");
	}
	
	public void didPutPhoto(String paramString, byte[] paramArrayOfByte, long paramLong) {
	    LocalInfo.gAirImgCache.add(paramString, paramArrayOfByte);
	    gotoImgView(APPEnum.AirImgCmd.didPutPhote.GetValue(), paramString, paramArrayOfByte);
	}
	
	public void didPutPhotoCacheOnly(String paramString, byte[] paramArrayOfByte, long paramLong) {
	    LocalInfo.gAirImgCache.add(paramString, paramArrayOfByte);
	}
	
	public void didSetPlaybackRate(long paramLong) {
	    AnyPlayUtils.LOG_DEBUG("didSetPlaybackRate", mVideoClass + " Rate:" + paramLong);
	    this.mPublishState.SetMediaVideoRate((int)paramLong);
	    if (mVideoClass == null) {
	    	return;
	    }
	      this.mPlaybackInfo.set_rate((int)paramLong);
	      Intent localIntent = new Intent(this.mContext, mVideoClass);
	      Bundle localBundle = new Bundle();
	      localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didSetPlaybackRate.GetValue());
	      localBundle.putLong("Rate", paramLong);
	      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	      localIntent.putExtras(localBundle);
	      this.mContext.startActivity(localIntent);
	}
	
	public void didSetVolume(float paramFloat) {
	    AnyPlayUtils.LOG_DEBUG("disSetVolume", mVideoClass + " Vol:" + paramFloat);
	    if (mVideoClass == null) {
	    	return;
	    }
	      Intent localIntent = new Intent(this.mContext, mVideoClass);
	      Bundle localBundle = new Bundle();
	      localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didSetVolume.GetValue());
	      localBundle.putFloat("Volume", paramFloat);
	      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	      localIntent.putExtras(localBundle);
	      this.mContext.startActivity(localIntent);
	}
	
	public void didStartPlayMusic(String paramString1, int paramInt, String paramString2) {
		trySendReady();
	    this.mApAirTunesServerListener.tryStop();
	    AnyPlayUtils.LOG_DEBUG("didStartPlayMusic", "URL:" + paramString1 + " POS:" + paramInt);
	    this.mPublishState.SetChannel(this.mChannel, paramString2);
	    this.mPublishState.UpdateResInfo("", "", "");
	    this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStatePlaying.GetValue());
	    this.mVideoInfo.setCur(paramInt);
	    start_video(paramString1, paramInt);
	}
	
	public void didStartPlayVideo(String paramString1, int paramInt, String paramString2) {
		trySendReady();
	    this.mApAirTunesServerListener.tryStop();
	    AnyPlayUtils.LOG_DEBUG("didStartPlayVideo", "URL:" + paramString1 + " POS:" + paramInt);
	    this.mPublishState.SetChannel(this.mChannel, paramString2);
	    this.mPublishState.UpdateResInfo("", "", "");
	    this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStatePlaying.GetValue());
	    this.mVideoInfo.setCur(paramInt);
	    start_video(paramString1, paramInt);
	}
	
	public void didStartSlideshows(int paramInt, String paramString) {
	    AnyPlayUtils.LOG_DEBUG("Slideshows", " didStartSlideshows");
	    this.mPublishState.SetChannel(this.mChannel, paramString);
	    gotoImgView(APPEnum.AirImgCmd.didSlidePictureStart.GetValue(), " ");
	}
	
	public void didStopPhotoOrSlideshow() {
	    if (this.mPublishState.iSlidesShowMode()) {
		    AnyPlayUtils.LOG_DEBUG("didStopSlideshow", "iSlidesShowMode = false");
	    	return;
	    }
	    this.mPublishState.setSlidesShowMode(false);
	    AnyPlayUtils.LOG_DEBUG("didStopSlideshow", "doing...");
		gotoImgView(APPEnum.AirImgCmd.didStopPhotoOrSlideshow.GetValue(), "");
	}
	
	public void didStopPlayback() {
	    AnyPlayUtils.LOG_DEBUG("didStopPlayback", mVideoClass + " Stop ");
	    AnyPlayUtils.is_anyplay = false;
	    if (mVideoClass == null) {
	      AnyPlayUtils.LOG_ERR("didStopPlayback", " ERR: Stop Class");
	      return;
	    }
	    tryExitVideo();
	    if(sg_image_show) {
	    	didStopPhotoOrSlideshow();
	    }
	    mVideoClass = null;
	}
	
	public void didStopSlideshows(String paramString) {
	    AnyPlayUtils.LOG_DEBUG("Slideshows", " didStopSlideshows");
	    gotoImgView(APPEnum.AirImgCmd.didSlidePictureStop.GetValue(), " ");
	}
	
	public void didSubscribeEvent(String paramString) {
	}
	
	public void didUnsubscribeEvent(String paramString) {
	}
	
	public PlaybackTime getCurrentPlaybackProgress() {
	    if (mVideoClass == null) {
	    	return mPlaybackTime;
	    }
	    long l1 = this.mVideoInfo.getPosition();
	    long l2 = this.mVideoInfo.getDuration();
	    this.mPlaybackTime.setPlayPosition(l1);
	    this.mPlaybackTime.setDuration(l2);
	    return this.mPlaybackTime;
	}
	
	public PlaybackInfo getPlaybackInfo2() {
   //     AnyPlayApi.LOG_DEBUG("getPlaybackInfo2", "Dur:" + mPlaybackInfo.get_duration() + " POS:" + mPlaybackInfo.get_playPosition());
	    return mPlaybackInfo;
	}
	
	public void setCurrentPlaybackProgress(long paramLong) {
	    AnyPlayUtils.LOG_DEBUG("setCurrentPlaybackProgress", mVideoClass + " POS:" + paramLong);
	    if (mVideoClass == null) {
	    	return;
	    }
	      this.mVideoInfo.setCur((int)paramLong);
	      Intent localIntent = new Intent(this.mContext, mVideoClass);
	      Bundle localBundle = new Bundle();
	      localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.setCurrentPlaybackProgress.GetValue());
	      localBundle.putLong("PlayPosition", paramLong);
	      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
	      localIntent.putExtras(localBundle);
	      this.mContext.startActivity(localIntent);
	}
	
	public void willPutPhoto(String paramString1, String paramString2) {
	    this.mApAirTunesServerListener.tryStop();
	    if (this.mPublishState.tryStopVideo()) {
	      tryExitVideo();
	    }
	    this.mPublishState.SetChannel(this.mChannel, paramString2);
	    this.mPublishState.UpdateResInfo(paramString1, "", "");
	    this.mPublishState.SetMediaPhotoState(APPEnum.EventState.EventStatePlaying.GetValue());
	    gotoImgView(APPEnum.AirImgCmd.willPutPhoto.GetValue(), paramString1);
	}
	
	public void willPutPhotoCacheOnly(String paramString1, String paramString2) {
	    this.mPublishState.SetChannel(this.mChannel, paramString2);
	}

}
