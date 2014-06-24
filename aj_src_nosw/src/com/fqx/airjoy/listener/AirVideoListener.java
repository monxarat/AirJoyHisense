package com.fqx.airjoy.listener;

import com.fqx.airjoy.server.AJPlayBackInfo;
import com.fqx.airjoy.server.AJVideoInfo;

public abstract interface AirVideoListener {
  public abstract AJPlayBackInfo getPlayVideoInfo();

  public abstract AJVideoInfo getPlayVideoProgress(String paramString);

  public abstract int playVideo(String paramString1, String paramString2, String paramString3, float paramFloat, String paramString4);

  public abstract int setPlayVideoProgress(String paramString, float paramFloat);

  public abstract int setPlayVideoRate(String paramString, float paramFloat);

  public abstract int stopPlayVideo(String paramString);
}