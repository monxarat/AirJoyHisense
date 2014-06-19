package com.fqx.airjoy.service;

import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import cn.airjoy.android.service.ShowImgView;
import com.fqx.airjoy.listener.AirPhotoListener;
import com.fqx.airjoy.server.AJEnum;
import com.fqx.anyplay.api.APPEnum;
import com.fqx.anyplay.api.AnyPlayUtils;
import com.fqx.anyplay.api.LocalInfo;
import com.fqx.anyplay.api.PublishState;

public class AJPhoto implements AirPhotoListener {
  private int mChannel;
  private Context mContext;
  private LocalInfo mLocalInfo;
  private PublishState mPublishState;

  public AJPhoto(Context paramContext) {
    this.mContext = paramContext;
    this.mLocalInfo = LocalInfo.getInstance(paramContext);
    this.mPublishState = PublishState.getInstance();
    this.mChannel = APPEnum.AirChannel.AirJoy.GetValue();
  }

  private void gotoImgView(int paramInt1, String paramString, int paramInt2, float paramFloat1, float paramFloat2) {
    Intent localIntent = new Intent(this.mContext, ShowImgView.class);
    Bundle localBundle = new Bundle();
    localBundle.putInt("IMGChannel", APPEnum.AirChannel.AirJoy.GetValue());
    localBundle.putInt("IMGCMD", paramInt1);
    localBundle.putString("IMGID", paramString);
    localBundle.putFloat("RATE", paramFloat1);
    localBundle.putFloat("RATEY", paramFloat2);
    localIntent.putExtras(localBundle);
    localIntent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
    this.mContext.startActivity(localIntent);
  }

  public int cachePhoto(String paramString1, String paramString2, String paramString3, String paramString4) {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "cachePhoto id=" + paramString1 + " name=" + paramString3);
    this.mPublishState.SetChannel(this.mChannel, paramString4);
    String str = paramString1 + AnyPlayUtils.getsuffix(paramString3, 0);
    LocalInfo.gAjPhotoManger.tryBulidDownloadTask(paramString2, paramString1, this.mLocalInfo.getCachePath(), str);
    gotoImgView(AJEnum.AirJoyPhotoCmd.cachePhoto.GetValue(), paramString1, 0, 0.0F, 0.0F);
    return AJEnum.ResultCode.ResultCodeOk.GetValue();
  }

  public int displayCachedPhoto(String paramString) {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "displayCachedPhoto id=" + paramString);
    this.mPublishState.UpdateResInfo(paramString, "", "");
    this.mPublishState.SetMediaPhotoState(APPEnum.EventState.EventStatePlaying.GetValue());
    gotoImgView(AJEnum.AirJoyPhotoCmd.displayCachedPhoto.GetValue(), paramString, 0, 0.0F, 0.0F);
    return AJEnum.ResultCode.ResultCodeOk.GetValue();
  }

  public int displayPhoto(String paramString1, String paramString2, String paramString3, String paramString4) {
    this.mPublishState.SetChannel(this.mChannel, paramString4);
    this.mPublishState.UpdateResInfo(paramString1, paramString2, paramString3);
    this.mPublishState.SetMediaPhotoState(APPEnum.EventState.EventStatePlaying.GetValue());
    AnyPlayUtils.LOG_DEBUG("AirJoy", "displayPhoto id=" + paramString1 + " name=" + paramString3);
    String str = paramString1 + AnyPlayUtils.getsuffix(paramString3, 0);
    LocalInfo.gAjPhotoManger.tryBulidDownloadTask(paramString2, paramString1, this.mLocalInfo.getCachePath(), str);
    gotoImgView(AJEnum.AirJoyPhotoCmd.displayPhoto.GetValue(), paramString1, 0, 0.0F, 0.0F);
    return AJEnum.ResultCode.ResultCodeOk.GetValue();
  }

  public int movePhoto(String paramString, float paramFloat1, float paramFloat2) {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "Move x=" + paramFloat1 + " y=" + paramFloat2);
    gotoImgView(AJEnum.AirJoyPhotoCmd.movePhoto.GetValue(), paramString, 0, paramFloat1, paramFloat2);
    return AJEnum.ResultCode.ResultCodeOk.GetValue();
  }

  public int rotatePhoto(String paramString, int paramInt) {
    gotoImgView(AJEnum.AirJoyPhotoCmd.rotatePhoto.GetValue(), paramString, paramInt, 0.0F, 0.0F);
    return AJEnum.ResultCode.ResultCodeOk.GetValue();
  }

  public int stopPhoto() {
    AnyPlayUtils.LOG_DEBUG("AirJoy", "stopPhoto");
    this.mPublishState.SetMediaPhotoState(APPEnum.EventState.EventStateStopped.GetValue());
    gotoImgView(AJEnum.AirJoyPhotoCmd.stopPhoto.GetValue(), "", 0, 0.0F, 0.0F);
    return AJEnum.ResultCode.ResultCodeOk.GetValue();
  }

  public int zoomPhoto(String paramString, float paramFloat) {
    gotoImgView(AJEnum.AirJoyPhotoCmd.zoomPhoto.GetValue(), paramString, 0, paramFloat, 0.0F);
    return AJEnum.ResultCode.ResultCodeOk.GetValue();
  }
}
