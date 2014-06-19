package com.fqx.airjoy.service;

import android.content.Context;
import android.media.AudioManager;
import com.fqx.airjoy.listener.AirMediaListener;
import com.fqx.anyplay.api.AnyPlayUtils;

public class AJMediaListener implements AirMediaListener {
  private AudioManager mAM;
  private Context mContext;
  private int mCurrentVolume;
  private int mMaxVolume;

  public AJMediaListener(Context paramContext) {
    this.mContext = paramContext;
    this.mAM = ((AudioManager)this.mContext.getSystemService("audio"));
    this.mMaxVolume = this.mAM.getStreamMaxVolume(3);
    this.mCurrentVolume = this.mAM.getStreamVolume(3);
    AnyPlayUtils.LOG_DEBUG("AJMediaListener vmax = " + this.mMaxVolume + " cvol=" + this.mCurrentVolume);
  }

  public void closeSession(String paramString) {
    AnyPlayUtils.LOG_DEBUG("closeSession peerIP = " + paramString);
  }

  public int getVolume() {
    this.mCurrentVolume = this.mAM.getStreamVolume(3);
    int i = 100 * this.mCurrentVolume / this.mMaxVolume;
    AnyPlayUtils.LOG_DEBUG("getVolume vs = " + i);
    return i;
  }

  public void initSession(String paramString) {
    AnyPlayUtils.LOG_DEBUG("initSession peerIP = " + paramString);
  }

  public int setVolume(int paramInt) {
    int i = paramInt * this.mMaxVolume / 100;
    this.mAM.setStreamVolume(3, i, 0);
    return 0;
  }

}

