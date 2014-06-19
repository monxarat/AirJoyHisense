package com.fqx.airjoy.service;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import cn.airjoy.android.service.APVideo;
import cn.airjoy.android.service.LocalPlayer;
import com.fqx.airjoy.listener.AirVideoListener;
import com.fqx.airjoy.server.AJPlayBackInfo;
import com.fqx.airjoy.server.AJVideoInfo;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.PublishState;

public class AJVideo implements AirVideoListener {
  private AJPlayBackInfo mAJPlayBackInfo;
  private AJVideoInfo mAJVideoInfo;
  private int mChannel;
  private Context mContext;
  private PublishState mPublishState;
  private Class<?> mVideoClass;

  public AJVideo(Context paramContext) {
    this.mContext = paramContext;
    this.mAJVideoInfo = AJVideoInfo.getInstance();
    this.mAJPlayBackInfo = AJPlayBackInfo.getInstance();
    this.mVideoClass = null;
    this.mPublishState = PublishState.getInstance();
    this.mChannel = APPEnum.AirChannel.AirJoy.GetValue();
  }

  private void start_video(String paramString, int paramInt) {
    if (paramString.indexOf(".m3u8") < 0) {
    	this.mVideoClass = LocalPlayer.class; 
    }else{
    	this.mVideoClass = APVideo.class;
    }
      Intent localIntent = new Intent(this.mContext, this.mVideoClass);
      Bundle localBundle = new Bundle();
      localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didStartPlayVideo.GetValue());
      localBundle.putString("UriString", paramString);
      localBundle.putInt("StartPositon", paramInt);
      localBundle.putInt("AirChannel", APPEnum.AirChannel.AirJoy.GetValue());
      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
      localIntent.putExtras(localBundle);
      this.mContext.startActivity(localIntent);
      return;
  }

  public AJPlayBackInfo getPlayVideoInfo() {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "getPlayVideoInfo  pos=" + this.mAJPlayBackInfo.getPositon() + " Dur=" + this.mAJPlayBackInfo.getDuration());
    return this.mAJPlayBackInfo;
  }

  public AJVideoInfo getPlayVideoProgress(String paramString) {
    return this.mAJVideoInfo;
  }

  public int playVideo(String paramString1, String paramString2, String paramString3, float paramFloat, String paramString4) {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "playVideo id=" + paramString1 + " name=" + paramString3 + " url=" + paramString2 + " pos=" + paramFloat);
    this.mPublishState.SetChannel(this.mChannel, paramString4);
    this.mPublishState.UpdateResInfo(paramString1, paramString2, paramString3);
    this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStatePlaying.GetValue());
    this.mAJVideoInfo.setPostion(paramFloat);
    this.mAJPlayBackInfo.setPositon(paramFloat);
    start_video(paramString2, (int)(1000.0F * paramFloat));
    return 0;
  }

  public int setPlayVideoProgress(String paramString, float paramFloat) {
    if (this.mVideoClass == null) {
    	return -1;
    }
      AnyPlayUtils.LOG_DEBUG("AirJoy", "setPlayVideoProgress  pos=" + paramFloat);
      this.mAJPlayBackInfo.setPositon(paramFloat);
      this.mAJVideoInfo.setPostion(paramFloat);
      Intent localIntent = new Intent(this.mContext, this.mVideoClass);
      Bundle localBundle = new Bundle();
      localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.setCurrentPlaybackProgress.GetValue());
      localBundle.putLong("PlayPosition", (int)paramFloat);
      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
      localIntent.putExtras(localBundle);
      this.mContext.startActivity(localIntent);
      return 0;
  }

  public int setPlayVideoRate(String paramString, float paramFloat) {
    this.mPublishState.SetMediaVideoRate((int)paramFloat);
    if (this.mVideoClass == null) {
    	return -1;
    }
      this.mAJPlayBackInfo.setRate(paramFloat);
      this.mAJVideoInfo.setRate(paramFloat);
      Intent localIntent = new Intent(this.mContext, this.mVideoClass);
      Bundle localBundle = new Bundle();
      localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didSetPlaybackRate.GetValue());
      localBundle.putLong("Rate", (int)paramFloat);
      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
      localIntent.putExtras(localBundle);
      this.mContext.startActivity(localIntent);
      return 0;
  }

  public int stopPlayVideo(String paramString) {
    this.mPublishState.SetMediaVideo(APPEnum.EventState.EventStateStopped.GetValue());
    if (this.mVideoClass == null) {
    	return -1;
    }
      if (!this.mPublishState.isMediaVideo()) {
        AnyPlayUtils.LOG_ERR("stopPlayVideo", " MediaVideo = false");
    	return -1;
      }
      Intent localIntent = new Intent(this.mContext, this.mVideoClass);
      Bundle localBundle = new Bundle();
      localBundle.putInt("VideoCmd", APPEnum.AirVideoCmd.didStopPlayback.GetValue());
      localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
      localIntent.putExtras(localBundle);
      this.mContext.startActivity(localIntent);
      this.mVideoClass = null;
      
      return 0;
  }
}